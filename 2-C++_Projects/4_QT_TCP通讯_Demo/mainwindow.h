#include "tcpserverwindow.h"
#include "ui_tcpserverwindow.h"
 
TcpServerWindow::TcpServerWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TcpServerWindow)
{
    ui->setupUi(this);
 
    tcpSocket=new QTcpSocket(this);
    tcpServer=new QTcpServer(this);
 
    setListener();
 
     /**信号与槽**/
 
    //newConnection()用于当有客户端访问时发出信号，acceptConnection()信号处理函数
    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
 
    //当tcpSocket在接受客户端连接时出现错误时，displayError(QAbstractSocket::SocketError)进行错误提醒并关闭tcpSocket。
    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),SLOT(displayError(QAbstractSocket::SocketError)));
 
}
 
 
    //监听是否有客户端来访，且对任何来访者监听，端口为6666
void TcpServerWindow::setListener(){
    if(!tcpServer->listen(QHostAddress::Any,6666)){
         qDebug()<<tcpServer->errorString();
        close();
    }else {
        qDebug()<<"listening";
    }
 
}
 
 
void TcpServerWindow::displayError(QAbstractSocket::SocketError)
 
{
 
   qDebug()<<tcpSocket->errorString();
 
   tcpSocket->close();
 
}
 
 
void TcpServerWindow::acceptConnection(){
    tcpSocket=tcpServer->nextPendingConnection();
}
 
 
 
TcpServerWindow::~TcpServerWindow()
{
    delete ui;
}
 
//点击发送按钮发送消息
void TcpServerWindow::on_SendBtn_clicked()
{
    //以utf-8字符集格式发送，支持中文
    tcpSocket->write(ui->SendText->text().toUtf8());
 
    //以拉丁字符集格式发送
//    tcpSocket->write(ui->SendText->text().toLatin1());
}
