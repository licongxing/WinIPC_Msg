#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <windows.h>
#define WM_UMSG WM_USER+1

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result);
private slots:
    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
