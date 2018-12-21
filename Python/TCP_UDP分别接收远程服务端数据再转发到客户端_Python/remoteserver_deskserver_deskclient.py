#!/usr/bin/env python
# -*- coding:utf8 -*-
 
import sys
import time
import os
from time import sleep
import socket
 
reload(sys)
sys.setdefaultencoding('utf-8')
 
 
# make a copy of original stdout route
stdout_backup = sys.stdout
# define the log file that receives your log info
log_file = open("forward_message.log", "a")
# redirect print output to log file
sys.stdout = log_file
log_file.close()
 
dest_host = '192.168.5.234'
dest_port = 8080
 
def showHex(s):
    for c in s:
        print("%x"%(ord(c))),
    print("\nreceive length :%d"%(len(s)))
 
def tcp_send_recv(sendAddr,sendPort,recvData):
    try:
        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)        # 创建一个socket
        client.connect((sendAddr,sendPort))
        client.send(recvData)
        client.close()
    except BaseException as e:
        print(e)
        return None
 
 
class TcpServer(object):
     def tcpServer(self):
        server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)        # 创建一个socket
        server.bind(('', 8080))       # 绑定同一个域名下的所有机器
        server.listen(5)              #传入的参数指定等待连接的最大数量
        print ('Waiting for connection...')
 
        while True:
            sock, addr = sock.accept()
            recvData = sock.recv(1024)
            log_file = open("forward_message.log", "a")
            sys.stdout = log_file
            print("****************************")
            print(time.strftime("%Y-%m-%d %X",time.localtime(time.time())))
            print("connect from: %s" % (addr))     # 接收客户端的ip, port
            if len(recvData)>6:
                showHex(recvData)
                print("recvData     :", recvData)
                sendPort = dest_port   # ord(recvData[4])+ord(recvData[5])*256
                sendAddr = dest_host   #"%d.%d.%d.%d"%(ord(recvData[0]),ord(recvData[1]),ord(recvData[2]),ord(recvData[3]))
                print("to:%s:%d"%(sendAddr,sendPort))
                #rx after tx
                tcp_send_recv(sendAddr,sendPort,recvData)
            else:
                print("recvData: ", recvData)
                tcp_send_recv(sendAddr,sendPort,recvData)
                #print("sendData(%3d):%s"%(sendDataLen,recvData))
            print("****************************\n")
            log_file.close()
            sys.stdout = stdout_backup
            sock.close()
 
def udp_send_recv(sendAddr, sendPort, recvData, remoteHost, remotePort,sock):
    try:
        sock2 = socket.socket(socket.AF_INET,type=socket.SOCK_DGRAM)
        sock2.settimeout(5)
        sock2.sendto(recvData, (sendAddr, sendPort))
        data2 = sock2.recv(512)
        #combine head and data
        #data2 = '%s%s'%(recvData,data2)
        sock2.close()
        print('forward ok')
        sendDataLen = sock.sendto(data2, (remoteHost, remotePort))
        print("return length:%d"%(len(data2)))
        return data2
    except socket.error as d:
        print(d)
        return None
    except BaseException as e:
        print(e)
        return None
 
 
class UdpServer(object):
      def udpServer(self):
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.bind(('', 8080))       # 绑定同一个域名下的所有机器
 
        while True:
            recvData, (remoteHost, remotePort) = sock.recvfrom(1024)
            log_file = open("forward_message.log", "a")
            sys.stdout = log_file
            print("****************************")
            print(time.strftime("%Y-%m-%d %X",time.localtime(time.time())))
            print("[%s:%s] connect" % (remoteHost, remotePort))     # 接收客户端的ip, port
            if len(recvData)>6:
                showHex(recvData)
                print("recvData     :", recvData)
                sendPort = dest_port   # ord(recvData[4])+ord(recvData[5])*256
                sendAddr = dest_host   #"%d.%d.%d.%d"%(ord(recvData[0]),ord(recvData[1]),ord(recvData[2]),ord(recvData[3]))
                print("to:%s:%d"%(sendAddr,sendPort))
                #rx after tx
                udp_send_recv(sendAddr,sendPort,recvData,remoteHost, remotePort,sock)
            else:
                print("recvData: ", recvData)
                sendDataLen = sock.sendto(recvData, (remoteHost, remotePort))
                print("sendDataLen: ", sendDataLen)
                #print("sendData(%3d):%s"%(sendDataLen,recvData))
            print("****************************\n")
            log_file.close()
            sys.stdout = stdout_backup
        sock.close()
 
if __name__ == "__main__":
    sys.stdout = sys.__stdout__
    if len(sys.argv) != 2:
        print("cmd : python udp_nc.py [IP]")
        print( ("参数个数: %d ")  % len(sys.argv))
        print( ("没有识别到数据的输入，将使用默认IP ") )
    else:
        print( ("识别到输入的IP: " % sys.argv[1]) )
        dest_host = sys.argv[1]
    print ('接收到的数据将会转发到IP: %s ' % dest_host)
    udpServer = UdpServer()
    udpServer.udpServer()