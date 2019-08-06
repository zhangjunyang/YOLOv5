# -*- coding:UTF-8 -*-
from multiprocessing import Process
import os
import time

def fun(name):
    time.sleep(3)
    print("子进程id:%d, 父进程id:%d, name:%s"%(os.getpid(),os.getppid(),name))

print("父进程")
# 创建子进程
p = Process(target=fun,args=("test",))
p.start()
p.join()
print("子进程结束")