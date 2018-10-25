#ifndef MYOPENGL_H
#define MYOPENGL_H

#include <QObject>
//#include <QtOpenGL/qgl.h>
#include <QKeyEvent>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
class MyOpenGL : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit MyOpenGL(QWidget *parent = nullptr);
    ~MyOpenGL();
    void MyPerspective( GLdouble fov, GLdouble aspectRatio, GLdouble zNear, GLdouble zFar );
signals:

public slots:
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void keyPressEvent(QKeyEvent *event);
private:
    bool m_FullScreen;
};


#endif // MYOPENGL_H
