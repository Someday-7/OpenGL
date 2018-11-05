#include "MyOpenGL.h"
#include <QDebug>
MyOpenGL::MyOpenGL(QWidget *parent) : QOpenGLWidget(parent)
{
    m_FullScreen = false;
    setGeometry(0,0,640,480);
    //setCaption("My OpenGL Demo!"); //已经废弃
    setWindowTitle("My OpenGL Demo!");
    m_VTri = 0.0;
    m_VQuad = 0.0;
    m_XRot = 0.0;
    m_YRot = 0.0;
    m_ZRot = 0.0;
    m_UpdateTimer = new QTimer;
    m_UpdateTimer->setInterval(50);
    connect(m_UpdateTimer,SIGNAL(timeout()),this,SLOT(slot_Update()));
}

MyOpenGL::~MyOpenGL()
{
    m_UpdateTimer->deleteLater();
    glDeleteTextures(1,&m_Texture[0]);
}

void MyOpenGL::initializeGL()
{
    //添加纹理
    LoadGLTextures();

    glShadeModel(GL_SMOOTH);//阴影平滑
    glClearColor(0.0f,0.0f,0.0f,0.0f);//颜色值范围从0到1.0 RGBA
    //深度缓存 将深度缓存设想为屏幕后面的层，深度缓存不断的对物体进入屏幕内部有多深，进行跟踪排序决定那个物体先画
    glClearDepth(1.0);//设置深度缓存
    glEnable(GL_DEPTH_TEST);//启用深度测试
    glDepthFunc(GL_LEQUAL);//所做深度测试的类型
    //真正精细的透视修正。这一行告诉OpenGL我们希望进行最好的透视修正。这会十分轻微的影响性能。但使得透视图看起来好一点
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    m_UpdateTimer->start();
}

//这个函数中包括了所有的绘图代码
void MyOpenGL::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除屏幕和深度缓存
    glLoadIdentity();//重置当前的模型观察矩阵
#if ENABLE_TRIANGLES_3D_MODEL
    glTranslatef( -1.5,  0.0, -6.0 );//glTranslatef(x, y, z)沿着 X, Y 和 Z 轴移动
    //glRotatef( Angle, Xvector, Yvector, Zvector )
    //负责让对象绕某个轴旋转,Angle 通常是个变量代表对象转过的角度。Xvector，Yvector和Zvector三个参数则共同决定旋转轴的方向。
    //比如( 1, 0, 0 )所描述的矢量经过X坐标轴的1个单位处并且方向向右。( -1, 0, 0 )所描述的矢量经过X坐标轴的1个单位处，但方向向左。
    glRotatef( m_VTri,  0.0,  1.0,  0.0 );
 //------画三角形-椎体--------------
    glBegin( GL_TRIANGLES );//开始绘制三角形。并不会将四点画成一个四边形，而是假定新的三角形开始了

    //1、如果要画第二个三角形的话，可以在这三点之后，再加三行代码(3点)。
    //所有六点代码都应包含在glBegin(GL_TRIANGLES)和glEnd()之间。
    //在他们之间再不会有多余的点出现，也就是说，(GL_TRIANGLES)和glEnd()之间的点都是以三点为一个集合的。
    //这同样适用于四边形。如果您知道实在绘制四边形的话，您必须在第一个四点之后，再加上四点为一个集合的点组。
    //另一方面，多边形可以由任意个顶点，(GL_POLYGON)不在乎glBegin(GL_TRIANGLES)和glEnd()之间有多少行代码。
    //2、这里要注意的是存在两种不同的坐标变换方式，glTranslatef(x, y, z)中的x, y, z是相对与您当前所在点的位移，
    //但glVertex(x,y,z)是相对于glTranslatef(x, y, z)移动后的新原点的位移。
    //因而这里可以认为glTranslate移动的是坐标原点，glVertex中的点是相对最新的坐标原点的坐标值。
    //逆时针
    glColor3f( 1.0, 0.0, 0.0 );//红色
    glVertex3f(  0.0,  1.0,  0.0 );//上顶点
    glColor3f( 0.0, 1.0, 0.0 );
    glVertex3f( -1.0, -1.0,  1.0 );//左下顶点
    glColor3f( 0.0, 0.0, 1.0 );
    glVertex3f(  1.0, -1.0,  1.0 );//右下顶点

    glColor3f( 1.0, 0.0, 0.0 );
    glVertex3f(  0.0,  1.0,  0.0 );
    glColor3f( 0.0, 1.0, 0.0 );
    glVertex3f( -1.0, -1.0,  1.0 );
    glColor3f( 0.0, 0.0, 1.0 );
    glVertex3f(  1.0, -1.0,  1.0 );

    glColor3f( 1.0, 0.0, 0.0 );
    glVertex3f(  0.0,  1.0,  0.0 );//上顶点
    glColor3f( 0.0, 0.0, 1.0 );
    glVertex3f(  1.0, -1.0,  1.0 );
    glColor3f( 0.0, 1.0, 0.0 );
    glVertex3f(  1.0, -1.0, -1.0 );

    glColor3f( 1.0, 0.0, 0.0 );
    glVertex3f(  0.0,  1.0,  0.0 );//上顶点
    glColor3f( 0.0, 1.0, 0.0 );
    glVertex3f(  1.0, -1.0, -1.0 );
    glColor3f( 0.0, 0.0, 1.0 );
    glVertex3f( -1.0, -1.0, -1.0 );

    glColor3f( 1.0, 0.0, 0.0 );
    glVertex3f(  0.0,  1.0,  0.0 );//上顶点
    glColor3f( 0.0, 0.0, 1.0 );
    glVertex3f( -1.0, -1.0, -1.0 );
    glColor3f( 0.0, 1.0, 0.0 );
    glVertex3f( -1.0, -1.0,  1.0 );


    glEnd();//三角形绘制结束
    //设置当前色之后绘制的所有东东都是当前色的
    glColor3f( 0, 0.8, 0.5 );

