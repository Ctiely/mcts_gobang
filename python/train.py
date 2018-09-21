import random
import logging
import numpy as np
from collections import deque
from env.cBoard import cBoard
from MCTS.MCTSPlayer import MCTSPlayer
from network.policy_value_network import preprocess_state, policy_value_network

logging.basicConfig(level=logging.INFO, format='%(asctime)s|%(levelname)s|%(message)s')


class TrainPipeline():
    def __init__(self, model_file=None):
        # params of the board and the game
        self.board_size = 10
        self.win_n = 5
        self.board = cBoard(self.board_size, self.win_n)
        # training params
        self.learn_rate = 2e-3
        self.lr_multiplier = 1.0  # adaptively adjust the learning rate based on KL
        self.tau = 1.0  # the temperature param
        self.n_playround = 1000  # num of simulations for each move
        self.c_puct = 5
        self.buffer_size = 100000
        self.batch_size = 512  # mini-batch size for training
        self.data_buffer = deque(maxlen=self.buffer_size)
        self.play_batch_size = 3
        self.epochs = 5  # num of train_steps for each update
        self.kl_targ = 0.02
        self.save_step = 1000
        self.game_batch_num = 50000
        if model_file:
            # start training from an initial policy-value net
            self.policy_value_net = policy_value_network(self.board_size,
                                                         model_file=model_file)
        else:
            # start training from a new policy-value net
            self.policy_value_net = policy_value_network(self.board_size)

        self.mcts_player = MCTSPlayer(self.policy_value_net.policy_value_fn,
                                      c_puct=self.c_puct,
                                      n_playround=self.n_playround,
                                      is_selfplay=True)

    def get_equi_data(self, play_data):
        """
            augment the data set by rotation and flipping
            play_data: [(state, mcts_prob, winner_z), ..., ...]
        """
        extend_data = []
        for state, mcts_porb, winner in play_data:
            for i in [1, 2, 3, 4]:
                # rotate counterclockwise
                equi_state = np.array([np.rot90(s, i) for s in state])
                equi_mcts_prob = np.rot90(np.flipud(
                    mcts_porb.reshape(self.board_size, self.board_size)), i)
                extend_data.append((equi_state,
                                    np.flipud(equi_mcts_prob).flatten(),
                                    winner))
                # flip horizontally
                equi_state = np.array([np.fliplr(s) for s in equi_state])
                equi_mcts_prob = np.fliplr(equi_mcts_prob)
                extend_data.append((equi_state,
                                    np.flipud(equi_mcts_prob).flatten(),
                                    winner))
        return extend_data

    def self_play(self):
        self.board.reset()
        states, mcts_probs, current_players = [], [], []
        while not self.board.over:
            #print("depth: ", self.mcts_player.mcts.depth())
            action, action_probs = self.mcts_player.get_action(self.board,
                                                               tau=self.tau,
                                                               return_prob=True)
            # store the data
            states.append(preprocess_state(self.board))
            mcts_probs.append(action_probs)
            current_players.append(self.board.current_player)
            # perform a action
            self.board.step(action)
        # winner from the perspective of the current player of each state
        winners_z = np.zeros(len(current_players))
        if self.board.winner != -1:
            winners_z[np.array(current_players) == self.board.winner] = 1.0
            winners_z[np.array(current_players) != self.board.winner] = -1.0
        # reset MCTS root node
        self.mcts_player.reset_player()
        return self.board.winner, zip(states, mcts_probs, winners_z)

    def collect_selfplay_data(self, n_games=1):
        """collect self-play data for training"""
        for i in range(n_games):
            winner, play_data = self.self_play()
            play_data = list(play_data)[:]
            self.episode_len = len(play_data)
            # augment the data
            play_data = self.get_equi_data(play_data)
            self.data_buffer.extend(play_data)

    def policy_update(self):
        """update the policy-value net"""
        mini_batch = random.sample(self.data_buffer, self.batch_size)
        state_batch = [data[0] for data in mini_batch]
        mcts_probs_batch = [data[1] for data in mini_batch]
        winner_batch = [data[2] for data in mini_batch]
        old_probs, old_v = self.policy_value_net.policy_value(state_batch)
        new_v = -1
        kl = -1
        loss = -1
        entropy = -1
        for i in range(self.epochs):
            loss, entropy = self.policy_value_net.train_step(state_batch,
                                                             mcts_probs_batch,
                                                             winner_batch,
                                                             self.learn_rate * self.lr_multiplier)
            new_probs, new_v = self.policy_value_net.policy_value(state_batch)
            kl = np.mean(np.sum(old_probs * (np.log(old_probs + 1e-10) - np.log(new_probs + 1e-10)), axis=1))
            if kl > self.kl_targ * 4:  # early stopping if D_KL diverges badly
                break
        # adaptively adjust the learning rate
        if kl > self.kl_targ * 2 and self.lr_multiplier > 0.1:
            self.lr_multiplier /= 1.5
        elif kl < self.kl_targ / 2 and self.lr_multiplier < 10:
            self.lr_multiplier *= 1.5

        explained_var_old = (1 - np.var(np.array(winner_batch) - old_v.flatten()) / np.var(np.array(winner_batch)))
        explained_var_new = (1 - np.var(np.array(winner_batch) - new_v.flatten()) / np.var(np.array(winner_batch)))
        logging.info("kl:{:.5f}, lr_multiplier:{:.3f}, loss:{}, entropy:{}, explained_var_old:{:.3f}, explained_var_new:{:.3f}"
                     .format(kl, self.lr_multiplier, loss, entropy, explained_var_old, explained_var_new))
        return loss, entropy

    def run(self):
        """run the training pipeline"""
        try:
            for step in range(self.game_batch_num):
                self.collect_selfplay_data(self.play_batch_size)
                logging.info("batch step:{}, episode_len:{}".format(step + 1, self.episode_len))
                if len(self.data_buffer) > self.batch_size:
                    loss, entropy = self.policy_update()
                    #logging.info("loss: {}, entropy: {}".format(loss, entropy))
                #save the model params
                if (step + 1) % self.save_step == 0:
                    logging.info("current self-play batch: {}".format(step + 1))
                    self.policy_value_net.save_model("./models/model", global_step=step)
        except KeyboardInterrupt:
            logging.warning("\n\rquit")
            
            
if __name__ == '__main__':
    training_pipeline = TrainPipeline()
    training_pipeline.run()