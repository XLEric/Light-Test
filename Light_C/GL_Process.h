#pragma once
#ifndef __GL_PROCESS__
#define __GL_PROCESS__


#include <cv.h>
#include <opencv2/opencv.hpp>
#include <highgui.h>
#include <stdio.h>
#include <GL/glut.h>
#include <vector>
#include <glut.h>
#include <GLAUX.h>

#include <iostream>
#include <cxcore.h>
#include <windows.h>
#include <wchar.h>
#include "GLB_Math.h"
#include "Param_GL.h"

using namespace cv;
using namespace std;

#define ABS(x)((x)>0?(x):-(x))

const int ESC = 27;
//
void reshape (int w, int h);

CvFont font;  
char buf_T[256];

int angle_GL=0;
bool flag_stop=0;

GLfloat eyex=0,eyey=130,eyez=320;
float Step_X1=0,Step_X2=0;
bool flag_StepX=0;
float Angle_Y=0,Angle_X=0;

bool Flag_X=0,Flag_Y=1;

float pos_x=-7,pos_y=51,pos_z=122;

int gxr_Global=0;
int gyr_Global=0;
int gzr_Global=0;

bool GL_Flag_Stop=0;
bool Best_Flag=0;//һ֡��ʾһ��������ϱ�־

//-------------------------------------------------------------------------------
float t1_Global=190,t2_Global=190,t3_Global=190,t4_Global=190,t5_Global=190;

float tN_GlobalS_4A[6][4];//6�� 4�㽨ģ �����Ĵ���
float tN_GlobalS_4B[6][4];//6�� 4�㽨ģ �����Ĵ���
float Energy_GlobalA[6];
float Energy_GlobalB[6];
float tStep_nn_G[6];

IplImage *frame_GL=cvCreateImage(cvSize(800,400),IPL_DEPTH_8U, 3);

int Step_Recursion=0;
int Cnt_Correct=0;
int Cnt_Err=0;
int Cnt_Correct3=0;
int Cnt_Err3=0;
int Cnt_Correct12=0;
int Cnt_Err12=0;

 float DistsX_min[10];
 float DistsY_min[10];

 bool New_FpsX[10],New_FpsY[10];
 int Fps_World=0;
