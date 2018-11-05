#include "LightingAndKeyBoard.h"
#include <QDebug>

LightingAndKeyBoard::LightingAndKeyBoard(QWidget *parent) : QOpenGLWidget(parent)
{
    m_FullScreen = false;
    setGeometry(0,0,640,480);
    //setCaption("My OpenGL Demo!"); //已经废弃
    setWindowTitle("My OpenGL Demo!");
    m_XRot = 0.0;
    m_YRot = 0.0;
    m_ZRot = 0.0;
    m_Zoom = -5.0;
    m_XSpeed = 0.0;
    m_YSpeed = 0.0;
    m_Filter = 0;
    m_Light = false;
    m_UpdateTimer = new QTimer;
    m_UpdateTimer->setInterval(50);
    connect(m_UpdateTimer,SIGNAL(timeout()),this,SLOT(slot_Update()));
}

LightingAndKeyBoard::~LightingAndKeyBoard()
{
    m_UpdateTimer->deleteLater();
    glDeleteTextures(1,&m_Texture[0]);
}
void LightingAndKeyBoard::initializeGL()
{
    //添加纹理
    LoadGLTextures();

    glShadeModel(GL_SMOOTH);//阴影平滑
    glClearColor(0.0f,0.0f,0.0f,0.5f);//颜色值范围从0到1.0 RGBA
    //深度缓存 将深度缓存设想为屏幕后面的层，深度缓存不断的对物体进入屏幕内部有多深，进行跟踪排序决定那个物体先画
    glClearDepth(1.0);//设置深度缓存
    glEnable(GL_DEPTH_TEST);//启用深度测试
    glDepthFunc(GL_LEQUAL);//所做深度测试的类型
    //真正精细的透视修正。这一行告诉OpenGL我们希望进行最好的透视修正。这会十分轻微的影响性能。但使得透视图看起来好一点
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

    // 这里开始设置光源。第一行设置环境光的发光量，光源GL_LIGHT1开始发光。这一课的开始处我们我们将环境光的发光量存放在lightAmbient数组中。现在我们就使用此数组（半亮度环境光）。
    //接下来我们设置漫射光的发光量。它存放在lightDiffuse数组中（全亮度白光）。
    //然后设置光源的位置。位置存放在lightPosition 数组中（正好位于木箱前面的中心，X－0.0，Y－0.0，Z方向移向观察者2个单位，位于屏幕外面）。
    //最后，我们启用一号光源。我们还没有启用GL_LIGHTING，所以您看不见任何光线。记住：只对光源进行设置、定位、甚至启用，光源都不会工作。除非我们启用GL_LIGHTING。

    //创建光源的过程和颜色的创建完全一致。前三个参数分别是RGB三色分量，最后一个是alpha通道参数
    GLfloat lightAmbient[4] = { 1, 1, 1, 1 };//环境光
    GLfloat lightDiffuse[4] = { 0.5, 0.5, 0.5, 0.5 };//漫反射光
    GLfloat lightPosition[4] = { 0.0, 0.0, 2.0, 1.0 };
    glLightfv( GL_LIGHT1, GL_AMBIENT, lightAmbient );
    glLightfv( GL_LIGHT1, GL_DIFFUSE, lightDiffuse );
    glLightfv( GL_LIGHT1, GL_POSITION, lightPosition );
    glEnable(GL_LIGHT1);
    glEnable( GL_LIGHTING );
    m_UpdateTimer->start();
}

//这个函数中包括了所有的绘图代码
void LightingAndKeyBoard::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除屏幕和深度缓存
    glLoadIdentity();//重置当前的模型观察矩阵
    glTranslatef(  0.0,  0.0, m_Zoom );
    glRotatef( m_XRot,  1.0,  0.0,  0.0 );
    glRotatef( m_YRot,  0.0,  1.0,  0.0 );
    glBindTexture( GL_TEXTURE_2D, m_Texture[m_Filter] );

    glBegin( GL_QUADS );
    glNormal3f( 0.0, 0.0, 1.0 );
    glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );
    glTexCoord2f( 1.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );
    glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0,  1.0 );
    glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0,  1.0,  1.0 );

    glNormal3f( 0.0, 0.0, -1.0 );
    glTexCoord2f( 1.0, 0.0 ); glVertex3f( -1.0, -1.0, -1.0 );
    glTexCoord2f( 1.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );
    glTexCoord2f( 0.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );
    glTexCoord2f( 0.0, 0.0 ); glVertex3f(  1.0, -1.0, -1.0 );

    glNormal3f( 0.0, 1.0, 0.0 );
    glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );
    glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0,  1.0,  1.0 );
    glTexCoord2f( 1.0, 0.0 ); glVertex3f(  1.0,  1.0,  1.0 );
    glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );

    glNormal3f( 0.0, -1.0, 0.0 );
    glTexCoord2f( 1.0, 1.0 ); glVertex3f( -1.0, -1.0, -1.0 );
    glTexCoord2f( 0.0, 1.0 ); glVertex3f(  1.0, -1.0, -1.0 );
    glTexCoord2f( 0.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );
    glTexCoord2f( 1.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );

    glNormal3f( 1.0, 0.0, 0.0 );
    glTexCoord2f( 1.0, 0.0 ); glVertex3f(  1.0, -1.0, -1.0 );
    glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );
    glTexCoord2f( 0.0, 1.0 ); glVertex3f(  1.0,  1.0,  1.0 );
    glTexCoord2f( 0.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );

    glNormal3f( -1.0, 0.0, 0.0 );
    glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0, -1.0, -1.0 );
    glTexCoord2f( 1.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );
    glTexCoord2f( 1.0, 1.0 ); glVertex3f( -1.0,  1.0,  1.0 );
    glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );
    glEnd();

    m_XRot += m_XSpeed;
    m_YRot += m_YSpeed;
}

