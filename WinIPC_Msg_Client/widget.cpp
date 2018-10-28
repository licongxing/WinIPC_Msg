#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <windows.h>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

//发送按钮 槽函数
void Widget::on_pushButton_clicked()
{
    QTextDocument * tx = ui->textEdit->document();
    QString content = tx->toPlainText();
    QString dest = "WM_COPYDATA服务端";
    char* tmp = dest.toUtf8().data();
    qDebug() <<tmp;

    LPCWSTR title = reinterpret_cast<LPCWSTR>(tmp);
    HWND server = ::FindWindowW(nullptr,title);
    if(!IsWindow(server))
    {
        qDebug() << "没有找到服务端！！！";
        return;
    }else
    {
        qDebug() << "找到窗口："+dest;
    }

    //设置COPYDATASTRUCT结构体
    COPYDATASTRUCT cds;
    cds.dwData = winId();//自定义数据
    cds.cbData = content.length() + 1;//lpData指向数据的长度，如果内容有汉字 貌似长度有问题
    cds.lpData = content.toUtf8().data();
    HWND sender = (HWND)effectiveWinId();//SendMessage 消息发送者可以不用填写

    ::SendMessage(server, WM_COPYDATA, reinterpret_cast<WPARAM>(sender), reinterpret_cast<LPARAM>(&cds));
}

bool Widget::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG* msg = (MSG*)message;
    switch(msg->message)
    {
    case WM_COPYDATA:
        qDebug() << "收到WM_COPYDATA服务端消息";

        COPYDATASTRUCT *cds = reinterpret_cast<COPYDATASTRUCT*>(msg->lParam);
        QString text = QString("服务端在[%1]接受到该消息").arg((char*)cds->lpData);
        ui->listWidget->addItem(text);
        return true;
    }
    //其他交给qt处理
    return QWidget::nativeEvent(eventType, message, result);
}

void Widget::on_pushButton_2_clicked()
{
    ui->listWidget->clear();
}