//------------------------------------------------------------------ OpenGL
/**********************************************************/
void LastData_Init()
{
	FILE *pFile;
	pFile = fopen(".//LastData.txt","r+");

	while(!feof(pFile))
	{
	 
	 fscanf(pFile,"(%f,%f,%f),(%f,%f,%f,%f),(%f,%f,%f),(%d,%d),(%d,%d),(%d,%d)\n",
		 &pos_x,&pos_y,&pos_z,&q0,&q1,&q2,&q3,&t1_Global,&t2_Global,&t3_Global,
		 &Cnt_Correct,&Cnt_Err,&Cnt_Correct3,&Cnt_Err3,&Cnt_Correct12,&Cnt_Err12
		 );

	}
	fclose(pFile);

}
/**********************************************************/
void init(void) 
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_FLAT);
}
/**********************************************************/
void glutKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case ESC:flag_stop=1;exit(0);break;
	case'q':eyex+=6;break;
	case'Q':eyex-=6;break;
	case'w':eyey+=6;break;
	case'W':eyey-=6;break;
	case'e':eyez+=6;break;
	case'E':eyez-=6;break;

	case'x':pos_x-=1;break;
	case'X':pos_x+=1;break;
	case'y':pos_y-=1;break;
	case'Y':pos_y+=1;break;
	case'z':pos_z-=1;break;
	case'Z':pos_z+=1;break;

	case'a':gxr_Global=3;break;
	case'A':gxr_Global=-3;break;
	case's':gyr_Global=3;break;
	case'S':gyr_Global=-3;break;
	case'd':gzr_Global=3;break;
	case'D':gzr_Global=-3;break;

	case'p':
	case'P':GL_Flag_Stop=1;break;
	case'o':
	case'O':GL_Flag_Stop=0;break;

	case'r':
	case'R':
		eyex=0;eyey=130;eyez=320;
		pos_x=0;pos_y=28;pos_z=150;
		q0=1;q1=0;q2=0;q3=0;Ww=0;
		rot_gl_look=0;
		break;

	case'b':
	case'B':rot_gl_look+=1;break;
	
	}

	//printf("Move:(%f,%f,%f)\n",pos_x,pos_y,pos_z);
}
/**********************************************************/
void glutMouse(int button, int state, int x, int y)
{
	/*if(state == GLUT_DOWN)
	{
		mOldX = x;
		mOldY = y;
		switch(button)
		{
		case GLUT_LEFT_BUTTON:
			if (glutGetModifiers() == GLUT_ACTIVE_CTRL)
			{
				mButton = BUTTON_LEFT_TRANSLATE;
				break;
			} else
			{
				mButton = BUTTON_LEFT;
				break;
			}
		case GLUT_RIGHT_BUTTON:
			mButton = BUTTON_RIGHT;
			break;
		}
	} else if (state == GLUT_UP)
		mButton = -1;*/
}
/**********************************************************/
void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(55.0, (GLfloat) w/(GLfloat) h, 1.0, 1550.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt (eyex,eyey,eyez, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glRotatef(rot_gl_look,0,1,0);
}
/**********************************************************/
void renderCube(float size)
{
	glBegin(GL_QUADS);
	// Front Face
	glNormal3f( 0.0f, 0.0f, 1.0f);
	glVertex3f( 0.0f,  0.0f,  0.0f);
	glVertex3f( size,  0.0f,  0.0f);
	glVertex3f( size,  size,  0.0f);
	glVertex3f( 0.0f,  size,  0.0f);
	// Back Face
	glNormal3f( 0.0f, 0.0f,-1.0f);
	glVertex3f( 0.0f,  0.0f, size);
	glVertex3f( 0.0f,  size, size);
	glVertex3f( size,  size, size);
	glVertex3f( size,  0.0f, size);		
	// Top Face
	glNormal3f( 0.0f, 1.0f, 0.0f);
	glVertex3f( 0.0f,  size,  0.0f);
	glVertex3f( size,  size,  0.0f);
	glVertex3f( size,  size, size);
	glVertex3f( 0.0f,  size, size);
	// Bottom Face
	glNormal3f( 0.0f,-1.0f, 0.0f);
	glVertex3f( 0.0f,  0.0f,  0.0f);
	glVertex3f( 0.0f,  0.0f, size);
	glVertex3f( size,  0.0f, size);
	glVertex3f( size,  0.0f,  0.0f);
	// Right face
	glNormal3f( 1.0f, 0.0f, 0.0f);
	glVertex3f( size,  0.0f, 0.0f);
	glVertex3f( size,  0.0f, size);
	glVertex3f( size,  size, size);
	glVertex3f( size,  size, 0.0f);
	// Left Face
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f( 0.0f,  0.0f, 0.0f);
	glVertex3f( 0.0f,  size, 0.0f);
	glVertex3f( 0.0f,  size, size);
	glVertex3f( 0.0f,  0.0f, size);
	glEnd();
}
/**********************************************************/
//���ر߳��� 45*4 ��λ
void GL_Draw_Filed()
{
	glPushMatrix();
	glLineWidth(1); 
	glTranslatef(0,0,0);

	glRotatef(angle_GL,0,1,0);

	glColor3f(1.5f, 0.2f, 0.2f); 

	glutWireSphere(5, 15, 15);
	glPopMatrix();
	GLUquadricObj *cylinder_obj;
	cylinder_obj=gluNewQuadric();

	float Edge=45*2;
	float Edge2=2*Edge;
	glPushMatrix();
	glTranslatef(Edge,0,0);
	glRotatef(-90,1,0,0);
	glColor3f(0.5f, 0.2f, 1.0f); 
	gluCylinder(cylinder_obj, 3, 3, 61, 20, 30);   
	glPopMatrix();

	glPushMatrix();//---- 1
	glLineWidth(1); 
	glTranslatef(Edge,61,0);

	glRotatef(angle_GL,0,1,0);

	glColor3f(0.0f, 1.0f, 0.0f); 

	glutWireSphere(6, 15, 15);
	glPopMatrix();

	//-------------------------------------
	glPushMatrix();
	glTranslatef(-Edge,0,0);
	glRotatef(-90,1,0,0);
	glColor3f(0.5f, 0.2f, 1.0f); 
	gluCylinder(cylinder_obj, 3, 3, 61, 20, 30);   
	glPopMatrix();

	glPushMatrix();//---- 2
	glLineWidth(1); 
	glTranslatef(-Edge,61,0);

	glRotatef(angle_GL,0,1,0);

	glColor3f(1.0f, 0.0f, 0.0f); 

	glutWireSphere(6, 15, 15);
	glPopMatrix();
	//-------------------------------
	glPushMatrix();
	glTranslatef(Edge,0,Edge2);
	glRotatef(-90,1,0,0);
	glColor3f(0.5f, 0.2f, 1.0f); 
	gluCylinder(cylinder_obj, 3, 3, 61, 20, 30);   
	glPopMatrix();

	glPushMatrix();
	glLineWidth(1); 
	glTranslatef(Edge,61,Edge2);//---- 3

	glRotatef(angle_GL,0,1,0);

	glColor3f(0.0f, 0.0f, 1.0f); 

	glutWireSphere(6, 15, 15);
	glPopMatrix();

	//-------------------------------------
	glPushMatrix();
	glTranslatef(-Edge,0,Edge2);
	glRotatef(-90,1,0,0);
	glColor3f(0.5f, 0.2f, 1.0f); 
	gluCylinder(cylinder_obj, 3, 3, 61, 20, 30);   
	glPopMatrix();

	glPushMatrix();//---- 4
	glLineWidth(1); 
	glTranslatef(-Edge,61,Edge2);

	glRotatef(angle_GL,0,1,0);

	glColor3f(1.0f, 1.0f, 0.0f); 

	glutWireSphere(6, 15, 15);
	glPopMatrix();


	//-------------------------------- Filed Lines

	glLineWidth(5); 
	glBegin(GL_LINES);//���ƾ��й켣
	glColor3f(0.85f, 0.6f, 0.0f); 
	glVertex3f(Edge,0,0);
	glVertex3f(-Edge,0,0);
	glVertex3f(-Edge,0,0);
	glVertex3f(-Edge,0,Edge2);
	glVertex3f(-Edge,0,Edge2);
	glVertex3f(Edge,0,Edge2);
	glVertex3f(Edge,0,Edge2);
	glVertex3f(Edge,0,0);
	glEnd();

	//------ Light A1 ����Moter�����˶�
	glPushMatrix();
	glLineWidth(1); 
	glTranslatef(Edge_HW_x,Edge_HW_y,Edge_HW_z);
	glRotatef(angle_GL,0,1,0);
	glColor3f(0.0f, 1.0f, 1.0f); 
	glutSolidSphere(1.5, 15, 15);
	glPopMatrix();

	//------ Light A2 ����Motor�����˶�
	glPushMatrix();
	glLineWidth(1); 
	glTranslatef(Edge_ZW_x,Edge_ZW_y,Edge_ZW_z);
	glRotatef(angle_GL,0,1,0);
	glColor3f(1.0f, 0.0f, 0.0f); 
	glutSolidSphere(1.5, 15, 15);
	glPopMatrix();

	//------ Light B1
	glPushMatrix();
	glLineWidth(1); 
	glTranslatef(Edge,81,Edge2);
	glRotatef(angle_GL,0,1,0);
	glColor3f(1.0f, 0.0f, 1.0f); 
	glutSolidSphere(6, 15, 15);
	glPopMatrix();
}
/******************************XY��ɨ��****************************/
//ɨ�貽�� ��Step_Scan
void GL_Scan(float Step_Scan)
{
	float Edge=45*2;
	float a1=2*Edge;
	float b1=0;
	float c1=2*Edge;

	float x1,y1,z1;
	//------ X Scan
	//(x-x0)/a1=(z-z0)/c1=t
#if 1
if(Flag_X==0 )
{
	if(!GL_Flag_Stop)
	Angle_X+=Step_Scan;

	float Rr=100;
	x1=Edge_ZW_x;
	y1=Edge_ZW_y-cos(float(Angle_X)*CV_PI/180)*Rr;
	z1=Edge_ZW_z+sin(float(Angle_X)*CV_PI/180)*Rr;

	if(Angle_X>90.0f)
	{
		Flag_X=1;
		Flag_Y=0;
	}
	glPushMatrix();
	glTranslatef(x1,y1,z1);
	glRotatef(angle_GL,0,1,0);
	glColor3f(1.0f, 0.0f, 0.0f); 
	glutSolidSphere(3, 15, 15);
	glPopMatrix();

	glLineWidth(2); 
	glBegin(GL_LINES);
	glColor3f(0.15f, 0.9f, 0.0f); 
	glVertex3f(Edge_ZW_x,Edge_ZW_y,Edge_ZW_z);
	glVertex3f(x1,y1,z1);

	glEnd();

	//------
	//˳ʱ��
	float Edge2=Edge;
	float x2=Edge_ZW_x+Edge2*2;
	float y2=y1;
	float z2=z1;

	glPushMatrix();
	glTranslatef(x2,y2,z2);
	glRotatef(angle_GL,0,1,0);
	glColor3f(0.0f, 1.0f, 0.0f); 
	glutSolidSphere(3, 15, 15);
	glPopMatrix();
	////------
	float x3=Edge_ZW_x+Edge2*2;
	float y3=Edge_ZW_y;
	float z3=Edge_ZW_z;



	glPushMatrix();
	glTranslatef(x3,y3,z3);
	glRotatef(angle_GL,0,1,0);
	glColor3f(1.0f, 1.0f, 0.0f); 
	glutSolidSphere(3, 15, 15);
	glPopMatrix();

	glLineWidth(2); 
	glBegin(GL_LINES);
	glColor3f(0.05f, 0.6f, 0.9f); 
	glVertex3f(Edge_ZW_x,Edge_ZW_y,Edge_ZW_z);
	glVertex3f(x1,y1,z1);

	glVertex3f(x1,y1,z1);
	glVertex3f(x2,y2,z2);

	glVertex3f(x2,y2,z2);
	glVertex3f(x3,y3,z3);

	glVertex3f(x3,y3,z3);
	glVertex3f(Edge_ZW_x,Edge_ZW_y,Edge_ZW_z);
	glEnd();

	//------
	//ƽ�������
	float ax=-Edge*2;
	float ay=0;
	float az=0;
//
	float bx=x1-Edge_ZW_x;
	float by=y1-Edge_ZW_y;
	float bz=z1-Edge_ZW_z;
	//���ƽ���� ������
	Plan_XNorml=GLB_CHAJI(ax, ay, az, bx, by, bz);
	GL_PlanX.A=Plan_XNorml.x;
	GL_PlanX.B=Plan_XNorml.y;
	GL_PlanX.C=Plan_XNorml.z;

	//AX+BY+CZ+D=0 => D=-AX-BY-CZ
	//GL_PlanX.D=-(GL_PlanX.A*x1+GL_PlanX.B*y1+GL_PlanX.C*z1);
	GL_PlanX.D=-(GL_PlanX.A*Edge_ZW_x+GL_PlanX.B*Edge_ZW_y+GL_PlanX.C*Edge_ZW_z);

	glLineWidth(3); //���ƴ���
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.2f, 0.9f); 
	glVertex3f(0,y1,z1);
	glVertex3f(GL_PlanX.A*20,y1+GL_PlanX.B*20,z1+GL_PlanX.C*20);
	glEnd();
}
#endif
	//------ Y Scan
	if(Flag_Y==0  )
	{
		float Rr=Edge*2*1.414;
		float x2=cos(float(90.0f-Angle_Y)*CV_PI/180)*Rr-Edge;
		float y2=0;
		float z2=sin(float(90.0f-Angle_Y)*CV_PI/180)*Rr;
		if(!GL_Flag_Stop)
		Angle_Y+=Step_Scan;
		if(Angle_Y>90)
		{
			Angle_Y=0;
			Angle_X=0;
			Flag_X=0;
			Flag_Y=1;

			int num=10;
			//��ʼ�� Ŀ����Xɨ���� �� Yɨ���� ��־
			for(int i=0;i<num;i++)
			{
				EnableX[i]=0;
				EnableY[i]=0;
				DistsY_min[i]=60000;
				DistsX_min[i]=60000;

			}
			Best_Flag=0;
			Fps_World++;
			//------
			int xr=rand()%21-10;
			int yr=rand()%13-6;
			int zr=rand()%21-10;

			if(abs(pos_x+xr)<80)
			{
				pos_x=pos_x+xr;
			}

			if(abs(pos_y+yr)>8 && abs(pos_y+yr)<81-8 )
			{
				pos_y=pos_y+yr;
			}

			if((pos_z+zr)<170 && (pos_z+zr)>10 )
			{
				pos_z=pos_z+zr;
			}	

			//-------
			//------
			int gxr=rand()%20-10;
			int gyr=rand()%20-10;
			int gzr=rand()%10-5;

			gxr_Global=gxr;
			gyr_Global=gyr;
			gzr_Global=gzr;

				
		}

		glPushMatrix();
		glTranslatef(x2,y2,z2);
		glRotatef(angle_GL,0,1,0);
		glColor3f(0.0f, 1.0f, 0.0f); 
		glutSolidSphere(3, 15, 15);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(x2,y2+81,z2);
		glRotatef(angle_GL,0,1,0);
		glColor3f(0.0f, 1.0f, 0.0f); 
		glutSolidSphere(3, 15, 15);
		glPopMatrix();

		glBegin(GL_LINES);//���ƾ��й켣
		glColor3f(0.01f, 1.0f, 1.0f); 
		glVertex3f(-Edge,0,0);
		glVertex3f(x2,y2,z2);
		glVertex3f(Edge_HW_x,Edge_HW_y,Edge_ZW_z);
		glVertex3f(x2,y2+81,z2);

		glEnd();

		glBegin(GL_LINES);
		glColor3f(1.1f, 0.0f, 0.0f); 

		glVertex3f(x2,y2,z2);
		glVertex3f(x2,y2+81,z2);

		glEnd();

		//ƽ�������
		float ax=-Edge-x2;
		float ay=81-y2;
		float az=0-z2;

		float bx=0;
		float by=81;
		float bz=0;
		Plan_YNorml=GLB_CHAJI(ax, ay, az, bx, by, bz);

		GL_PlanY.A=Plan_YNorml.x;
		GL_PlanY.B=Plan_YNorml.y;
		GL_PlanY.C=Plan_YNorml.z;

		//AX+BY+CZ+D=0 => D=-AX-BY-CZ
		//GL_PlanY.D=-(GL_PlanY.A*x2+GL_PlanY.B*y2+GL_PlanY.C*z2);
		GL_PlanY.D=-(GL_PlanY.A*Edge_HW_x+GL_PlanY.B*Edge_HW_y+GL_PlanY.C*Edge_HW_z);

		glLineWidth(3); //���ƴ���
		glBegin(GL_LINES);
		glColor3f(0.0f, 1.0f, 1.0f); 
		glVertex3f(x2,y2+81.0f/2,z2);
		glVertex3f(x2+GL_PlanY.A*20,y2+81.0F/2+GL_PlanY.B*20,z2+GL_PlanY.C*20);
		glEnd();
	}

}
/****************************��Ԫ����ת��ȡͷ�ԣ�����������㡣******************************/
// PtB: ��ȡ��Դ�����λ�ã����ڽ�ģ�ݹ�ʱʹ�á�
GL_Point GL_Quater_Point(GL_Quater Qt0,GL_Point &PtB,float x_offset,float y_offset,float z_offset,float r,float g,float b)
{
	GL_Point Pt;
	
	GL_Quater Qt0_N=Conjugate_Q(Qt0); //------ ���ڵ���Ԫ�� ����

	GL_Quater Q_Point;
	Q_Point.q0=0;
	Q_Point.q1=x_offset;
	Q_Point.q2=y_offset;
	Q_Point.q3=z_offset;//���ڵ�Ǽܳ���

	PtB.x=x_offset;// PtB��ȡ��Դ�����λ�ã����ڽ�ģ�ݹ�ʱʹ�á�
	PtB.y=y_offset;
	PtB.z=z_offset;

	//��Ԫ����ֲ�����ϵ���¼��� Loc_New_Point=Q*Loc_Old_Point*Q_N
	//------����1�����
	GL_Quater Q011=MUL_Q(Qt0,Q_Point);
	//------����2�����
	GL_Quater Q022=MUL_Q(Q011,Qt0_N);

	glPushMatrix();//���浱ǰ��ͼ����
	glLineWidth(1); 
	glColor3f(r,g,b); 
	glTranslatef(pos_x+Q022.q1,pos_y+Q022.q2,pos_z+Q022.q3);
	glutSolidSphere(0.6, 20, 20);
	glPopMatrix();//�����ϴα����λ��

	Pt.x=pos_x+Q022.q1;
	Pt.y=pos_y+Q022.q2;
	Pt.z=pos_z+Q022.q3;

	return Pt;
}
/**********************************************************/
// Flag_XY :XYƽ�棬0��δ�ڸ�ƽ�棬1���ڸ�ƽ���ϡ�
bool GL_Plan_Dis(GL_Point Pt,GL_Plan &GL_PlanS,GL_Plan Plan,bool &Flag_XY,float &dis_minxy)
{
	bool flag=0;
	float A=Plan.A;
	float B=Plan.B;
	float C=Plan.C;
	float D=Plan.D;

	float x=Pt.x;
	float y=Pt.y;
	float z=Pt.z;
	if((A*A+B*B+C*C)!=0)
	{
		float dist=ABS(A*x+B*y+C*z+D)/sqrt(A*A+B*B+C*C);
		if(dist<4.0f)
		{
			glPushMatrix();//���浱ǰ��ͼ����
			glLineWidth(1); 
			glColor3f(0.8,0.8,0.8); 
			glTranslatef(x,y,z);
			glutSolidSphere(0.8, 20, 20);
			glPopMatrix();//�����ϴα����λ��

			//------������� ����ƽ��Ҫ������Ϊ ����ƽ����
			if(dis_minxy>dist)
			{
				GL_PlanS.A=Plan.A;
				GL_PlanS.B=Plan.B;
				GL_PlanS.C=Plan.C;
				GL_PlanS.D=Plan.D;
				dis_minxy=dist;
				flag=1;
				Flag_XY=1;
			}
			
			
		}
	}
	

	return flag;
}
/**********************************************************/
void GL_Head_Display()
{
	//���� ��ת������
	glPushMatrix();
	glTranslatef(pos_x,pos_y,pos_z);         
	glRotatef(Ww,q1,q2,q3);
	glColor3f(0.8f, 0.2f, 0.0f );
	glScaled(10,5,2);
	glutSolidCube(1);
	glPopMatrix(); 

	glPushMatrix();
	glTranslatef(pos_x,pos_y,pos_z);         
	glRotatef(Ww,q1,q2,q3);
	glColor3f(0.0f, 1.0f, 0.0f );
	glScaled(6,2.5,2);
	glutSolidCube(1);
	glPopMatrix();

	//------
	
}
/**********************************************************/
float GL_Distance(float x1,float y1,float z1,float x2,float y2,float z2)
{
	float dis=0;
	dis=(x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2);
	dis=sqrt(dis);
	return dis;
}
/**********************************************************/
void GL_GotHead_DisPlay_Pt()
{
	GL_Quater Qt0;
	Qt0.q0=q0;
	Qt0.q1=q1;
	Qt0.q2=q2;
	Qt0.q3=q3;
	
	
	HeadPlay_Pt[0]=GL_Quater_Point(Qt0, HeadPlay_PtB[0], 0,0,0, 1.0,0.1,1.0);
	
	HeadPlay_Pt[1]=GL_Quater_Point(Qt0, HeadPlay_PtB[1], 5,0,0, 1,0,0);
	HeadPlay_Pt[2]=GL_Quater_Point(Qt0, HeadPlay_PtB[2], -5,0,0, 1,0,0);
	HeadPlay_Pt[3]=GL_Quater_Point(Qt0, HeadPlay_PtB[3], 0,-4,0, 0,0,1);
	HeadPlay_Pt[4]=GL_Quater_Point(Qt0, HeadPlay_PtB[4], 0,4,0, 0,0,1);

	//
	HeadPlay_Pt[5]=GL_Quater_Point(Qt0, HeadPlay_PtB[5], 4,4,0, 0,0.8,0);
	HeadPlay_Pt[6]=GL_Quater_Point(Qt0, HeadPlay_PtB[6], -4,4,0, 0,0.8,0);
	HeadPlay_Pt[7]=GL_Quater_Point(Qt0, HeadPlay_PtB[7], 4,-4,0, 0.9,0.8,0);
	HeadPlay_Pt[8]=GL_Quater_Point(Qt0, HeadPlay_PtB[8], -4,-4,0, 0.9,0.8,0);

	//(0,0,z)��
	HeadPlay_Pt[9]=GL_Quater_Point(Qt0, HeadPlay_PtB[9], 0,0,-4, 0,0,0);

	int num=10;
	/*for(int i=0;i<num;i++)
	{
		New_FpsX[i]=0;
		New_FpsY[i]=0;
	}*/
	for(int i=0;i<num;i++)
	{
		bool flagx=GL_Plan_Dis(HeadPlay_Pt[i],GL_PlanXS[i],GL_PlanX,EnableX[i],DistsX_min[i]);
		bool flagy=GL_Plan_Dis(HeadPlay_Pt[i],GL_PlanYS[i],GL_PlanY,EnableY[i],DistsY_min[i]);
		/*if(flagx)
		{
		New_FpsX[i]=1;
		}
		if(flagy)
		{
		New_FpsY[i]=1;
		}*/
	}


	/*FILE *file=fopen(".//log.txt","a+");
	for(int i=0;i<num;i++)
	{
		if(EnableX[i] && EnableY[i])
		{
			fprintf(file,"XY:%d %d:(%f,%f,%f) (%f,%f,%f)\n",EnableX[i],EnableY[i],
				GL_PlanXS[i].A,GL_PlanXS[i].B,GL_PlanXS[i].C,
				GL_PlanYS[i].A,GL_PlanYS[i].B,GL_PlanYS[i].C);
		}
	}
	fclose(file);*/
}
/**********************************************************/
// V ��Ϊɨ������ ����
// (m1,m2,m3):Ϊɨ����ԭ�� ����
//  CV_PointN:Ϊ��άӳ���
void CV_Cross(IplImage *frame,GL_Line V,GL_Point &CV_PointN,int num)
{
	cvCircle(frame,cvPoint(20,20),8,CV_RGB(0,0,255),3);

	float vp1=0;
	float vp2=0;
	float vp3=1;

	float v1=V.x;
	float v2=V.y;
	float v3=V.z;

	float n1=0;
	float n2=0;
	float n3=180;

	float u1=(n1-V.pt0.x)*vp1+(n2-V.pt0.y)*vp2+(n3-V.pt0.z)*vp3;
	float u2=vp1*v1+vp2*v2+vp3*v3;
	float t=0;

	if(u2!=0)
	{
		t=u1/u2;//��0,0,180��Z=180 ƽ��� ������ ��Z=180�����ཻ�� ������

		float x=(V.pt0.x+v1*t);
		float y=(V.pt0.y+v2*t);
		float z=(V.pt0.z+v3*t);

		CV_PointN.x=x;
		CV_PointN.y=y;
		CV_PointN.z=z;
		
		if(num==9)
		{
			cvCircle(frame,cvPoint(int(500+5*x),int(300+5*y)),5,CV_RGB(0,255,255),-1);
		}
		else
		{
			cvCircle(frame,cvPoint(int(500+5*x),int(300+5*y)),5,CV_RGB(0,0,255),-1);
		}
		
	}
	else
	{
		CV_PointN.x=65535;
		CV_PointN.y=65535;
	}
}
/**********************************************************/
void GL_Model(IplImage *frame,GL_Point CV_Pt0,GL_Point CV_Pt1,float x_min,float y_min,int num0,int num1 )
{
	CvPoint pt1,pt0;

	//-------------------
	pt0.x=int(200+5.0f*(CV_Pt0.x-x_min));
	pt0.y=int(300+5.0f*(CV_Pt0.y-y_min));
	pt1.x=int(200+5.0f*(CV_Pt1.x-x_min));
	pt1.y=int(300+5.0f*(CV_Pt1.y-y_min));

	if(num0!=0)
	cvLine(frame_GL,pt0,pt1,CV_RGB(0,255,0),2);
	else if(num0==0)
	{
		if(!(num1==5 || num1==6 || num1==7 || num1==8))
		{
			cvLine(frame_GL,pt0,pt1,CV_RGB(25,55,255),2);
		}
		else
		{
			cvLine(frame_GL,pt0,pt1,CV_RGB(205,85,0),2);
		}
	}
	


	cvInitFont( &font, CV_FONT_VECTOR0,0.65, 0.65, 0, 2, 2);
	sprintf_s(buf_T,256,"%d",num0);
	cvPutText(frame_GL,buf_T,pt0,&font,CV_RGB(255,0,255));
	sprintf_s(buf_T,256,"%d",num1);
	cvPutText(frame_GL,buf_T,pt1,&font,CV_RGB(255,0,255));
}
/**********************************************************/
void GL_ModelLine_Connect(IplImage *frame,GL_Point *CV_PointS,float x_min,float y_min)
{
	//HeadPlay_Pt[1]=GL_Quater_Point(Qt0,5,0,0,1,0,0);
	//HeadPlay_Pt[2]=GL_Quater_Point(Qt0,-5,0,0,1,0,0);
	//HeadPlay_Pt[3]=GL_Quater_Point(Qt0,0,-4,0,0,0,1);
	//HeadPlay_Pt[4]=GL_Quater_Point(Qt0,0,4,0,0,0,1);

	////
	//HeadPlay_Pt[5]=GL_Quater_Point(Qt0,4,4,0,0,0.8,0);
	//HeadPlay_Pt[6]=GL_Quater_Point(Qt0,-4,4,0,0,0.8,0);
	//HeadPlay_Pt[7]=GL_Quater_Point(Qt0,4,-4,0,0.9,0.8,0);
	//HeadPlay_Pt[8]=GL_Quater_Point(Qt0,-4,-4,0,0.9,0.8,0);

	if(CV_PointS[0].x!=65535 && CV_PointS[5].x!=65535)
	{
		GL_Model(frame,CV_PointS[0],CV_PointS[5], x_min, y_min,0,5 );
	}
	if(CV_PointS[0].x!=65535 && CV_PointS[6].x!=65535)
	{
		GL_Model(frame,CV_PointS[0],CV_PointS[6], x_min, y_min,0,6 );
	}
	if(CV_PointS[0].x!=65535 && CV_PointS[7].x!=65535)
	{
		GL_Model(frame,CV_PointS[0],CV_PointS[7], x_min, y_min,0,7 );
	}
	if(CV_PointS[0].x!=65535 && CV_PointS[8].x!=65535)
	{
		GL_Model(frame,CV_PointS[0],CV_PointS[8], x_min, y_min,0,8 );
	}
	//-------------------------------------------------
	if(CV_PointS[0].x!=65535 && CV_PointS[3].x!=65535)
	{
		GL_Model(frame,CV_PointS[0],CV_PointS[3], x_min, y_min,0,3 );
	}

	if(CV_PointS[0].x!=65535 && CV_PointS[4].x!=65535)
	{
		GL_Model(frame,CV_PointS[0],CV_PointS[4], x_min, y_min,0,4 );
	}

	if(CV_PointS[0].x!=65535 && CV_PointS[1].x!=65535)
	{
		GL_Model(frame,CV_PointS[0],CV_PointS[1], x_min, y_min,0,1 );
	}

	if(CV_PointS[0].x!=65535 && CV_PointS[2].x!=65535)
	{
		GL_Model(frame,CV_PointS[0],CV_PointS[2], x_min, y_min,0,2 );
	}

	if(CV_PointS[4].x!=65535 && CV_PointS[5].x!=65535)
	{
		GL_Model(frame,CV_PointS[4],CV_PointS[5], x_min, y_min,4,5 );
	}

	if(CV_PointS[4].x!=65535 && CV_PointS[6].x!=65535)
	{
		GL_Model(frame,CV_PointS[4],CV_PointS[6], x_min, y_min ,4,6);
	}

	if(CV_PointS[3].x!=65535 && CV_PointS[7].x!=65535)
	{
		GL_Model(frame,CV_PointS[3],CV_PointS[7], x_min, y_min ,3,7);
	}
	if(CV_PointS[3].x!=65535 && CV_PointS[8].x!=65535)
	{
		GL_Model(frame,CV_PointS[3],CV_PointS[8], x_min, y_min ,3,8);
	}

	if(CV_PointS[1].x!=65535 && CV_PointS[5].x!=65535)
	{
		GL_Model(frame,CV_PointS[1],CV_PointS[5], x_min, y_min,1,5 );
	}

	if(CV_PointS[2].x!=65535 && CV_PointS[6].x!=65535)
	{
		GL_Model(frame,CV_PointS[2],CV_PointS[6], x_min, y_min ,2,6);
	}

	if(CV_PointS[1].x!=65535 && CV_PointS[7].x!=65535)
	{
		GL_Model(frame,CV_PointS[1],CV_PointS[7], x_min, y_min ,1,7);
	}

	if(CV_PointS[2].x!=65535 && CV_PointS[8].x!=65535)
	{
		GL_Model(frame,CV_PointS[2],CV_PointS[8], x_min, y_min ,2,8);
	}
}
/**********************************************************/
void GL_LineRays()
{
	cvSet(frame_GL,CV_RGB(255,255,255));
	frame_GL->origin=1;
	

	for(int i=0;i<10;i++)
	{
		CV_PointS[i].x=65535;
		CV_PointS[i].y=65535;
	}
	
	int num=10;
	float x_min=65535;
	float y_min=65535;
	for(int i=0;i<num;i++)
	{
		if(EnableX[i] && EnableY[i])
		{
			//��ȡ��ƽ��Ľ�������
			float bx=GL_PlanXS[i].A;
			float by=GL_PlanXS[i].B;
			float bz=GL_PlanXS[i].C;

			float ax=GL_PlanYS[i].A;
			float ay=GL_PlanYS[i].B;
			float az=GL_PlanYS[i].C;

			//------------AX+BY+CZ+D=0
			float Da=GL_PlanYS[i].D;
			float Db=GL_PlanXS[i].D;

			//���ƽ���� ������,����ƽ��Ľ�������
			LineRays[i]=GLB_Line( ax, ay, az, Da, bx, by, bz, Db);

			CV_Cross(frame_GL,LineRays[i],CV_PointS[i],i);
			if(x_min>CV_PointS[i].x)
			{
				x_min=CV_PointS[i].x;
			}

			if(y_min>CV_PointS[i].y)
			{
				y_min=CV_PointS[i].y;
			}

			float x=HeadPlay_Pt[i].x;
			float y=HeadPlay_Pt[i].y;
			float z=HeadPlay_Pt[i].z;
			//--------------
			//float DisH=sqrt((x-Edge_HW_x)*(x-Edge_HW_x)+(y-Edge_HW_y)*(y-Edge_HW_y)+(z-Edge_HW_z)*(z-Edge_HW_z));
			float Dis=sqrt((x-LineRays[i].pt0.x)*(x-LineRays[i].pt0.x)+(y-LineRays[i].pt0.y)*(y-LineRays[i].pt0.y)+(z-LineRays[i].pt0.z)*(z-LineRays[i].pt0.z));
			//--------------
			glLineWidth(1); 
			glBegin(GL_LINES);
			glColor3f(0.05f, 0.6f, 0.9f); 
			glVertex3f(LineRays[i].pt0.x,LineRays[i].pt0.y,LineRays[i].pt0.z);
			glVertex3f(LineRays[i].pt0.x+LineRays[i].x*Dis,
				       LineRays[i].pt0.y+LineRays[i].y*Dis,
				       LineRays[i].pt0.z+LineRays[i].z*Dis);
			glEnd();

			//--------------
			//glPushMatrix();//���浱ǰ��ͼ����
			//glLineWidth(1); 
			//glColor3f(0.8,0.8,0.8); 
			//glTranslatef(HeadPlay_Pt[i].x,HeadPlay_Pt[i].y,HeadPlay_Pt[i].z);
			//glutSolidSphere(0.8, 20, 20);
			//glPopMatrix();//�����ϴα����λ��
		}

	}

	//------------------

	for(int i=0;i<num;i++)
	{
		float x=CV_PointS[i].x;
		float y=CV_PointS[i].y;
		if(x!=65535 && y!=65535)
		{
			cvCircle(frame_GL,cvPoint(int(200+5*(x-x_min)),int(300+5*(y-y_min))),5,CV_RGB(255,0,55),-1);
		}
		
	}
	//------------------
	GL_ModelLine_Connect(frame_GL,CV_PointS, x_min, y_min);

	//------------------
	cvInitFont( &font, CV_FONT_VECTOR0,1.0, 1.0, 0, 2, 2);
	sprintf_s(buf_T,256,"Q(%f,%f,%f,%f)",q0,q1,q2,q3);
	cvPutText(frame_GL,buf_T,cvPoint(10,10),&font,CV_RGB(0,255,255));

	sprintf_s(buf_T,256,"RPY:(%d,%d,%d)",Rool_angle,Pitch_angle,Yaw_angle);
	cvPutText(frame_GL,buf_T,cvPoint(10,40),&font,CV_RGB(0,255,255));
	

	sprintf_s(buf_T,256,"pos x:%f",pos_x);
	cvPutText(frame_GL,buf_T,cvPoint(10,70),&font,CV_RGB(0,255,255));
	sprintf_s(buf_T,256,"pos y:%f",pos_y);
	cvPutText(frame_GL,buf_T,cvPoint(10,100),&font,CV_RGB(0,255,255));
	sprintf_s(buf_T,256,"pos z:%f",pos_z);
	cvPutText(frame_GL,buf_T,cvPoint(10,130),&font,CV_RGB(0,255,255));

	//-------------------------------------
	float Edge=45*2;
	GL_Point Pt0;
	Pt0.x=-Edge;
	Pt0.y=81;
	Pt0.z=0;

	float x0=Pt0.x;
	float y0=Pt0.y;
	float z0=Pt0.z;

	float xx=LineRays[0].x*t1_Global;
	float yy=LineRays[0].y*t1_Global;
	float zz=LineRays[0].z*t1_Global;
	float dst_print=GL_Distance(0,0,0,xx,yy,zz);
	sprintf_s(buf_T,256,"Dist: %f",dst_print);
	cvPutText(frame_GL,buf_T,cvPoint(10,160),&font,CV_RGB(0,255,255));

	cvNamedWindow("Frame_GL2",1);
	cvShowImage("Frame_GL2",frame_GL);
}
/*****************************����ģ�ͷ���ϵ��*****************************/
void GLB_Knn(float Knn[][3],float a1,float b1,float c1,float a2,float b2,float c2,int num)
{
	Knn[num][0]=a1*a1+b1*b1+c1*c1;
	Knn[num][1]=a2*a2+b2*b2+c2*c2;
	Knn[num][2]=-2.0f*(a1*a2+b1*b2+c1*c2);
}

