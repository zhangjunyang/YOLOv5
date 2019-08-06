#include "tcpclientwindow.h"
#include "ui_tcpclientwindow.h"
 
TcpClientWindow::TcpClientWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TcpClientWindow)
{
    ui->setupUi(this);
 
    tcpSocket=new QTcpSocket(this);
 
    newTcpConnection();
 
    /**信号与槽**/
 
    //readyRead()表示服务端发送数据过来即发动信号，接着revData()进行处理。
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(revData()));
 
    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
 
 
}
 
    //接收字符串
void TcpClientWindow::revData(){
    QString data=tcpSocket->readAll();
    ui->ReceiveText->setText(data);
}
 
//新建连接
void TcpClientWindow::newTcpConnection(){
    tcpSocket->abort();
    tcpSocket->connectToHost("127.0.0.1",6666);
 
    }
 
void TcpClientWindow::displayError(QAbstractSocket::SocketError){
    qDebug()<<tcpSocket->errorString();
    tcpSocket->close();
}
 
 
TcpClientWindow::~TcpClientWindow()
{
    delete ui;
}
 
 
 
