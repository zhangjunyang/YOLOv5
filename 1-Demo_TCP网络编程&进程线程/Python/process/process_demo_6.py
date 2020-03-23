# -*- coding:UTF-8 -*-
from multiprocessing import Pool
import os
import time

def worker(msg):
    print("子进程id：%d"%os.getpid())
    startTime = time.time()
    time.sleep(3)
    stopTime = time.time()
    print("子进程结束, 执行了%d"%(stopTime-startTime))

# 创建进程池
pool = Pool(5)
for x in range(10):
   pool.apply(worker,(x,)) #同步请求

# 关闭进程池
pool.close()
# 父进程等待进程池的结束
pool.join()
print("进程池结束")