void GLB_Knn2(float Knn[][6],float a1,float b1,float c1,GL_Vector pt1,float a2,float b2,float c2,GL_Vector pt2,int num)
{
	Knn[num][0]=a1*a1+b1*b1+c1*c1; // t1*t1
	Knn[num][1]=a2*a2+b2*b2+c2*c2; // t2*t2
	Knn[num][2]=-2.0f*(a1*a2+b1*b2+c1*c2); // t1*t2
	Knn[num][3]= 2.0f*( (pt1.x-pt2.x)*a1 + (pt1.y-pt2.y)*b1 + (pt1.z-pt2.z)*c1 ); // t1
	Knn[num][4]=-2.0f*( (pt1.x-pt2.x)*a2 + (pt1.y-pt2.y)*b2 + (pt1.z-pt2.z)*c2 ); // t2
	Knn[num][5]=(pt1.x-pt2.x)*(pt1.x-pt2.x) + (pt1.y-pt2.y)*(pt1.y-pt2.y) + (pt1.z-pt2.z)*(pt1.z-pt2.z);// ����
}
/***************************** �߳����뽨ģ 4�� ���ٵ��� *****************************/
// DiguNum : ��¼�ݹ��㷨Сѭ��
// DiguCnt : ��¼�ݹ��㷨��ѭ�� N*65535
// Point_Check ��ģʽѡ���㷨����
// ID_Pt����ϵ�ID��� �����ӣ�0,4,5,6��
// ID_Length: ��Ӧ��ID��֮�䳤�ȣ�
//            ��Ӧ����// 1->2
//						   2->3
//						   1->3
//						   1->4
//						   2->4
//						   3->4
 //���ӣ�float E12=16;// 1->2
	//	 float E22=16;// 2->3
	//	 float E32=32;// 1->3
	//	 float E42=32;// 1->4
	//	 float E52=16;// 2->4
	//	 float E62=64;// 3->4
