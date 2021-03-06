﻿#ifndef MYOPENGL_H
#define MYOPENGL_H

#include <QObject>
//#include <QtOpenGL/qgl.h>
#include <QKeyEvent>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QTimer>
#include <QImage>
#include <QCoreApplication>
#define ENABLE_TRIANGLES_3D_MODEL 0
#define ENABLE_TEXTURE_MODEL 1
class MyOpenGL : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit MyOpenGL(QWidget *parent = nullptr);
    ~MyOpenGL();
    void MyPerspective( GLdouble fov, GLdouble aspectRatio, GLdouble zNear, GLdouble zFar );
    void LoadGLTextures();
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
    QTimer* m_UpdateTimer;//刷新
    bool m_FullScreen;//是否全屏
    GLfloat m_VTri;//三角形的移动值
    GLfloat m_VQuad;//矩形的移动值
    GLfloat m_XRot;//处理立方体在三个方向上的旋转
    GLfloat m_YRot;
    GLfloat m_ZRot;
    GLuint m_Texture[1];//存储一个纹理
};


#endif // MYOPENGL_H
