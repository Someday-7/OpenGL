#ifndef LIGHTINGANDKEYBOARD_H
#define LIGHTINGANDKEYBOARD_H

#include <QWidget>
#include <QKeyEvent>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QTimer>
#include <QImage>
#include <QCoreApplication>
#include "glut/include/glut.h"
class LightingAndKeyBoard : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit LightingAndKeyBoard(QWidget *parent = nullptr);
    ~LightingAndKeyBoard();
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
    GLfloat m_XRot;//处理立方体在三个方向上的旋转
    GLfloat m_YRot;
    GLfloat m_ZRot;
    GLfloat m_Zoom;//场景深入屏幕的距离
    GLfloat m_XSpeed;//立方体在X轴和Y轴上旋转的速度
    GLfloat m_YSpeed;
    GLuint m_Filter;//表明使用哪个纹理
    GLuint m_Texture[3];//存储纹理
    bool m_Light;
};

#endif // LIGHTINGANDKEYBOARD_H
