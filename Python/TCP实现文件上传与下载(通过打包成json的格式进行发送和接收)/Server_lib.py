import os
import struct
import operator
import time
import json


#登录验证
def read(user,psd):
    with open('info','r',encoding='utf-8')as f:
        for i in f:     #读取所有记录
            i=i.strip()     #去空格
            if user in i:   #如果用户名相匹配
                n_user,n_psd=i.split('|')     #分割字符串 得到用户名密码
                if n_psd==psd:
                    return True     #密码正确则为TRUE
                else:
                    return False    #错误则为FALSE
#上传
def up(conn,buffer):
    long=conn.recv(4)   #4字节的数据长度包
    pack_long=struct.unpack('i',long)[0]  #报头的长度
    filejson=conn.recv(pack_long).decode('utf-8')
    fileinfo=json.loads(filejson) #报头内容

    filename = fileinfo['filename']
    filesize = fileinfo['filesize']

    with open(filename, 'wb')as f:  #将接收到的数据写入文件
        while filesize:
            if filesize >= buffer:   #如果文件大小 大于读取数据量
                filedata = conn.recv(buffer)
                f.write(filedata)     #就将读取的数据量写入
                filesize -= buffer
            else:
                filedata = conn.recv(filesize)
                f.write(filedata)    #直接把文件写入
                break
        print('文件%s上传成功' % filename)


#下载
def down(conn):
    files = os.listdir()
    lis_file = ''
    for i in files:
        lis_file += i + '\n'
    if operator.eq(lis_file, ''):  # 如果文件列表为空
        conn.send(''.encode('utf-8'))
    else:
        conn.send(lis_file.encode('utf-8'))  #把文件列表发给客户端
        while True:
            filename=conn.recv(1024).decode('utf-8')  #接收要下载的文件名
            if filename not in files:
                conn.send(bytes('文件名不存在!'.encode('utf-8')))
            else:
                conn.send(bytes('开始文件传输!'.encode('utf-8')))
                break
        size=os.stat(filename).st_size
        info=struct.pack('i',size)
        conn.send(info)    #文件信息大小打包发送给客户端

        with open(filename,'rb')as f:
            while True:
                file_data=f.read(4096)
                if not file_data:
                    break
                conn.send(file_data)

        show='{0}\n下载文件：{1}:\n{2}byte\nSuccessful...\n\n'.format(time.strftime('%Y-%m-%d:%H:%M:%S',time.localtime(time.time())),filename,size)
        print(show)
        path=os.path.dirname(os.path.abspath(__file__))
        path=os.path.join(path,'log')

        os.chdir(path)    #临时把当前目录变为 E:\program\ms\log
        with open('data_log','a',encoding='utf-8')as f:
            f.write(show)              #把输出信息记录入日志