// tN_GlobalS_4N���ĵ���϶�ά����
// numc:�ĵ���ϵ�����

//tN_GlobalS_4N :Ϊ���ߵı�������Ϊģ��������������������Ϊ[][4].
//Energy_GlobalN:�������� ����Ϊ��Energy_GlobalA �� Energy_GlobalB�����������Ƚϣ��Ӷ��ı��������tStep_nn_GX����
//tStep_nn_GX   :�����㷨������
//Flag_Step     :Ϊ1ʱ���������� STEPNN��Ϊ0ʱ���������� STEPNN��
//Rude_Step_Control���Ŷ�����������ת
void GL_Build_Steepest_M4Point(int *ID_Pt,float*ID_Length,float tN_GlobalS_4N[][4],float* Energy_GlobalN,float *tStep_nn_GX,int numc,
	                     int &Point_Check,int &DiguCnt,int &DiguNum,bool Flag_Step,int Rude_Step_Control )
{
	//�������ʱ��DiguNum �� DiguCnt�������㡣
	DiguNum=0;
	DiguCnt=0;

	Point_Check=4;//ģʽѡ���㷨����
	float t1=tN_GlobalS_4N[numc][0];
	float t2=tN_GlobalS_4N[numc][1];
	float t3=tN_GlobalS_4N[numc][2];
	float t4=tN_GlobalS_4N[numc][3];
	//-------------------
	float Ft1,Ft2,Ft3,Ft4;
	float t1b=170,t2b=170,t3b=170,t4b=170;;

	//��������
	float a1=LineRays[ID_Pt[0]].x; float b1=LineRays[ID_Pt[0]].y; float c1=LineRays[ID_Pt[0]].z;
	float a2=LineRays[ID_Pt[1]].x; float b2=LineRays[ID_Pt[1]].y; float c2=LineRays[ID_Pt[1]].z;
	float a3=LineRays[ID_Pt[2]].x; float b3=LineRays[ID_Pt[2]].y; float c3=LineRays[ID_Pt[2]].z;
	float a4=LineRays[ID_Pt[3]].x; float b4=LineRays[ID_Pt[3]].y; float c4=LineRays[ID_Pt[3]].z;
	// 
	GL_Vector pt0[4];
	pt0[0]=LineRays[ID_Pt[0]].pt0;
	pt0[1]=LineRays[ID_Pt[1]].pt0;
	pt0[2]=LineRays[ID_Pt[2]].pt0;
	pt0[3]=LineRays[ID_Pt[3]].pt0;
	//
	float x=a1*t1;
	float y=b1*t1;
	float z=c1*t1;
	float track_dst=GL_Distance(0,0,0,x,y,z);

	/**********************************************************/
	//Խ����ɨ��� ����ԽС -->> 0.00268531

	//float nn=0.002515;// �ݹ鲽�� ���У�����̫��Ҳ����̫С����
	//if(track_dst<160)
	//{
	//	nn=0.001815;
	//}

	//if(!Fps_Track_Start)
	//{
	//	nn=0.001815;
	//	nn=0.002685;
	//}

	//Fps_Track_Start=1;

	float nn = 0.001515;// �ݹ鲽�� ���У�����̫��Ҳ����̫С����
	/*if (track_dst < 160)
	{
		nn = 0.001815;
	}

	if (!Fps_Track_Start)
	{
		nn = 0.001815;
	}*/

	Fps_Track_Start = 1;
	//�߳�ƽ��
	float E12=ID_Length[0];//16;// 1->2
	float E22=ID_Length[1];//16;// 2->3
	float E32=ID_Length[2];//32;// 1->3
	float E42=ID_Length[3];//32;// 1->4
	float E52=ID_Length[4];//16;// 2->4
	float E62=ID_Length[5];//64;// 3->4
	//------����ģ��ϵ��

#if 0
	float knn[10][3];
	GLB_Knn(knn,a1,b1,c1,a2,b2,c2,0);
	GLB_Knn(knn,a2,b2,c2,a3,b3,c3,1);
	GLB_Knn(knn,a1,b1,c1,a3,b3,c3,2);

	GLB_Knn(knn,a1,b1,c1,a4,b4,c4,3);
	GLB_Knn(knn,a2,b2,c2,a4,b4,c4,4);
	GLB_Knn(knn,a3,b3,c3,a4,b4,c4,5);
	 nn=0.00268531;
#else
	float knn[10][6];
	GLB_Knn2(knn,a1,b1,c1,pt0[0], a2,b2,c2,pt0[1], 0);
	GLB_Knn2(knn,a2,b2,c2,pt0[1], a3,b3,c3,pt0[2], 1);
	GLB_Knn2(knn,a1,b1,c1,pt0[0], a3,b3,c3,pt0[2], 2);

	GLB_Knn2(knn,a1,b1,c1,pt0[0], a4,b4,c4,pt0[3], 3);
	GLB_Knn2(knn,a2,b2,c2,pt0[1], a4,b4,c4,pt0[3], 4);
	GLB_Knn2(knn,a3,b3,c3,pt0[2], a4,b4,c4,pt0[3], 5);
	// nn=0.00298531;

	//if(Flag_Step)
	//{
	//	nn=tStep_nn_GX[numc]+STEPNN;
	//}
	//else if(!Flag_Step)
	//{
		nn=tStep_nn_GX[numc];
	//}
#endif
	 //if(track_dst>160.0f)
	 //{
		// nn=0.00288531;
	 //}

	 //if(track_dst>180.0f)
	 //{
		// nn=0.00318531;
	 //}
	 //Step_nn_Global=nn;
	// F(t1,t2,t3)=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2+knn[0][3]*t1+knn[0][4]*t2+knn[0][5]-E12)*(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2+knn[0][3]*t1+knn[0][4]*t2+knn[0][5]-E12)
	//            +(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3+knn[1][3]*t2+knn[1][4]*t3+knn[1][5]-E22)*(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3+knn[1][3]*t2+knn[1][4]*t3+knn[1][5]-E22)
	//            +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3+knn[2][3]*t1+knn[2][4]*t3+knn[2][5]-E32)*(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3+knn[2][3]*t1+knn[2][4]*t3+knn[2][5]-E32)

	//            +(knn[3][0]*t1*t1+knn[3][1]*t4*t4+knn[3][2]*t1*t4+knn[3][3]*t1+knn[3][4]*t4+knn[3][5]-E42)*(knn[3][0]*t1*t1+knn[3][1]*t4*t4+knn[3][2]*t1*t4+knn[3][3]*t1+knn[3][4]*t4+knn[3][5]-E42)
	//            +(knn[4][0]*t2*t2+knn[4][1]*t4*t4+knn[4][2]*t2*t4+knn[4][3]*t2+knn[4][4]*t4+knn[4][5]-E52)*(knn[4][0]*t2*t2+knn[4][1]*t4*t4+knn[4][2]*t2*t4+knn[4][3]*t2+knn[4][4]*t4+knn[4][5]-E52)
	//            +(knn[5][0]*t3*t3+knn[5][1]*t4*t4+knn[5][2]*t3*t4+knn[5][3]*t3+knn[5][4]*t4+knn[5][5]-E62)*(knn[5][0]*t3*t3+knn[5][1]*t4*t4+knn[5][2]*t3*t4+knn[5][3]*t3+knn[5][4]*t4+knn[5][5]-E62)

	//-----------------
	// F(t1,t2,t3)/t1ƫ����=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2+knn[0][3]*t1+knn[0][4]*t2+knn[0][5]-E12)* (2*knn[0][0]*t1+knn[0][2]*t2+knn[0][3])
	//                     +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3+knn[2][3]*t1+knn[2][4]*t3+knn[2][5]-E32)* (2*knn[2][0]*t1+knn[2][2]*t3+knn[2][3])
	//                     +(knn[3][0]*t1*t1+knn[3][1]*t4*t4+knn[3][2]*t1*t4+knn[3][3]*t1+knn[3][4]*t4+knn[3][5]-E42)* (2*knn[3][0]*t1+knn[3][2]*t4+knn[3][3])

	// F(t1,t2,t3)/t2ƫ����=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2+knn[0][3]*t1+knn[0][4]*t2+knn[0][5]-E12)* (2*knn[0][1]*t2+knn[0][2]*t1+knn[0][4])
	//                     +(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3+knn[1][3]*t2+knn[1][4]*t3+knn[1][5]-E22)* (2*knn[1][0]*t2+knn[1][2]*t3+knn[1][3])
	//                     +(knn[4][0]*t2*t2+knn[4][1]*t4*t4+knn[4][2]*t2*t4+knn[4][3]*t2+knn[4][4]*t4+knn[4][5]-E52)* (2*knn[4][0]*t2+knn[4][2]*t4+knn[4][3])

	// F(t1,t2,t3)/t3ƫ����=(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3+knn[1][3]*t2+knn[1][4]*t3+knn[1][5]-E22)* (2*knn[1][1]*t3+knn[1][2]*t2+knn[1][4])
	//                     +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3+knn[2][3]*t1+knn[2][4]*t3+knn[2][5]-E32)* (2*knn[2][1]*t3+knn[2][2]*t1+knn[2][4])
	//                     +(knn[5][0]*t3*t3+knn[5][1]*t4*t4+knn[5][2]*t3*t4+knn[5][3]*t3+knn[5][4]*t4+knn[5][5]-E62)* (2*knn[5][0]*t3+knn[5][2]*t4+knn[5][3])

	// F(t1,t2,t3)/t4ƫ����=(knn[3][0]*t1*t1+knn[3][1]*t4*t4+knn[3][2]*t1*t4+knn[3][3]*t1+knn[3][4]*t4+knn[3][5]-E42)* (2*knn[3][1]*t4+knn[3][2]*t1+knn[3][4])
	//                     +(knn[4][0]*t2*t2+knn[4][1]*t4*t4+knn[4][2]*t2*t4+knn[4][3]*t2+knn[4][4]*t4+knn[4][5]-E52)* (2*knn[4][1]*t4+knn[4][2]*t2+knn[4][4])
	//                     +(knn[5][0]*t3*t3+knn[5][1]*t4*t4+knn[5][2]*t3*t4+knn[5][3]*t3+knn[5][4]*t4+knn[5][5]-E62)* (2*knn[5][1]*t4+knn[5][2]*t3+knn[5][4])
	//-------�Ƕ�Լ��
	//GL_Vector Line1_V;// 0->1
	//Line1_V.x=a1*t1-a2*t2;
	//Line1_V.y=b1*t1-b2*t2;
	//Line1_V.z=c1*t1-c2*t2;

	//GL_Vector Line2_V;// 2->3
	//Line1_V.x=a3*t3-a2*t2;
	//Line1_V.y=b3*t3-b2*t2;
	//Line1_V.z=c3*t3-c2*t2;

	//---------------------------------------------------------------------------------------------------------------------
	// �Ƕ�Լ������ 0->1 2->3 �нǣ�F_Angle1
	//                   =( (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) )
	//                   *( (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) );


	// F_Angle/t1ƫ����=2*( (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) )
	//                 *( (a1)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) )

	// F_Angle/t2ƫ����=2*( (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) )
	//                 *(   (-a2)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(-a2)
	//                    + (-b2)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(-b2)
	//                    + (-c2)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(-c2))

	// F_Angle/t3ƫ����=2*( (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) )
	//                 *(  (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3) 
	//                   + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3) 
	//                   + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3) )
		float Energy_Min = 65535;
		float t_min[4];
		t_min[0] = t1;
		t_min[1] = t2;
		t_min[2] = t3;
		t_min[3] = t4;
	//-------------------------------------------------------------
	 while(DiguCnt==0 ||(DiguCnt!=0 && DiguNum==65535))
		 {
			 /*if(DiguCnt>15)nn=0.000023;*/
			 if(DiguCnt>1)break;
			 DiguNum=0;
			 
			 int i=0;
			 bool flag = 0;
		 for( i=0;i<65535;i++)
		 {

			 if(Point_Check==4)//�ĵ�ݹ�
			 {
				 float t1F = t1*t1;
				 float t2F = t2*t2;
				 float t3F = t3*t3;
				 float t4F = t4*t4;

				 float t12F = t1*t2;
				 float t13F = t1*t3;
				 float t14F = t1*t4;

				 float t23F = t2*t3;
				 float t24F = t2*t4;

				 float t34F = t3*t4;

				 Ft1 = (knn[0][0] * t1F + knn[0][1] * t2F + knn[0][2] * t12F + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12) * (2 * knn[0][0] * t1 + knn[0][2] * t2 + knn[0][3])
					 + (knn[2][0] * t1F + knn[2][1] * t3F + knn[2][2] * t13F + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32) * (2 * knn[2][0] * t1 + knn[2][2] * t3 + knn[2][3])
					 + (knn[3][0] * t1F + knn[3][1] * t4F + knn[3][2] * t14F + knn[3][3] * t1 + knn[3][4] * t4 + knn[3][5] - E42) * (2 * knn[3][0] * t1 + knn[3][2] * t4 + knn[3][3])
					 /*+0.2f*((a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) )
					 *( (a1)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) )*/
					 ;

				 Ft2 = (knn[0][0] * t1F + knn[0][1] * t2F + knn[0][2] * t12F + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12) * (2 * knn[0][1] * t2 + knn[0][2] * t1 + knn[0][4])
					 + (knn[1][0] * t2F + knn[1][1] * t3F + knn[1][2] * t23F + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22) * (2 * knn[1][0] * t2 + knn[1][2] * t3 + knn[1][3])
					 + (knn[4][0] * t2F + knn[4][1] * t4F + knn[4][2] * t24F + knn[4][3] * t2 + knn[4][4] * t4 + knn[4][5] - E52) * (2 * knn[4][0] * t2 + knn[4][2] * t4 + knn[4][3])
					 /*+0.2f*( (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) )
					 *(   (-a2)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(-a2)
					 + (-b2)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(-b2)
					 + (-c2)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(-c2))*/
					 ;

				 Ft3 = (knn[1][0] * t2F + knn[1][1] * t3F + knn[1][2] * t23F + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22) * (2 * knn[1][1] * t3 + knn[1][2] * t2 + knn[1][4])
					 + (knn[2][0] * t1F + knn[2][1] * t3F + knn[2][2] * t13F + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32) * (2 * knn[2][1] * t3 + knn[2][2] * t1 + knn[2][4])
					 + (knn[5][0] * t3F + knn[5][1] * t4F + knn[5][2] * t34F + knn[5][3] * t3 + knn[5][4] * t4 + knn[5][5] - E62) * (2 * knn[5][0] * t3 + knn[5][2] * t4 + knn[5][3])
					 /* +0.2f*( (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) )
					 *(  (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3)
					 + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3)
					 + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3) )*/
					 ;

				 Ft4 = (knn[3][0] * t1F + knn[3][1] * t4F + knn[3][2] * t14F + knn[3][3] * t1 + knn[3][4] * t4 + knn[3][5] - E42) * (2 * knn[3][1] * t4 + knn[3][2] * t1 + knn[3][4])
					 + (knn[4][0] * t2F + knn[4][1] * t4F + knn[4][2] * t24F + knn[4][3] * t2 + knn[4][4] * t4 + knn[4][5] - E52) * (2 * knn[4][1] * t4 + knn[4][2] * t2 + knn[4][4])
					 + (knn[5][0] * t3F + knn[5][1] * t4F + knn[5][2] * t34F + knn[5][3] * t3 + knn[5][4] * t4 + knn[5][5] - E62) * (2 * knn[5][1] * t4 + knn[5][2] * t3 + knn[5][4])
					 ;
			 }

			
			
			//-----------------------------------------------------------------------------------------------------------
#if 0
			 float EnergyN=1000;
			 if (Ft1 != 0 && Ft2 != 0 && Ft3!=0 && Ft4!=0)
			 {
				  EnergyN = (knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12)*(knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12)
					 + (knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22)*(knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22)
					 + (knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32)*(knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32)

					 + (knn[3][0] * t1*t1 + knn[3][1] * t4*t4 + knn[3][2] * t1*t4 + knn[3][3] * t1 + knn[3][4] * t4 + knn[3][5] - E42)*(knn[3][0] * t1*t1 + knn[3][1] * t4*t4 + knn[3][2] * t1*t4 + knn[3][3] * t1 + knn[3][4] * t4 + knn[3][5] - E42)
					 + (knn[4][0] * t2*t2 + knn[4][1] * t4*t4 + knn[4][2] * t2*t4 + knn[4][3] * t2 + knn[4][4] * t4 + knn[4][5] - E52)*(knn[4][0] * t2*t2 + knn[4][1] * t4*t4 + knn[4][2] * t2*t4 + knn[4][3] * t2 + knn[4][4] * t4 + knn[4][5] - E52)
					 + (knn[5][0] * t3*t3 + knn[5][1] * t4*t4 + knn[5][2] * t3*t4 + knn[5][3] * t3 + knn[5][4] * t4 + knn[5][5] - E62)*(knn[5][0] * t3*t3 + knn[5][1] * t4*t4 + knn[5][2] * t3*t4 + knn[5][3] * t3 + knn[5][4] * t4 + knn[5][5] - E62)
					 ;
				  float E1 = EnergyN / Ft1;
				  float E2 = EnergyN / Ft2;
				  float E3 = EnergyN / Ft3;
				  float E4 = EnergyN / Ft4;

				 

				  t1b = t1 - E1 / 100.0f;
				  t2b = t2 - E2 / 100.0f;
				  t3b = t3 - E3 / 100.0f;
				  t4b = t4 - E4 / 100.0f;
			 }

			 if (EnergyN == 0)
			 {
				 flag = 1;
				 break;
			}
#else
			 t1b = t1 - nn*Ft1;
			 t2b = t2 - nn*Ft2;
			 t3b = t3 - nn*Ft3;
			 t4b = t4 - nn*Ft4;

			 float  EnergyN = (knn[0][0] * t1b*t1b + knn[0][1] * t2b*t2b + knn[0][2] * t1b*t2b + knn[0][3] * t1b + knn[0][4] * t2b + knn[0][5] - E12)*(knn[0][0] * t1b*t1b + knn[0][1] * t2b*t2b + knn[0][2] * t1b*t2b + knn[0][3] * t1b + knn[0][4] * t2b + knn[0][5] - E12)
				 + (knn[1][0] * t2b*t2b + knn[1][1] * t3b*t3b + knn[1][2] * t2b*t3b + knn[1][3] * t2b + knn[1][4] * t3b + knn[1][5] - E22)*(knn[1][0] * t2b*t2b + knn[1][1] * t3b*t3b + knn[1][2] * t2b*t3b + knn[1][3] * t2b + knn[1][4] * t3b + knn[1][5] - E22)
				 + (knn[2][0] * t1b*t1b + knn[2][1] * t3b*t3b + knn[2][2] * t1b*t3b + knn[2][3] * t1b + knn[2][4] * t3b + knn[2][5] - E32)*(knn[2][0] * t1b*t1b + knn[2][1] * t3b*t3b + knn[2][2] * t1b*t3b + knn[2][3] * t1b + knn[2][4] * t3b + knn[2][5] - E32)

				 + (knn[3][0] * t1b*t1b + knn[3][1] * t4b*t4b + knn[3][2] * t1b*t4b + knn[3][3] * t1b + knn[3][4] * t4b + knn[3][5] - E42)*(knn[3][0] * t1b*t1b + knn[3][1] * t4b*t4b + knn[3][2] * t1b*t4b + knn[3][3] * t1b + knn[3][4] * t4b + knn[3][5] - E42)
				 + (knn[4][0] * t2b*t2b + knn[4][1] * t4b*t4b + knn[4][2] * t2b*t4b + knn[4][3] * t2b + knn[4][4] * t4b + knn[4][5] - E52)*(knn[4][0] * t2b*t2b + knn[4][1] * t4b*t4b + knn[4][2] * t2b*t4b + knn[4][3] * t2b + knn[4][4] * t4b + knn[4][5] - E52)
				 + (knn[5][0] * t3b*t3b + knn[5][1] * t4b*t4b + knn[5][2] * t3b*t4b + knn[5][3] * t3b + knn[5][4] * t4b + knn[5][5] - E62)*(knn[5][0] * t3b*t3b + knn[5][1] * t4b*t4b + knn[5][2] * t3b*t4b + knn[5][3] * t3b + knn[5][4] * t4b + knn[5][5] - E62)
				 ;
			 if (EnergyN < Energy_Min)
			 {
				 Energy_Min = EnergyN;
				 t_min[0] = t1;
				 t_min[1] = t2;
				 t_min[2] = t3;
				 t_min[3] = t4;
			 }

			 if (Point_Check == 4)
			 {
				 if (t1 == t1b && t2 == t2b && t3 == t3b &&t4 == t4b)// && (ABS(t1-t1b)<0.005 && ABS(t2-t2b)<0.005 && ABS(t3-t3b)<0.005 && ABS(t4-t4b)<0.005)) 
				 {
					 flag = 1;
					 break;
				 }
			 }

			 if (DiguCnt >= 1)
			 {
				 if (i > 20000)break;
			 }
#endif

			 t1=t1b;
			 t2=t2b;
			 t3=t3b;
			 t4=t4b;
		 }
			
		    if(flag == 1)
			DiguNum = 1;
			else
			DiguNum = i;

			DiguCnt++;

		 }

		 //---------------------------------------------------------------------------------------------------------------------------------------------------
		 //��С�������Ŷ�
#if 0
		 t1+=float(rand()%101-50)/50.0f *3.3f;
		 t2+=float(rand()%101-50)/50.0f *3.3f;
		 t3+=float(rand()%101-50)/50.0f *3.3f;
		 t4+=float(rand()%101-50)/50.0f *3.3f;
		 if(Rude_Step_Control==0)
		 {
			 nn=0.001613;
		 }
		 else if(Rude_Step_Control==1)
		 {
			 nn=0.000753;
		 }

		 int DiguCnt_R=0;
		 int DiguNum_R=0;
		 while(DiguCnt_R==0 ||(DiguCnt_R!=0 && DiguNum_R==65535))
		 {
			 /*if(DiguCnt>15)nn=0.000023;*/
			 if(DiguCnt_R>8)break;
			 DiguNum_R=0;
			 DiguCnt_R++;
			 int i=0;
			 bool flag = 0;
		 for( i=0;i<65535;i++)
		 {
			 if(Point_Check==4)//�ĵ�ݹ�
			 {
			  Ft1=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2+knn[0][3]*t1+knn[0][4]*t2+knn[0][5]-E12) * (2*knn[0][0]*t1+knn[0][2]*t2+knn[0][3])
			     +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3+knn[2][3]*t1+knn[2][4]*t3+knn[2][5]-E32) * (2*knn[2][0]*t1+knn[2][2]*t3+knn[2][3])
			     +(knn[3][0]*t1*t1+knn[3][1]*t4*t4+knn[3][2]*t1*t4+knn[3][3]*t1+knn[3][4]*t4+knn[3][5]-E42) * (2*knn[3][0]*t1+knn[3][2]*t4+knn[3][3])
				 /*+0.2f*((a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) )
				 *( (a1)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) )*/
				 ;

			  Ft2=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2+knn[0][3]*t1+knn[0][4]*t2+knn[0][5]-E12) * (2*knn[0][1]*t2+knn[0][2]*t1+knn[0][4])
			     +(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3+knn[1][3]*t2+knn[1][4]*t3+knn[1][5]-E22) * (2*knn[1][0]*t2+knn[1][2]*t3+knn[1][3])
			     +(knn[4][0]*t2*t2+knn[4][1]*t4*t4+knn[4][2]*t2*t4+knn[4][3]*t2+knn[4][4]*t4+knn[4][5]-E52) * (2*knn[4][0]*t2+knn[4][2]*t4+knn[4][3])
				 /*+0.2f*( (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) )
				                  *(   (-a2)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(-a2)
				                     + (-b2)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(-b2)
				                     + (-c2)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(-c2))*/
								  ;

			  Ft3=(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3+knn[1][3]*t2+knn[1][4]*t3+knn[1][5]-E22) * (2*knn[1][1]*t3+knn[1][2]*t2+knn[1][4])
			     +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3+knn[2][3]*t1+knn[2][4]*t3+knn[2][5]-E32) * (2*knn[2][1]*t3+knn[2][2]*t1+knn[2][4])
			     +(knn[5][0]*t3*t3+knn[5][1]*t4*t4+knn[5][2]*t3*t4+knn[5][3]*t3+knn[5][4]*t4+knn[5][5]-E62) * (2*knn[5][0]*t3+knn[5][2]*t4+knn[5][3])
				/* +0.2f*( (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) )
				                  *(  (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3) 
				                    + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3) 
				                    + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3) )*/
								  ;

			  Ft4=(knn[3][0]*t1*t1+knn[3][1]*t4*t4+knn[3][2]*t1*t4+knn[3][3]*t1+knn[3][4]*t4+knn[3][5]-E42) * (2*knn[3][1]*t4+knn[3][2]*t1+knn[3][4])
			     +(knn[4][0]*t2*t2+knn[4][1]*t4*t4+knn[4][2]*t2*t4+knn[4][3]*t2+knn[4][4]*t4+knn[4][5]-E52) * (2*knn[4][1]*t4+knn[4][2]*t2+knn[4][4])
			     +(knn[5][0]*t3*t3+knn[5][1]*t4*t4+knn[5][2]*t3*t4+knn[5][3]*t3+knn[5][4]*t4+knn[5][5]-E62) * (2*knn[5][1]*t4+knn[5][2]*t3+knn[5][4])
								  ;
			 }
			 //-----------------------------------------------------------------------------------------------------------
			 t1b=t1-nn*Ft1;
			 t2b=t2-nn*Ft2;
			 t3b=t3-nn*Ft3;
			 t4b=t4-nn*Ft4;


			 if(Point_Check==4)
			 {
				 if(t1==t1b && t2==t2b && t3==t3b &&t4==t4b)// && (ABS(t1-t1b)<0.005 && ABS(t2-t2b)<0.005 && ABS(t3-t3b)<0.005 && ABS(t4-t4b)<0.005)) 
				 { 
					 flag = 1;
					 break;
				 } 
			 }

			 t1=t1b;
			 t2=t2b;
			 t3=t3b;
			 t4=t4b;
		 }
		 if (flag == 1)
			 DiguNum_R = 1;
		 else
			DiguNum_R=i;
		 }

		 printf("/******************************************* �Ŷ���ݹ� ************************************************/\n");
		 printf("�Ŷ���ݹ������65535*%d + %d\n",DiguCnt_R,DiguNum_R);