//------画四边形/立方体--------------
    //在屏幕的左半部分画完三角形后，我们要移到右半部分来画正方形。为此要再次使用glTranslate。
    //这次右移，所以X坐标值为正值。因为前面左移了1.5个单位，这次要先向右移回屏幕中心(1.5个单位)，再向右移动1.5个单位。
    //总共要向右移3.0个单位。
    //所有的四边形都以逆时针次序绘制。就是说先画右上角，然后左上角、左下角、最后右下角
    glLoadIdentity();//重置当前的模型观察矩阵
    glTranslatef(  1.5,  0.0, -7.0 );
    glRotatef( m_VQuad,  1.0,  1.0,  1.0 );

    glBegin( GL_QUADS );

    //立方体的顶面
    glColor3f( 0, 1.0, 0 );
    glVertex3f( 1.0,  1.0,  -1.0 );//右上顶点
    glVertex3f(  -1.0,  1.0,  -1.0 );//左上顶点
    glVertex3f(  -1.0, 1.0,  1.0 );//左下顶点
    glVertex3f( 1.0, 1.0,  1.0 );//右下顶点

    //立方体的底面
    glColor3f( 1.0, 0.5, 0.0 );
    glVertex3f(  1.0, -1.0,  1.0 );
    glVertex3f( -1.0, -1.0,  1.0 );
    glVertex3f( -1.0, -1.0, -1.0 );
    glVertex3f(  1.0, -1.0, -1.0 );

    //立方体的前面
    glColor3f( 1.0, 0.0, 0.0 );
    glVertex3f(  1.0,  1.0,  1.0 );
    glVertex3f( -1.0,  1.0,  1.0 );
    glVertex3f( -1.0, -1.0,  1.0 );
    glVertex3f(  1.0, -1.0,  1.0 );

    //立方体后面
    glColor3f( 1.0, 1.0, 0.0 );
    glVertex3f(  1.0, -1.0, -1.0 );
    glVertex3f( -1.0, -1.0, -1.0 );
    glVertex3f( -1.0,  1.0, -1.0 );
    glVertex3f(  1.0,  1.0, -1.0 );

    //左侧面
    glColor3f( 0.0, 0.0, 1.0 );
    glVertex3f( -1.0,  1.0,  1.0 );
    glVertex3f( -1.0,  1.0, -1.0 );
    glVertex3f( -1.0, -1.0, -1.0 );
    glVertex3f( -1.0, -1.0,  1.0 );

    //右侧面
    glColor3f( 1.0, 0.0, 1.0 );
    glVertex3f(  1.0,  1.0, -1.0 );
    glVertex3f(  1.0,  1.0,  1.0 );
    glVertex3f(  1.0, -1.0,  1.0 );
    glVertex3f(  1.0, -1.0, -1.0 );

    glEnd();

    m_VTri += 10.0;
    m_VQuad -= 2.0;
