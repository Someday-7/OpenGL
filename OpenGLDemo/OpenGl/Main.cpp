#include <stdlib.h>
#include <stdio.h>
#include "glew.h"
#include "glut.h"
#include <iostream>
#include <Windows.h>
#include  <direct.h> 
using namespace std;
GLint v, f, p;
float lpos[4] = {1,0.5,1,0};
float a = 0;
GLint time_id;
GLfloat PI = 3.1415926;
void changeSize(int w, int h)
{
	if (h == 0)//防止窗口太矮造成分数过大
		h = 1;
	float ratio = 1.0*w / h;
	glMatrixMode(GL_PROJECTION);//在更改之前重设坐标系
	glLoadIdentity();
	glViewport(0, 0, w, h);//设置视口为整个窗口
	gluPerspective(45, ratio, 1, 1000);//设置正确的透视投影方法
	glMatrixMode(GL_MODELVIEW);
}

void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0.0,0.0, 5.0,
		0.0, 0.0, -1.0,
		0.0f, 1.0f, 0.0f);
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	glRotatef(a, 0, 1, 1);
	glutSolidTeapot(1);
	a += 0.1;
	glUniform1f(time_id, a);
	glutSwapBuffers();
}

static const GLchar* ReadShader(const char* filename)
{
#ifdef WIN32
	FILE* infile;
	fopen_s(&infile, filename, "rb");
#else
	FILE* infile = fopen(filename, "rb");
#endif // WIN32

	if (!infile) {
#ifdef _DEBUG
		std::cerr << "Unable to open file '" << filename << "'" << std::endl;
#endif /* DEBUG */
		return NULL;
	}

	fseek(infile, 0, SEEK_END);
	int len = ftell(infile);
	fseek(infile, 0, SEEK_SET);

	GLchar* source = new GLchar[len + 1];


	fread(source, 1, len, infile);
	fclose(infile);

	source[len] = 0;
	
	return const_cast<const GLchar*>(source);
}
void setShaders()
{
	const char *vs = NULL, *fs = NULL;
	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);
	char pathV[500] = { '0' };
	char pathF[500] = { '0' };

	getcwd(pathV, 1000);
	getcwd(pathF, 1000);
	
	strcat(pathV, "\\passthrough.vert");
	strcat(pathF, "\\uniform.frag");
	vs = ReadShader(pathV);
	fs = ReadShader(pathF);

	cout << "path: " << pathV << endl;
	cout << "vs:" << endl << vs << endl;

	cout << "path: " << pathF <<endl;
	cout << "fs:" << endl << fs << endl;

	glShaderSource(v, 1, &vs, NULL);
	glShaderSource(f, 1, &fs, NULL);



	glCompileShader(v);
	glCompileShader(f);

	p = glCreateProgram();
	cout << "glCreateProgram : " << p<<endl;

	glAttachShader(p, v);
	glAttachShader(p, f);
	glLinkProgram(p);
	glUseProgram(p);

	time_id = glGetUniformLocation(p, "v_time");

	free(const_cast<char *>(vs));
	free(const_cast<char *>(fs));
}



int main(int argc, char** argv) 
{
	glutInit(&argc, argv);
	glutCreateWindow("OpengGL Demo");
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(400, 400);
#if 1
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0,0.0,0.0,1.0);
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_CULL_FACE);

	glewInit();
	setShaders();
#endif
	glClear(GL_COLOR_BUFFER_BIT);
	//用这个方法将背景换乘绿色  
	//glColor3f(0.0, 0.0, 1.0);
#if 0
	glBegin(GL_POLYGON);
	glVertex2f(-1,-1);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2f(-1, 1);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2f(1, 1);
	glColor3f(0.0, 0.0, 1.0);
	glVertex2f(1, -1);
	glColor3f(1.0, 1.0, 1.0);
	glEnd();
#endif
#if  0 //圆
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		glColor3f(1.0, 0.0, 0.0);
		glVertex2f(cos(2*PI/100*i), sin(2*PI/100*i));
	}
	glEnd();
#endif


#if 0//方块
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(10);
	glBegin(GL_POINTS);
	glVertex2f(0.5, 0.5);
	//glVertex2i(1.0,1.0);  
	glEnd();
#endif
#if 1//三维方块
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-2, -1, 0);
	glTexCoord2f(0, 1.0);
	glVertex3f(-2, 1, 0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0, 1, 0);
	glTexCoord2f(1.0, 0);
	glVertex3f(0, -1, 0);
	
	glEnd();
#endif
	glFlush();

	glutMainLoop();

	return 0;
}
