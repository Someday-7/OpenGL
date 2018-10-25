#ifndef MYOPENGL_H
#define MYOPENGL_H

#include <QObject>
//#include <QtOpenGL/qgl.h>
#include <QKeyEvent>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>
class MyOpenGL : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit MyOpenGL(QWidget *parent = nullptr);
    ~MyOpenGL();
    void MyPerspective( GLdouble fov, GLdouble aspectRatio, GLdouble zNear, GLdouble zFar );
signals:

public slots:
private slots:
    void slot_Update();
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void keyPressEvent(QKeyEvent *event);
private:
    QTimer* m_UpdateTimer;
    bool m_FullScreen;
    GLfloat m_VTri;
    GLfloat m_VQuad;
};


#endif // MYOPENGL_H
