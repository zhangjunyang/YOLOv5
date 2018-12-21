package com.jCuckoo.server;
 
import java.io.IOException;
 
import javax.websocket.OnClose;
import javax.websocket.OnMessage;
import javax.websocket.OnOpen;
import javax.websocket.Session;
import javax.websocket.server.ServerEndpoint;
/**
 * 在tomcat7中存在WebSocketServlet类（但已经过时），在tomcat8中彻底删除
 * 此处使用@ServerEndpoint注解，主要是将目前的类定义成一个websocket服务器端
 * 注解的值将被用于监听用户连接的终端访问URL地址
 */
@ServerEndpoint("/websocket")
public class WebSocketTest {
	/**
	 * 当服务器接收到客户端发送的消息时所调用的方法
	 * 该方法可能包含一个javax.websocket.Session可选参数
	 * 如果有这个参数，容器将会把当前发送消息客户端的连接Session注入进去
	 */
	@OnMessage
	public void onMessage(String message,Session session) throws IOException, InterruptedException {
		// 打印从客户端获取到的信息
		System.out.println("从客户端接收到的信息: " + message);
		//向客户端第一次发送信息
		session.getBasicRemote().sendText("=======向客户端第一次发送信息=======");
		//每秒向客户端发送一次信息，连续发送3次
		int sentMessages = 0;
		while (sentMessages < 3) {
			Thread.sleep(1000);
			session.getBasicRemote().sendText("即时发送信息，当前是第 " + sentMessages+"次...");
			sentMessages++;
		}
		// 向客户端发送最后一次信息
		session.getBasicRemote().sendText("=======向客户端发送最后一次信息=======");
	}
	/**
	 * 当一个新用户连接时所调用的方法
	 * 该方法可能包含一个javax.websocket.Session可选参数
	 * 如果有这个参数，容器将会把当前发送消息客户端的连接Session注入进去
	 */
	@OnOpen
	public void onOpen(Session session) {
		System.out.println("客户端连接成功");
	}
	/** 当一个用户断开连接时所调用的方法*/
	@OnClose
	public void onClose() {
		System.out.println("客户端关闭");
	}
}