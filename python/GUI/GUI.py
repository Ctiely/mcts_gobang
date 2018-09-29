import sys
from PyQt5 import QtCore, QtGui
from PyQt5.QtWidgets import QApplication, QWidget, QLabel, QMessageBox
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QPixmap, QIcon, QPalette, QPainter
#from PyQt5.QtMultimedia import QSound


from env.cBoard import cBoard
from MCTS.MCTSPlayer import _MCTSPlayer


WIDTH = 700
HEIGHT = 700
MARGIN = 68
GRID = (WIDTH - 2 * MARGIN) / (9 - 1)
PIECE = 60
EMPTY = -1
BLACK = 0
WHITE = 1


class AI(QtCore.QThread):
    finishSignal = QtCore.pyqtSignal(int, int)

    # 构造函数里增加形参
    def __init__(self, board, parent=None):
        super(AI, self).__init__(parent)
        self.player = _MCTSPlayer(5, 10000, 100)
        self.board = board

    # 重写 run() 函数
    def run(self):
        action = self.player.get_action(self.board)
        self.finishSignal.emit(action // self.board.size, action % self.board.size)

class LaBel(QLabel):
    def __init__(self, parent):
        super().__init__(parent)
        self.setMouseTracking(True)

    def enterEvent(self, e):
        e.ignore()

class GoBang(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        self.board = cBoard(9, 5)  # 棋盘类
        self.board.render()
        palette1 = QPalette()  # 设置棋盘背景
        palette1.setBrush(self.backgroundRole(), QtGui.QBrush(QtGui.QPixmap('img/board.png')))
        self.setPalette(palette1)
        self.setCursor(Qt.PointingHandCursor)  # 鼠标变成手指形状
        #self.sound_piece = QSound("sound/luozi.wav")  # 加载落子音效
        #self.sound_win = QSound("sound/win.wav")  # 加载胜利音效
        #self.sound_defeated = QSound("sound/defeated.wav")  # 加载失败音效

        self.resize(WIDTH, HEIGHT)  # 固定大小 540*540
        self.setMinimumSize(QtCore.QSize(WIDTH, HEIGHT))
        self.setMaximumSize(QtCore.QSize(WIDTH, HEIGHT))

        self.setWindowTitle("GoBang")  # 窗口名称
        self.setWindowIcon(QIcon('img/black.png'))  # 窗口图标

        self.black = QPixmap('img/black.png')
        self.white = QPixmap('img/white.png')

        self.piece_now = BLACK  # 黑棋先行
        self.my_turn = True  # 玩家先行
        self.step = 0  # 步数
        self.x, self.y = 1000, 1000

        self.mouse_point = LaBel(self)  # 将鼠标图片改为棋子
        self.mouse_point.setScaledContents(True)
        self.mouse_point.setPixmap(self.black)  # 加载黑棋
        self.mouse_point.setGeometry(270, 270, PIECE, PIECE)
        self.pieces = [LaBel(self) for _ in range(225)]  # 新建棋子标签，准备在棋盘上绘制棋子
        for piece in self.pieces:
            piece.setVisible(True)  # 图片可视
            piece.setScaledContents(True)  # 图片大小根据标签大小可变

        self.mouse_point.raise_()  # 鼠标始终在最上层
        self.ai_down = True  # AI已下棋，主要是为了加锁，当值是False的时候说明AI正在思考，这时候玩家鼠标点击失效，要忽略掉 mousePressEvent

        self.setMouseTracking(True)
        self.show()

    def paintEvent(self, event):  # 画出指示箭头
        qp = QPainter()
        qp.begin(self)
        self.drawLines(qp)
        qp.end()

    def mouseMoveEvent(self, e):  # 黑色棋子随鼠标移动
        self.mouse_point.move(e.x() - 16, e.y() - 16)

    def mousePressEvent(self, e):  # 玩家下棋
        if e.button() == Qt.LeftButton and self.ai_down == True:
            x, y = e.x(), e.y()  # 鼠标坐标
            i, j = self.coordinate_transform_pixel2map(x, y)  # 对应棋盘坐标
            if not i is None and not j is None:  # 棋子落在棋盘上，排除边缘
                if i * self.board.size + j in self.board.availables:  # 棋子落在空白处
                    self.draw(i, j)
                    self.ai_down = False
                    self.AI = AI(self.board)  # 新建线程对象，传入棋盘参数
                    self.AI.finishSignal.connect(self.AI_draw)  # 结束线程，传出参数
                    self.AI.start()  # run

    def draw(self, i, j):
        x, y = self.coordinate_transform_map2pixel(i, j)

        if self.piece_now == BLACK:
            self.pieces[self.step].setPixmap(self.black)  # 放置黑色棋子
            self.piece_now = WHITE
        else:
            self.pieces[self.step].setPixmap(self.white)  # 放置白色棋子
            self.piece_now = BLACK

        self.board.step_(i, j)
        self.board.render()
        self.pieces[self.step].setGeometry(x, y, PIECE, PIECE)  # 画出棋子
        #self.sound_piece.play()  # 落子音效
        self.step = self.board.round  # 步数+1

        if self.board.spaces == 0:
            self.gameover(-1)
        winner = self.board.winner  # 判断输赢
        if winner != EMPTY:
            self.mouse_point.clear()

            self.gameover(winner)

    def AI_draw(self, i, j):
        if self.step != 0:
            self.draw(i, j)  # AI
            self.x, self.y = self.coordinate_transform_map2pixel(i, j)
        self.ai_down = True
        self.update()

    def coordinate_transform_map2pixel(self, i, j):
        # 从 chessMap 里的逻辑坐标到 UI 上的绘制坐标的转换
        return MARGIN + j * GRID - PIECE / 2, MARGIN + i * GRID - PIECE / 2

    def coordinate_transform_pixel2map(self, x, y):
        # 从 UI 上的绘制坐标到 chessMap 里的逻辑坐标的转换
        i, j = int(round((y - MARGIN) / GRID)), int(round((x - MARGIN) / GRID))
        # 有MAGIN, 排除边缘位置导致 i,j 越界
        if i < 0 or i >= 10 or j < 0 or j >= 10:
            return None, None
        else:
            return i, j

    def drawLines(self, qp):  # 指示AI当前下的棋子
        if self.step != 0:
            pen = QtGui.QPen(QtCore.Qt.black, 2, QtCore.Qt.SolidLine)
            qp.setPen(pen)
            qp.drawLine(self.x - 5, self.y - 5, self.x + 3, self.y + 3)
            qp.drawLine(self.x + 3, self.y, self.x + 3, self.y + 3)

            qp.drawLine(self.x, self.y + 3, self.x + 3, self.y + 3)

    def gameover(self, winner):
        if winner == BLACK:
            #self.sound_win.play()
            reply = QMessageBox.question(self, '', 'You Win! Continue?',
                                             QMessageBox.Yes | QMessageBox.No, QMessageBox.No)
        elif winner == WHITE:
            #self.sound_defeated.play()
            reply = QMessageBox.question(self, '', 'You Lose! Continue?',
                                             QMessageBox.Yes | QMessageBox.No, QMessageBox.No)
        else:
            reply = QMessageBox.question(self, '', 'TIE! Continue?',
                                         QMessageBox.Yes | QMessageBox.No, QMessageBox.No)

        if reply == QMessageBox.Yes:  # 复位
            self.piece_now = BLACK
            self.mouse_point.setPixmap(self.black)
            self.board.reset()
            self.step = 0
            for piece in self.pieces:
                piece.clear()
            self.update()
        else:
            self.close()

if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = GoBang()
    sys.exit(app.exec_())