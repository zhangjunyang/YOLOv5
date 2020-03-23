import socket
import struct
import os,sys
import operator
import time
import json

buffer=1024

sk=socket.socket()
sk.connect(('127.0.0.1',8080))
user=input('请输入用户名：')
psd=input('请输入密码：')
login=str(user)+'|'+str(psd)
sk.send(bytes(login.encode('utf-8')))  #传用户名密码给客户端
ret=sk.recv(1024).decode('utf-8')   #接收登录状态  ok

if ret=='ok':
    menu=sk.recv(1024).decode('utf-8')
    print(menu)              #显示操作菜单
    do=input('请输入要执行的操作...').upper()
    sk.send(bytes(do.encode('utf-8')))    #输入上传还是下载

    #选择上传
    if do=='U':
        filepath = input('请输入要上传的文件路径\n')
        filename = input('请输入文件名\n')

        path = os.path.join(filepath, filename)
        filesize = os.stat(path).st_size  # 文件大小
        fileinfo = {'filename': filename, 'filepath': filepath,'filesize':filesize}
        filejson=json.dumps(fileinfo).encode('utf-8')
        long=len(filejson) #报头的长度
        pack_long=struct.pack('i',long)  #报头长度变为4字节
        sk.send(pack_long)
        sk.send(filejson)

        with open(path, 'rb')as f:
            while filesize:
                if filesize >= 1024:
                    filedata = f.read(buffer)
                    sk.send(filedata)
                    filesize -= buffer
                else:
                    filedata = f.read(filesize)
                    sk.send(filedata)
                    break

        show = '{0}\n上传文件：{1}:\n{2}byte\nSuccessful...\n\n'.format(
            time.strftime('%Y-%m-%d:%H:%M:%S', time.localtime(time.time())), filename,filesize)
        print(show)


    # 选择下载
    elif do=='D':
        r=sk.recv(1024).decode('utf-8')   #收到文件列表
        if operator.eq(r,''):
            print('没有可以下载的文件')
        print(r)
        filename= input('请输入要下载的文件名>>>')
        sk.send(bytes(filename.encode('utf-8')))   #发送要下载的文件名
        state=sk.recv(1024).decode('utf-8')  #文件下载状态
        print(state)

        info=sk.recv(4)   #4字节的包
        size=struct.unpack('i',info)[0]  #文件的字节大小
        print(filename,size)

        data=sk.recv(size).decode('utf-8')

        path = os.path.dirname(os.path.abspath(__file__))
        path = os.path.join(path, 'down')
        os.chdir(path)
        with open(filename,'w+',encoding='utf-8')as f:
            f.write(data)
        print(data)

    else:
        print('error，错误的操作指令！')


sk.close()