#endif
		 //���ݹ�������ȫ�ֱ�������
		 /*tN_GlobalS_4N[numc][0]=t1;
		 tN_GlobalS_4N[numc][1]=t2;
		 tN_GlobalS_4N[numc][2]=t3;
		 tN_GlobalS_4N[numc][3]=t4;*/

		 tN_GlobalS_4N[numc][0] = t_min[0];
		 tN_GlobalS_4N[numc][1] = t_min[1];
		 tN_GlobalS_4N[numc][2] = t_min[2];
		 tN_GlobalS_4N[numc][3] = t_min[3];

		 t1=t_min[0];
		 t2=t_min[1];
		 t3=t_min[2];
		 t4=t_min[3];

		 Energy_GlobalN[numc]=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2+knn[0][3]*t1+knn[0][4]*t2+knn[0][5]-E12)*(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2+knn[0][3]*t1+knn[0][4]*t2+knn[0][5]-E12)
			             +(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3+knn[1][3]*t2+knn[1][4]*t3+knn[1][5]-E22)*(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3+knn[1][3]*t2+knn[1][4]*t3+knn[1][5]-E22)
			             +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3+knn[2][3]*t1+knn[2][4]*t3+knn[2][5]-E32)*(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3+knn[2][3]*t1+knn[2][4]*t3+knn[2][5]-E32)

			             +(knn[3][0]*t1*t1+knn[3][1]*t4*t4+knn[3][2]*t1*t4+knn[3][3]*t1+knn[3][4]*t4+knn[3][5]-E42)*(knn[3][0]*t1*t1+knn[3][1]*t4*t4+knn[3][2]*t1*t4+knn[3][3]*t1+knn[3][4]*t4+knn[3][5]-E42)
			             +(knn[4][0]*t2*t2+knn[4][1]*t4*t4+knn[4][2]*t2*t4+knn[4][3]*t2+knn[4][4]*t4+knn[4][5]-E52)*(knn[4][0]*t2*t2+knn[4][1]*t4*t4+knn[4][2]*t2*t4+knn[4][3]*t2+knn[4][4]*t4+knn[4][5]-E52)
			             +(knn[5][0]*t3*t3+knn[5][1]*t4*t4+knn[5][2]*t3*t4+knn[5][3]*t3+knn[5][4]*t4+knn[5][5]-E62)*(knn[5][0]*t3*t3+knn[5][1]*t4*t4+knn[5][2]*t3*t4+knn[5][3]*t3+knn[5][4]*t4+knn[5][5]-E62)
						 ;
		  //---------------------------------------------------
}

