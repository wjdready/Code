#coding=utf-8

import pygame
from pygame import *

class Gobang():

    # 颜色定义
    bgcolor = [128, 94, 23]
    black = [0, 0, 0]
    white = [255, 255, 255]
    
    # 存放棋子位置信息
    array = [[0 for i in range(15)] for j in range(15)]

    turnflag = 1        # 回合标志位
    num = 15            # 每行或列格子的数量
    gridWidth = 0       # 格子宽度
    winflag = False

    # 刷新回合
    def refreshRound(self):
        if self.turnflag == 1:
            self.turnflag = 2
        else:
            self.turnflag = 1

    # 构造函数
    def __init__(self, surface):
        
        self.surface = surface
        self.surface.fill(self.bgcolor) 
        self.size = surface.get_size()
        self.drawBoardGrid()
        for row in range(0, 15):
            for col in range(0, 15):
                self.array[row][col] = 0
    
    # 绘制棋盘的边框和网格
    def drawBoardGrid(self): 
        self.size = self.surface.get_size()
        minsize = min(self.size[0], self.size[1])
        w = self.gridWidth = minsize/16
        n = self.num
        
        # 边框
        pygame.draw.rect(self.surface, self.black, [w, w, w*(n-1),  w*(n-1)], 3)

        # 网格
        for i in range(1, 16):
            pygame.draw.line(self.surface, self.black, [w*i, w], [w*i, w*n], 1)
        for i in range(1, 16):
            pygame.draw.line(self.surface, self.black, [w, w*i], [w*n,w*i], 1)

    # 处理点击事件
    def clickEvent(self, point):

        if(self.winflag == True):
            return
        self.size = self.surface.get_size()
        minsize = min(self.size[0], self.size[1])
        w = self.gridWidth = int(minsize/16)
        n = self.num
        x, y = point

        row = round( y / w )   
        col = round( x / w)
         
        if row > 15 or col > 15 or row < 1 or col < 1:
            return

        if self.array[row-1][col-1] != 0:
            return

        self.array[row-1][col-1] = self.turnflag

        if self.turnflag == 1:
            self.drawPoint(self.black, row, col)
        else:
            self.drawPoint(self.white, row, col)
        
        # 检测胜利
        if self.checkWin(row, col) == True:
            self.winflag = True
            self.drawWinInfo()
        
        self.refreshRound()

    # 在棋盘指定位置画点
    def drawPoint(self, color, row, col):
        
        self.size = self.surface.get_size()
        minsize = min(self.size[0], self.size[1])
        w = self.gridWidth = int(minsize/16)
        n = self.num
        r = int(w*0.4)
        pygame.draw.circle(self.surface, color, (col*w, row*w), r)

    # 五子连珠检查: 左斜线
    def __checkline1(self, row, col):
        index = 1
        for i in range(1, 5):
            if row - i < 1 or col - i < 1:
                break
            if self.array[row-i-1][col-i-1] != self.turnflag:
                break
            else: index += 1
                
        for i in range(1, 5):
            if row + i > 15 or col + i > 15:
                break
            if self.array[row+i-1][col+i-1] != self.turnflag:
                break
            else: index += 1
        return index
    
    # 五子连珠检查: 右斜线
    def __checkline2(self, row, col):
        index = 1
        for i in range(1, 5):
            if row + i > 15 or col - i < 1:
                break
            if self.array[row+i-1][col-i-1] != self.turnflag:
                break
            else: index += 1
                
        for i in range(1, 5):
            if row - i < 1 or col + i > 15:
                break
            if self.array[row-i-1][col+i-1] != self.turnflag:
                break
            else: index += 1
        return index

    # 五子连珠检查: 横线
    def __checkline3(self, row, col):
        index = 1
        for i in range(1, 5):
            if row + i > 15 :
                break
            if self.array[row+i-1][col-1] != self.turnflag:
                break
            else: index += 1
                
        for i in range(1, 5):
            if row - i < 1:
                break
            if self.array[row-i-1][col-1] != self.turnflag:
                break
            else: index += 1
        return index
    
    # 五子连珠检查: 竖线
    def __checkline4(self, row, col):
        index = 1
        for i in range(1, 5):
            if col + i > 15 :
                break
            if self.array[row-1][col+i-1] != self.turnflag:
                break
            else: index += 1
                
        for i in range(1, 5):
            if col - i < 1:
                break
            if self.array[row-1][col-i-1] != self.turnflag:
                break
            else: index += 1
        return index
        
    # 检查当前步骤是否能赢
    def checkWin(self, row, col):
        if self.__checkline1(row, col) >= 5:
            return True
        if self.__checkline2(row, col) >= 5:
            return True
        if self.__checkline3(row, col) >= 5:
            return True
        if self.__checkline4(row, col) >= 5:
            return True

    # 绘制胜利窗口
    def drawWinInfo(self):
        font = pygame.font.SysFont("宋体",50)
        if self.turnflag == 1:
            s = "GAME OVER BLACK WIN!"
        else:
            s = "GAME OVER WHITE WIN!"
        textRender = font.render(s, 5, (0, 0, 0))
        boxSurfce = pygame.Surface(self.size)
        boxSurfce.set_alpha(75)
        boxSurfce.fill((255, 255, 255, 90))
        boxSurfce.blit(textRender, ((self.gridWidth)*3, (self.gridWidth)*0))
        self.surface.blit(boxSurfce, (0, 0))

if __name__ == "__main__":
    
    # 窗口初始化
    pygame.init()
    screen = pygame.display.set_mode((640,640))
    pygame.display.set_caption("五子棋")

    # 新建图层
    boardSurfce = pygame.Surface(screen.get_size())
    boardSurfce = boardSurfce.convert()

    # 新建五子棋
    gobang = Gobang(boardSurfce)
    clock = pygame.time.Clock()

    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
            elif event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
                gobang.clickEvent(event.pos)
                
        # 更新所有图层
        screen.blit(boardSurfce, (0, 0))
        pygame.display.flip()
        time_passed = clock.tick(60)
