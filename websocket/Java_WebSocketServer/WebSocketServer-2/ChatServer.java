@ServerEndpoint(value="/server")
public class ChatServer{

    private static int count = 0;
    @OnOpen
    public void open(){
        System.out.println("有客户端进行连接");
    }

    @OnMessage
    public void message(String message,Session session){
        System.out.println("客户端发送数据 -> "+message);
        session.getBasicRemote().sendText(message);
        count++;
        if(count == 10){
            System.out.println("发送的数据超过了十次，服务器告诉客户端关闭连接");
            count = 0;
            session.getBasicRemote().sendText(message);
        }   
    }

    @OnClose
    public void close()
        System.out.println("有客户端关闭了连接");
    }
}