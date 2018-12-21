# -*- coding:UTF-8 -*-
from multiprocessing import Pool,Manager
import os,time,random

def write(q):
    for item in "ABC":
        print("正在往消息队列写入%s"%item)
        q.put(item)
        time.sleep(random.random())

def read(q):
    while True:
        if not q.empty():
            item = q.get()
            print("从消息队列里读出%s"%item)
            time.sleep(random.random())
        else:
             break

# 创建消息队列
q = Manager().Queue()
# 创建进程池
pool = Pool(5)
# 创建写入进程
pool.apply(write,(q,)) #同步请求
# 创建读入进程
pool.apply(read,(q,))
