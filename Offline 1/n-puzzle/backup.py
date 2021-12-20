import sys
from functools import partial
from PyQt5.Qt import *

import numpy as np

import heapq
from typing import Protocol, Dict, List, Iterator, Tuple, TypeVar, Optional

# import resource, sys
# resource.setrlimit(resource.RLIMIT_STACK, (2**29,-1))
# sys.setrecursionlimit(10**6)

T = TypeVar('T')

class PriorityQueue:
    def __init__(self):
        self.elements: List[Tuple[float, T]] = []

    def empty(self) -> bool:
        return not self.elements

    def put(self, element: T, priority: float):
        heapq.heappush(self.elements, (priority, element))

    def get(self) -> T:
        ret = heapq.heappop(self.elements)
        return (ret[1], ret[0])

class Node:
    def __init__(self,curBoard,prevBoard,move,cost):
        self.curBoard = curBoard
        self.prevBoard = prevBoard
        self.move = move
        self.cost = cost

    def __lt__(self, other):
        return self.cost < other.cost


class PushButton(QPushButton):
    def __init__(self, text, parent=None):
        super(PushButton, self).__init__(text, parent)

        self.setText(text)
        self.setMinimumSize(QSize(100, 100))
        self.setMaximumSize(QSize(100, 100))


class Solver:
    def __init__(self, N: int, board, boardTarget):
        self.N = N
        assert (N == board.shape[0])
        self.board = board
        self.boardTarget = boardTarget
        self.dX = [-1, 0, 1, 0]
        self.dY = [0, 1, 0, -1]

    def withinGrid(self, posR, posC):
        if posR < 0 or posC < 0:
            return False
        if posR >= self.N or posC >= self.N:
            return False
        return True

    def hash(self, grid):
        return tuple({tuple(row) for row in grid})

    def solveBrute(self):
        print(self.board)
        print(self.boardTarget)

        pq = PriorityQueue()
        pq.put(self.board, 0)

        visited = set()
        visited.add(self.hash(self.board))

        while not pq.empty():

            cur, curCost = pq.get()

            if (cur == self.boardTarget).all():
                print("Target found")
                print("Cost ", curCost)
                break

            for r in range(self.N):
                for c in range(self.N):
                    if cur[r][c] == 0:
                        for i in range(4):
                            if self.withinGrid(r + self.dX[i], c + self.dY[i]):
                                newCost = curCost + 1

                                ## create next grid by swapping
                                nextGrid = cur.copy()
                                temp = nextGrid[r + self.dX[i]][c + self.dY[i]]
                                nextGrid[r + self.dX[i]][c + self.dY[i]] = nextGrid[r][c]
                                nextGrid[r][c] = temp

                                if self.hash(nextGrid) in visited:
                                    continue

                                # if not vis:
                                visited.add(self.hash(nextGrid))
                                priority = newCost
                                pq.put(nextGrid, priority)

        print("Done")

    def h_1_hammingDistance(self, boardNow):
        cnt = 0
        for i in range(self.N):
            for j in range(self.N):
                if self.boardTarget[i][j] != 0:
                    cnt += (boardNow[i][j] != self.boardTarget[i][j])
        return cnt

    def getPos(self,digit,boardNow):
        N = boardNow.shape[0]
        for i in range(N):
            for j in range(N):
                if boardNow[i][j] == digit:
                    return i,j

    def h_2_manhattanDistance(self,boardNow):
        cnt = 0
        for d in range(1,self.N*self.N):
            r1 , c1 = self.getPos(d,boardNow)
            r2 , c2 = self.getPos(d,self.boardTarget)
            cnt += abs(r1-r2) + abs(c1-c2)
        return cnt

    def solveAStar(self, heuristic):

        print(self.board)
        print(self.boardTarget)

        pq = PriorityQueue()
        pq.put(Node(self.board,None,0,0), 0)

        visited = set(); D = {}
        hashBoard = self.hash(self.board)
        visited.add(hashBoard)
        D[hashBoard] = 0

        while not pq.empty():

            curNode, curCost = pq.get()
            cur = curNode.curBoard

            if (cur == self.boardTarget).all():
                print("Target found")
                print("Cost ", curCost)
                print("Move ", curNode.move)
                break

            ## optimization
            if curCost > D[self.hash(cur)]:
                continue

            for r in range(self.N):
                for c in range(self.N):
                    if cur[r][c] == 0:
                        for i in range(4):
                            if self.withinGrid(r + self.dX[i], c + self.dY[i]):

                                ## create next grid by swapping
                                nextGrid = cur.copy()
                                temp = nextGrid[r + self.dX[i]][c + self.dY[i]]
                                nextGrid[r + self.dX[i]][c + self.dY[i]] = nextGrid[r][c]
                                nextGrid[r][c] = temp
                                hashNextGrid = self.hash(nextGrid)

                                ## calculate priority
                                newMove = curNode.move + 1
                                priority = newMove + heuristic(nextGrid)

                                ## if not visited yet or visited before but optimal now
                                if hashNextGrid not in visited or priority <= D[hashNextGrid]:
                                    visited.add(hashNextGrid)
                                    pq.put(Node(nextGrid,cur,newMove,priority), priority)
                                    D[hashNextGrid] = priority

        print("A Star Done")

    def solve(self):
        # print("Using Hamming Distance")
        # self.solveAStar(self.h_1_hammingDistance)

        print("Using Manhattan Distance")
        self.solveAStar(self.h_2_manhattanDistance)


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

    def drawGrid(self, grid):
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

    def findRowCol(self, num):
        for row in range(self.rows):
            for column in range(self.columns):
                if self.grid[row][column] == num:
                    return (row, column)

    def onClicked(self, num, posR, posC):
        dX = [-1, 0, 1, 0]
        dY = [0, 1, 0, -1]

        for i in range(len(dX)):
            if self.checkEmpty(posR + dX[i], posC + dY[i]):
                print("Empty adjacent to ", num)
                print(posR, "---", posC)
                print(posR + dX[i], "---", posC + dY[i])
                temp = self.grid[posR + dX[i]][posC + dY[i]]
                self.grid[posR + dX[i]][posC + dY[i]] = self.grid[posR][posC]
                self.grid[posR][posC] = temp
                print(self.grid)
                for i in reversed(range(self.layout.count())):
                    self.layout.itemAt(i).widget().setParent(None)
                self.drawGrid(self.grid)

        # self.label.setNum(num)


