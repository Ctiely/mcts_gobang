import os
import logging
import numpy as np
import tensorflow as tf


logging.basicConfig(level=logging.INFO, format='%(asctime)s|%(levelname)s|%(message)s')


def create_session_config():
    cf = tf.ConfigProto()
    cf.gpu_options.allow_growth = True
    cf.allow_soft_placement = True
    cf.log_device_placement = False
    return cf


def preprocess_state(board):
    square_state = np.zeros((4, board.size, board.size))
    if board.states:
        square_state[0] = board.moves[0]
        square_state[1] = board.moves[1]
        # indicate the last move location
        square_state[2][board.last_action // board.size,
                        board.last_action % board.size] = 1.0
    if len(board.states) % 2 == 0:
        square_state[3][:, :] = 1.0  # indicate the colour to play
    return square_state


class policy_value_network(object):
    def __init__(self, size, model_file=None):
        self.size = size

        # Define the tensorflow neural network
        # 1. Input:
        self.input_states = tf.placeholder(tf.float32, shape=[None, 4, size, size])
        self.input_state = tf.transpose(self.input_states, [0, 2, 3, 1])
        # 2. Common Networks Layers
        self.conv1 = tf.layers.conv2d(inputs=self.input_state,
                                      filters=32,
                                      kernel_size=[3, 3],
                                      padding="same",
                                      data_format="channels_last",
                                      activation=tf.nn.relu)
        self.conv2 = tf.layers.conv2d(inputs=self.conv1,
                                      filters=64,
                                      kernel_size=[3, 3],
                                      padding="same",
                                      data_format="channels_last",
                                      activation=tf.nn.relu)
        self.conv3 = tf.layers.conv2d(inputs=self.conv2,
                                      filters=128,
                                      kernel_size=[3, 3],
                                      padding="same",
                                      data_format="channels_last",
                                      activation=tf.nn.relu)
        # 3-1 Action Networks
        self.action_conv = tf.layers.conv2d(inputs=self.conv3,
                                            filters=4,
                                            kernel_size=[1, 1],
                                            padding="same",
                                            data_format="channels_last",
                                            activation=tf.nn.relu)
        # Flatten the tensor
        self.action_conv_flat = tf.reshape(self.action_conv, [-1, 4 * size ** 2])
        # 3-2 Full connected layer, the output is the log probability of moves
        # on each slot on the board
        self.action_fc = tf.layers.dense(inputs=self.action_conv_flat,
                                         units=size ** 2,
                                         activation=tf.nn.log_softmax)
        # 4 Evaluation Networks
        self.evaluation_conv = tf.layers.conv2d(inputs=self.conv3,
                                                filters=2,
                                                kernel_size=[1, 1],
                                                padding="same",
                                                data_format="channels_last",
                                                activation=tf.nn.relu)
        self.evaluation_conv_flat = tf.reshape(self.evaluation_conv, [-1, 2 * size ** 2])
        self.evaluation_fc1 = tf.layers.dense(inputs=self.evaluation_conv_flat,
                                              units=64,
                                              activation=tf.nn.relu)
        # output the score of evaluation on current state
        self.evaluation_fc2 = tf.layers.dense(inputs=self.evaluation_fc1,
                                              units=1,
                                              activation=tf.nn.tanh)

        # Define the Loss function
        # 1. Label: the array containing if the game wins or not for each state
        self.labels = tf.placeholder(tf.float32, shape=[None, 1])
        # 2. Predictions: the array containing the evaluation score of each state
        # which is self.evaluation_fc2
        # 3-1. Value Loss function
        self.value_loss = tf.losses.mean_squared_error(self.labels,
                                                       self.evaluation_fc2)
        # 3-2. Policy Loss function
        self.mcts_probs = tf.placeholder(tf.float32, shape=[None, size ** 2])
        self.policy_loss = tf.negative(tf.reduce_mean(tf.reduce_sum(tf.multiply(self.mcts_probs, self.action_fc), 1)))
        # 3-3. L2 penalty (regularization)
        l2_penalty_beta = 1e-4
        vars = tf.trainable_variables()
        l2_penalty = l2_penalty_beta * tf.add_n([tf.nn.l2_loss(v) for v in vars if 'bias' not in v.name.lower()])
        # 3-4 Add up to be the Loss function
        self.loss = self.value_loss + self.policy_loss + l2_penalty

        # Define the optimizer we use for training
        self.learning_rate = tf.placeholder(tf.float32)
        self.optimizer = tf.train.AdamOptimizer(learning_rate=self.learning_rate).minimize(self.loss)

        # Make a session
        self.session = tf.Session(config=create_session_config())

        # calc policy entropy, for monitoring only
        self.entropy = tf.negative(tf.reduce_mean(tf.reduce_sum(tf.exp(self.action_fc) * self.action_fc, 1)))

        init = tf.global_variables_initializer()
        self.saver = tf.train.Saver()
        # Initialize variables
        if model_file is not None and os.path.isfile("{}.index".format(model_file)):
            self.restore_model(model_file)
            logging.info("load model from {}.".format(model_file))
        else:
            self.session.run(init)
            logging.info("init model.")

    def policy_value(self, state_batch):
        """
        input: a batch of states
        output: a batch of action probabilities and state values
        """
        log_act_probs, value = self.session.run([self.action_fc, self.evaluation_fc2],
                                                 feed_dict={self.input_states: state_batch})
        act_probs = np.exp(log_act_probs)
        return act_probs, value

    def policy_value_fn(self, board):
        """
        input: board
        output: a list of (action, probability) tuples for each available
        action and the score of the board state
        """
        legal_positions = board.availables
        current_state = np.ascontiguousarray(preprocess_state(board).reshape(-1, 4, self.size, self.size))
        act_probs, value = self.policy_value(current_state)
        act_probs = zip(legal_positions, act_probs[0][legal_positions])
        return act_probs, value

    def train_step(self, state_batch, mcts_probs, winner_batch, lr):
        """perform a training step"""
        winner_batch = np.reshape(winner_batch, (-1, 1))
        loss, entropy, _ = self.session.run([self.loss, self.entropy, self.optimizer],
                                             feed_dict={self.input_states: state_batch,
                                                        self.mcts_probs: mcts_probs,
                                                        self.labels: winner_batch,
                                                        self.learning_rate: lr})
        return loss, entropy

    def save_model(self, model_path, global_step):
        self.saver.save(self.session, model_path, global_step=global_step)

    def restore_model(self, model_path):
        self.saver.restore(self.session, model_path)