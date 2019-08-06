#ifndef TCPSERVERWINDOW_H
#define TCPSERVERWINDOW_H
 
#include<QtNetwork/QTcpSocket>
#include<QtNetwork/QTcpServer>
#include <QDialog>
 
namespace Ui {
class TcpServerWindow;
}
 
class TcpServerWindow : public QDialog
{
    Q_OBJECT
 
public:
    explicit TcpServerWindow(QWidget *parent = 0);
    ~TcpServerWindow();
 
private:
    Ui::TcpServerWindow *ui;
 
    QTcpSocket *tcpSocket;
    QTcpServer *tcpServer;
 
    void setListener();
 
 
private slots:
 
    void acceptConnection();
 
    void displayError(QAbstractSocket::SocketError);
 
    void on_SendBtn_clicked();
};
 
#endif // TCPSERVERWINDOW_H