void LightingAndKeyBoard::resizeGL(int w, int h)
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
void LightingAndKeyBoard::MyPerspective( GLdouble fov, GLdouble aspectRatio, GLdouble zNear, GLdouble zFar )
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

void LightingAndKeyBoard::LoadGLTextures()
{
    QImage image;
    if(!image.load(QCoreApplication::applicationDirPath() + "/images/box.jpg"))
    {
        qWarning("Load Images Failed!");
    }
    qWarning("Load Images Success!");
    //QImage 坐标系左上角为（0,0） OpenGL坐标系左下角（0,0）
    image = image.mirrored();//OpenGL的坐标系和QImage不一致，

    //数字零代表图像的详细程度，通常就由它为零去了。数字三是数据的成分数。因为图像是由红色数据，绿色数据，蓝色数据三种组分组成。
    //tex.width()是纹理的宽度。tex.height()是纹理的高度。数字零是边框的值，一般就是零。
    //GL_BGRA 告诉OpenGL图像数据由蓝、绿、红三色数据以及alpha通道数据组成，BMP JPG保存图片是BGR排列的
    glGenTextures(3, &m_Texture[0]);

    //创建的第一种纹理使用GL_NEAREST方式。从原理上讲，这种方式没有真正进行滤波。它只占用很小的处理能力，看起来也很差。
    //唯一的好处是这样我们的工程在很快和很慢的机器上都可以正常运行。您会注意到我们在MIN和MAG时都采用了GL_NEAREST,
    //你可以混合使用GL_NEAREST和GL_LINEAR。纹理看起来效果会好些，但我们更关心速度，所以全采用低质量贴图。
    //MIN_FILTER在图像绘制时小于贴图的原始尺寸时采用。MAG_FILTER在图像绘制时大于贴图的原始尺寸时采用。
    glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image.width(), image.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image.bits());

    glBindTexture(GL_TEXTURE_2D, m_Texture[1]);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image.width(), image.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image.bits());

    glBindTexture(GL_TEXTURE_2D, m_Texture[2]);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    //当您告诉OpenGL创建一个 mipmapped的纹理后，OpenGL将尝试创建不同尺寸的高质量纹理。当您向屏幕绘制一个mipmapped纹理的时候，
    //OpenGL将选择它已经创建的外观最佳的纹理(带有更多细节)来绘制，而不仅仅是缩放原先的图像（这将导致细节丢失）
    //使用glTexImage2D()时所采用的位图文件分辨率必须为：64×64、128×128、256×256三种格式，如果其他大小则会出现绘制不正常。
    //gluBuild2DMipmaps()支持任意分辨率位图文件。
    gluBuild2DMipmaps( GL_TEXTURE_2D, GL_BGR, image.width(), image.height(), GL_BGRA, GL_UNSIGNED_BYTE, image.bits() );
    glEnable(GL_TEXTURE_2D);//启用纹理映射
}

void LightingAndKeyBoard::slot_Update()
{
    update();
}

void LightingAndKeyBoard::keyPressEvent(QKeyEvent *event)
{
    //qDebug()<<"Key:"<<event->key();
    switch ( event->key() )
    {
        case Qt::Key_L://按下了L键，就可以切换是否打开光源。
          m_Light = !m_Light;
          if ( !m_Light )
          {
            glDisable( GL_LIGHTING );
          }
          else
          {
            glEnable( GL_LIGHTING );
          }
          update();
          break;
        case Qt::Key_F://按下了F键，就可以转换一下所使用的纹理（就是变换了纹理滤波方式的纹理）。
          m_Filter += 1;;
          if ( m_Filter > 2 )
          {
            m_Filter = 0;
          }
          update();
          break;
        case Qt::Key_PageUp://按下了PageUp键，将木箱移向屏幕内部。
          m_Zoom -= 0.2;
          update();
          break;
        case Qt::Key_PageDown://按下了PageDown键，将木箱移向屏幕外部。
          m_Zoom += 0.2;
          update();
          break;
        case Qt::Key_Up://按下了Up方向键，减少xSpeed。
          m_XSpeed -= 0.01;
          update();
          break;
        case Qt::Key_Down://按下了Dowm方向键，增加xSpeed。
          m_XSpeed += 0.01;
          update();
          break;
        case Qt::Key_Right://按下了Right方向键，增加ySpeed。
          m_YSpeed += 0.01;
          update();
          break;
        case Qt::Key_Left://按下了Left方向键，减少ySpeed。
          m_YSpeed -= 0.01;
          update();
          break;
        case Qt::Key_F2:
          m_FullScreen = !m_FullScreen;
          if ( m_FullScreen )
          {
            showFullScreen();
          }
          else
          {
            showNormal();
            setGeometry( 0, 0, 640, 480 );
          }
          update();
          break;
        case Qt::Key_Escape:
          close();
    }

}