#endif
#if ENABLE_TEXTURE_MODEL
    glTranslatef(0.0f, 0.0f, -5.0f);                    //移入屏幕5.0单位
    glRotatef(m_XRot, 1.0f, 0.0f, 0.0f);                //绕x轴旋转
    glRotatef(m_YRot, 0.0f, 1.0f, 0.0f);                //绕y轴旋转
    glRotatef(m_ZRot, 0.0f, 0.0f, 1.0f);                //绕z轴旋转

    glBindTexture(GL_TEXTURE_2D, m_Texture[0]);            //选择纹理
    glBegin(GL_QUADS);                                  //开始绘制立方体
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);                  //右上(顶面)
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f);                 //左上(顶面)
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);                  //左下(顶面)
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);                   //右下(顶面)

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);                  //右上(底面)
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);                 //左上(底面)
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);                //左下(底面)
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);                 //右下(底面)

        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);                   //右上(前面)
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);                  //左上(前面)
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);                 //左下(前面)
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);                  //右下(前面)

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);                 //右上(后面)
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);                //左上(后面)
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f);                 //左下(后面)
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);                  //右下(后面)

        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);                  //右上(左面)
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f);                 //左上(左面)
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);                //左下(左面)
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);                 //右下(左面)

        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);                  //右上(右面)
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);                   //左上(右面)
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);                  //左下(右面)
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);                 //右下(右面)
    glEnd();                                            //立方体绘制结束

    m_XRot += 0.6f;                                     //x轴旋转
    m_YRot += 0.4f;                                     //y轴旋转
    m_ZRot += 0.8f;                                     //z轴旋转

#endif
}

void MyOpenGL::resizeGL(int w, int h)
{
    if(h == 0)
    {
        h = 1;
    }
    glViewport(0,0,(GLint)w,(GLint)h);//重置当前视口
    glMatrixMode(GL_PROJECTION);//选择投影矩阵
    glLoadIdentity();//重置投影矩阵
    MyPerspective(45.0,(GLfloat)w/(GLfloat)h,0.1,100.0);//建立透视投影矩阵

    glMatrixMode(GL_MODELVIEW);//选择模型观察矩阵
    glLoadIdentity();//重置模型观察矩阵
}
void MyOpenGL::MyPerspective( GLdouble fov, GLdouble aspectRatio, GLdouble zNear, GLdouble zFar )
{
    // 使用glu库函数，需要添加glu.h头文件
    //gluPerspective( fov, aspectRatio, zNear, zFar );
    // 使用OpenGL函数，但是需要添加math.h头文件
    GLdouble rFov = fov * 3.14159265 / 180.0;
    glFrustum( -zNear * tan( rFov / 2.0 ) * aspectRatio,
               zNear * tan( rFov / 2.0 ) * aspectRatio,
               -zNear * tan( rFov / 2.0 ), zNear * tan( rFov / 2.0 ),
               zNear,
               zFar );
}

void MyOpenGL::LoadGLTextures()
{
    glGenTextures(1, &m_Texture[0]);
    glBindTexture(GL_TEXTURE_2D, m_Texture[0]);

    QImage image;
    if(!image.load(QCoreApplication::applicationDirPath() + "/images/texture_1.bmp"))
    {
        qWarning("Load Images Failed!");
    }
    //QImage 坐标系左上角为（0,0） OpenGL坐标系左下角（0,0）
    image = image.mirrored();//OpenGL的坐标系和QImage不一致，
    //数字零代表图像的详细程度，通常就由它为零去了。数字三是数据的成分数。因为图像是由红色数据，绿色数据，蓝色数据三种组分组成。
    //tex.width()是纹理的宽度。tex.height()是纹理的高度。数字零是边框的值，一般就是零。
    //GL_BGRA 告诉OpenGL图像数据由蓝、绿、红三色数据以及alpha通道数据组成，BMP JPG保存图片是BGR排列的
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image.width(), image.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image.bits());
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glEnable(GL_TEXTURE_2D);//启用纹理映射
}

void MyOpenGL::slot_Update()
{
    update();
}

void MyOpenGL::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<"Key:"<<event->key();
    switch ( event->key() )
    {
        case Qt::Key_F2:
          m_FullScreen = !m_FullScreen;
          if ( m_FullScreen )
          {
            showFullScreen();
          }
          else
          {
            //showNormal();
            showMinimized();
            setGeometry( 0, 0, 640, 480 );

          }
          update();
          break;

        //如果按下了F2键，那么屏幕是否全屏的状态就切换一次。然后再根据需要，显示所要的全屏窗口或者普通窗口。

        case Qt::Key_Escape:
          close();
    }

}
