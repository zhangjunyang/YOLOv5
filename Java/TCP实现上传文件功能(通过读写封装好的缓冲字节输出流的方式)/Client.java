import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.net.Socket;

public class Client {

    public static void main_1(String[] args) throws Exception {

        //1.创建客户端Socket对象，并指定ip跟端口
        Socket s = new Socket("127.0.0.1",6666);

        //2.从指定的路径中读取要上传的文件上传到服务端
        BufferedInputStream bis = new BufferedInputStream(new FileInputStream("E:/待上传文件.txt"));

        //3.将Socket中封装好的字节输出流包装成缓冲字节输出流
        BufferedOutputStream bos = new BufferedOutputStream(s.getOutputStream());

        //IO流经典4行代码
        byte[] b = new byte[1024];
        int len; // int len = 1024;
        while((len=bis.read(b))!=-1){ // while((bis.read(b, 0, len))!=-1){
            bos.write(b, 0, len);
        }
        // 将文件内容从缓存器上传到服务端，写入文件，清空缓存器。
        // 当我们将数据预存到缓冲池中时，当数据的长度满足缓冲池中的大小后，才会将缓冲池中的数据成块的发送，若数据的长度不满足缓冲池中的大小，
        // 需要继续存入，待数据满足预存大小后再成块的发送。往往在发送文件过程中，文件末尾的数据大小不能满足缓冲池的大小。
        // 最终导致这部分的数据停留在缓冲池无法发送。
        bos.flush();
        //告诉服务端，客户端已经上传完毕
        s.shutdownOutput();

        //4.获取服务端的返回结果
        //将Socket中封装好的字节输入流包装成字节缓冲输入流
        BufferedReader br = new BufferedReader(new InputStreamReader(s.getInputStream()));
        //读取
        String string = br.readLine();
        System.out.println(string);

        //关闭资源
        bis.close();//自己创建的字节缓冲输出流
        s.close();//关闭Socket，其自己封装好的输入输出流也将关闭
    }

    public static void main_2(String[] args) throws Exception {
        // 创建客户端
        Socket s = new Socket("192.168.1.102", 8888);
 
        // 创建要上传的文件对象
        InputStream input = new FileInputStream("F:\\b.txt");
        // 创建写入流
        OutputStream out = s.getOutputStream();
 
        // 自定义缓冲区
        byte[] buf = new byte[1024];
        int len = 0;
        while ((len = input.read(buf)) != -1) {
            out.write(buf, 0, len);
        }
        // 关闭输出流对象
        s.shutdownOutput();
 
        BufferedReader bufs = new BufferedReader(new InputStreamReader(s.getInputStream()));
        System.out.println(new String(bufs.readLine()));
 
        input.close();
        s.close();
 
    }
}