/***************************** �߳����뽨ģ ���ٵ��� *****************************/
// DiguNum : ��¼�ݹ��㷨Сѭ��
// DiguCnt : ��¼�ݹ��㷨��ѭ�� N*65535
// Point_Check ��ģʽѡ���㷨����
void GL_Build_Steepest_M5Point(int &Point_Check,int &DiguCnt,int &DiguNum )
{
	
}

/************************************************************************/
float GL_Distance2(GL_Point pt1,GL_Point pt2)
{
	float dst=(pt1.x-pt2.x)*(pt1.x-pt2.x) + (pt1.y-pt2.y)*(pt1.y-pt2.y) + (pt1.z-pt2.z);
	return dst;
}
/************************************************************************/
// ID_Length: ��Ӧ��ID��֮�䳤�ȣ�
//            ��Ӧ����// 1->2
//						   2->3
//						   1->3
//						   1->4
//						   2->4
//						   3->4
void GL_Got_Length(int *ID_PtN,float *ID_LengthN)
{
	ID_LengthN[0]=GL_Distance2(HeadPlay_PtB[ID_PtN[0]],HeadPlay_PtB[ID_PtN[1]]);
	ID_LengthN[1]=GL_Distance2(HeadPlay_PtB[ID_PtN[1]],HeadPlay_PtB[ID_PtN[2]]);
	ID_LengthN[2]=GL_Distance2(HeadPlay_PtB[ID_PtN[0]],HeadPlay_PtB[ID_PtN[2]]);

	ID_LengthN[3]=GL_Distance2(HeadPlay_PtB[ID_PtN[0]],HeadPlay_PtB[ID_PtN[3]]);
	ID_LengthN[4]=GL_Distance2(HeadPlay_PtB[ID_PtN[1]],HeadPlay_PtB[ID_PtN[3]]);
	ID_LengthN[5]=GL_Distance2(HeadPlay_PtB[ID_PtN[2]],HeadPlay_PtB[ID_PtN[3]]);

}

/************************************* ����Ӧ�����ݹ鲽������ **********************************/
// DiguNum : ��¼�ݹ��㷨Сѭ��
// DiguCnt : ��¼�ݹ��㷨��ѭ�� N*65535
// Point_Check ��ģʽѡ���㷨����
// ID_Pt����ϵ�ID��� �����ӣ�0,4,5,6��
// ID_Length: ��Ӧ��ID��֮�䳤�ȣ�
//            ��Ӧ����// 1->2
//						   2->3
//						   1->3
//						   1->4
//						   2->4
//						   3->4
//���ӣ�float E12=16;// 1->2
//	 float E22=16;// 2->3
//	 float E32=32;// 1->3
//	 float E42=32;// 1->4
//	 float E52=16;// 2->4
//	 float E62=64;// 3->4

