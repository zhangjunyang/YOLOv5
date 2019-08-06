#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import threading
import time

def fun(num):
    time.sleep(3)
    print("线程执行%d"%num)

print("主线程")
for i in range(10):
    t = threading.Thread(target=fun,args=(i+1,))
    t.start()
    t.join()