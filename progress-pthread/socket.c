using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Net;
using System.Threading;
using System.IO;
namespace CAServer
{
    class Program
    {

        //当前主机ip
        static string _ip = "192.168.1.2";
        //端口号
        static int _point = 8000;

        static void Main(string[] args)
        {
            //Thread thread = new Thread(startServer);
            //thread.Start();

            startServer();

            Console.ReadLine();

        }

        public static void startServer()
        {

            //第一步：创建监听用的socket
            Socket socket = new Socket
            (
                AddressFamily.InterNetwork, //使用ip4
                SocketType.Stream,//流式Socket，基于TCP
                ProtocolType.Tcp //tcp协议
            );

            //第二步：监听的ip地址和端口号
            //ip地址
            IPAddress ip = IPAddress.Parse(_ip);
            //ip地址和端口号
            IPEndPoint point = new IPEndPoint(ip, _point);

            //绑定ip和端口
            //端口号不能占用：否则：以一种访问权限不允许的方式做了一个访问套接字的尝试
            //通常每个套接字地址(协议/网络地址/端口)只允许使用一次。
            try
            {
                socket.Bind(point);
            }
            catch (Exception)
            {

                if (new IOException().InnerException is SocketException)
                    Console.WriteLine("端口被占用");
            }
            //socket.Bind(point);

            //第三步：开始监听端口

            //监听队列的长度
            /*比如：同时有3个人来连接该服务器，因为socket同一个时间点。只能处理一个连接
             * 所以其他的就要等待。当处理第一个。然后在处理第二个。以此类推
             * 
             * 这里的10就是同一个时间点等待的队列长度为10，即。只能有10个人等待，当第11个的时候。是连接不上的
             */
            socket.Listen(10);

            string msg = string.Format("服务器已经启动........\n监听ip为：{0}\n监听端口号为：{1}\n", _ip, _point);
            showMsg(msg);

            Thread listen = new Thread(Listen);
            listen.IsBackground = true;
            listen.Start(socket);

        }
        /// <summary>
        /// 多线程执行
        /// Accept方法。会阻塞线程。所以用多线程
        /// </summary>
        /// <param name="o"></param>
        static void Listen(object o)
        {
            Socket socket = o as Socket;

            //不停的接收来自客服端的连接
            while (true)
            {
                //如果有客服端连接，则创建通信用是socket  
                //Accept方法。会阻塞线程。所以用多线程
                //Accept方法会一直等待。直到有连接过来
                Socket connSocket = socket.Accept();

                //获取连接成功的客服端的ip地址和端口号
                string msg = connSocket.RemoteEndPoint.ToString();
                showMsg(msg + "连接");

                //获取本机的ip地址和端口号
                //connSocket.LocalEndPoint.ToString();

                /*
                 如果不用多线程。则会一直执行ReceiveMsg
                 * 就不会接收客服端连接了
                 */
                Thread th = new Thread(ReceiveMsg);
                th.IsBackground = true;
                th.Start(connSocket);

            }
        }
        /// <summary>
        /// 接收数据
        /// </summary>
        /// <param name="o"></param>
        static void ReceiveMsg(object o)
        {
            Socket connSocket = o as Socket;
            while (true)
            {

                //接收数据
                byte[] buffer = new byte[1024 * 1024];//1M
                int num = 0;
                try
                {
                    //接收数据保存发送到buffer中
                    //num则为实际接收到的字节个数

                    //这里会遇到这个错误：远程主机强迫关闭了一个现有的连接。所以try一下
                    num = connSocket.Receive(buffer);
                    //当num=0.说明客服端已经断开
                    if (num == 0)
                    {
                        connSocket.Shutdown(SocketShutdown.Receive);
                        connSocket.Close();
                        break;
                    }
                }
                catch (Exception ex)
                {
                    if (new IOException().InnerException is SocketException)
                        Console.WriteLine("网络中断");
                    else
                        Console.WriteLine(ex.Message);
                    break;
                }

                //把实际有效的字节转化成字符串
                string str = Encoding.UTF8.GetString(buffer, 0, num);
                showMsg(connSocket.RemoteEndPoint + "说：\n" + str);



            }
        }
        /// <summary>
        /// 显示消息
        /// </summary>
        static void showMsg(string msg)
        {
            Console.WriteLine(msg);
            //Console.ReadKey();
        }
    }
}
