#ifndef TCPCLIENTWINDOW_H
#define TCPCLIENTWINDOW_H
 
#include<QtNetwork/QTcpSocket>
#include<QtNetwork>
#include <QDialog>
 
namespace Ui {
class TcpClientWindow;
}
 
class TcpClientWindow : public QDialog
{
    Q_OBJECT
 
public:
    explicit TcpClientWindow(QWidget *parent = 0);
    ~TcpClientWindow();
 
private:
    Ui::TcpClientWindow *ui;
 
    QTcpSocket *tcpSocket;
 
    void newTcpConnection();
 
private slots:
    void displayError(QAbstractSocket::SocketError);
    void revData();
};
 
#endif // TCPCLIENTWINDOW_H
