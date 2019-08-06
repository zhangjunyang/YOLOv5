#include "mainwindow.h"
#include "ui_mainwindow.h"
 
 
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
 
    tcpServerWindow=new TcpServerWindow;
    tcpClientWindow=new TcpClientWindow;
 
 
}
 
MainWindow::~MainWindow()
{
    delete ui;
}
 
void MainWindow::on_server_clicked()
{
    tcpServerWindow->show();
 
}
 
void MainWindow::on_client_clicked()
{
    tcpClientWindow->show();
}
