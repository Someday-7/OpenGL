#include "mainwindow.h"
#include <QApplication>
#include "MyOpenGL.h"
#include "LightingAndKeyBoard.h"
#include "ImageIn3DMovement.h"
#define ENABLE_WHICH_DEMO 2
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
#if (ENABLE_WHICH_DEMO == 1)
    MyOpenGL MyOpenGLObj;
    //一个拥有焦点(focus)的QWidget才可以接受键盘事件。有输入焦点的窗口是活动窗口或活动窗口子窗口或子子窗口等
    //用户移动焦点--->程序将决定被设置focus的Widget的哪一个子Widget获得焦点
    MyOpenGLObj.setFocusPolicy(Qt::StrongFocus);
    w.setCentralWidget(&MyOpenGLObj);
#elif (ENABLE_WHICH_DEMO==2)
    LightingAndKeyBoard LightingAndKeyBoardObj;
    LightingAndKeyBoardObj.setFocusPolicy(Qt::StrongFocus);
    w.setCentralWidget(&LightingAndKeyBoardObj);
#elif (ENABLE_WHICH_DEMO==3)
    ImageIn3DMovement ImageIn3DMovementObj;
    ImageIn3DMovementObj.setFocusPolicy(Qt::StrongFocus);
    w.setCentralWidget(&ImageIn3DMovementObj);
#endif
    w.show();
    return a.exec();
}