// numc:�ĵ���ϵ�����
//Rude_Step_Control���Ŷ�����������ת
void GL_Build_Steepest_M4Point_Process(int *ID_Pt,float*ID_Length,int numc,int &Point_Check,int &DiguCnt,int &DiguNum,int Rude_Step_Control)
{
	GL_Build_Steepest_M4Point(ID_Pt,ID_Length, tN_GlobalS_4A,Energy_GlobalA,tStep_nn_G,numc,Point_Check,DiguCnt,DiguNum,1,Rude_Step_Control);
	//GL_Build_Steepest_M4Point(ID_Pt,ID_Length, tN_GlobalS_4B,Energy_GlobalB,tStep_nn_G,numc,Point_Check,DiguCnt,DiguNum,0 );
	////if(Energy_GlobalA[numc]>Energy_GlobalB[numc])
	////{
	////	tStep_nn_G[numc]+=STEPNN;
	////}
	////else
	////{
	////	tStep_nn_G[numc]-=STEPNN;
	////}
}
/***************************** ������������ *****************************/
void GL_Energy()
{
	//-------------------------------- �����ݶ��½��㷨
#if 0
	int Ces=1;
	if(Ces==0)
	{
		// y=x*x/2-2*x
		// yƫ��= x-2
		float x=1;
		float Nn=0.9;
		float Ee=0;
		for(int i=0;i<6;i++)
		{
			float Fx=x-2;
			float xb=x-Nn*Fx;
			Ee=-Nn*Fx;
			x=xb;
		}

		printf("��� = %f ,���Ž� = %f \n",Ee,x);
	}
	if(Ces==1)
	{
		// f(x,y)=(x-7)*(x-7) + 2*(y-4)*(y-4)+3
		// f��xƫ���� = 2*(x-7)/2
		// f��yƫ���� = 4*(y-4)/2
		float x=1,y=1;
		float Nn=0.1;
		float Eex=0,Eey=0;
		for(int i=0;i<600;i++)
		{
			
			float Fx=2*(x-7);
			
			float Fy=4*(y-4);

			float xb=x-Nn*Fx;
			float yb=y-Nn*Fy;

			x=xb;
			y=yb;

			Eex=-Nn*Fx;
			Eey=-Nn*Fy;

			
		}

		//printf("���xy = (%f, %f) ,���Ž�xy = (%f,%f) \n",Eex,Eey,x,y);
	}
#endif
	//--------------------------------
	//float Edge=45*2;
	//GL_Point Pt0;
	//Pt0.x=-Edge;
	//Pt0.y=81;
	//Pt0.z=0;

	//float x0=Pt0.x;
	//float y0=Pt0.y;
	//float z0=Pt0.z;

	//LineRays[];
	// (x-x0)/a=(y-y0)/b=(z-z0)/c=t
	// x=at+x0
	// y=bt+y0
	// z=ct+z0

	//-------------------------------------------------------------------------------------------------------
	if(!Best_Flag && EnableX[0]&&EnableY[0] && EnableX[5]&&EnableY[5] && EnableX[4]&&EnableY[4]// ɨ��� ʹ���жϣ�����ʹ������ �� �����㷨������
	    && EnableX[6]&&EnableY[6] 
		&& EnableX[1]&&EnableY[1] 
	)
	{
		 int DiguNum=0;//��¼�ݹ��㷨Сѭ��
		 int DiguCnt=0;//��¼�ݹ��㷨��ѭ�� N*65535
		 int Point_Check;//ģʽѡ���㷨����
		 int i=0;
		 
		 long start_time=clock();//Cpu�����㷨ʱ�� ��ʼ��

		 //---------------����ģʽѡ��
		 Point_Check=4;
		 //
		 //��ģ�õ�ͷ�� ID
		 int ID_Pt1[4]={0,4,5,6};
		 int ID_Pt2[4]={0,4,5,1};
		 //��ģ�õ�ͷ�� ID֮��ĳ���ƽ��
		 float ID_Length1[6];
		 float ID_Length2[6];

		 GL_Got_Length(ID_Pt1,ID_Length1);
		 GL_Got_Length(ID_Pt2,ID_Length2);
		 //-------------------------------------------------------------------------------------------�ݹ��㷨 Start
		 //GL_Build_Steepest_M(Point_Check,DiguCnt,DiguNum );
		 //4�����
		 if(Point_Check==4)
		 {
			 int numc=0;//�ĵ��������ѡ��

			 /*GL_Build_Steepest_M4Point(ID_Pt1,ID_Length1, tN_GlobalS_4A,Energy_GlobalA,tStep_nn_G,numc,Point_Check,DiguCnt,DiguNum,1 );
			 GL_Build_Steepest_M4Point(ID_Pt1,ID_Length1, tN_GlobalS_4B,Energy_GlobalB,tStep_nn_G,numc,Point_Check,DiguCnt,DiguNum,0 );
			 if(Energy_GlobalA[numc]>Energy_GlobalB[numc])
			 {
			 tStep_nn_G[numc]+=STEPNN;
			 }
			 else
			 {
			 tStep_nn_G[numc]-=STEPNN;
			 }*/

			 GL_Build_Steepest_M4Point_Process(ID_Pt1,ID_Length1,numc,Point_Check,DiguCnt,DiguNum,0);
			 numc=1;//�ĵ��������ѡ��
			 GL_Build_Steepest_M4Point_Process(ID_Pt2,ID_Length2,numc,Point_Check,DiguCnt,DiguNum,1);
			/* GL_Build_Steepest_M4Point(ID_Pt2,ID_Length2, tN_GlobalS_4A,Energy_GlobalA,tStep_nn_G,numc,Point_Check,DiguCnt,DiguNum,1 );
			 GL_Build_Steepest_M4Point(ID_Pt2,ID_Length2, tN_GlobalS_4B,Energy_GlobalB,tStep_nn_G,numc,Point_Check,DiguCnt,DiguNum,0 );
			 if(Energy_GlobalA[numc]>Energy_GlobalB[numc])
			 {
				 tStep_nn_G[numc]+=STEPNN;
			 }
			 else
			 {
				 tStep_nn_G[numc]-=STEPNN;
			 }*/

			 //�������ڴ�ӡ��ʾ
			 numc=0;
			 t1_Global=tN_GlobalS_4A[numc][0];
			 t2_Global=tN_GlobalS_4A[numc][1];
			 t3_Global=tN_GlobalS_4A[numc][2];
			 t4_Global=tN_GlobalS_4A[numc][3];
		 }
		 
		 //5�����
		 /*if(Point_Check==5)
		 {
			 GL_Build_Steepest_M5Point(Point_Check,DiguCnt,DiguNum );
		 }*/
		 
		 Best_Flag=1;//�ñ�־λ
		 //-------------------------------------------------------------------------------------------�ݹ��㷨 Finsh
		
		 long end_time=clock();//Cpu �㷨��ʱ���
		 float timex=float(end_time-start_time)/1000;
		 printf("/****************************************************************************/\n");
		 printf("                       ------<< %d ����� >>------ \n",Point_Check);
		 printf("                             <<< �� %d ֡ >>>\n",Fps_World);
		 printf("�ݹ������%d*65535 + %d \n",DiguCnt-1,DiguNum);
		 printf("CPU�ݹ�ʱ�� �� %f \n",timex);
		 if(timex>0.02)
		 {
			 printf("------------------->> �㷨��ʱ !!! \n");
			 Cnt_Cpu_Best++;
		 }
		 else
		 {
			 printf("------------------->> �㷨OK   !!! \n");
		 }
		 printf("------------------->> �㷨��ʱ���� ��%d \n",Cnt_Cpu_Best);
		 //--------------------------------------------------
		 for(int i=0;i<2;i++)
		 {
			 printf(" %d) �ݹ鲽���� %f \n",i+1,tStep_nn_G[i]);
		 }
		 
		 //------------------------
		 float xx=LineRays[0].x*t1_Global+LineRays[0].pt0.x;
		 float yy=LineRays[0].y*t1_Global+LineRays[0].pt0.y;
		 float zz=LineRays[0].z*t1_Global+LineRays[0].pt0.z;
		 float dst_print=GL_Distance(LineRays[0].pt0.x,LineRays[0].pt0.y,LineRays[0].pt0.z,xx,yy,zz);
		 printf("Ŀ�굽ɨ��ԭ����룺%f\n",dst_print);
		 //------------------------
		 for(int i=0;i<10;i++)
		 {
			 printf("  %d) dis_minxy: (%f,%f)\n",i+1,DistsX_min[i],DistsY_min[i]);
		 }
		 /* for(int i=0;i<10;i++)
		 {
		 printf("  %d) ��άͶӰ : (%f,%f)\n",i+1,CV_PointS[i].x,CV_PointS[i].y);
		 }*/
		 
		 FILE *file=fopen(".//logcpu.txt","a+");
		 fprintf(file,"/***************************************/\n");
		 fprintf(file,"CPU�ݹ�ʱ�� �� %f \n",timex);
		
		//printf("------ >>>  ��С���� �� %f\n",dst_min);
		//------
		float x1=LineRays[4].x*t2_Global+LineRays[4].pt0.x;
		float y1=LineRays[4].y*t2_Global+LineRays[4].pt0.y;
		float z1=LineRays[4].z*t2_Global+LineRays[4].pt0.z;
		float xp0=HeadPlay_Pt[4].x;
		float yp0=HeadPlay_Pt[4].y;
		float zp0=HeadPlay_Pt[4].z;
		//printf("/***************************************/\n");
		printf("��λ��ÿ���� Ϊ 0.4 \n");
		printf("��ʵֵ : (%f,%f,%f)\n",xp0,yp0,zp0);
		printf("����ֵ : (%f,%f,%f)\n",x1,y1,z1);

		printf("��һ֡��ʵֵ : (%f,%f,%f)\n",xp0_Old,yp0_Old,zp0_Old);
		printf("��һ֡����ֵ : (%f,%f,%f)\n",x1_Old,y1_Old,z1_Old);

		printf("��ʵ�ٶ�= (%f,%f,%f)\n",(xp0-xp0_Old),(yp0-yp0_Old),(zp0-zp0_Old));
		printf("�����ٶ�= (%f,%f,%f)\n",(x1-x1_Old),(y1-y1_Old),(z1-z1_Old));

		printf("���ֵ ��(%f,%f,%f)\n",abs(xp0-x1),abs(yp0-y1),abs(zp0-z1));
		//------
		if(abs(xp0-x1)<2 && abs(yp0-y1)<2 && abs(zp0-z1)<2)
		{
			Cnt_Correct++;
		}
		else
		{
			Cnt_Err++;
		}

		printf("4.5*4.5�ռ� >>> �� 5cm��Χ ������벻�������� ( %d , %d )\n",Cnt_Correct,Cnt_Err);
		
		//------------------------------------
		//�����ʾ
		float x_ture1[5],y_ture1[5],z_ture1[5];//��ʵ
		float x_etm1[5],y_etm1[5],z_etm1[5];//����
		float x_ture2[5],y_ture2[5],z_ture2[5];//��ʵ
		float x_etm2[5],y_etm2[5],z_etm2[5];//����

		for(int i=0;i<Point_Check;i++)
		{
			//��ȡ����ֵ
			x_etm1[i]=LineRays[ID_Pt1[i]].x*tN_GlobalS_4A[0][i]+LineRays[ID_Pt1[i]].pt0.x;
			y_etm1[i]=LineRays[ID_Pt1[i]].y*tN_GlobalS_4A[0][i]+LineRays[ID_Pt1[i]].pt0.y;
			z_etm1[i]=LineRays[ID_Pt1[i]].z*tN_GlobalS_4A[0][i]+LineRays[ID_Pt1[i]].pt0.z;
			//
			x_etm2[i]=LineRays[ID_Pt2[i]].x*tN_GlobalS_4A[1][i]+LineRays[ID_Pt2[i]].pt0.x;
			y_etm2[i]=LineRays[ID_Pt2[i]].y*tN_GlobalS_4A[1][i]+LineRays[ID_Pt2[i]].pt0.y;
			z_etm2[i]=LineRays[ID_Pt2[i]].z*tN_GlobalS_4A[1][i]+LineRays[ID_Pt2[i]].pt0.z;
			//��ȡ��ʵֵ
			x_ture1[i]=HeadPlay_Pt[ID_Pt1[i]].x;
			y_ture1[i]=HeadPlay_Pt[ID_Pt1[i]].y;
			z_ture1[i]=HeadPlay_Pt[ID_Pt1[i]].z;
			//
			x_ture2[i]=HeadPlay_Pt[ID_Pt2[i]].x;
			y_ture2[i]=HeadPlay_Pt[ID_Pt2[i]].y;
			z_ture2[i]=HeadPlay_Pt[ID_Pt2[i]].z;
		}
	
		//-----------------------------
		printf("-------------------------- ��������Ϣ ��\n");
		printf("---------------------- Point 1 ----------------------\n");
		FILE *file_Err=fopen(".//ErronPos.txt","a+");
		//-------------------------------------- 1
		for(int i=0;i<Point_Check;i++)
		{
			printf(" %d) ��ʵֵ��(%f,%f,%f) \n",i+1,x_ture1[i],y_ture1[i],z_ture1[i]);
			printf("     ����ֵ��(%f,%f,%f) \n",    x_etm1[i],y_etm1[i],z_etm1[i]);
			printf("     ���ֵ��(%f,%f,%f) \n",ABS(x_ture1[i]-x_etm1[i]),ABS(y_ture1[i]-y_etm1[i]),ABS(z_ture1[i]-z_etm1[i]) );
			if( ABS(x_ture1[i]-x_etm1[i])<1.0f && ABS(y_ture1[i]-y_etm1[i])<1.0f && ABS(z_ture1[i]-z_etm1[i])<1.0f )
			{
				printf("------------------------------->> Success !!!\n");
				Right_CntBest[0]++;
			}
			else
			{
				printf("------------------------------->> Erron   !!!������������\n");
				Erron_CntBest[0]++;

				//����������м�¼���ڷ���
				fprintf(file_Err,"                       ------<< %d ����� >>------ \n",Point_Check);
				fprintf(file_Err,"                             <<< �� %d ֡ >>>\n",Fps_World);
				fprintf(file_Err,"�ݹ������%d*65535 + %d \n",DiguCnt-1,DiguNum);
				fprintf(file_Err,"CPU�ݹ�ʱ�� �� %f \n",timex);
				fprintf(file_Err,"Ŀ�굽ɨ��ԭ����룺%f\n",dst_print);
				fprintf(file_Err,"%d) �������: ��ID:%d\n",Erron_CntBest,i+1);
				fprintf(file_Err,"    ��ʵֵ��(%f,%f,%f)\n",x_ture1[i],y_ture1[i],z_ture1[i]);
				fprintf(file_Err,"    ����ֵ��(%f,%f,%f)\n",x_etm1[i],y_etm1[i],z_etm1[i]);
				fprintf(file_Err,"    ���ֵ��(%f,%f,%f)\n",ABS(x_ture1[i]-x_etm1[i]),ABS(y_ture1[i]-y_etm1[i]),ABS(z_ture1[i]-z_etm1[i]));
				fprintf(file_Err,"    PRY��̬�ǣ�(%d,%d,%d)\n",Pitch_angle,Rool_angle,Yaw_angle);
			}
		}
		fclose(file_Err);
		printf("---------------------- Point 2 ----------------------\n");
		//------------------------------------ 2
		for(int i=0;i<Point_Check;i++)
		{
			printf(" %d) ��ʵֵ��(%f,%f,%f) \n",i+1,x_ture2[i],y_ture2[i],z_ture2[i]);
			printf("     ����ֵ��(%f,%f,%f) \n",    x_etm2[i],y_etm2[i],z_etm2[i]);
			printf("     ���ֵ��(%f,%f,%f) \n",ABS(x_ture2[i]-x_etm2[i]),ABS(y_ture2[i]-y_etm2[i]),ABS(z_ture2[i]-z_etm2[i]) );
			if( ABS(x_ture2[i]-x_etm2[i])<1.0f && ABS(y_ture2[i]-y_etm2[i])<1.0f && ABS(z_ture2[i]-z_etm2[i])<1.0f )
			{
				printf("------------------------------->> Success !!!\n");
				Right_CntBest[1]++;
			}
			else
			{
				printf("------------------------------->> Erron   !!!������������\n");
				Erron_CntBest[1]++;

			}
		}
		//��ʾ����ֵ
		for(int i=0;i<2;i++)
		{
			printf( "%d) ����ֵ �� %f\n",i+1,Energy_GlobalA[i]);
		}

		for(int i=0;i<2;i++)
		{
			printf("@:::::::::::::::::: Point%d (2.5cm����)���Ŵ������ �� %d\n",i+1,Erron_CntBest[i]);
			printf("@!!!!!!!!!!!!!!!!!! Point%d (2.5cm����)������ȷ���� �� %d\n",i+1,Right_CntBest[i]);
			printf("\n");
		}

		//---------------------
		
		fprintf(file,"��λ��ÿ���� Ϊ 0.4 \n");
		fprintf(file,"��ʵֵ : (%f,%f,%f)\n",xp0,yp0,zp0);
		fprintf(file,"����ֵ : (%f,%f,%f)\n",x1,y1,z1);

		fprintf(file,"���ֵ ��(%f,%f,%f)\n",abs(xp0-x1),abs(yp0-y1),abs(zp0-z1));

		
		xp0_Old=xp0;
		yp0_Old=yp0;
		zp0_Old=zp0;
		x1_Old=x1;
		y1_Old=y1;
		z1_Old=z1;

		fclose(file);

		 FILE *file2=fopen(".//LastData.txt","a+");
		 fprintf(file2,"(%f,%f,%f),(%f,%f,%f,%f),(%d,%d),(%d,%d),(%d,%d)\n",
					pos_x,pos_y,pos_z,q0,q1,q2,q3,
					Cnt_Correct,Cnt_Err,Cnt_Correct3,Cnt_Err3,Cnt_Correct12,Cnt_Err12
					);
		fclose(file2);
	}
	

}
/**********************************************************/
void GL_Draw_Track_Point()
{
	float Edge=45*2;
	GL_Point Pt0;
	Pt0.x=-Edge;
	Pt0.y=81;
	Pt0.z=0;

	float x0=Pt0.x;
	float y0=Pt0.y;
	float z0=Pt0.z;

	//if(Best_Flag)
	{
		//------------------------
		float x1=LineRays[0].x*t1_Global+LineRays[0].pt0.x;
		float y1=LineRays[0].y*t1_Global+LineRays[0].pt0.y;
		float z1=LineRays[0].z*t1_Global+LineRays[0].pt0.z;

		float x2=LineRays[4].x*t2_Global+LineRays[4].pt0.x;
		float y2=LineRays[4].y*t2_Global+LineRays[4].pt0.y;
		float z2=LineRays[4].z*t2_Global+LineRays[4].pt0.z;

		float x3=LineRays[5].x*t3_Global+LineRays[5].pt0.x;
		float y3=LineRays[5].y*t3_Global+LineRays[5].pt0.y;
		float z3=LineRays[5].z*t3_Global+LineRays[5].pt0.z;

		float x4=LineRays[6].x*t4_Global+LineRays[6].pt0.x;
		float y4=LineRays[6].y*t4_Global+LineRays[6].pt0.y;
		float z4=LineRays[6].z*t4_Global+LineRays[6].pt0.z;

		glPushMatrix();//���浱ǰ��ͼ����
		glLineWidth(1); 
		glColor3f(0.8,0.0,0.8); 
		glTranslatef(x1,y1,z1);
		glutSolidSphere(3.6, 20, 20);
		glPopMatrix();//�����ϴα����λ��

		glPushMatrix();//���浱ǰ��ͼ����
		glLineWidth(1); 

		glColor3f(0.1,0.0,0.98); 

		glTranslatef(x2,y2,z2);
		glutSolidSphere(0.9, 20, 20);
		glPopMatrix();//�����ϴα����λ��

		//glPushMatrix();//���浱ǰ��ͼ����
		//glLineWidth(1); 
		//glColor3f(0.0,0.8,0.0); 
		//glTranslatef(x3,y3,z3);
		//glutSolidSphere(0.8, 20, 20);
		//glPopMatrix();//�����ϴα����λ��

		//if(Point_Check==4)
		//{
		//	glPushMatrix();//���浱ǰ��ͼ����
		//	glLineWidth(1); 
		//	glColor3f(0.0,0.8,0.0); 
		//	glTranslatef(x4,y4,z4);
		//	glutSolidSphere(0.8, 20, 20);
		//	glPopMatrix();//�����ϴα����λ��
		//}


		/*glLineWidth(5); 
		glBegin(GL_LINES);
		glColor3f(1.0f, 0.6f, 0.0f); 
		glVertex3f(x1,y1,z1);
		glVertex3f(x2,y2,z2);
		glEnd();*/


	}
}
/**********************************************************/
void display(void)
{  
	glClear (GL_COLOR_BUFFER_BIT);
	glClearColor( 0.1f, 0.2f, 0.2f, 1.0f );
	reshape (320*2, 240*2);
	//glPushMatrix();//���浱ǰ��ͼ����
	//glRotatef(angle,0,1,0);
	//glPopMatrix();//���浱ǰ��ͼ����
	//glEnable(GL_AUTO_NORMAL);
	/*************����������*************/
#if 0
	glBegin(GL_TRIANGLES);  
	glColor3f(0.92f, 0.08f, 0.02f );
	glVertex3f(0.0f,0.0f,0.0f);  
	glVertex3f(50.0f,0.0f,0.0f);  
	glVertex3f(25.0f,25.0f,0.0f);  

	glVertex3f(-25.0f,0.0f,0.0f);  
	glVertex3f(-75.0f,0.0f,0.0f);  
	glVertex3f(-50.0f,25.0f,0.0f);  
	//������������  
	glEnd(); 
#endif
	angle_GL++;
//--------------------------------
	//glPushMatrix();//���浱ǰ��ͼ����
	//glTranslatef(45,0,0);         
	////glRotatef(Wwn,QtN.q1,QtN.q2,QtN.q3);
	//glColor3f(0.8f, 0.2f, 0.0f );
	//glScaled(2,15,2);
	//glutSolidCube(2);
	//printf("-------------------- step1.1\n");
	GL_Draw_Filed();
	//printf("-------------------- step1.2\n");
	//GL_Scan(0.0091);

	GL_Scan(Scan_Step);

	//printf("-------------------- step1.3\n");
	GLB_IMU(gxr_Global,gyr_Global, gzr_Global,1 ) ;
	gxr_Global=0;
	gyr_Global=0;
	gzr_Global=0;
	//printf("-------------------- step1.4\n");
	GL_Head_Display();
	//printf("-------------------- step1.5\n");
	GL_GotHead_DisPlay_Pt();
	//printf("-------------------- step1.6\n");
	GL_LineRays();
	//printf("-------------------- step1.7\n");
	if(Angle_Y>87)
	GL_Energy();
	
	//printf("-------------------- step1.8\n");
	if(Fps_Track_Start)
	GL_Draw_Track_Point();
	//printf("-------------------- step1.9\n");
	cvWaitKey(1);
	/********************************************/
	glutSwapBuffers();
}
/**********************************************************/
void myIdle(void) 
{  
	angle_GL+=1;
	
}
/**********************************************************/
DWORD _stdcall ThreadProc(LPVOID lpParameter)//�߳�ִ�к���
{
	//LastData_Init();
	glutCreateWindow("��άģ��"); 
	int num=10;//��ʼ�� Ŀ����Xɨ���� �� Yɨ���� ��־
	for(int i=0;i<num;i++)
	{
		EnableX[i]=0;
		EnableY[i]=0;
		DistsX_min[i]=60000;
		DistsY_min[i]=60000;
	
		New_FpsX[i]=0;
		New_FpsY[i]=0;
	}
	init ();
	glutIdleFunc(&myIdle);
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc(glutKeyboard);    // called when the application receives a input from the keyboard
    
	glutMainLoop();
	return 0;
}

#endif