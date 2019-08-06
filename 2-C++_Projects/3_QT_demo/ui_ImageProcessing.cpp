// cpp
#include <QMainWindow>
#include <QtGui>
#include <QtWidgets>
#include "MainWindow.h"
#include "ui_ImageProcessing.h"

#pragma execution_character_set("utf-8")    // 解决汉字乱码问题，注意！！！

ImageProcessing::ImageProcessing(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    this->resize(400, 400);                 // 改变主窗口大小，后续教程中还有其他方式可以设置

    helloWorld();
}

ImageProcessing::~ImageProcessing()
{
}

void ImageProcessing::helloWorld()                   // 函数定义
{
    QPushButton *button = new QPushButton(this);    // QPushButton是QT的按钮类
    button->setText(tr("你好，QT !"));               // 按钮显示的文本
    button->move(100,100);                          // 重设按钮位置
    connect(button, SIGNAL(clicked()), this, SLOT(iSlot()));  // 连接信号与槽
}

void ImageProcessing::iSlot()                       // 槽函数
{
    QMessageBox::information(this, tr("hello"), tr("Hello World!"));
}