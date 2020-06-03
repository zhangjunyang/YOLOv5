# -*- coding:UTF-8 -*-
import os

pid = os.fork()
if pid==0:
    print('子进程11')
else:
    print('父进程11')

pid = os.fork()
if pid==0:
    print('子进程22')
else:
    print('父进程22')