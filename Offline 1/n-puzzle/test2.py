import sys
from functools import partial
from PyQt5.Qt import *

import numpy as np


class PushButton(QPushButton):
    def __init__(self, text, parent=None):
        super(PushButton, self).__init__(text, parent)

        self.setText(text)
        self.setMinimumSize(QSize(100, 100))
        self.setMaximumSize(QSize(100, 100))


class MyWindow(QMainWindow):
    def __init__(self):
        super(MyWindow, self).__init__()

        self.rows = 4
        self.columns = 4

        centralWidget = QWidget()
        self.setCentralWidget(centralWidget)

        self.label = QLabel(self, alignment=Qt.AlignRight)
        self.label.setFont(QFont("Times", 12, QFont.Bold))

        self.layout = QGridLayout(centralWidget)
        self.layout.addWidget(self.label, 0, 0, 1, 3)

        self.N = self.rows * self.columns

        _list = []
        for i in range(1, self.N):
            _list.append(i)
        _list.append(0)

        # _list = [1, 2, 3, 4, 5, 6, 7, 8, 9, 0]
        # len_list = len(_list)

        self.grid = np.zeros(shape=(self.rows, self.columns), dtype=int)
        print(_list)
        idx = 0
        for row in range(self.rows):
            for column in range(self.columns):
                self.grid[row][column] = _list[idx]
                idx += 1

        self.drawGrid(self.grid)

    def drawGrid(self,grid):
        i = 0
        for row in range(self.rows):
            for column in range(self.columns):
                if grid[row][column] != 0:
                    button = PushButton(f'{grid[row][column]}', self)
                    button.clicked.connect(partial(self.onClicked, grid[row][column], row, column))
                    self.layout.addWidget(button, row + 1, column)
                    i += 1

        print(self.grid)

    def checkEmpty(self, posR, posC):
        if posR < 0 or posC < 0:
            return False
        if posR >= self.rows or posC >= self.columns:
            return False
        if self.grid[posR][posC] == 0:
            return True
        return False

    def findRowCol(self,num):
        for row in range(self.rows):
            for column in range(self.columns):
                if self.grid[row][column] == num:
                    return (row,column)


    def onClicked(self, num, posR, posC):
        dX = [-1, 0, 1, 0]
        dY = [0, 1, 0, -1]

        for i in range(len(dX)):
            if self.checkEmpty(posR + dX[i], posC + dY[i]):
                print("Empty adjacent to ", num)
                print(posR,"---",posC)
                print(posR + dX[i],"---",posC + dY[i])
                temp = self.grid[posR + dX[i]][posC + dY[i]]
                self.grid[posR + dX[i]][posC + dY[i]] = self.grid[posR][posC]
                self.grid[posR][posC] = temp
                print(self.grid)
                for i in reversed(range(self.layout.count())):
                    self.layout.itemAt(i).widget().setParent(None)
                self.drawGrid(self.grid)

        # self.label.setNum(num)


if __name__ == '__main__':
    app = QApplication(sys.argv)
    w = MyWindow()
    w.setWindowTitle('N Puzzle')
    w.show()
    sys.exit(app.exec_())
