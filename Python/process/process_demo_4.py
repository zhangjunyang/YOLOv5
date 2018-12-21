# -*- coding:UTF-8 -*-
from multiprocessing import Process
import os
import time

# 定义自己的初始化进程类
class MyProcess(Process):
    def __init__(self,interval):
        super.__init__(self)
        self.interval = interval

    def run():
         print("子进程")
         startTime = time.time()
         time.sleep(self.interval)
         stopTime = time.time()
         print("子进程id：%d, 父进程id：%d, 执行了%ds"%(os.getpid(),os.getppid(), stopTime-startTime))

print("主进程")
startTime = time.time()
childs=[]
for x in range(5):
    p = MyProcess(x+1)
    p.start()
    childs.append(p)
    # p.join()

for item in childs:
    item.join()

stopTime = time.time()
print("子进程结束, 执行了%ds"%(stopTime-startTime))
