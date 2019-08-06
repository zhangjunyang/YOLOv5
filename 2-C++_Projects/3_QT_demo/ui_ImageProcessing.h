// 头文件
#include <QtWidgets/QMainWindow>

class ImageProcessing : public QMainWindow
{
    Q_OBJECT

public:
    ImageProcessing(QWidget *parent = Q_NULLPTR);
    ~ImageProcessing();

    void helloWorld();     // 函数声明

private:
    Ui::ImageProcessingClass ui;

private slots:             // 槽函数
    void iSlot();
};