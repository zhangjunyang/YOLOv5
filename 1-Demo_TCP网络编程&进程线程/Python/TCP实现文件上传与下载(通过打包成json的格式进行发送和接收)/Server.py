import socket
from Server_lib import read
from Server_lib import down
from Server_lib import up

buffer=1024  #一次读取内容长度字节
sk=socket.socket()   #创建socket对象
sk.bind(('127.0.0.1',8080))  #绑定地址
sk.listen()  #监听

conn,addr=sk.accept() #建立一个连接

info=conn.recv(50).decode('utf-8')   #接收客户端的用户名密码
user,psd=info.split('|')   #分割获取用户名 和 密码

ret=read(user,psd)  #调read读取用户名信息 验证用户名密码是否正确
if ret:  #登录成功
    conn.send(bytes('ok'.encode('utf-8')))   #返回一个OK
    conn.send(bytes('用户登录成功,请选择要执行的操作【U：上传文件】【D：下载文件】'.encode('utf-8')))

    choice=conn.recv(4).decode('utf-8')   #客户端选择上传还是下载

    if choice=='U':  #文件上传
        up(conn,buffer)    #调上传方法

    elif choice=='D':   # 文件下载
        down(conn)  #调下载方法
    else:
        print('error，错误的操作指令！')
else:   #登录失败 关闭连接和对象
    conn.send(bytes('用户登录失败，断开连接...'.encode('utf-8')))
    conn.close()
    sk.close()