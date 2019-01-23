#!/bin/sh

cd ~

mkdir shell_tut

cd shell_tut

for ((i=0; i<10; i++)); do

        touch test_$i.txt

done



# ·第1行：指定脚本解释器，这里是用/bin/sh做解释器的
# ·第2行：切换到当前用户的home目录
# ·第3行：创建一个目录shell_tut
# ·第4行：切换到shell_tut目录
# ·第5行：循环条件，一共循环10次
# ·第6行：创建一个test_1…10.txt文件
# ·第7行：循环体结束