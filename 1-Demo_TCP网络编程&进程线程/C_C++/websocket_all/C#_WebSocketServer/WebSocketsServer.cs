using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Collections;

namespace WebSocketServer
{
    public class Logger
    {
        public bool LogEvents { get; set; }

        public Logger()
        {
            LogEvents = true;
        }

        public void Log(string Text)
        {
            if (LogEvents) Console.WriteLine(Text);
        }
    }

    public enum ServerStatusLevel { Off, WaitingConnection, ConnectionEstablished };

    public delegate void NewConnectionEventHandler(string loginName,EventArgs e);
    public delegate void DataReceivedEventHandler(Object sender, string message, EventArgs e);
    public delegate void DisconnectedEventHandler(Object sender,EventArgs e);
    public delegate void BroadcastEventHandler(string message, EventArgs e);

    public class WebSocketServer : IDisposable
    {
        private bool AlreadyDisposed;
        private Socket Listener;
        private int ConnectionsQueueLength;
        private int MaxBufferSize;
        private string Handshake;
        private StreamReader ConnectionReader;
        private StreamWriter ConnectionWriter;
        private Logger logger;
        private byte[] FirstByte;
        private byte[] LastByte;
        private byte[] ServerKey1;
        private byte[] ServerKey2;

        List<SocketConnection> connectionSocketList = new List<SocketConnection>();

        public ServerStatusLevel Status { get; private set; }
        public int ServerPort { get; set; }
        public string ServerLocation { get; set; }
        public string ConnectionOrigin { get; set; }
        public bool LogEvents { 
            get { return logger.LogEvents; }
            set { logger.LogEvents = value; }
        }

        public event NewConnectionEventHandler NewConnection;
        public event DataReceivedEventHandler DataReceived;
        public event DisconnectedEventHandler Disconnected;

        private void Initialize()
        {
            AlreadyDisposed = false;
            logger = new Logger();

            Status = ServerStatusLevel.Off;
            ConnectionsQueueLength = 500;
            MaxBufferSize = 1024 * 100;
            FirstByte = new byte[MaxBufferSize];
            LastByte = new byte[MaxBufferSize];
            FirstByte[0] = 0x00;
            LastByte[0] = 0xFF;
            logger.LogEvents = true;
        }

        public WebSocketServer() 
        {
            ServerPort = 4141;
            ServerLocation = string.Format("ws://{0}:4141/chat", getLocalmachineIPAddress());
            Initialize();
        }

        public WebSocketServer(int serverPort, string serverLocation, string connectionOrigin)
        {
            ServerPort = serverPort;
            ConnectionOrigin = connectionOrigin;
            ServerLocation = serverLocation;
            Initialize();
        }


        ~WebSocketServer()
        {
            Close();
        }


        public void Dispose()
        {
            Close();
        }

        private void Close()
        {
            if (!AlreadyDisposed)
            {
                AlreadyDisposed = true;
                if (Listener != null) Listener.Close();
                foreach (SocketConnection item in connectionSocketList)
                {
                    item.ConnectionSocket.Close();
                }
                connectionSocketList.Clear();
                GC.SuppressFinalize(this);
            }
        }

        public static  IPAddress getLocalmachineIPAddress()
        {
            string strHostName = Dns.GetHostName();
            IPHostEntry ipEntry = Dns.GetHostEntry(strHostName);

            foreach (IPAddress ip in ipEntry.AddressList)
            {
                //IPV4
                if (ip.AddressFamily == AddressFamily.InterNetwork)
                    return ip;
            }

            return ipEntry.AddressList[0];
        }

        public void StartServer()
        {
            Char char1 = Convert.ToChar(65533);

            Listener = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.IP);  
            Listener.Bind(new IPEndPoint(getLocalmachineIPAddress(), ServerPort));  
            
            Listener.Listen(ConnectionsQueueLength);  

            logger.Log(string.Format("聊天服务器启动。监听地址：{0}, 端口：{1}",getLocalmachineIPAddress(),ServerPort));
            logger.Log(string.Format("WebSocket服务器地址: ws://{0}:{1}/chat", getLocalmachineIPAddress(), ServerPort));

            while (true)
            {
                Socket sc = Listener.Accept();

                if (sc != null)
                {
                    System.Threading.Thread.Sleep(100);
                    SocketConnection socketConn = new SocketConnection();
                    socketConn.ConnectionSocket = sc;
                    socketConn.NewConnection += new NewConnectionEventHandler(socketConn_NewConnection);
                    socketConn.DataReceived += new DataReceivedEventHandler(socketConn_BroadcastMessage);
                    socketConn.Disconnected += new DisconnectedEventHandler(socketConn_Disconnected);

                    socketConn.ConnectionSocket.BeginReceive(socketConn.receivedDataBuffer,
                                                             0, socketConn.receivedDataBuffer.Length, 
                                                             0, new AsyncCallback(socketConn.ManageHandshake), 
                                                             socketConn.ConnectionSocket.Available);
                    connectionSocketList.Add(socketConn);
                }
            }
        }

        void socketConn_Disconnected(Object sender, EventArgs e)
        {
            SocketConnection sConn = sender as SocketConnection;
            if (sConn != null)
            {
                Send(string.Format("【{0}】离开了聊天室！", sConn.Name));
                sConn.ConnectionSocket.Close();
                connectionSocketList.Remove(sConn);
            }
        }

        void socketConn_BroadcastMessage(Object sender, string message, EventArgs e)
        {
            if (message.IndexOf("login:") != -1)
            {
                SocketConnection sConn = sender as SocketConnection;
                sConn.Name = message.Substring(message.IndexOf("login:") + "login:".Length);
                message = string.Format("欢迎【{0}】来到聊天室！",message.Substring(message.IndexOf("login:") + "login:".Length));
            }
            Send(message);
        }

        void socketConn_NewConnection(string name, EventArgs e)
        {
            if (NewConnection != null)
                NewConnection(name,EventArgs.Empty);
        }

        public void Send(string message)
        {
            foreach (SocketConnection item in connectionSocketList)
            {
                if (!item.ConnectionSocket.Connected) return;
                try
                {
                    if (item.IsDataMasked)
                    {
                        DataFrame dr = new DataFrame(message);
                        item.ConnectionSocket.Send(dr.GetBytes());
                    }
                    else
                    {
                        item.ConnectionSocket.Send(FirstByte);
                        item.ConnectionSocket.Send(Encoding.UTF8.GetBytes(message));
                        item.ConnectionSocket.Send(LastByte);
                    }
                }
                catch(Exception ex)
                {
                    logger.Log(ex.Message);
                }
            }
        }
    }
}