if __name__ == '__main__':
    # app = QApplication(sys.argv)
    # w = MyWindow()
    # w.setWindowTitle('N Puzzle')
    # w.show()
    # sys.exit(app.exec_())

    '''
    test without gui
    '''

    # N = 2
    # grid = np.zeros(shape=(N, N), dtype=int)

    # D = {}
    # D[grid.tobytes()] = 10
    # print(D[grid.tobytes()])

    # idx = 1
    # for row in range(N):
    #     for column in range(N):
    #         grid[row][column] = idx%(N*N)
    #         idx += 1
    # gridTarget = grid.copy()
    # np.random.shuffle(grid)
    # print(grid)

    # grid = np.array([[1,2,3],[4,0,6],[7,5,8]],np.int32)
    # grid = np.array([[0,1,3],[4,2,5],[7,8,6]],np.int32)
    # grid = np.array([[8, 1, 3], [4, 0, 2], [7, 6, 5]], np.int32)  ## 14
    grid = np.array([[1,3,2],[4,6,5],[7,8,0]],np.int32) ## 18

    gridNow = np.array([[7,2,4],[6,0,5],[8,3,1]],np.int32)
    gridTarget = np.array([[1, 2, 3], [4, 5, 6], [7, 8, 0]], np.int32)

    solver = Solver(3, grid, gridTarget)

    solver.solve()
    # print(solver.h_2_manhattanDistance(gridNow))

    # pq = PriorityQueue()
    # pq.put("asd",12)
    # pq.put("asdasdas",1)
    # pq.put("asafd",4)
    # pq.put("sadd",2)
    #
    # print(pq.get())
    # print(pq.get())
    # print(pq.get())
    # print(pq.get())
