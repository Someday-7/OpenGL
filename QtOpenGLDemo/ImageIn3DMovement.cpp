#include "ImageIn3DMovement.h"

ImageIn3DMovement::ImageIn3DMovement(QWidget *parent) : QOpenGLWidget(parent)
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
    tilt = 90.0;
    spin = 0.0;
    loop = 0;
    twinkle = false;
    m_UpdateTimer = new QTimer;
    m_UpdateTimer->setInterval(50);
    connect(m_UpdateTimer,SIGNAL(timeout()),this,SLOT(slot_Update()));
}

ImageIn3DMovement::~ImageIn3DMovement()
{
    m_UpdateTimer->deleteLater();
    glDeleteTextures(1,&m_Texture[0]);
}
void ImageIn3DMovement::initializeGL()
{
    //添加纹理
    LoadGLTextures();
    glShadeModel( GL_SMOOTH );
    glClearColor( 0.0, 0.0, 0.0, 0.5 );
    glClearDepth( 1.0 );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE );
    glEnable( GL_BLEND );

    /*
     *  设置了每颗星星的起始角度、距离和颜色。您会注意到修改结构的属性有多容易。全部50颗星星都会被循环设置。要改变star[1]的角度我们所要做的只是star[1].angle={某个数值}；就这么简单！

第loop颗星星离中心的距离是将loop的值除以星星的总颗数，然后乘上5.0。基本上这样使得后一颗星星比前一颗星星离中心更远一点。这样当loop为50时（最后一颗星星），loop除以StarCount正好是1.0。之所以要乘以5.0是因为1.0*5.0就是5.0。5.0已经很接近屏幕边缘。我不想星星飞出屏幕，5.0是最好的选择了。当然如果如果您将场景设置的更深入屏幕里面的话，也许可以使用大于5.0的数值，但星星看起来就更小一些（都是透视的缘故）。

您还会注意到每颗星星的颜色都是从0～255之间的一个随机数。也许您会奇怪为何这里的颜色得取值范围不是OpenGL通常的0.0～1.0之间。这里我们使用的颜色设置函数是glColor4ub，而不是以前的glColor4f。ub意味着参数是unsigned byte型的。一个byte的取值范围是0～255。这里使用byte值取随机整数似乎要比取一个浮点的随机数更容易一些。
*/
    for ( loop = 0; loop < StarCount; loop++ )
    {
      star[loop].angle = 0.0;
      star[loop].dist = ( float(loop)/StarCount ) * 5.0;
      star[loop].r = rand() % 256;
      star[loop].g = rand() % 256;
      star[loop].b = rand() % 256;
    }
    m_UpdateTimer->start();
}

//这个函数中包括了所有的绘图代码
void ImageIn3DMovement::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除屏幕和深度缓存
    glBindTexture( GL_TEXTURE_2D, m_Texture[0] );
    for ( loop = 0; loop < StarCount; loop++ )
    {
        glLoadIdentity();//重置当前的模型观察矩阵
        glTranslatef(  0.0,  0.0, m_Zoom );
        glRotatef( tilt,  1.0,  0.0,  0.0 );
        glRotatef( star[loop].angle, 0.0, 1.0, 0.0 );//倾斜视角（使用“tilt”的值）。
        glTranslatef( star[loop].dist, 0.0, 0.0 );
        glRotatef( -star[loop].angle, 0.0, 1.0, 0.0 );
        glRotatef( -tilt, 1.0, 0.0, 0.0 );

        if ( twinkle )
        {
          glColor4ub( star[(StarCount-loop)-1].r,
             star[(StarCount-loop)-1].g,
             star[(StarCount-loop)-1].b, 255 );
          glBegin( GL_QUADS );
            glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0, -1.0, 0.0 );
            glTexCoord2f( 1.0, 0.0 ); glVertex3f( 1.0, -1.0, 0.0 );
            glTexCoord2f( 1.0, 1.0 ); glVertex3f( 1.0, 1.0, 0.0 );
            glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0, 1.0, 0.0 );
          glEnd();
        }
        glRotatef( spin, 0.0, 0.0, 1.0 );
        glColor4ub( star[loop].r, star[loop].g, star[loop].b, 255 );
        glBegin( GL_QUADS );
          glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0, -1.0, 0.0 );
          glTexCoord2f( 1.0, 0.0 ); glVertex3f( 1.0, -1.0, 0.0 );
          glTexCoord2f( 1.0, 1.0 ); glVertex3f( 1.0, 1.0, 0.0 );
          glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0, 1.0, 0.0 );
        glEnd();
        spin += 0.01;
        star[loop].angle += float(loop)/StarCount;
        star[loop].dist -= 0.01;
        if ( star[loop].dist < 0.0 )
        {
          star[loop].dist += 5.0;
          star[loop].r = rand() % 256;
          star[loop].g = rand() % 256;
          star[loop].b = rand() % 256;
        }
    }
}

void ImageIn3DMovement::resizeGL(int w, int h)
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
void ImageIn3DMovement::MyPerspective( GLdouble fov, GLdouble aspectRatio, GLdouble zNear, GLdouble zFar )
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

void ImageIn3DMovement::LoadGLTextures()
{
    QImage image;
    if(!image.load(QCoreApplication::applicationDirPath() + "/images/Star.jpg"))
    {
        qWarning("Load Images Failed!");
    }
    qWarning("Load Images Success!");
    //QImage 坐标系左上角为（0,0） OpenGL坐标系左下角（0,0）
    image = image.mirrored();//OpenGL的坐标系和QImage不一致，

    //数字零代表图像的详细程度，通常就由它为零去了。数字三是数据的成分数。因为图像是由红色数据，绿色数据，蓝色数据三种组分组成。
    //tex.width()是纹理的宽度。tex.height()是纹理的高度。数字零是边框的值，一般就是零。
    //GL_BGRA 告诉OpenGL图像数据由蓝、绿、红三色数据以及alpha通道数据组成，BMP JPG保存图片是BGR排列的
    glGenTextures(1, &m_Texture[0]);

    //创建的第一种纹理使用GL_NEAREST方式。从原理上讲，这种方式没有真正进行滤波。它只占用很小的处理能力，看起来也很差。
    //唯一的好处是这样我们的工程在很快和很慢的机器上都可以正常运行。您会注意到我们在MIN和MAG时都采用了GL_NEAREST,
    //你可以混合使用GL_NEAREST和GL_LINEAR。纹理看起来效果会好些，但我们更关心速度，所以全采用低质量贴图。
    //MIN_FILTER在图像绘制时小于贴图的原始尺寸时采用。MAG_FILTER在图像绘制时大于贴图的原始尺寸时采用。
    glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image.width(), image.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image.bits());

    glEnable(GL_TEXTURE_2D);//启用纹理映射
}

void ImageIn3DMovement::slot_Update()
{
    update();
}

void ImageIn3DMovement::keyPressEvent(QKeyEvent *event)
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
        case Qt::Key_T:
          twinkle = !twinkle;
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
