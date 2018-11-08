#ifndef IMAGEIN3DMOVEMENT_H
#define IMAGEIN3DMOVEMENT_H
#include <QWidget>
#include <QKeyEvent>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QTimer>
#include <QImage>
#include <QCoreApplication>
#include "glut/include/glut.h"
const GLuint StarCount = 10;
typedef struct
{
  int r, g, b;
  GLfloat dist;
  GLfloat angle;
}stars;
class ImageIn3DMovement : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit ImageIn3DMovement(QWidget *parent = nullptr);
    ~ImageIn3DMovement();
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
    GLuint m_Texture[1];//存储纹理
    bool m_Light;
    bool m_Blend;
    GLfloat tilt;//tilt是星星的倾角，
    GLfloat spin;//是闪烁星星的自转，
    GLuint loop;//是用来绘制所有50个星星的全局变量，
    bool twinkle;//是用来表示星星是否闪烁，
    stars star[StarCount];//是用来存储50个星星的数据。
};

#endif // IMAGEIN3DMOVEMENT_H
