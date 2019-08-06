import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedWriter;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.UUID;

public class Server {

    public static void main_1(String[] args) throws Exception {

        //1.创建服务端ServerSocket对象，并指定端口号
        ServerSocket socket = new ServerSocket("127.0.0.1", 6666);

        //2.无限循环接受客户端发的请求
        while(true){
            Socket s = socket.accept();//阻塞式，即客户端木有请求就一直等待

            //3.将ServerSocket中封装好的输入流包装成字节输入流
            BufferedInputStream bis = new BufferedInputStream(s.getInputStream());

            //4.将客户端上传的文件保存到指定的路径中
            //为防止文件名重复     用UUID随机数+当前上传的毫秒值
            BufferedOutputStream bos = new BufferedOutputStream(new FileOutputStream("D:/"+UUID.randomUUID().toString().replaceAll("-", "")+System.currentTimeMillis()+".txt"));
            //IO流经典4行代码
            byte[] b = new byte[1024];
            int len;
            while((len=bis.read(b))!=-1){
                bos.write(b, 0, len);
            }

            //5.通过客户端连接对象获取字节输出流并包装成字节缓冲输出流
            BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(s.getOutputStream()));
            bw.write("文件上传成功");
            bw.newLine();
            bw.flush();

            //6.关闭资源
            bos.close();
            s.close();
        }
    }
    public static void main(String[] args) throws Exception {
        // 创建服务端，并指定监听端口号
        ServerSocket ss = new ServerSocket("192.168.1.102", 8888);
 
        // 获取客户端
        Socket s = ss.accept();
 
        // 获取客户端Ip
        String ip = s.getInetAddress().getHostAddress();
        System.out.println(ip + "...连接成功！");
 
        // 得到客户端网络流(字节流文件)
        InputStream in = s.getInputStream();
 
        // 创建服务器端的写入流对象
        OutputStream out = new FileOutputStream("F:\\a.txt");
 
        // 定义缓冲区
        byte[] ch = new byte[1024];
        int len = 0;
        while ((len = in.read(ch)) != -1) {
            // 把客户端上传的文件写入到服务器中
            out.write(ch, 0, len);
        }
 
        // 返回给客户端的信息
        OutputStream outSoc = s.getOutputStream();
 
        outSoc.write("上传成功！".getBytes());
 
        out.close();
        s.close();
        ss.close();
 
    }
}