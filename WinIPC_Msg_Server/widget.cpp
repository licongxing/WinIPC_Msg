#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <windows.h>
#include <QTime>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle(QString("WM_COPYDATA服务端"));
}

Widget::~Widget()
{
    delete ui;
}

bool Widget::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG* msg = (MSG*)message;
    switch(msg->message)
    {
    case WM_COPYDATA:
        qDebug() << "收到WM_COPYDATA客户端消息";
        DWORD dwPid = 0;
        GetWindowThreadProcessId((HWND)msg->wParam,&dwPid);
        COPYDATASTRUCT *cds = reinterpret_cast<COPYDATASTRUCT*>(msg->lParam);
        QString text = QString("PID=[%1]的进程发来的消息为：%2").arg((int)dwPid).arg((char*)cds->lpData);
        ui->listWidget->addItem(text);

        //给客户端回一个时间
        QTime time = QTime::currentTime();
        QString str = time.toString("hh:mm:ss");
        COPYDATASTRUCT cds2;
        cds2.dwData = 0;
        cds2.cbData = str.length() + 1;
        cds2.lpData = str.toUtf8().data();

        ::SendMessage((HWND)msg->wParam, WM_COPYDATA, NULL, reinterpret_cast<LPARAM>(&cds2));
        return true;
    }
    //其他交给qt处理
    return QWidget::nativeEvent(eventType, message, result);
}
//清空按钮槽函数
void Widget::on_pushButton_clicked()
{
    ui->listWidget->clear();
}
