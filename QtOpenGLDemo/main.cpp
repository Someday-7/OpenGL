#include "mainwindow.h"
#include <QApplication>
#include "MyOpenGL.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyOpenGL MyOpenGLObj;
    //一个拥有焦点(focus)的QWidget才可以接受键盘事件。有输入焦点的窗口是活动窗口或活动窗口子窗口或子子窗口等
    //用户移动焦点--->程序将决定被设置focus的Widget的哪一个子Widget获得焦点
    MyOpenGLObj.setFocusPolicy(Qt::StrongFocus);

    MainWindow w;
    w.setCentralWidget(&MyOpenGLObj);
    w.show();

    return a.exec();
}
