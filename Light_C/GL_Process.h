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
#include "Optimization_Math.h"

using namespace cv;
using namespace std;

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

float pos_x=37,pos_y=55,pos_z=158;

int gxr_Global=0;
int gyr_Global=0;
int gzr_Global=0;

bool GL_Flag_Stop=0;
bool Best_Flag=0;//一帧显示一次最优拟合标志

//-------------------------------------------------------------------------------
float t1_Global=190,t2_Global=190,t3_Global=190,t4_Global=190,t5_Global=190;

float tN_GlobalS_4A[6][4];//6组 4点建模 迭代寄存器
float tN_GlobalS_4B[6][4];//6组 4点建模 迭代寄存器
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
//场地边长： 45*4 单位
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
	glBegin(GL_LINES);//绘制居中轨迹
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

	//------ Light A1 用于Moter横向运动
	glPushMatrix();
	glLineWidth(1); 
	glTranslatef(Edge_HW_x,Edge_HW_y,Edge_HW_z);
	glRotatef(angle_GL,0,1,0);
	glColor3f(0.0f, 1.0f, 1.0f); 
	glutSolidSphere(1.5, 15, 15);
	glPopMatrix();

	//------ Light A2 用于Motor纵向运动
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
/******************************XY面扫描****************************/
//扫描步长 ：Step_Scan
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
	//顺时针
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
	//平面的向量
	float ax=-Edge*2;
	float ay=0;
	float az=0;
//
	float bx=x1-Edge_ZW_x;
	float by=y1-Edge_ZW_y;
	float bz=z1-Edge_ZW_z;
	//获得平面叉积 法向量
	Plan_XNorml=GLB_CHAJI(ax, ay, az, bx, by, bz);
	GL_PlanX.A=Plan_XNorml.x;
	GL_PlanX.B=Plan_XNorml.y;
	GL_PlanX.C=Plan_XNorml.z;

	//AX+BY+CZ+D=0 => D=-AX-BY-CZ
	//GL_PlanX.D=-(GL_PlanX.A*x1+GL_PlanX.B*y1+GL_PlanX.C*z1);
	GL_PlanX.D=-(GL_PlanX.A*Edge_ZW_x+GL_PlanX.B*Edge_ZW_y+GL_PlanX.C*Edge_ZW_z);

	glLineWidth(3); //绘制垂线
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
			//初始化 目标点的X扫描面 与 Y扫描面 标志
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

		glBegin(GL_LINES);//绘制居中轨迹
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

		//平面的向量
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

		glLineWidth(3); //绘制垂线
		glBegin(GL_LINES);
		glColor3f(0.0f, 1.0f, 1.0f); 
		glVertex3f(x2,y2+81.0f/2,z2);
		glVertex3f(x2+GL_PlanY.A*20,y2+81.0F/2+GL_PlanY.B*20,z2+GL_PlanY.C*20);
		glEnd();
	}

}
/****************************四元数旋转获取头显，传感器坐标点。******************************/
// PtB: 获取相对传感器位置，用于建模递归时使用。
GL_Point GL_Quater_Point(GL_Quater Qt0,GL_Point &PtB,float x_offset,float y_offset,float z_offset,float r,float g,float b)
{
	GL_Point Pt;
	
	GL_Quater Qt0_N=Conjugate_Q(Qt0); //------ 根节点四元数 求逆

	GL_Quater Q_Point;
	Q_Point.q0=0;
	Q_Point.q1=x_offset;
	Q_Point.q2=y_offset;
	Q_Point.q3=z_offset;//根节点骨架长度

	PtB.x=x_offset;// PtB获取相对传感器位置，用于建模递归时使用。
	PtB.y=y_offset;
	PtB.z=z_offset;

	//四元数点局部坐标系更新计算 Loc_New_Point=Q*Loc_Old_Point*Q_N
	//------计算1次相乘
	GL_Quater Q011=MUL_Q(Qt0,Q_Point);
	//------计算2次相乘
	GL_Quater Q022=MUL_Q(Q011,Qt0_N);

	glPushMatrix();//储存当前视图矩阵
	glLineWidth(1); 
	glColor3f(r,g,b); 
	glTranslatef(pos_x+Q022.q1,pos_y+Q022.q2,pos_z+Q022.q3);
	glutSolidSphere(0.6, 20, 20);
	glPopMatrix();//弹出上次保存的位置

	Pt.x=pos_x+Q022.q1;
	Pt.y=pos_y+Q022.q2;
	Pt.z=pos_z+Q022.q3;

	return Pt;
}
/**********************************************************/
// Flag_XY :XY平面，0点未在该平面，1点在该平面上。
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
			glPushMatrix();//储存当前视图矩阵
			glLineWidth(1); 
			glColor3f(0.8,0.8,0.8); 
			glTranslatef(x,y,z);
			glutSolidSphere(0.8, 20, 20);
			glPopMatrix();//弹出上次保存的位置

			//------如果满足 点在平面要求则认为 点在平面上
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
	//绘制 旋转长方体
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
//#if 0
//	//遍历欧拉角转四元数
//
//
//	if (Rool_angleB >= 179)
//	{
//		Rool_angleB = 0;
//		Pitch_angleB+=2;
//
//		if (Pitch_angleB >= 90)
//		{
//			Pitch_angleB = -90;
//		}
//	}
//	else
//	{
//		Rool_angleB+=2;
//	}
//		
//	
//	
//
//	PitchB = float(Pitch_angleB)*CV_PI / 180;
//	RoolB = float(Rool_angleB)*CV_PI / 180;
//	YawB = float(Yaw_angleB)*CV_PI / 180;
//
//	OLA_2_Q4(qB0, qB1, qB2, qB3, PitchB, RoolB, YawB);
//
//	WwB = acos(qB0) * 2 * 180 / CV_PI;
//
//
//	//显示欧拉角转四元数旋转效果 覆盖所有翻滚与俯仰平面效果
//	glPushMatrix();
//	glTranslatef(0,40,0);
//	glRotatef(WwB, qB1, qB2, qB3);
//	glColor3f(1.0f, 0.0f, 1.0f);
//	glScaled(40, 8,80);
//	glutSolidCube(1);
//	glPopMatrix();
//
//	glPushMatrix();
//	glTranslatef(0, 40, 0);
//	glRotatef(WwB, qB1, qB2, qB3);
//	glColor3f(0.0f, 1.0f, 0.0f);
//	glScaled(20, 6, 50);
//	glutSolidCube(1);
//	glPopMatrix();
//
//	glPushMatrix();
//	glLineWidth(6);
//	glTranslatef(0, 40, 0);
//	glRotatef(WwB, qB1, qB2, qB3);
//	glColor3f(0.7f, 0.8f, 0.8f);
//	glBegin(GL_LINES);
//	glVertex3f(0,0,0);
//	glVertex3f(0,0,60);
//	glEnd();
//	glPopMatrix();
//
//
//	GL_Quater Qt0;
//	Qt0.q0 = qB0;
//	Qt0.q1 = qB1;
//	Qt0.q2 = qB2;
//	Qt0.q3 = qB3;
//
////
//	GL_Point Pt;
//
//	GL_Quater Qt0_N = Conjugate_Q(Qt0); //------ 根节点四元数 求逆
//
//	GL_Quater Q_Point;
//	Q_Point.q0 = 0;
//	Q_Point.q1 = 0;
//	Q_Point.q2 = 40;
//	Q_Point.q3 = 0;//根节点骨架长度
//
//	//四元数点局部坐标系更新计算 Loc_New_Point=Q*Loc_Old_Point*Q_N
//	//------计算1次相乘
//	GL_Quater Q011 = MUL_Q(Qt0, Q_Point);
//	//------计算2次相乘
//	GL_Quater Q022 = MUL_Q(Q011, Qt0_N);
//
//	glPushMatrix();//储存当前视图矩阵
//	glLineWidth(1);
//	glColor3f(1.0,0,0);
//	glTranslatef(0 + Q022.q1, 40 + Q022.q2, 0 + Q022.q3);
//	glutSolidSphere(6, 20, 20);
//	glPopMatrix();//弹出上次保存的位置
//
//	//显示欧拉角转四元数旋转效果 覆盖所有翻滚与俯仰平面效果  法向量
//	glLineWidth(5);
//	glColor3f(0.0, 0, 1.00);
//	glBegin(GL_LINES);
//	glVertex3f(0,40,0);
//	glVertex3f(0 + Q022.q1, 40 + Q022.q2, 0 + Q022.q3);
//	glEnd();
//#else
//bool flag = 0;
//long start_time = clock();//Cpu测试算法时间 起始：
//while (!flag)
//{
//	if (Rool_angleB >= 179)
//	{
//		Rool_angleB = 0;
//		Pitch_angleB += 1;
//
//		if (Pitch_angleB >= 90)
//		{
//			Pitch_angleB = -90;
//			flag = 1;
//			break;
//		}
//	}
//	else
//	{
//		Rool_angleB += 1;
//	}
//
//	PitchB = float(Pitch_angleB)*CV_PI / 180;
//	RoolB = float(Rool_angleB)*CV_PI / 180;
//	YawB = float(Yaw_angleB)*CV_PI / 180;
//
//	OLA_2_Q4(qB0, qB1, qB2, qB3, PitchB, RoolB, YawB);
//
//
//	//欧拉角转四元数旋转 覆盖所有翻滚与俯仰平面效果
//
//	GL_Quater Qt0;
//	Qt0.q0 = qB0;
//	Qt0.q1 = qB1;
//	Qt0.q2 = qB2;
//	Qt0.q3 = qB3;
//
//	//
//	GL_Point Pt;
//
//	GL_Quater Qt0_N = Conjugate_Q(Qt0); //------ 根节点四元数 求逆
//
//	GL_Quater Q_Point;
//	Q_Point.q0 = 0;
//	Q_Point.q1 = 0;
//	Q_Point.q2 = 40;
//	Q_Point.q3 = 0;//根节点骨架长度
//
//	//四元数点局部坐标系更新计算 Loc_New_Point=Q*Loc_Old_Point*Q_N
//	//------计算1次相乘
//	GL_Quater Q011 = MUL_Q(Qt0, Q_Point);
//	//------计算2次相乘
//	GL_Quater Q022 = MUL_Q(Q011, Qt0_N);
//}
//
//long end_time = clock();//Cpu 算法计时完成
//float timex = float(end_time - start_time);
//printf("180*180姿态覆盖 = %f 毫秒\n", timex);
//#endif
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

	//(0,0,z)点
	HeadPlay_Pt[9]=GL_Quater_Point(Qt0, HeadPlay_PtB[9], 0,1,-1, 0,0,0);

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
// V ：为扫描射线 向量
// (m1,m2,m3):为扫描球原点 坐标
//  CV_PointN:为二维映射点
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
		t=u1/u2;//（0,0,180）Z=180 平面的 射线与 （Z=180）的相交点 比例尺

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
			//获取两平面的交线向量
			float bx=GL_PlanXS[i].A;
			float by=GL_PlanXS[i].B;
			float bz=GL_PlanXS[i].C;

			float ax=GL_PlanYS[i].A;
			float ay=GL_PlanYS[i].B;
			float az=GL_PlanYS[i].C;

			//------------AX+BY+CZ+D=0
			float Da=GL_PlanYS[i].D;
			float Db=GL_PlanXS[i].D;

			//获得平面叉积 法向量,即两平面的交线向量
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
			//if (i < 3)//为了方便看 Quick算法
			{
				glLineWidth(1);
				glBegin(GL_LINES);
				glColor3f(0.05f, 0.6f, 0.9f);
				glVertex3f(LineRays[i].pt0.x, LineRays[i].pt0.y, LineRays[i].pt0.z);
				glVertex3f(LineRays[i].pt0.x + LineRays[i].x*Dis,
					LineRays[i].pt0.y + LineRays[i].y*Dis,
					LineRays[i].pt0.z + LineRays[i].z*Dis);
				glEnd();
			}

			//--------------
			//glPushMatrix();//储存当前视图矩阵
			//glLineWidth(1); 
			//glColor3f(0.8,0.8,0.8); 
			//glTranslatef(HeadPlay_Pt[i].x,HeadPlay_Pt[i].y,HeadPlay_Pt[i].z);
			//glutSolidSphere(0.8, 20, 20);
			//glPopMatrix();//弹出上次保存的位置
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


/*************************************************************************************/
void GLB_Line_3Point_Quick()
{
	/*HeadPlay_Pt[0] = GL_Quater_Point(Qt0, HeadPlay_PtB[0], 0, 0, 0, 1.0, 0.1, 1.0);

	HeadPlay_Pt[1] = GL_Quater_Point(Qt0, HeadPlay_PtB[1], 5, 0, 0, 1, 0, 0);
	HeadPlay_Pt[2] = GL_Quater_Point(Qt0, HeadPlay_PtB[2], -5, 0, 0, 1, 0, 0);*/
	
	int ID_Pt[3] = { 1, 0, 2 };

	//射线向量
	float a1 = LineRays[ID_Pt[0]].x; float b1 = LineRays[ID_Pt[0]].y; float c1 = LineRays[ID_Pt[0]].z;
	float a2 = LineRays[ID_Pt[1]].x; float b2 = LineRays[ID_Pt[1]].y; float c2 = LineRays[ID_Pt[1]].z;
	float a3 = LineRays[ID_Pt[2]].x; float b3 = LineRays[ID_Pt[2]].y; float c3 = LineRays[ID_Pt[2]].z;

	float k = 1.0f;

	float t2 = 100.0f;
	float t3 = 0;
	
	if (a1 != 0 && b1 != 0)
	{
		if ((b3*k / b1 - a3*k / a1) != 0)
		{
			t3 = (b2*t2 / b1 + b2*k*t2 / b1 - a2*t2 / a1 - a2*k*t2 / a1) / (b3*k / b1 - a3*k / a1);
		}
	}
	
	float t1 = 0;
	
	if (a1 != 0)
	{
		t1 = a2*t2 / a1 - k*(a3*t3 - a2*t2)/a1;
	}

	printf("/t t1=%f t2=%f t3=%f\n", t1, t2, t3);

	float x1 = a1*t1 + LineRays[ID_Pt[0]].pt0.x;
	float y1 = b1*t1 + LineRays[ID_Pt[0]].pt0.y;
	float z1 = c1*t1 + LineRays[ID_Pt[0]].pt0.z;

	float x2 = a2*t2 + LineRays[ID_Pt[1]].pt0.x;
	float y2 = b2*t2 + LineRays[ID_Pt[1]].pt0.y;
	float z2 = c2*t2 + LineRays[ID_Pt[1]].pt0.z;

	float x3 = a3*t3 + LineRays[ID_Pt[2]].pt0.x;
	float y3 = b3*t3 + LineRays[ID_Pt[2]].pt0.y;
	float z3 = c3*t3 + LineRays[ID_Pt[2]].pt0.z;

	//float dst = (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + (z1 - z2)*(z1 - z2);
	float dst = (x3 - x2)*(x3 - x2) + (y3 - y2)*(y3 - y2) + (z3 - z2)*(z3 - z2);
	dst = sqrt(dst);

	float k_ture = 0;

	if (dst != 0)
	{
		printf("100-距离=%f\n", dst);
		/*k_ture = 5.0f / dst;
		printf("真实比例=%f\n",k_ture);*/

		/*t1 = sqrt((a1*t1)*(a1*t1) + (b1*t1)*(b1*t1) + (c1*t1)*(c1*t1)) * 5 / dst;
		t2 = sqrt((a2*t2)*(a2*t2) + (b2*t2)*(b2*t2) + (c2*t2)*(c2*t2)) * 5 / dst;
		t3 = sqrt((a3*t3)*(a3*t3) + (b3*t3)*(b3*t3) + (c3*t3)*(c3*t3)) * 5 / dst;*/
		
		t1 = t1*5 / dst;
		t2 = t2*5 / dst;
		t3 = t3*5 / dst;

		float x = LineRays[ID_Pt[1]].x*t2 + LineRays[ID_Pt[1]].pt0.x;
		float y = LineRays[ID_Pt[1]].y*t2 + LineRays[ID_Pt[1]].pt0.y;
		float z = LineRays[ID_Pt[1]].z*t2 + LineRays[ID_Pt[1]].pt0.z;

		x_Quick[1] = x;
		y_Quick[1] = y;
		z_Quick[1] = z;

		printf("Quick:真实值(%f,%f,%f)\n", pos_x, pos_y, pos_z);
		printf("Quick:估计值(%f,%f,%f)\n", x, y, z);

		x = LineRays[ID_Pt[0]].x*t1 + LineRays[ID_Pt[0]].pt0.x;
		y = LineRays[ID_Pt[0]].y*t1 + LineRays[ID_Pt[0]].pt0.y;
		z = LineRays[ID_Pt[0]].z*t1 + LineRays[ID_Pt[0]].pt0.z;

		x_Quick[0] = x;
		y_Quick[0] = y;
		z_Quick[0] = z;

		x = LineRays[ID_Pt[2]].x*t3 + LineRays[ID_Pt[2]].pt0.x;
		y = LineRays[ID_Pt[2]].y*t3 + LineRays[ID_Pt[2]].pt0.y;
		z = LineRays[ID_Pt[2]].z*t3 + LineRays[ID_Pt[2]].pt0.z;

		x_Quick[2] = x;
		y_Quick[2] = y;
		z_Quick[2] = z;
		

		

	}
	
}
/***************************** 边长距离建模 3点 最速迭代 *****************************/
// DiguNum : 记录递归算法小循环
// DiguCnt : 记录递归算法大循环 N*65535
// Point_Check ：模式选择：算法点数
// ID_Pt：拟合的ID点号 （例子：0,4,5,6）
// ID_Length: 对应的ID点之间长度：
//            对应规则：// 1->2
//						   2->3
//						   1->3
//						   1->4
//						   2->4
//						   3->4
//例子：float E12=16;// 1->2
//	 float E22=16;// 2->3
//	 float E32=32;// 1->3
//	 float E42=32;// 1->4
//	 float E52=16;// 2->4
//	 float E62=64;// 3->4
// tN_GlobalS_4N：四点拟合二维数组
// numc:四点拟合的组数

//tN_GlobalS_4N :为射线的变量，因为模型有四条射线所以数组为[][4].
//Energy_GlobalN:能量函数 （分为：Energy_GlobalA 和 Energy_GlobalB，用于能量比较，从而改变迭代步长tStep_nn_GX）。

//Flag_Step     :为1时迭代步长加 STEPNN，为0时迭代步长减 STEPNN。
//Rude_Step_Control：扰动步长控制旋转
void GL_Build_Steepest_M3Point(int *ID_Pt, float*ID_Length, float tN_GlobalS_3N[][4], float* Energy_GlobalN, int numc,
	int &Point_Check, int &DiguCnt, int &DiguNum, bool Flag_Step, int Rude_Step_Control)
{
	//当多组合时，DiguNum 与 DiguCnt必须清零。
	DiguNum = 0;
	DiguCnt = 0;

	Point_Check = 3;//模式选择：算法点数
	float t1 = tN_GlobalS_3N[numc][0];
	float t2 = tN_GlobalS_3N[numc][1];
	float t3 = tN_GlobalS_3N[numc][2];
	float t1b = 170, t2b = 170, t3b = 170;
	//-------------------
	float Ft1, Ft2, Ft3;
	
	//射线向量
	float a1 = LineRays[ID_Pt[0]].x; float b1 = LineRays[ID_Pt[0]].y; float c1 = LineRays[ID_Pt[0]].z;
	float a2 = LineRays[ID_Pt[1]].x; float b2 = LineRays[ID_Pt[1]].y; float c2 = LineRays[ID_Pt[1]].z;
	float a3 = LineRays[ID_Pt[2]].x; float b3 = LineRays[ID_Pt[2]].y; float c3 = LineRays[ID_Pt[2]].z;
	// 
	GL_Vector pt0[3];
	pt0[0] = LineRays[ID_Pt[0]].pt0;
	pt0[1] = LineRays[ID_Pt[1]].pt0;
	pt0[2] = LineRays[ID_Pt[2]].pt0;
	//
	float x = a1*t1;
	float y = b1*t1;
	float z = c1*t1;
	float track_dst = GL_Distance(0, 0, 0, x, y, z);

	/**********************************************************/
	Fps_Track_Start = 1;
	//边长平方
	float E12 = ID_Length[0];// 1->2
	float E22 = ID_Length[1];// 2->3
	float E32 = ID_Length[2];// 1->3
	
	//------构建模型系数
	float(*knn)[6] = new float[3][6];
	GLB_Knn2(knn, a1, b1, c1, pt0[0], a2, b2, c2, pt0[1], 0);
	GLB_Knn2(knn, a2, b2, c2, pt0[1], a3, b3, c3, pt0[2], 1);
	GLB_Knn2(knn, a1, b1, c1, pt0[0], a3, b3, c3, pt0[2], 2);

	// F(t1,t2,t3)=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2+knn[0][3]*t1+knn[0][4]*t2+knn[0][5]-E12)*(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2+knn[0][3]*t1+knn[0][4]*t2+knn[0][5]-E12)
	//            +(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3+knn[1][3]*t2+knn[1][4]*t3+knn[1][5]-E22)*(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3+knn[1][3]*t2+knn[1][4]*t3+knn[1][5]-E22)
	//            +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3+knn[2][3]*t1+knn[2][4]*t3+knn[2][5]-E32)*(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3+knn[2][3]*t1+knn[2][4]*t3+knn[2][5]-E32)

	
	//-----------------
	// F(t1,t2,t3)/t1偏导数=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2+knn[0][3]*t1+knn[0][4]*t2+knn[0][5]-E12)* (2*knn[0][0]*t1+knn[0][2]*t2+knn[0][3])
	//                     +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3+knn[2][3]*t1+knn[2][4]*t3+knn[2][5]-E32)* (2*knn[2][0]*t1+knn[2][2]*t3+knn[2][3])
	
	// F(t1,t2,t3)/t2偏导数=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2+knn[0][3]*t1+knn[0][4]*t2+knn[0][5]-E12)* (2*knn[0][1]*t2+knn[0][2]*t1+knn[0][4])
	//                     +(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3+knn[1][3]*t2+knn[1][4]*t3+knn[1][5]-E22)* (2*knn[1][0]*t2+knn[1][2]*t3+knn[1][3])
	
	// F(t1,t2,t3)/t3偏导数=(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3+knn[1][3]*t2+knn[1][4]*t3+knn[1][5]-E22)* (2*knn[1][1]*t3+knn[1][2]*t2+knn[1][4])
	//                     +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3+knn[2][3]*t1+knn[2][4]*t3+knn[2][5]-E32)* (2*knn[2][1]*t3+knn[2][2]*t1+knn[2][4])
	
	//-------角度约束
	//GL_Vector Line1_V;// 0->1
	//Line1_V.x=a1*t1-a2*t2;
	//Line1_V.y=b1*t1-b2*t2;
	//Line1_V.z=c1*t1-c2*t2;

	//GL_Vector Line2_V;// 2->3
	//Line1_V.x=a3*t3-a2*t2;
	//Line1_V.y=b3*t3-b2*t2;
	//Line1_V.z=c3*t3-c2*t2;

	//---------------------------------------------------------------------------------------------------------------------
	// 角度约束函数 0->1 2->3 夹角：F_Angle1
	//                   =( (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) )
	//                   *( (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) );

	// F_Angle/t1偏导数=2*( (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) )
	//                 *( (a1)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) )

	// F_Angle/t2偏导数=2*( (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) )
	//                 *(   (-a2)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(-a2)
	//                    + (-b2)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(-b2)
	//                    + (-c2)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(-c2))

	// F_Angle/t3偏导数=2*( (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) )
	//                 *(  (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3) 
	//                   + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3) 
	//                   + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3) )
	/*float Energy_Min = 65535;
	float t_min[3];
	t_min[0] = t1;
	t_min[1] = t2;
	t_min[2] = t3;*/
	
	//-------------------------------------------------------------
	bool Flag_DiGu = 0;//递归运算运行标识符
	float nn = 0.002f;// 递归步长 适中（不能太大也不能太小）。
	//float (*m)[3] = new float[3][3];
	while (!Flag_DiGu)
	{
		if (DiguCnt>=4)break;//递归次数大循环 限制
		DiguNum = 0;
		DiguCnt++;
		int i = 0;
		
		for (i = 0; i<65535; i++)
		{
			// 3点递归
				float D1 = (knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12);
				float D2 = (knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22);
				float D3 = (knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32);

				Ft1 = D1* (2.0f * knn[0][0] * t1 + knn[0][2] * t2 + knn[0][3])
					+ D3* (2.0f * knn[2][0] * t1 + knn[2][2] * t3 + knn[2][3])
					;

				Ft2 = D1* (2.0f * knn[0][1] * t2 + knn[0][2] * t1 + knn[0][4])
					+ D2* (2.0f * knn[1][0] * t2 + knn[1][2] * t3 + knn[1][3])
					;

				Ft3 = D2* (2.0f * knn[1][1] * t3 + knn[1][2] * t2 + knn[1][4])
					+ D3* (2.0f * knn[2][1] * t3 + knn[2][2] * t1 + knn[2][4])
					;
				//------ 雅克比矩阵 最速下降法算法步长必须 雅克比矩阵为正定矩阵
	/*			m[0][0] = D1* (2 * knn[0][0] * t1 + knn[0][2] * t2 + knn[0][3]);
				m[0][1] = D1* (2 * knn[0][1] * t2 + knn[0][2] * t1 + knn[0][4]);
				m[0][2] = 0;

				m[1][0] = 0;
				m[1][1] = D2* (2 * knn[1][0] * t2 + knn[1][2] * t3 + knn[1][3]);
				m[1][2] = D2* (2 * knn[1][1] * t3 + knn[1][2] * t2 + knn[1][4]);

				m[2][0] = D3* (2 * knn[2][0] * t1 + knn[2][2] * t3 + knn[2][3]);
				m[2][1] = 0;
				m[2][2] = D3* (2 * knn[2][1] * t3 + knn[2][2] * t1 + knn[2][4]);

				float Fz = Ft1*Ft1 + Ft2*Ft2 + Ft3*Ft3;//步长分子
				//步长分母
				float Fm1 = Ft1*m[0][0] + Ft2*m[1][0] + Ft3*m[2][0];
				float Fm2 = Ft1*m[0][1] + Ft2*m[1][1] + Ft3*m[2][1];
				float Fm3 = Ft1*m[0][2] + Ft2*m[1][2] + Ft3*m[2][2];
				//
				nn = 0.002f;
				float Fm = Ft1*Fm1 + Ft2*Fm2 + Ft3*Fm3;
				
				if (Fm != 0)
				{
					nn = Fz / Fm;
				}
*/
			float t1br = t1 - nn*Ft1;
			float t2br = t2 - nn*Ft2;
			float t3br = t3 - nn*Ft3;

			//----------------------------
			if ((t1br*c1) > 0 && (t1br*c1) < 350 && (t2br*c2) > 0 && (t2br*c2) < 350 && (t3br*c3) > 0 && (t3br*c3) < 350)
			{
				t1b = t1br;
				t2b = t2br;
				t3b = t3br;
			}
			else
			{
				//printf("----------------------------------------------- >>>> 计算最优峰值 溢出报错！！！\n");
				//printf("tnbr:(%f,%f,%f)\n", t1br, t2br, t3br);
				Flag_DiGu = 1;
				break;
			}
			
			//------ 当梯度变化趋势很小时，认为找到极值点，跳出循环。
			if (ABS(Ft1)<0.003 && ABS(Ft2)<0.003 && ABS(Ft3)<0.003)//if (t1 == t1b && t2 == t2b && t3 == t3b )
			{
				t1 = t1b;
				t2 = t2b;
				t3 = t3b;
				Flag_DiGu = 1;
				break;
			}
			
			t1 = t1b;
			t2 = t2b;
			t3 = t3b;
		}

		DiguNum = i;
	}
	
	//---------------------------------------------------------------------------------------------------------------------------------------------------
	//在小区域内扰动
#if 0
	t1 += float(rand() % 101 - 50) / 50.0f *3.3f;
	t2 += float(rand() % 101 - 50) / 50.0f *3.3f;
	t3 += float(rand() % 101 - 50) / 50.0f *3.3f;
	t4 += float(rand() % 101 - 50) / 50.0f *3.3f;
	if (Rude_Step_Control == 0)
	{
		nn = 0.001613;
	}
	else if (Rude_Step_Control == 1)
	{
		nn = 0.000753;
	}

	int DiguCnt_R = 0;
	int DiguNum_R = 0;
	while (DiguCnt_R == 0 || (DiguCnt_R != 0 && DiguNum_R == 65535))
	{
		/*if(DiguCnt>15)nn=0.000023;*/
		if (DiguCnt_R>8)break;
		DiguNum_R = 0;
		DiguCnt_R++;
		int i = 0;
		bool flag = 0;
		for (i = 0; i<65535; i++)
		{
			if (Point_Check == 4)//四点递归
			{
				Ft1 = (knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12) * (2 * knn[0][0] * t1 + knn[0][2] * t2 + knn[0][3])
					+ (knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32) * (2 * knn[2][0] * t1 + knn[2][2] * t3 + knn[2][3])
					+ (knn[3][0] * t1*t1 + knn[3][1] * t4*t4 + knn[3][2] * t1*t4 + knn[3][3] * t1 + knn[3][4] * t4 + knn[3][5] - E42) * (2 * knn[3][0] * t1 + knn[3][2] * t4 + knn[3][3])
					/*+0.2f*((a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) )
					*( (a1)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) )*/
					;

				Ft2 = (knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12) * (2 * knn[0][1] * t2 + knn[0][2] * t1 + knn[0][4])
					+ (knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22) * (2 * knn[1][0] * t2 + knn[1][2] * t3 + knn[1][3])
					+ (knn[4][0] * t2*t2 + knn[4][1] * t4*t4 + knn[4][2] * t2*t4 + knn[4][3] * t2 + knn[4][4] * t4 + knn[4][5] - E52) * (2 * knn[4][0] * t2 + knn[4][2] * t4 + knn[4][3])
					/*+0.2f*( (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) )
					*(   (-a2)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(-a2)
					+ (-b2)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(-b2)
					+ (-c2)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(-c2))*/
					;

				Ft3 = (knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22) * (2 * knn[1][1] * t3 + knn[1][2] * t2 + knn[1][4])
					+ (knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32) * (2 * knn[2][1] * t3 + knn[2][2] * t1 + knn[2][4])
					+ (knn[5][0] * t3*t3 + knn[5][1] * t4*t4 + knn[5][2] * t3*t4 + knn[5][3] * t3 + knn[5][4] * t4 + knn[5][5] - E62) * (2 * knn[5][0] * t3 + knn[5][2] * t4 + knn[5][3])
					/* +0.2f*( (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) )
					*(  (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3)
					+ (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3)
					+ (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3) )*/
					;

				Ft4 = (knn[3][0] * t1*t1 + knn[3][1] * t4*t4 + knn[3][2] * t1*t4 + knn[3][3] * t1 + knn[3][4] * t4 + knn[3][5] - E42) * (2 * knn[3][1] * t4 + knn[3][2] * t1 + knn[3][4])
					+ (knn[4][0] * t2*t2 + knn[4][1] * t4*t4 + knn[4][2] * t2*t4 + knn[4][3] * t2 + knn[4][4] * t4 + knn[4][5] - E52) * (2 * knn[4][1] * t4 + knn[4][2] * t2 + knn[4][4])
					+ (knn[5][0] * t3*t3 + knn[5][1] * t4*t4 + knn[5][2] * t3*t4 + knn[5][3] * t3 + knn[5][4] * t4 + knn[5][5] - E62) * (2 * knn[5][1] * t4 + knn[5][2] * t3 + knn[5][4])
					;
			}
			//-----------------------------------------------------------------------------------------------------------
			t1b = t1 - nn*Ft1;
			t2b = t2 - nn*Ft2;
			t3b = t3 - nn*Ft3;
			t4b = t4 - nn*Ft4;


			if (Point_Check == 4)
			{
				if (t1 == t1b && t2 == t2b && t3 == t3b &&t4 == t4b)// && (ABS(t1-t1b)<0.005 && ABS(t2-t2b)<0.005 && ABS(t3-t3b)<0.005 && ABS(t4-t4b)<0.005)) 
				{
					flag = 1;
					break;
				}
			}

			t1 = t1b;
			t2 = t2b;
			t3 = t3b;
			t4 = t4b;
		}
		if (flag == 1)
			DiguNum_R = 1;
		else
			DiguNum_R = i;
	}

	printf("/******************************************* 扰动后递归 ************************************************/\n");
	printf("扰动后递归次数：65535*%d + %d\n", DiguCnt_R, DiguNum_R);
#endif
	//将递归结果返回全局变量保存
	tN_GlobalS_3N[numc][0] = t1;
	tN_GlobalS_3N[numc][1] = t2;
	tN_GlobalS_3N[numc][2] = t3;
	

	/* tN_GlobalS_4N[numc][0] = t_min[0];
	tN_GlobalS_4N[numc][1] = t_min[1];
	tN_GlobalS_4N[numc][2] = t_min[2];
	tN_GlobalS_4N[numc][3] = t_min[3];*/

	/* t1=t_min[0];
	t2=t_min[1];
	t3=t_min[2];
	t4=t_min[3];*/

	Energy_GlobalN[numc] = (knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12)*(knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12)
		+ (knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22)*(knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22)
		+ (knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32)*(knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32)
		;

	delete[]knn;
	//delete[]m;
	//---------------------------------------------------
}

/*********************************** 尾递归 **********************************/
int WDiGui(int &i,float knn[][6],float t1,float t2,float t3,float t4,float t1b,float t2b,float t3b,float t4b,float E12,float E32,float E42,float E22,float E52,float E62)
{
	float Ft1, Ft2, Ft3, Ft4;
	float nn = 0.01531;
	//四点递归
	
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
	

	//-----------------------------------------------------------------------------------------------------------
	/*	 if (i < 20000)
	{
	nn = 0.002315;
	}
	else
	{
	nn = 0.001513;
	}*/
	t1b = t1 - nn*Ft1;
	t2b = t2 - nn*Ft2;
	t3b = t3 - nn*Ft3;
	t4b = t4 - nn*Ft4;

	i++;
	if (i > 65525) return 0;

	if (ABS(Ft1) < 0.007 && ABS(Ft2) < 0.007 && ABS(Ft3) < 0.007 && ABS(Ft4) < 0.007)// && (ABS(t1-t1b)<0.005 && ABS(t2-t2b)<0.005 && ABS(t3-t3b)<0.005 && ABS(t4-t4b)<0.005)) 
	{
		t1 = t1b;
		t2 = t2b;
		t3 = t3b;
		t4 = t4b;
		return 0;
	}
	else
		return WDiGui(i, knn, t1, t2, t3, t4, t1b, t2b, t3b, t4b,  E12,  E32,  E42,  E22,  E52,E62);
}
/***************************** 边长距离建模 4点 最速迭代 *****************************/
// DiguNum : 记录递归算法小循环
// DiguCnt : 记录递归算法大循环 N*65535
// Point_Check ：模式选择：算法点数
// ID_Pt：拟合的ID点号 （例子：0,4,5,6）
// ID_Length: 对应的ID点之间长度：
//            对应规则：// 1->2
//						   2->3
//						   1->3
//						   1->4
//						   2->4
//						   3->4
 //例子：float E12=16;// 1->2
	//	 float E22=16;// 2->3
	//	 float E32=32;// 1->3
	//	 float E42=32;// 1->4
	//	 float E52=16;// 2->4
	//	 float E62=64;// 3->4
// tN_GlobalS_4N：四点拟合二维数组
// numc:四点拟合的组数

//tN_GlobalS_4N :为射线的变量，因为模型有四条射线所以数组为[][4].
//Energy_GlobalN:能量函数 （分为：Energy_GlobalA 和 Energy_GlobalB，用于能量比较，从而改变迭代步长tStep_nn_GX）。
//tStep_nn_GX   :迭代算法步长。
//Flag_Step     :为1时迭代步长加 STEPNN，为0时迭代步长减 STEPNN。
//Rude_Step_Control：扰动步长控制旋转
void GL_Build_Steepest_M4Point(int *ID_Pt,float*ID_Length,float tN_GlobalS_4N[][4],float* Energy_GlobalN,float *tStep_nn_GX,int numc,
	                     int &Point_Check,int &DiguCnt,int &DiguNum,bool Flag_Step,int Rude_Step_Control )
{
	//当多组合时，DiguNum 与 DiguCnt必须清零。
	DiguNum=0;
	DiguCnt=0;

	Point_Check=4;//模式选择：算法点数
	float t1=tN_GlobalS_4N[numc][0];
	float t2=tN_GlobalS_4N[numc][1];
	float t3=tN_GlobalS_4N[numc][2];
	float t4=tN_GlobalS_4N[numc][3];
	//-------------------
	float Ft1,Ft2,Ft3,Ft4;
	float t1b=170,t2b=170,t3b=170,t4b=170;;

	//射线向量
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
	//越靠近扫描点 步长越小 -->> 0.00268531

	//float nn=0.002515;// 递归步长 适中（不能太大也不能太小）。
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

	float nn = 0.001815;// 递归步长 适中（不能太大也不能太小）。
	/*if (track_dst < 160)
	{
		nn = 0.001815;
	}

	if (!Fps_Track_Start)
	{
		nn = 0.001815;
	}*/

	Fps_Track_Start = 1;
	//边长平方
	float E12=ID_Length[0];//16;// 1->2
	float E22=ID_Length[1];//16;// 2->3
	float E32=ID_Length[2];//32;// 1->3
	float E42=ID_Length[3];//32;// 1->4
	float E52=ID_Length[4];//16;// 2->4
	float E62=ID_Length[5];//64;// 3->4
	//------构建模型系数

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
	// F(t1,t2,t3)/t1偏导数=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2+knn[0][3]*t1+knn[0][4]*t2+knn[0][5]-E12)* (2*knn[0][0]*t1+knn[0][2]*t2+knn[0][3])
	//                     +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3+knn[2][3]*t1+knn[2][4]*t3+knn[2][5]-E32)* (2*knn[2][0]*t1+knn[2][2]*t3+knn[2][3])
	//                     +(knn[3][0]*t1*t1+knn[3][1]*t4*t4+knn[3][2]*t1*t4+knn[3][3]*t1+knn[3][4]*t4+knn[3][5]-E42)* (2*knn[3][0]*t1+knn[3][2]*t4+knn[3][3])

	// F(t1,t2,t3)/t2偏导数=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2+knn[0][3]*t1+knn[0][4]*t2+knn[0][5]-E12)* (2*knn[0][1]*t2+knn[0][2]*t1+knn[0][4])
	//                     +(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3+knn[1][3]*t2+knn[1][4]*t3+knn[1][5]-E22)* (2*knn[1][0]*t2+knn[1][2]*t3+knn[1][3])
	//                     +(knn[4][0]*t2*t2+knn[4][1]*t4*t4+knn[4][2]*t2*t4+knn[4][3]*t2+knn[4][4]*t4+knn[4][5]-E52)* (2*knn[4][0]*t2+knn[4][2]*t4+knn[4][3])

	// F(t1,t2,t3)/t3偏导数=(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3+knn[1][3]*t2+knn[1][4]*t3+knn[1][5]-E22)* (2*knn[1][1]*t3+knn[1][2]*t2+knn[1][4])
	//                     +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3+knn[2][3]*t1+knn[2][4]*t3+knn[2][5]-E32)* (2*knn[2][1]*t3+knn[2][2]*t1+knn[2][4])
	//                     +(knn[5][0]*t3*t3+knn[5][1]*t4*t4+knn[5][2]*t3*t4+knn[5][3]*t3+knn[5][4]*t4+knn[5][5]-E62)* (2*knn[5][0]*t3+knn[5][2]*t4+knn[5][3])

	// F(t1,t2,t3)/t4偏导数=(knn[3][0]*t1*t1+knn[3][1]*t4*t4+knn[3][2]*t1*t4+knn[3][3]*t1+knn[3][4]*t4+knn[3][5]-E42)* (2*knn[3][1]*t4+knn[3][2]*t1+knn[3][4])
	//                     +(knn[4][0]*t2*t2+knn[4][1]*t4*t4+knn[4][2]*t2*t4+knn[4][3]*t2+knn[4][4]*t4+knn[4][5]-E52)* (2*knn[4][1]*t4+knn[4][2]*t2+knn[4][4])
	//                     +(knn[5][0]*t3*t3+knn[5][1]*t4*t4+knn[5][2]*t3*t4+knn[5][3]*t3+knn[5][4]*t4+knn[5][5]-E62)* (2*knn[5][1]*t4+knn[5][2]*t3+knn[5][4])
	//-------角度约束
	//GL_Vector Line1_V;// 0->1
	//Line1_V.x=a1*t1-a2*t2;
	//Line1_V.y=b1*t1-b2*t2;
	//Line1_V.z=c1*t1-c2*t2;

	//GL_Vector Line2_V;// 2->3
	//Line1_V.x=a3*t3-a2*t2;
	//Line1_V.y=b3*t3-b2*t2;
	//Line1_V.z=c3*t3-c2*t2;

	//---------------------------------------------------------------------------------------------------------------------
	// 角度约束函数 0->1 2->3 夹角：F_Angle1
	//                   =( (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) )
	//                   *( (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) );


	// F_Angle/t1偏导数=2*( (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) )
	//                 *( (a1)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) )

	// F_Angle/t2偏导数=2*( (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) )
	//                 *(   (-a2)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(-a2)
	//                    + (-b2)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(-b2)
	//                    + (-c2)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(-c2))

	// F_Angle/t3偏导数=2*( (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) )
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
		bool Flag_DiGu = 0;
		while (!Flag_DiGu)
		 {
			 /*if(DiguCnt>15)nn=0.000023;*/
			 if(DiguCnt>5)break;
			 DiguNum=0;
			 
			 int i=0;
			 
			 float t1F;
			 float t2F;
			 float t3F;
			 float t4F;

			 float t12F;
			 float t13F;
			 float t14F;

			 float t23F;
			 float t24F;

			 float t34F;
		 for( i=0;i<65535;i++)
		 {
			 if(Point_Check==4)//四点递归
			 {
				  t1F = t1*t1;
				  t2F = t2*t2;
				  t3F = t3*t3;
				  t4F = t4*t4;

				  t12F = t1*t2;
				  t13F = t1*t3;
				  t14F = t1*t4;

				  t23F = t2*t3;
				  t24F = t2*t4;

				  t34F = t3*t4;

#if 1
				  float aa1 = (knn[0][0] * t1F + knn[0][1] * t2F + knn[0][2] * t12F + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12);
				  float aa2 = (2 * knn[0][0] * t1 + knn[0][2] * t2 + knn[0][3]);
				  float bb1 = (knn[2][0] * t1F + knn[2][1] * t3F + knn[2][2] * t13F + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32);
				  float bb2 = (2 * knn[2][0] * t1 + knn[2][2] * t3 + knn[2][3]);
				  float cc1 = (knn[3][0] * t1F + knn[3][1] * t4F + knn[3][2] * t14F + knn[3][3] * t1 + knn[3][4] * t4 + knn[3][5] - E42);
				  float cc2 = (2 * knn[3][0] * t1 + knn[3][2] * t4 + knn[3][3]);
				  Ft1 = aa1*aa2 + bb1*bb2 + cc1*cc2;

				  aa1 = (knn[0][0] * t1F + knn[0][1] * t2F + knn[0][2] * t12F + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12);
				  aa2 = (2 * knn[0][1] * t2 + knn[0][2] * t1 + knn[0][4]);
				  bb1 = (knn[1][0] * t2F + knn[1][1] * t3F + knn[1][2] * t23F + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22);
				  bb2 = (2 * knn[1][0] * t2 + knn[1][2] * t3 + knn[1][3]);
				  cc1 = (knn[4][0] * t2F + knn[4][1] * t4F + knn[4][2] * t24F + knn[4][3] * t2 + knn[4][4] * t4 + knn[4][5] - E52);
				  cc2 = (2 * knn[4][0] * t2 + knn[4][2] * t4 + knn[4][3]);
				  Ft2 = aa1*aa2 + bb1*bb2 + cc1*cc2;

				  aa1 = (knn[1][0] * t2F + knn[1][1] * t3F + knn[1][2] * t23F + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22);
				  aa2 = (2 * knn[1][1] * t3 + knn[1][2] * t2 + knn[1][4]);
				  bb1 = (knn[2][0] * t1F + knn[2][1] * t3F + knn[2][2] * t13F + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32);
				  bb2 = (2 * knn[2][1] * t3 + knn[2][2] * t1 + knn[2][4]);
				  cc1 = (knn[5][0] * t3F + knn[5][1] * t4F + knn[5][2] * t34F + knn[5][3] * t3 + knn[5][4] * t4 + knn[5][5] - E62);
				  cc2 = (2 * knn[5][0] * t3 + knn[5][2] * t4 + knn[5][3]);
				  Ft3 = aa1*aa2 + bb1*bb2 + cc1*cc2;

				  aa1 = (knn[3][0] * t1F + knn[3][1] * t4F + knn[3][2] * t14F + knn[3][3] * t1 + knn[3][4] * t4 + knn[3][5] - E42);
				  aa2 = (2 * knn[3][1] * t4 + knn[3][2] * t1 + knn[3][4]);
				  bb1 = (knn[4][0] * t2F + knn[4][1] * t4F + knn[4][2] * t24F + knn[4][3] * t2 + knn[4][4] * t4 + knn[4][5] - E52);
				  bb2 = (2 * knn[4][1] * t4 + knn[4][2] * t2 + knn[4][4]);
				  cc1 = (knn[5][0] * t3F + knn[5][1] * t4F + knn[5][2] * t34F + knn[5][3] * t3 + knn[5][4] * t4 + knn[5][5] - E62);
				  cc2 = (2 * knn[5][1] * t4 + knn[5][2] * t3 + knn[5][4]);
				  Ft4 = aa1*aa2 + bb1*bb2 + cc1*cc2;
					  //
#else
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
#endif
			 }
			
			//-----------------------------------------------------------------------------------------------------------

		/*	 if (i < 20000)
			 {
				 nn = 0.002315;
			 }
			 else
			 {
				 nn = 0.001513;
			 }*/
			 t1b = t1 - nn*Ft1;
			 t2b = t2 - nn*Ft2;
			 t3b = t3 - nn*Ft3;
			 t4b = t4 - nn*Ft4;

			/* float  EnergyN = (knn[0][0] * t1b*t1b + knn[0][1] * t2b*t2b + knn[0][2] * t1b*t2b + knn[0][3] * t1b + knn[0][4] * t2b + knn[0][5] - E12)*(knn[0][0] * t1b*t1b + knn[0][1] * t2b*t2b + knn[0][2] * t1b*t2b + knn[0][3] * t1b + knn[0][4] * t2b + knn[0][5] - E12)
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
			 }*/

			
			 if (ABS(Ft1)<0.007 && ABS(Ft2)<0.007 && ABS(Ft3)<0.007 && ABS(Ft4)<0.007)// && (ABS(t1-t1b)<0.005 && ABS(t2-t2b)<0.005 && ABS(t3-t3b)<0.005 && ABS(t4-t4b)<0.005)) 
			{
				t1 = t1b;
				t2 = t2b;
				t3 = t3b;
				t4 = t4b;
				Flag_DiGu = 1;
				break;
			}



			 t1=t1b;
			 t2=t2b;
			 t3=t3b;
			 t4=t4b;
		 }
		 DiguCnt++;
		 DiguNum = i;
		 }

		 //---------------------------------------------------------------------------------------------------------------------------------------------------
		 //在小区域内扰动
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
			 if(Point_Check==4)//四点递归
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

		 printf("/******************************************* 扰动后递归 ************************************************/\n");
		 printf("扰动后递归次数：65535*%d + %d\n",DiguCnt_R,DiguNum_R);
#endif
		 //将递归结果返回全局变量保存
		 tN_GlobalS_4N[numc][0]=t1;
		 tN_GlobalS_4N[numc][1]=t2;
		 tN_GlobalS_4N[numc][2]=t3;
		 tN_GlobalS_4N[numc][3]=t4;

		/* tN_GlobalS_4N[numc][0] = t_min[0];
		 tN_GlobalS_4N[numc][1] = t_min[1];
		 tN_GlobalS_4N[numc][2] = t_min[2];
		 tN_GlobalS_4N[numc][3] = t_min[3];*/

		/* t1=t_min[0];
		 t2=t_min[1];
		 t3=t_min[2];
		 t4=t_min[3];*/

		 Energy_GlobalN[numc]=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2+knn[0][3]*t1+knn[0][4]*t2+knn[0][5]-E12)*(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2+knn[0][3]*t1+knn[0][4]*t2+knn[0][5]-E12)
			             +(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3+knn[1][3]*t2+knn[1][4]*t3+knn[1][5]-E22)*(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3+knn[1][3]*t2+knn[1][4]*t3+knn[1][5]-E22)
			             +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3+knn[2][3]*t1+knn[2][4]*t3+knn[2][5]-E32)*(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3+knn[2][3]*t1+knn[2][4]*t3+knn[2][5]-E32)

			             +(knn[3][0]*t1*t1+knn[3][1]*t4*t4+knn[3][2]*t1*t4+knn[3][3]*t1+knn[3][4]*t4+knn[3][5]-E42)*(knn[3][0]*t1*t1+knn[3][1]*t4*t4+knn[3][2]*t1*t4+knn[3][3]*t1+knn[3][4]*t4+knn[3][5]-E42)
			             +(knn[4][0]*t2*t2+knn[4][1]*t4*t4+knn[4][2]*t2*t4+knn[4][3]*t2+knn[4][4]*t4+knn[4][5]-E52)*(knn[4][0]*t2*t2+knn[4][1]*t4*t4+knn[4][2]*t2*t4+knn[4][3]*t2+knn[4][4]*t4+knn[4][5]-E52)
			             +(knn[5][0]*t3*t3+knn[5][1]*t4*t4+knn[5][2]*t3*t4+knn[5][3]*t3+knn[5][4]*t4+knn[5][5]-E62)*(knn[5][0]*t3*t3+knn[5][1]*t4*t4+knn[5][2]*t3*t4+knn[5][3]*t3+knn[5][4]*t4+knn[5][5]-E62)
						 ;
		 //printf("能量值 = %f \n", Energy_GlobalN[numc]);
		 //float u1 = (knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12)*(knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12);
		 //float u2 = (knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22)*(knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22);
		 //float u3 = (knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32)*(knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32);

		 //float u4 = (knn[3][0] * t1*t1 + knn[3][1] * t4*t4 + knn[3][2] * t1*t4 + knn[3][3] * t1 + knn[3][4] * t4 + knn[3][5] - E42)*(knn[3][0] * t1*t1 + knn[3][1] * t4*t4 + knn[3][2] * t1*t4 + knn[3][3] * t1 + knn[3][4] * t4 + knn[3][5] - E42);
		 //float u5 = (knn[4][0] * t2*t2 + knn[4][1] * t4*t4 + knn[4][2] * t2*t4 + knn[4][3] * t2 + knn[4][4] * t4 + knn[4][5] - E52)*(knn[4][0] * t2*t2 + knn[4][1] * t4*t4 + knn[4][2] * t2*t4 + knn[4][3] * t2 + knn[4][4] * t4 + knn[4][5] - E52);
		 //float u6 = (knn[5][0] * t3*t3 + knn[5][1] * t4*t4 + knn[5][2] * t3*t4 + knn[5][3] * t3 + knn[5][4] * t4 + knn[5][5] - E62)*(knn[5][0] * t3*t3 + knn[5][1] * t4*t4 + knn[5][2] * t3*t4 + knn[5][3] * t3 + knn[5][4] * t4 + knn[5][5] - E62);
		 //printf("u1=%f\n", u1);
		 //printf("u2=%f\n", u2);
		 //printf("u3=%f\n", u3);
		 //printf("u4=%f\n", u4);
		 //printf("u5=%f\n", u5);
		 //printf("u6=%f\n", u6);
		 //printf("\n");
		  //---------------------------------------------------
}

/***************************** 边长距离建模 最速迭代 *****************************/
// DiguNum : 记录递归算法小循环
// DiguCnt : 记录递归算法大循环 N*65535
// Point_Check ：模式选择：算法点数
void GL_Build_Steepest_M5Point(int &Point_Check,int &DiguCnt,int &DiguNum )
{
	
}

/************************************************************************/
float GL_Distance2(GL_Point pt1,GL_Point pt2)
{
	float dst = (pt1.x - pt2.x)*(pt1.x - pt2.x) + (pt1.y - pt2.y)*(pt1.y - pt2.y) + (pt1.z - pt2.z)* (pt1.z - pt2.z);
	return dst;
}
/************************************************************************/
// ID_Length: 对应的ID点之间长度：
//            对应规则：// 1->2
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



/***************************** 牛顿迭代函数 *****************************/
void GL_Build_NewTon_M3Point(int *ID_Pt, float*ID_Length, float tN_GlobalS_3N[][4], float* Energy_GlobalN, int numc,
	int &Point_Check, int &DiguCnt, int &DiguNum, bool Flag_Step, int Rude_Step_Control)
{
	//当多组合时，DiguNum 与 DiguCnt必须清零。
	DiguNum = 0;
	DiguCnt = 0;

	Point_Check = 3;//模式选择：算法点数
	float t1 = tN_GlobalS_3N[numc][0];
	float t2 = tN_GlobalS_3N[numc][1];
	float t3 = tN_GlobalS_3N[numc][2];

	//-------------------
	float Ft1, Ft2, Ft3, Ft4;
	float t1b = 170, t2b = 170, t3b = 170;

	//射线向量
	float a1 = LineRays[ID_Pt[0]].x; float b1 = LineRays[ID_Pt[0]].y; float c1 = LineRays[ID_Pt[0]].z;
	float a2 = LineRays[ID_Pt[1]].x; float b2 = LineRays[ID_Pt[1]].y; float c2 = LineRays[ID_Pt[1]].z;
	float a3 = LineRays[ID_Pt[2]].x; float b3 = LineRays[ID_Pt[2]].y; float c3 = LineRays[ID_Pt[2]].z;

	// 
	GL_Vector pt0[3];
	pt0[0] = LineRays[ID_Pt[0]].pt0;
	pt0[1] = LineRays[ID_Pt[1]].pt0;
	pt0[2] = LineRays[ID_Pt[2]].pt0;

	//
	/*float x = a1*t1;
	float y = b1*t1;
	float z = c1*t1;
	float track_dst = GL_Distance(0, 0, 0, x, y, z);*/

	/**********************************************************/
	//float nn = 0.002315;// 递归步长 适中（不能太大也不能太小）。

	//Fps_Track_Start = 1;
	//边长平方
	float E12 = ID_Length[0];// 1->2
	float E22 = ID_Length[1];// 2->3
	float E32 = ID_Length[2];// 1->3

	//------构建模型系数
	float knn[10][6];
	GLB_Knn2(knn, a1, b1, c1, pt0[0], a2, b2, c2, pt0[1], 0);
	GLB_Knn2(knn, a2, b2, c2, pt0[1], a3, b3, c3, pt0[2], 1);
	GLB_Knn2(knn, a1, b1, c1, pt0[0], a3, b3, c3, pt0[2], 2);

	//建模方程
	// (knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2+knn[0][3]*t1+knn[0][4]*t2+knn[0][5]-E12)*(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2+knn[0][3]*t1+knn[0][4]*t2+knn[0][5]-E12)=0
	// (knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3+knn[1][3]*t2+knn[1][4]*t3+knn[1][5]-E22)*(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3+knn[1][3]*t2+knn[1][4]*t3+knn[1][5]-E22)=0
	// (knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3+knn[2][3]*t1+knn[2][4]*t3+knn[2][5]-E32)*(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3+knn[2][3]*t1+knn[2][4]*t3+knn[2][5]-E32)=0


	//-----------------雅可比矩阵 3*3
	// 1*3
	// (knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2+knn[0][3]*t1+knn[0][4]*t2+knn[0][5]-E12)* (2*knn[0][0]*t1+knn[0][2]*t2+knn[0][3])
	// (knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2+knn[0][3]*t1+knn[0][4]*t2+knn[0][5]-E12)* (2*knn[0][1]*t2+knn[0][2]*t1+knn[0][4])
	//  0

	// 2*3
	// 0
	// (knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3+knn[1][3]*t2+knn[1][4]*t3+knn[1][5]-E22)* (2*knn[1][0]*t2+knn[1][2]*t3+knn[1][3])
	// (knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3+knn[1][3]*t2+knn[1][4]*t3+knn[1][5]-E22)* (2*knn[1][1]*t3+knn[1][2]*t2+knn[1][4])

	// 3*3
	// (knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3+knn[2][3]*t1+knn[2][4]*t3+knn[2][5]-E32)* (2*knn[2][0]*t1+knn[2][2]*t3+knn[2][3])
	// 0
	// (knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3+knn[2][3]*t1+knn[2][4]*t3+knn[2][5]-E32)* (2*knn[2][1]*t3+knn[2][2]*t1+knn[2][4])

	//-------角度约束
	//GL_Vector Line1_V;// 0->1
	//Line1_V.x=a1*t1-a2*t2;
	//Line1_V.y=b1*t1-b2*t2;
	//Line1_V.z=c1*t1-c2*t2;

	//GL_Vector Line2_V;// 2->3
	//Line1_V.x=a3*t3-a2*t2;
	//Line1_V.y=b3*t3-b2*t2;
	//Line1_V.z=c3*t3-c2*t2;

	//---------------------------------------------------------------------------------------------------------------------
	// 角度约束函数 0->1 2->3 夹角：F_Angle1
	//                   =( (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) )
	//                   *( (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) );

	// F_Angle/t1偏导数=2*( (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) )
	//                 *( (a1)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) )

	// F_Angle/t2偏导数=2*( (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) )
	//                 *(   (-a2)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(-a2)
	//                    + (-b2)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(-b2)
	//                    + (-c2)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(-c2))

	// F_Angle/t3偏导数=2*( (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3*t3-a2*t2+pt0[2].x-pt0[1].x) + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3*t3-b2*t2+pt0[2].y-pt0[1].y) + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3*t3-c2*t2+pt0[2].z-pt0[1].z) )
	//                 *(  (a1*t1-a2*t2+pt0[0].x-pt0[1].x)*(a3) 
	//                   + (b1*t1-b2*t2+pt0[0].y-pt0[1].y)*(b3) 
	//                   + (c1*t1-c2*t2+pt0[0].z-pt0[1].z)*(c3) )
	float m[3][3];
	float result[3][3];
	Fps_Track_Start = 1;

	//-------------------------------------------------------------
	//printf("------------------------------->>>     使用牛顿迭代法\n");
	bool Flag_DiGu = 0;
	bool Flag_NewTon_use = 0;
	while (!Flag_DiGu)
	{
		/*if(DiguCnt>15)nn=0.000023;*/
		if (DiguCnt >= 1)break;
		DiguNum = 0;
		DiguCnt++;
		int i = 0;
		
		for (i = 0; i<65535; i++)
		{
			float D1 = (knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12);
			float D2 = (knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22);
			float D3 = (knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32);
			//------ 雅克比矩阵
			m[0][0] = D1* (2 * knn[0][0] * t1 + knn[0][2] * t2 + knn[0][3]);
			m[0][1] = D1* (2 * knn[0][1] * t2 + knn[0][2] * t1 + knn[0][4]);
			m[0][2] = 0;

			m[1][0] = 0;
			m[1][1] = D2* (2 * knn[1][0] * t2 + knn[1][2] * t3 + knn[1][3]);
			m[1][2] = D2* (2 * knn[1][1] * t3 + knn[1][2] * t2 + knn[1][4]);

			m[2][0] = D3* (2 * knn[2][0] * t1 + knn[2][2] * t3 + knn[2][3]);
			m[2][1] = 0;
			m[2][2] = D3* (2 * knn[2][1] * t3 + knn[2][2] * t1 + knn[2][4]);

			//----- 求逆矩阵
			bool Flag_Good = invert(m, result);
			float E1 = 1;
			float E2 = 1;
			float E3 = 1;
			if (Flag_Good)
			{
				float bb1 = D1*D1;
				float bb2 = D2*D2;
				float bb3 = D3*D3;

				 E1 = -(bb1*result[0][0] + bb2*result[0][1] + bb3*result[0][2]);
				 E2 = -(bb1*result[1][0] + bb2*result[1][1] + bb3*result[1][2]);
				 E3 = -(bb1*result[2][0] + bb2*result[2][1] + bb3*result[2][2]);

				t1 = t1 + E1;
				t2 = t2 + E2;
				t3 = t3 + E3;
				Flag_NewTon_use = 1;
			}
			else
			{
				//printf("-------------------------------------------------- >>> 雅克比矩阵求逆失败 ！！！\n");
				//printf("雅克比运行次数 ：%d*65535 + %d \n", DiguCnt - 1, i);
				Flag_DiGu = 1;
				//雅克比矩阵为0，扰动
				//扰动
				/*t1 += float(rand() % 101 - 50) / 50.0f *3.3f;
				t2 += float(rand() % 101 - 50) / 50.0f *3.3f;
				t3 += float(rand() % 101 - 50) / 50.0f *3.3f;*/
				break;
			}

			/*float t1br = t1 - nn*Ft1;
			float t2br = t2 - nn*Ft2;
			float t3br = t3 - nn*Ft3;

			if (t1br*c1 > 0 && t1br*c1 < 300 && t2br*c2 > 0 && t2br*c2 < 300 && t3br*c3 > 0 && t3br*c3 < 300)
			{
			t1b = t1br;
			t2b = t2br;
			t3b = t3br;
			}
			else
			{
			flag = 1;
			}*/

			
				if (E1==0 && E2==0 && E3==0)// && (ABS(t1-t1b)<0.005 && ABS(t2-t2b)<0.005 && ABS(t3-t3b)<0.005 && ABS(t4-t4b)<0.005)) 
				{
					Flag_DiGu = 1;
					break;
				}
		}
		DiguNum = i;
		
	}
	//检测是否使用牛顿迭代
	if (Flag_NewTon_use == 1)
	{
		printf("\t\t----------------------- >>> 已使用牛顿迭代法\n");
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------
	//在小区域内扰动
#if 1
	//牛顿迭代后进行最速下降递归
	//扰动
	t1 += float(rand() % 101 - 50) / 50.0f *0.3f;
	t2 += float(rand() % 101 - 50) / 50.0f *0.3f;
	t3 += float(rand() % 101 - 50) / 50.0f *0.3f;
	
	//printf("------------------------------->>>     +梯度下降法\n");
	 Flag_DiGu = 0;//递归运算运行标识符
	 DiguCnt = 0;
	float nn = 0.002f;// 递归步长 适中（不能太大也不能太小）。
	//float (*m)[3] = new float[3][3];
	while (!Flag_DiGu)
	{
		if (DiguCnt>=1)break;//递归次数大循环 限制
		DiguNum = 0;
		DiguCnt++;
		int i = 0;

		for (i = 0; i<65535; i++)
		{
			// 3点递归
			float D1 = (knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12);
			float D2 = (knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22);
			float D3 = (knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32);

			Ft1 = D1* (2.0f * knn[0][0] * t1 + knn[0][2] * t2 + knn[0][3])
				+ D3* (2.0f * knn[2][0] * t1 + knn[2][2] * t3 + knn[2][3])
				;

			Ft2 = D1* (2.0f * knn[0][1] * t2 + knn[0][2] * t1 + knn[0][4])
				+ D2* (2.0f * knn[1][0] * t2 + knn[1][2] * t3 + knn[1][3])
				;

			Ft3 = D2* (2.0f * knn[1][1] * t3 + knn[1][2] * t2 + knn[1][4])
				+ D3* (2.0f * knn[2][1] * t3 + knn[2][2] * t1 + knn[2][4])
				;
			//------ 雅克比矩阵 最速下降法算法步长必须 雅克比矩阵为正定矩阵
			/*			m[0][0] = D1* (2 * knn[0][0] * t1 + knn[0][2] * t2 + knn[0][3]);
			m[0][1] = D1* (2 * knn[0][1] * t2 + knn[0][2] * t1 + knn[0][4]);
			m[0][2] = 0;

			m[1][0] = 0;
			m[1][1] = D2* (2 * knn[1][0] * t2 + knn[1][2] * t3 + knn[1][3]);
			m[1][2] = D2* (2 * knn[1][1] * t3 + knn[1][2] * t2 + knn[1][4]);

			m[2][0] = D3* (2 * knn[2][0] * t1 + knn[2][2] * t3 + knn[2][3]);
			m[2][1] = 0;
			m[2][2] = D3* (2 * knn[2][1] * t3 + knn[2][2] * t1 + knn[2][4]);

			float Fz = Ft1*Ft1 + Ft2*Ft2 + Ft3*Ft3;//步长分子
			//步长分母
			float Fm1 = Ft1*m[0][0] + Ft2*m[1][0] + Ft3*m[2][0];
			float Fm2 = Ft1*m[0][1] + Ft2*m[1][1] + Ft3*m[2][1];
			float Fm3 = Ft1*m[0][2] + Ft2*m[1][2] + Ft3*m[2][2];
			//
			nn = 0.002f;
			float Fm = Ft1*Fm1 + Ft2*Fm2 + Ft3*Fm3;

			if (Fm != 0)
			{
			nn = Fz / Fm;
			}
			*/
			float t1br = t1 - nn*Ft1;
			float t2br = t2 - nn*Ft2;
			float t3br = t3 - nn*Ft3;

			//----------------------------
			if ((t1br*c1) > 0 && (t1br*c1) < 350 && (t2br*c2) > 0 && (t2br*c2) < 350 && (t3br*c3) > 0 && (t3br*c3) < 350)
			{
				t1b = t1br;
				t2b = t2br;
				t3b = t3br;
			}
			else
			{
				//printf("----------------------------------------------- >>>> 计算最优峰值 溢出报错！！！\n");
				//printf("tnbr:(%f,%f,%f)\n", t1br, t2br, t3br);
				Flag_DiGu = 1;
				break;
			}

			//------ 当梯度变化趋势很小时，认为找到极值点，跳出循环。
			if (ABS(Ft1)<0.003 && ABS(Ft2)<0.003 && ABS(Ft3)<0.003)//if (t1 == t1b && t2 == t2b && t3 == t3b )
			{
				t1 = t1b;
				t2 = t2b;
				t3 = t3b;
				Flag_DiGu = 1;
				break;
			}

			t1 = t1b;
			t2 = t2b;
			t3 = t3b;
		}

		DiguNum = i;
	}

	//printf("/******************************************* 扰动后递归 ************************************************/\n");
	//printf("扰动后递归次数：65535*%d + %d\n", DiguCnt_R, DiguNum_R);
#endif
	//将递归结果返回全局变量保存
	tN_GlobalS_3N[numc][0] = t1;
	tN_GlobalS_3N[numc][1] = t2;
	tN_GlobalS_3N[numc][2] = t3;


	/* tN_GlobalS_4N[numc][0] = t_min[0];
	tN_GlobalS_4N[numc][1] = t_min[1];
	tN_GlobalS_4N[numc][2] = t_min[2];
	tN_GlobalS_4N[numc][3] = t_min[3];*/

	/* t1=t_min[0];
	t2=t_min[1];
	t3=t_min[2];
	t4=t_min[3];*/

	Energy_GlobalN[numc] = (knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12)*(knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12)
		+ (knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22)*(knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22)
		+ (knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32)*(knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32)
		;
	//---------------------------------------------------
}

/********************************拟牛顿测试 DFP******************************/
void QuasiNewTon_DFP_Test()
{
	// min f(x)=x1*x1+2*x2*x2-2*x1*x2-4*x1
	// x1偏导数  fx1=2*x1-2*x2-4
	// x2偏导数  fx2=4*x2-2*x1

	//H0=单位矩阵
	//初始值： x1=1;x2=1

	//H0矩阵初始化
	float H11 = 1, H12 = 0, H21 = 0, H22 = 1;

	float x1 = 3113;//初始化 未知数
	float x2 = -213;
	float fx1 ;// x1 偏导数
	float fx2 ;// x2 偏导数

	for (int i = 0; i < 65000; i++)
	{
		 fx1 = 2 * x1 - 2 * x2 - 4;//求偏导数
		 fx2 = 4 * x2 - 2 * x1;
		//所求未知数下降值
		float px1 = -H11*fx1 + (-H12)*fx2;
		float px2 = -H21*fx1 + (-H22)*fx2;

		//所求未知数变化步长
		float step = 0.0025f;
		
		//更新未知数
		float x1b = x1 + step*px1;
		float x2b = x2 + step*px2;

		float fx1b = 2 * x1b - 2 * x2b - 4;//更新梯度
		float fx2b = 4 * x2b - 2 * x1b;

		float sx1 = x1b - x1; //未知数差值，当未知数差值都为0找到极小值
		float sx2 = x2b - x2;

		float yx1 = fx1b - fx1;//梯度差值，当梯度差值都为0找到极小值
		float yx2 = fx2b - fx2;

		//计算 H0矩阵相关参数
		float C = (yx1*H11 + yx2*H21)*yx1 + (yx1*H12 + yx2*H22)*yx2;
		float E = yx1*sx1 + yx2*sx2;

		float D11 = sx1*sx1; float D12 = sx1*sx2;
		float D21 = sx1*sx2; float D22 = sx2*sx2;

		float B11 = (H11*yx1 + H12*yx2)*yx1*H11 + (H11*yx1 + H12*yx2)*yx2*H21;
		float B12 = (H11*yx1 + H12*yx2)*yx1*H12 + (H11*yx1 + H12*yx2)*yx2*H22;
		float B21 = (H21*yx1 + H22*yx2)*yx1*H11 + (H21*yx1 + H22*yx2)*yx2*H21;
		float B22 = (H21*yx1 + H22*yx2)*yx1*H12 + (H21*yx1 + H22*yx2)*yx2*H22;

		float H11b, H12b, H21b, H22b;

		if (C != 0 && E != 0)        //Hesse阵更新
		{									
			H11b = H11 - B11 / C + D11 / E;
			H12b = H12 - B12 / C + D12 / E;
			H21b = H21 - B21 / C + D21 / E;
			H22b = H22 - B22 / C + D22 / E;
		}
		else
		{   //寻找到极小值退出
			x1 = x1b;
			x2 = x2b;

			break;
		}
		//------------------
		//迭代
		x1 = x1b;
		x2 = x2b;
		
		H11 = H11b;
		H12 = H12b;
		H21 = H21b;
		H22 = H22b;
		//printf("%d)\n",i);
		//if (i == 1)break;
	}

	printf("拟牛顿迭代法 DFP ：(x1,x2) = （%f,%f）\n",x1,x2);
	printf("\n");
}
/********************************拟牛顿测试 BFGS******************************/
void QuasiNewTon_BFGS_Test()
{
	// min f(x)=x1*x1+2*x2*x2-2*x1*x2-4*x1
	// x1偏导数  fx1=2*x1-2*x2-4
	// x2偏导数  fx2=4*x2-2*x1

	//H0=单位矩阵
	//初始值： x1=1;x2=1

	//H0矩阵初始化
	float H11 = 1, H12 = 0, H21 = 0, H22 = 1;

	float x1 = 3113;//初始化 未知数
	float x2 = -213;
	float fx1;// x1 偏导数
	float fx2;// x2 偏导数

	for (int i = 0; i < 65000; i++)
	{
		fx1 = 2 * x1 - 2 * x2 - 4;//求偏导数
		fx2 = 4 * x2 - 2 * x1;
		//所求未知数下降值
		float px1 = -H11*fx1 + (-H12)*fx2;
		float px2 = -H21*fx1 + (-H22)*fx2;

		//所求未知数变化步长
		float step = 0.0025f;

		//更新未知数
		float x1b = x1 + step*px1;
		float x2b = x2 + step*px2;

		float fx1b = 2 * x1b - 2 * x2b - 4;//更新梯度
		float fx2b = 4 * x2b - 2 * x1b;

		float sx1 = x1b - x1; //未知数差值，当未知数差值都为0找到极小值
		float sx2 = x2b - x2;

		float yx1 = fx1b - fx1;//梯度差值，当梯度差值都为0找到极小值
		float yx2 = fx2b - fx2;

		//计算 H0矩阵相关参数
		float C = (sx1*H11 + sx2*H21)*sx1 + (sx1*H12 + sx2*H22)*sx2;
		float E = yx1*sx1 + yx2*sx2;

		float D11 = yx1*yx1; float D12 = yx1*yx2;
		float D21 = yx1*yx2; float D22 = yx2*yx2;

		float B11 = (H11*sx1 + H12*sx2)*sx1*H11 + (H11*sx1 + H12*sx2)*sx2*H21;
		float B12 = (H11*sx1 + H12*sx2)*sx1*H12 + (H11*sx1 + H12*sx2)*sx2*H22;
		float B21 = (H21*sx1 + H22*sx2)*sx1*H11 + (H21*sx1 + H22*sx2)*sx2*H21;
		float B22 = (H21*sx1 + H22*sx2)*sx1*H12 + (H21*sx1 + H22*sx2)*sx2*H22;

		float H11b, H12b, H21b, H22b;

		if (C != 0 && E != 0)        //Hesse阵更新
		{
			H11b = H11 - B11 / C + D11 / E;
			H12b = H12 - B12 / C + D12 / E;
			H21b = H21 - B21 / C + D21 / E;
			H22b = H22 - B22 / C + D22 / E;
		}
		else
		{   //寻找到极小值退出
			x1 = x1b;
			x2 = x2b;

			break;
		}
		//------------------
		//迭代
		x1 = x1b;
		x2 = x2b;

		H11 = H11b;
		H12 = H12b;
		H21 = H21b;
		H22 = H22b;
		//printf("%d)\n", i);
		//if (i == 1)break;
	}

	printf("拟牛顿迭代法 BFGS：(x1,x2) = （%f,%f）\n", x1, x2);
	printf("\n");
}

/****************************************************先梯度下降法 后 拟牛顿 :DFP **********************************************************************/
void GL_Build_QuasiNewTon_DFP_M3Point(int *ID_Pt, float*ID_Length, float tN_GlobalS_3N[][4], float* Energy_GlobalN, int numc,
	int &Point_Check, int &DiguCnt, int &DiguNum, bool Flag_Step, int Rude_Step_Control)
{
	
	

	Point_Check = 3;//模式选择：算法点数
	float t1 = tN_GlobalS_3N[numc][0];
	float t2 = tN_GlobalS_3N[numc][1];
	float t3 = tN_GlobalS_3N[numc][2];
	float t1b = 170, t2b = 170, t3b = 170;
	//-------------------
	float Ft1, Ft2, Ft3;

	//射线向量
	float a1 = LineRays[ID_Pt[0]].x; float b1 = LineRays[ID_Pt[0]].y; float c1 = LineRays[ID_Pt[0]].z;
	float a2 = LineRays[ID_Pt[1]].x; float b2 = LineRays[ID_Pt[1]].y; float c2 = LineRays[ID_Pt[1]].z;
	float a3 = LineRays[ID_Pt[2]].x; float b3 = LineRays[ID_Pt[2]].y; float c3 = LineRays[ID_Pt[2]].z;
	// 
	GL_Vector pt0[3];
	pt0[0] = LineRays[ID_Pt[0]].pt0;
	pt0[1] = LineRays[ID_Pt[1]].pt0;
	pt0[2] = LineRays[ID_Pt[2]].pt0;
	//
	float x = a1*t1;
	float y = b1*t1;
	float z = c1*t1;
	float track_dst = GL_Distance(0, 0, 0, x, y, z);

	/**********************************************************/
	Fps_Track_Start = 1;
	//边长平方
	float E12 = ID_Length[0];// 1->2
	float E22 = ID_Length[1];// 2->3
	float E32 = ID_Length[2];// 1->3

	//------构建模型系数
	float(*knn)[6] = new float[3][6];
	GLB_Knn2(knn, a1, b1, c1, pt0[0], a2, b2, c2, pt0[1], 0);
	GLB_Knn2(knn, a2, b2, c2, pt0[1], a3, b3, c3, pt0[2], 1);
	GLB_Knn2(knn, a1, b1, c1, pt0[0], a3, b3, c3, pt0[2], 2);

	// F(t1,t2,t3)=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2+knn[0][3]*t1+knn[0][4]*t2+knn[0][5]-E12)*(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2+knn[0][3]*t1+knn[0][4]*t2+knn[0][5]-E12)
	//            +(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3+knn[1][3]*t2+knn[1][4]*t3+knn[1][5]-E22)*(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3+knn[1][3]*t2+knn[1][4]*t3+knn[1][5]-E22)
	//            +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3+knn[2][3]*t1+knn[2][4]*t3+knn[2][5]-E32)*(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3+knn[2][3]*t1+knn[2][4]*t3+knn[2][5]-E32)

	//-----------------
	// F(t1,t2,t3)/t1偏导数=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2+knn[0][3]*t1+knn[0][4]*t2+knn[0][5]-E12)* (2*knn[0][0]*t1+knn[0][2]*t2+knn[0][3])
	//                     +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3+knn[2][3]*t1+knn[2][4]*t3+knn[2][5]-E32)* (2*knn[2][0]*t1+knn[2][2]*t3+knn[2][3])

	// F(t1,t2,t3)/t2偏导数=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2+knn[0][3]*t1+knn[0][4]*t2+knn[0][5]-E12)* (2*knn[0][1]*t2+knn[0][2]*t1+knn[0][4])
	//                     +(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3+knn[1][3]*t2+knn[1][4]*t3+knn[1][5]-E22)* (2*knn[1][0]*t2+knn[1][2]*t3+knn[1][3])

	// F(t1,t2,t3)/t3偏导数=(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3+knn[1][3]*t2+knn[1][4]*t3+knn[1][5]-E22)* (2*knn[1][1]*t3+knn[1][2]*t2+knn[1][4])
	//                     +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3+knn[2][3]*t1+knn[2][4]*t3+knn[2][5]-E32)* (2*knn[2][1]*t3+knn[2][2]*t1+knn[2][4])
	//------------------------------------------------------------------------------------------------------------------------------------------------------------
	//当多组合时，DiguNum 与 DiguCnt必须清零。


	bool flag_NewTon = 0;
	bool Flag_DiGu = 0;//递归运算运行标识符
	DiguCnt = 0;
	float nn = 0.002f;// 递归步长 适中（不能太大也不能太小）。
	//float (*m)[3] = new float[3][3];
	while (!Flag_DiGu)
	{
		if (DiguCnt >= 1)break;//递归次数大循环 限制
		DiguNum = 0;
		DiguCnt++;
		int i = 0;

		for (i = 0; i < 61803; i++)// 61803黄金比例
		{
			// 3点递归
			float D1 = (knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12);
			float D2 = (knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22);
			float D3 = (knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32);

			Ft1 = D1* (2.0f * knn[0][0] * t1 + knn[0][2] * t2 + knn[0][3])
				+ D3* (2.0f * knn[2][0] * t1 + knn[2][2] * t3 + knn[2][3])
				;

			Ft2 = D1* (2.0f * knn[0][1] * t2 + knn[0][2] * t1 + knn[0][4])
				+ D2* (2.0f * knn[1][0] * t2 + knn[1][2] * t3 + knn[1][3])
				;

			Ft3 = D2* (2.0f * knn[1][1] * t3 + knn[1][2] * t2 + knn[1][4])
				+ D3* (2.0f * knn[2][1] * t3 + knn[2][2] * t1 + knn[2][4])
				;

			float t1br = t1 - nn*Ft1;
			float t2br = t2 - nn*Ft2;
			float t3br = t3 - nn*Ft3;

			//----------------------------
			if ((t1br*a1) > 0 && (t1br*a1) < 350 && ABS(t1br*b1) > 0 && ABS(t1br*b1) < 350 && (t1br*c1) > 0 && (t1br*c1) < 350)
			{
				t1b = t1br;
				t2b = t2br;
				t3b = t3br;
			}
			else
			{
				//printf("----------------------------------------------- >>>> 计算最优峰值 溢出报错！！！\n");
				//printf("tnbr:(%f,%f,%f)\n", t1br, t2br, t3br);
				Flag_DiGu = 1;
				break;
			}

			//------ 当梯度变化趋势很小时，认为找到极值点，跳出循环。
			if (t1 == t1b && t2 == t2b && t3 == t3b)//if (ABS(Ft1)<0.003 && ABS(Ft2)<0.003 && ABS(Ft3)<0.003)//
			{
				t1 = t1b;
				t2 = t2b;
				t3 = t3b;
				Flag_DiGu = 1;
				break;
			}

			t1 = t1b;
			t2 = t2b;
			t3 = t3b;
		}

		DiguNum = i;
	}

	DiguNum = 0;
	DiguCnt = 0;
	Flag_DiGu = 0;//递归运算运行标识符
	//H0=单位矩阵，H0矩阵初始化
	float H11 = 1, H12 = 0, H13 = 0;
	float H21 = 0, H22 = 1, H23 = 0;
	float H31 = 0, H32 = 0, H33 = 1;

	while (!Flag_DiGu)
	{
		if (DiguCnt >= 4)break;//递归次数大循环 限制
		DiguNum = 0;
		DiguCnt++;
		int i = 0;

		for (i = 0; i<65535; i++)
		{
			// 3点递归
			//求偏导数
			float D1 = (knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12);
			float D2 = (knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22);
			float D3 = (knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32);

			Ft1 = D1* (2.0f * knn[0][0] * t1 + knn[0][2] * t2 + knn[0][3])
				+ D3* (2.0f * knn[2][0] * t1 + knn[2][2] * t3 + knn[2][3])
				;

			Ft2 = D1* (2.0f * knn[0][1] * t2 + knn[0][2] * t1 + knn[0][4])
				+ D2* (2.0f * knn[1][0] * t2 + knn[1][2] * t3 + knn[1][3])
				;

			Ft3 = D2* (2.0f * knn[1][1] * t3 + knn[1][2] * t2 + knn[1][4])
				+ D3* (2.0f * knn[2][1] * t3 + knn[2][2] * t1 + knn[2][4])
				;

			float Pt1 = -(H11*Ft1 + H12*Ft2 + H13*Ft3);
			float Pt2 = -(H21*Ft1 + H22*Ft2 + H23*Ft3);
			float Pt3 = -(H31*Ft1 + H32*Ft2 + H33*Ft3);

			//所求未知数变化步长
			float step = 10.0f;
			float stepr = 1.10f;

			float EnergyNewton = (knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12)*(knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12)
				+ (knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22)*(knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22)
				+ (knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32)*(knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32)
				;
			bool Flag_Step = 0;
			for (int ii = 0; ii < 1000; ii++)//线性搜索步长
			{
				float t1r = t1 + stepr*Pt1;
				float t2r = t2 + stepr*Pt2;
				float t3r = t3 + stepr*Pt3;

				float EnergyNewtonR = (knn[0][0] * t1r*t1r + knn[0][1] * t2r*t2r + knn[0][2] * t1r*t2r + knn[0][3] * t1r + knn[0][4] * t2r + knn[0][5] - E12)*(knn[0][0] * t1r*t1r + knn[0][1] * t2r*t2r + knn[0][2] * t1r*t2r + knn[0][3] * t1r + knn[0][4] * t2r + knn[0][5] - E12)
					+ (knn[1][0] * t2r*t2r + knn[1][1] * t3r*t3r + knn[1][2] * t2r*t3r + knn[1][3] * t2r + knn[1][4] * t3r + knn[1][5] - E22)*(knn[1][0] * t2r*t2r + knn[1][1] * t3r*t3r + knn[1][2] * t2r*t3r + knn[1][3] * t2r + knn[1][4] * t3r + knn[1][5] - E22)
					+ (knn[2][0] * t1r*t1r + knn[2][1] * t3r*t3r + knn[2][2] * t1r*t3r + knn[2][3] * t1r + knn[2][4] * t3r + knn[2][5] - E32)*(knn[2][0] * t1r*t1r + knn[2][1] * t3r*t3r + knn[2][2] * t1r*t3r + knn[2][3] * t1r + knn[2][4] * t3r + knn[2][5] - E32);
				if (EnergyNewtonR < EnergyNewton)
				{
					step = stepr;
					Flag_Step = 1;
					break;
				}
				else
				{
					stepr = stepr*0.818;
				}
			}

			//
			if (!Flag_Step)
			{
				Flag_DiGu = 1;
				break;
			}
			//----------------------------

			//更新未知数
			t1b = t1 + step*Pt1;
			t2b = t2 + step*Pt2;
			t3b = t3 + step*Pt3;

			 D1 = (knn[0][0] * t1b*t1b + knn[0][1] * t2b*t2b + knn[0][2] * t1b*t2b + knn[0][3] * t1b + knn[0][4] * t2b + knn[0][5] - E12);
			 D2 = (knn[1][0] * t2b*t2b + knn[1][1] * t3b*t3b + knn[1][2] * t2b*t3b + knn[1][3] * t2b + knn[1][4] * t3b + knn[1][5] - E22);
			 D3 = (knn[2][0] * t1b*t1b + knn[2][1] * t3b*t3b + knn[2][2] * t1b*t3b + knn[2][3] * t1b + knn[2][4] * t3b + knn[2][5] - E32);

			float Ft1b = D1* (2.0f * knn[0][0] * t1b + knn[0][2] * t2b + knn[0][3])
				+ D3* (2.0f * knn[2][0] * t1b + knn[2][2] * t3b + knn[2][3])
				;

			float Ft2b = D1* (2.0f * knn[0][1] * t2b + knn[0][2] * t1b + knn[0][4])
				+ D2* (2.0f * knn[1][0] * t2b + knn[1][2] * t3b + knn[1][3])
				;

			float Ft3b = D2* (2.0f * knn[1][1] * t3b + knn[1][2] * t2b + knn[1][4])
				+ D3* (2.0f * knn[2][1] * t3b + knn[2][2] * t1b + knn[2][4])
				;

			float sx1 = t1b - t1; //未知数差值，当未知数差值都为0找到极小值
			float sx2 = t2b - t2;
			float sx3 = t3b - t3;

			float yx1 = Ft1b - Ft1;//梯度差值，当梯度差值都为0找到极小值
			float yx2 = Ft2b - Ft2;
			float yx3 = Ft3b - Ft3;
			
			//计算 H0矩阵相关参数
			float C = (yx1*H11 + yx2*H21 + yx3*H31)*yx1
				    + (yx1*H12 + yx2*H22 + yx3*H32)*yx2
				    + (yx1*H13 + yx2*H23 + yx3*H33)*yx3;

			float E = yx1*sx1 + yx2*sx2 + yx3*sx3;

			float D11 = sx1*sx1, D12 = sx1*sx2, D13 = sx1*sx3;
			float D21 = sx1*sx2, D22 = sx2*sx2, D23 = sx2*sx3;
			float D31 = sx1*sx3, D32 = sx2*sx3, D33 = sx3*sx3;

			//计算B矩阵
			//矩阵中间缓存
			float BB11 = (H11*yx1 + H12*yx2 + H13*yx3)*yx1; float BB12 = (H11*yx1 + H12*yx2 + H13*yx3)*yx2; float BB13 = (H11*yx1 + H12*yx2 + H13*yx3)*yx3;
			float BB21 = (H21*yx1 + H22*yx2 + H23*yx3)*yx1; float BB22 = (H21*yx1 + H22*yx2 + H23*yx3)*yx2; float BB23 = (H21*yx1 + H22*yx2 + H23*yx3)*yx3;
			float BB31 = (H31*yx1 + H32*yx2 + H33*yx3)*yx1; float BB32 = (H31*yx1 + H32*yx2 + H33*yx3)*yx2; float BB33 = (H31*yx1 + H32*yx2 + H33*yx3)*yx3;

			//

			float B11 = BB11*H11 + BB12*H21 + BB13*H31;
			float B12 = BB11*H12 + BB12*H22 + BB13*H32;
			float B13 = BB11*H13 + BB12*H23 + BB13*H33;

			float B21 = BB21*H11 + BB22*H21 + BB23*H31;
			float B22 = BB21*H12 + BB22*H22 + BB23*H32;
			float B23 = BB21*H13 + BB22*H23 + BB23*H33;

			float B31 = BB31*H11 + BB32*H21 + BB33*H31;
			float B32 = BB31*H12 + BB32*H22 + BB33*H32;
			float B33 = BB31*H13 + BB32*H23 + BB33*H33;


			//
			float H11b, H12b, H13b;
			float H21b, H22b, H23b;
			float H31b, H32b, H33b;

			if (C != 0 && E != 0)        //Hesse阵更新
			{
				if (E > 0)
				{
					H11b = H11 - B11 / C + D11 / E;
					H12b = H12 - B12 / C + D12 / E;
					H13b = H13 - B13 / C + D13 / E;

					H21b = H21 - B21 / C + D21 / E;
					H22b = H22 - B22 / C + D22 / E;
					H23b = H23 - B23 / C + D23 / E;

					H31b = H31 - B31 / C + D31 / E;
					H32b = H32 - B32 / C + D32 / E;
					H33b = H33 - B33 / C + D33 / E;
				}
				else
				{
					H11b = H11;
					H12b = H12;
					H13b = H13;

					H21b = H21;
					H22b = H22;
					H23b = H23;

					H31b = H31;
					H32b = H32;
					H33b = H33;
				}
			}
			else
			{   //寻找到极小值退出
				if ((t1b*a1) > 0 && (t1b*a1) < 350 && ABS(t1b*b1) > 0 && ABS(t1b*b1) < 350 && (t1b*c1) > 0 && (t1b*c1) < 350)
				{
					t1 = t1b;
					t2 = t2b;
					t3 = t3b;
				}
				Flag_DiGu = 1;
				break;
			}

			//迭代
			if (ABS(t1b*a1) > 0 && ABS(t1b*a1) < 350 && ABS(t1b*b1) > 0 && ABS(t1b*b1) < 350 && ABS(t1b*c1) > 0 && ABS(t1b*c1) < 350)
			{
				t1 = t1b;
				t2 = t2b;
				t3 = t3b;

				H11 = H11b;
				H12 = H12b;
				H13 = H13b;

				H21 = H21b;
				H22 = H22b;
				H23 = H23b;

				H31 = H31b;
				H32 = H32b;
				H33 = H33b;
				flag_NewTon = 1;
			}
			else
			{
				//printf("----------------------------------------------- >>>> 计算最优峰值 溢出报错！！！\n");
				//printf("tnbr:(%f,%f,%f)\n", t1br, t2br, t3br);
				Flag_DiGu = 1;
				break;
			}
			

			////------ 当梯度变化趋势很小时，认为找到极值点，跳出循环。
			//if (ABS(Ft1)<0.003 && ABS(Ft2)<0.003 && ABS(Ft3)<0.003)//if (t1 == t1b && t2 == t2b && t3 == t3b )
			//{
			//	t1 = t1b;
			//	t2 = t2b;
			//	t3 = t3b;
			//	Flag_DiGu = 1;
			//	break;
			//}

			
		}

		DiguNum = i;
	}
	//---------------------------------------------------
	if (flag_NewTon == 1)
	{
		printf("\t\t----------------------- >>> 采用拟牛顿算法 DFP\n");
	}
	

	
	//将递归结果返回全局变量保存
	tN_GlobalS_3N[numc][0] = t1;
	tN_GlobalS_3N[numc][1] = t2;
	tN_GlobalS_3N[numc][2] = t3;


	/* tN_GlobalS_4N[numc][0] = t_min[0];
	tN_GlobalS_4N[numc][1] = t_min[1];
	tN_GlobalS_4N[numc][2] = t_min[2];
	tN_GlobalS_4N[numc][3] = t_min[3];*/

	/* t1=t_min[0];
	t2=t_min[1];
	t3=t_min[2];
	t4=t_min[3];*/

	Energy_GlobalN[numc] = (knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12)*(knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12)
		+ (knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22)*(knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22)
		+ (knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32)*(knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32)
		;

	delete[]knn;
	//delete[]m;
	//---------------------------------------------------
}

/****************************************************先牛顿，再梯度下降法 ，后 拟牛顿 :BFGS，再梯度 **********************************************************************/
void GL_Build_QuasiNewTon_BFGS_M3Point(int *ID_Pt, float*ID_Length, float tN_GlobalS_3N[][4], float* Energy_GlobalN, int numc,
	int &Point_Check, int &DiguCnt, int &DiguNum, bool Flag_Step, int Rude_Step_Control)
{
	Point_Check = 3;//模式选择：算法点数
	float t1 = tN_GlobalS_3N[numc][0];
	float t2 = tN_GlobalS_3N[numc][1];
	float t3 = tN_GlobalS_3N[numc][2];
	float t1b = 170, t2b = 170, t3b = 170;
	//-------------------
	float Ft1, Ft2, Ft3;

	//射线向量
	float a1 = LineRays[ID_Pt[0]].x; float b1 = LineRays[ID_Pt[0]].y; float c1 = LineRays[ID_Pt[0]].z;
	float a2 = LineRays[ID_Pt[1]].x; float b2 = LineRays[ID_Pt[1]].y; float c2 = LineRays[ID_Pt[1]].z;
	float a3 = LineRays[ID_Pt[2]].x; float b3 = LineRays[ID_Pt[2]].y; float c3 = LineRays[ID_Pt[2]].z;
	// 
	GL_Vector pt0[3];
	pt0[0] = LineRays[ID_Pt[0]].pt0;
	pt0[1] = LineRays[ID_Pt[1]].pt0;
	pt0[2] = LineRays[ID_Pt[2]].pt0;
	//
	float x = a1*t1;
	float y = b1*t1;
	float z = c1*t1;
	float track_dst = GL_Distance(0, 0, 0, x, y, z);

	/**********************************************************/
	Fps_Track_Start = 1;
	//边长平方
	float E12 = ID_Length[0];// 1->2
	float E22 = ID_Length[1];// 2->3
	float E32 = ID_Length[2];// 1->3

	//------构建模型系数
	float(*knn)[6] = new float[3][6];
	GLB_Knn2(knn, a1, b1, c1, pt0[0], a2, b2, c2, pt0[1], 0);
	GLB_Knn2(knn, a2, b2, c2, pt0[1], a3, b3, c3, pt0[2], 1);
	GLB_Knn2(knn, a1, b1, c1, pt0[0], a3, b3, c3, pt0[2], 2);

	// F(t1,t2,t3)=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2+knn[0][3]*t1+knn[0][4]*t2+knn[0][5]-E12)*(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2+knn[0][3]*t1+knn[0][4]*t2+knn[0][5]-E12)
	//            +(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3+knn[1][3]*t2+knn[1][4]*t3+knn[1][5]-E22)*(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3+knn[1][3]*t2+knn[1][4]*t3+knn[1][5]-E22)
	//            +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3+knn[2][3]*t1+knn[2][4]*t3+knn[2][5]-E32)*(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3+knn[2][3]*t1+knn[2][4]*t3+knn[2][5]-E32)

	//-----------------
	// F(t1,t2,t3)/t1偏导数=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2+knn[0][3]*t1+knn[0][4]*t2+knn[0][5]-E12)* (2*knn[0][0]*t1+knn[0][2]*t2+knn[0][3])
	//                     +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3+knn[2][3]*t1+knn[2][4]*t3+knn[2][5]-E32)* (2*knn[2][0]*t1+knn[2][2]*t3+knn[2][3])

	// F(t1,t2,t3)/t2偏导数=(knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2+knn[0][3]*t1+knn[0][4]*t2+knn[0][5]-E12)* (2*knn[0][1]*t2+knn[0][2]*t1+knn[0][4])
	//                     +(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3+knn[1][3]*t2+knn[1][4]*t3+knn[1][5]-E22)* (2*knn[1][0]*t2+knn[1][2]*t3+knn[1][3])

	// F(t1,t2,t3)/t3偏导数=(knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3+knn[1][3]*t2+knn[1][4]*t3+knn[1][5]-E22)* (2*knn[1][1]*t3+knn[1][2]*t2+knn[1][4])
	//                     +(knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3+knn[2][3]*t1+knn[2][4]*t3+knn[2][5]-E32)* (2*knn[2][1]*t3+knn[2][2]*t1+knn[2][4])
	//------------------------------------------------------------------------------------------------------------------------------------------------------------
	//当多组合时，DiguNum 与 DiguCnt必须清零。

	bool Flag_DiGu = 0;
	bool Flag_NewTon_use = 0;
	DiguCnt = 0;

	float m[3][3];
	float result[3][3];
	//-------------------------------------------------------------------------------------- 牛顿迭代法
	while (!Flag_DiGu)
	{
		/*if(DiguCnt>15)nn=0.000023;*/
		if (DiguCnt >= 1)break;
		DiguNum = 0;
		DiguCnt++;
		int i = 0;

		for (i = 0; i<10000; i++)
		{
			float D1 = (knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12);
			float D2 = (knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22);
			float D3 = (knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32);
			//------ 雅克比矩阵
			m[0][0] = D1* (2 * knn[0][0] * t1 + knn[0][2] * t2 + knn[0][3]);
			m[0][1] = D1* (2 * knn[0][1] * t2 + knn[0][2] * t1 + knn[0][4]);
			m[0][2] = 0;

			m[1][0] = 0;
			m[1][1] = D2* (2 * knn[1][0] * t2 + knn[1][2] * t3 + knn[1][3]);
			m[1][2] = D2* (2 * knn[1][1] * t3 + knn[1][2] * t2 + knn[1][4]);

			m[2][0] = D3* (2 * knn[2][0] * t1 + knn[2][2] * t3 + knn[2][3]);
			m[2][1] = 0;
			m[2][2] = D3* (2 * knn[2][1] * t3 + knn[2][2] * t1 + knn[2][4]);

			//----- 求逆矩阵
			bool Flag_Good = invert(m, result);
			float E1 = 1;
			float E2 = 1;
			float E3 = 1;
			if (Flag_Good)
			{
				float bb1 = D1*D1;
				float bb2 = D2*D2;
				float bb3 = D3*D3;

				E1 = -(bb1*result[0][0] + bb2*result[0][1] + bb3*result[0][2]);
				E2 = -(bb1*result[1][0] + bb2*result[1][1] + bb3*result[1][2]);
				E3 = -(bb1*result[2][0] + bb2*result[2][1] + bb3*result[2][2]);

				t1 = t1 + E1;
				t2 = t2 + E2;
				t3 = t3 + E3;
				Flag_NewTon_use = 1;
			}
			else
			{
				//printf("-------------------------------------------------- >>> 雅克比矩阵求逆失败 ！！！\n");
				//printf("雅克比运行次数 ：%d*65535 + %d \n", DiguCnt - 1, i);
				Flag_DiGu = 1;
				break;
			}

			if (E1 == 0 && E2 == 0 && E3 == 0)// && (ABS(t1-t1b)<0.005 && ABS(t2-t2b)<0.005 && ABS(t3-t3b)<0.005 && ABS(t4-t4b)<0.005)) 
			{
				Flag_DiGu = 1;
				break;
			}
		}
		DiguNum = i;
	}
	//检测是否使用牛顿迭代
	if (Flag_NewTon_use == 1)
	{
		printf("\t\t----------------------- >>> 已使用牛顿迭代法\n");
	}
	//--------------------------------------------------------------------- 梯度下降法

	//t1 += float(rand() % 101 - 50) / 50.0f *0.3f;
	//t2 += float(rand() % 101 - 50) / 50.0f *0.3f;
	//t3 += float(rand() % 101 - 50) / 50.0f *0.3f;
	
	Flag_DiGu = 0;//递归运算运行标识符
	DiguCnt = 0;
	float nn = 0.002f;// 递归步长 适中（不能太大也不能太小）。
	while (!Flag_DiGu)
	{
		if (DiguCnt >= 2)break;//递归次数大循环 限制
		DiguNum = 0;
		DiguCnt++;
		int i = 0;

		for (i = 0; i < 65535; i++)// 61803黄金比例
		{
			// 3点递归
			float D1 = (knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12);
			float D2 = (knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22);
			float D3 = (knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32);

			Ft1 = D1* (2.0f * knn[0][0] * t1 + knn[0][2] * t2 + knn[0][3])
				+ D3* (2.0f * knn[2][0] * t1 + knn[2][2] * t3 + knn[2][3])
				;

			Ft2 = D1* (2.0f * knn[0][1] * t2 + knn[0][2] * t1 + knn[0][4])
				+ D2* (2.0f * knn[1][0] * t2 + knn[1][2] * t3 + knn[1][3])
				;

			Ft3 = D2* (2.0f * knn[1][1] * t3 + knn[1][2] * t2 + knn[1][4])
				+ D3* (2.0f * knn[2][1] * t3 + knn[2][2] * t1 + knn[2][4])
				;

			float t1br = t1 - nn*Ft1;
			float t2br = t2 - nn*Ft2;
			float t3br = t3 - nn*Ft3;
			//----------------------------
			if ((t1br*a1) > 0 && (t1br*a1) < 350 && ABS(t1br*b1) > 0 && ABS(t1br*b1) < 350 && (t1br*c1) > 0 && (t1br*c1) < 350)
			{
				t1b = t1br;
				t2b = t2br;
				t3b = t3br;
			}
			else
			{
				//printf("----------------------------------------------- >>>> 计算最优峰值 溢出报错！！！\n");
				//printf("tnbr:(%f,%f,%f)\n", t1br, t2br, t3br);
				Flag_DiGu = 1;
				break;
			}
			//------ 当梯度变化趋势很小时，认为找到极值点，跳出循环。
			if (t1 == t1b && t2 == t2b && t3 == t3b)//if (ABS(Ft1)<0.003 && ABS(Ft2)<0.003 && ABS(Ft3)<0.003)//
			{
				t1 = t1b;
				t2 = t2b;
				t3 = t3b;
				Flag_DiGu = 1;
				break;
			}

			t1 = t1b;
			t2 = t2b;
			t3 = t3b;
		}
		DiguNum = i;
	}

	//------------------------------------------------------------------------拟牛顿
	t1 += float(rand() % 101 - 50) / 50.0f *0.1f;
	t2 += float(rand() % 101 - 50) / 50.0f *0.1f;
	t3 += float(rand() % 101 - 50) / 50.0f *0.1f;

	DiguNum = 0;
	DiguCnt = 0;
	Flag_DiGu = 0;//递归运算运行标识符
	//H0=单位矩阵，H0矩阵初始化
	float H11 = 1, H12 = 0, H13 = 0;
	float H21 = 0, H22 = 1, H23 = 0;
	float H31 = 0, H32 = 0, H33 = 1;
	bool flag_NewTon = 0;
	while (!Flag_DiGu)
	{
		if (DiguCnt >= 1)break;//递归次数大循环 限制
		DiguNum = 0;
		DiguCnt++;
		int i = 0;

		for (i = 0; i<10000; i++)
		{
			// 3点递归
			//求偏导数
			float D1 = (knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12);
			float D2 = (knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22);
			float D3 = (knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32);

			Ft1 = D1* (2.0f * knn[0][0] * t1 + knn[0][2] * t2 + knn[0][3])
				+ D3* (2.0f * knn[2][0] * t1 + knn[2][2] * t3 + knn[2][3])
				;

			Ft2 = D1* (2.0f * knn[0][1] * t2 + knn[0][2] * t1 + knn[0][4])
				+ D2* (2.0f * knn[1][0] * t2 + knn[1][2] * t3 + knn[1][3])
				;

			Ft3 = D2* (2.0f * knn[1][1] * t3 + knn[1][2] * t2 + knn[1][4])
				+ D3* (2.0f * knn[2][1] * t3 + knn[2][2] * t1 + knn[2][4])
				;

			float Pt1 = -(H11*Ft1 + H12*Ft2 + H13*Ft3);
			float Pt2 = -(H21*Ft1 + H22*Ft2 + H23*Ft3);
			float Pt3 = -(H31*Ft1 + H32*Ft2 + H33*Ft3);

			//所求未知数变化步长
			float step = 10.0f;
			float stepr = 10.10f;

			float EnergyNewton = (knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12)*(knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12)
				+ (knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22)*(knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22)
				+ (knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32)*(knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32)
				;
			bool Flag_Step = 0;
			for (int ii = 0; ii < 1000; ii++)//线性搜索步长
			{
				float t1r = t1 + stepr*Pt1;
				float t2r = t2 + stepr*Pt2;
				float t3r = t3 + stepr*Pt3;

				float EnergyNewtonR = (knn[0][0] * t1r*t1r + knn[0][1] * t2r*t2r + knn[0][2] * t1r*t2r + knn[0][3] * t1r + knn[0][4] * t2r + knn[0][5] - E12)*(knn[0][0] * t1r*t1r + knn[0][1] * t2r*t2r + knn[0][2] * t1r*t2r + knn[0][3] * t1r + knn[0][4] * t2r + knn[0][5] - E12)
					+ (knn[1][0] * t2r*t2r + knn[1][1] * t3r*t3r + knn[1][2] * t2r*t3r + knn[1][3] * t2r + knn[1][4] * t3r + knn[1][5] - E22)*(knn[1][0] * t2r*t2r + knn[1][1] * t3r*t3r + knn[1][2] * t2r*t3r + knn[1][3] * t2r + knn[1][4] * t3r + knn[1][5] - E22)
					+ (knn[2][0] * t1r*t1r + knn[2][1] * t3r*t3r + knn[2][2] * t1r*t3r + knn[2][3] * t1r + knn[2][4] * t3r + knn[2][5] - E32)*(knn[2][0] * t1r*t1r + knn[2][1] * t3r*t3r + knn[2][2] * t1r*t3r + knn[2][3] * t1r + knn[2][4] * t3r + knn[2][5] - E32);
				if (EnergyNewtonR < EnergyNewton)
				{
					step = stepr;
					Flag_Step = 1;
					break;
				}
				else
				{
					stepr *= 0.99f;
					//stepr *= 0.618f;
				}
			}

			//
			if (!Flag_Step)
			{
				Flag_DiGu = 1;
				break;
			}
			//----------------------------

			//更新未知数
			t1b = t1 + step*Pt1;
			t2b = t2 + step*Pt2;
			t3b = t3 + step*Pt3;

			D1 = (knn[0][0] * t1b*t1b + knn[0][1] * t2b*t2b + knn[0][2] * t1b*t2b + knn[0][3] * t1b + knn[0][4] * t2b + knn[0][5] - E12);
			D2 = (knn[1][0] * t2b*t2b + knn[1][1] * t3b*t3b + knn[1][2] * t2b*t3b + knn[1][3] * t2b + knn[1][4] * t3b + knn[1][5] - E22);
			D3 = (knn[2][0] * t1b*t1b + knn[2][1] * t3b*t3b + knn[2][2] * t1b*t3b + knn[2][3] * t1b + knn[2][4] * t3b + knn[2][5] - E32);

			float Ft1b = D1* (2.0f * knn[0][0] * t1b + knn[0][2] * t2b + knn[0][3])
				+ D3* (2.0f * knn[2][0] * t1b + knn[2][2] * t3b + knn[2][3])
				;

			float Ft2b = D1* (2.0f * knn[0][1] * t2b + knn[0][2] * t1b + knn[0][4])
				+ D2* (2.0f * knn[1][0] * t2b + knn[1][2] * t3b + knn[1][3])
				;

			float Ft3b = D2* (2.0f * knn[1][1] * t3b + knn[1][2] * t2b + knn[1][4])
				+ D3* (2.0f * knn[2][1] * t3b + knn[2][2] * t1b + knn[2][4])
				;

			float sx1 = t1b - t1; //未知数差值，当未知数差值都为0找到极小值
			float sx2 = t2b - t2;
			float sx3 = t3b - t3;

			float yx1 = Ft1b - Ft1;//梯度差值，当梯度差值都为0找到极小值
			float yx2 = Ft2b - Ft2;
			float yx3 = Ft3b - Ft3;

			//计算 H0矩阵相关参数
			float C = (sx1*H11 + sx2*H21 + sx3*H31)*sx1
				+ (sx1*H12 + sx2*H22 + sx3*H32)*sx2
				+ (sx1*H13 + sx2*H23 + sx3*H33)*sx3;

			float E = yx1*sx1 + yx2*sx2 + yx3*sx3;

			float D11 = yx1*yx1, D12 = yx1*yx2, D13 = yx1*yx3;
			float D21 = yx1*yx2, D22 = yx2*yx2, D23 = yx2*yx3;
			float D31 = yx1*yx3, D32 = yx2*yx3, D33 = yx3*yx3;

			//计算B矩阵
			//矩阵中间缓存
			float BB11 = (H11*sx1 + H12*sx2 + H13*sx3)*sx1; float BB12 = (H11*sx1 + H12*sx2 + H13*sx3)*sx2; float BB13 = (H11*sx1 + H12*sx2 + H13*sx3)*sx3;
			float BB21 = (H21*sx1 + H22*sx2 + H23*sx3)*sx1; float BB22 = (H21*sx1 + H22*sx2 + H23*sx3)*sx2; float BB23 = (H21*sx1 + H22*sx2 + H23*sx3)*sx3;
			float BB31 = (H31*sx1 + H32*sx2 + H33*sx3)*sx1; float BB32 = (H31*sx1 + H32*sx2 + H33*sx3)*sx2; float BB33 = (H31*sx1 + H32*sx2 + H33*sx3)*sx3;
			//

			float B11 = BB11*H11 + BB12*H21 + BB13*H31;
			float B12 = BB11*H12 + BB12*H22 + BB13*H32;
			float B13 = BB11*H13 + BB12*H23 + BB13*H33;

			float B21 = BB21*H11 + BB22*H21 + BB23*H31;
			float B22 = BB21*H12 + BB22*H22 + BB23*H32;
			float B23 = BB21*H13 + BB22*H23 + BB23*H33;

			float B31 = BB31*H11 + BB32*H21 + BB33*H31;
			float B32 = BB31*H12 + BB32*H22 + BB33*H32;
			float B33 = BB31*H13 + BB32*H23 + BB33*H33;


			//
			float H11b, H12b, H13b;
			float H21b, H22b, H23b;
			float H31b, H32b, H33b;

			if (C != 0 )        //Hesse阵更新
			{
				if (E > 0)
				{
					H11b = H11 - B11 / C + D11 / E;
					H12b = H12 - B12 / C + D12 / E;
					H13b = H13 - B13 / C + D13 / E;

					H21b = H21 - B21 / C + D21 / E;
					H22b = H22 - B22 / C + D22 / E;
					H23b = H23 - B23 / C + D23 / E;

					H31b = H31 - B31 / C + D31 / E;
					H32b = H32 - B32 / C + D32 / E;
					H33b = H33 - B33 / C + D33 / E;
				}
				else
				{
					H11b = H11 ;
					H12b = H12 ;
					H13b = H13 ;

					H21b = H21 ;
					H22b = H22 ;
					H23b = H23 ;

					H31b = H31 ;
					H32b = H32 ;
					H33b = H33 ;
				}
			}
			else
			{   
				H11b = H11;
				H12b = H12;
				H13b = H13;

				H21b = H21;
				H22b = H22;
				H23b = H23;

				H31b = H31;
				H32b = H32;
				H33b = H33;
				//寻找到极小值
				if ((t1b*a1) > 0 && (t1b*a1) < 350 && ABS(t1b*b1) > 0 && ABS(t1b*b1) < 350 && (t1b*c1) > 0 && (t1b*c1) < 350)
				{
					t1 = t1b;
					t2 = t2b;
					t3 = t3b;
				}
				Flag_DiGu = 1;
				//break;
			}

			//迭代
			if ((t1b*a1) > 0 && (t1b*a1) < 350 && ABS(t1b*b1) > 0 && ABS(t1b*b1) < 350 && (t1b*c1) > 0 && (t1b*c1) < 350)
			{
				t1 = t1b;
				t2 = t2b;
				t3 = t3b;

				
				H11 = H11b;
				H12 = H12b;
				H13 = H13b;

				H21 = H21b;
				H22 = H22b;
				H23 = H23b;

				H31 = H31b;
				H32 = H32b;
				H33 = H33b;
				flag_NewTon = 1;
			}
			else
			{
				//printf("----------------------------------------------- >>>> 计算最优峰值 溢出报错！！！\n");
				//printf("tnbr:(%f,%f,%f)\n", t1br, t2br, t3br);
				Flag_DiGu = 1;
				break;
			}


			////------ 当梯度变化趋势很小时，认为找到极值点，跳出循环。
			//if (ABS(Ft1)<0.003 && ABS(Ft2)<0.003 && ABS(Ft3)<0.003)//if (t1 == t1b && t2 == t2b && t3 == t3b )
			//{
			//	t1 = t1b;
			//	t2 = t2b;
			//	t3 = t3b;
			//	Flag_DiGu = 1;
			//	break;
			//}


		}

		DiguNum = i;
	}

	//--------------------------------------------------------------------- 2次 梯度下降法

	//t1 += float(rand() % 101 - 50) / 50.0f *0.3f;
	//t2 += float(rand() % 101 - 50) / 50.0f *0.3f;
	//t3 += float(rand() % 101 - 50) / 50.0f *0.3f;

	Flag_DiGu = 0;//递归运算运行标识符
	DiguCnt = 0;
	 nn = 0.002f;// 递归步长 适中（不能太大也不能太小）。
	while (!Flag_DiGu)
	{
		if (DiguCnt >= 1)break;//递归次数大循环 限制
		DiguNum = 0;
		DiguCnt++;
		int i = 0;

		for (i = 0; i < 10000; i++)// 61803黄金比例
		{
			// 3点递归
			float D1 = (knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12);
			float D2 = (knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22);
			float D3 = (knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32);

			Ft1 = D1* (2.0f * knn[0][0] * t1 + knn[0][2] * t2 + knn[0][3])
				+ D3* (2.0f * knn[2][0] * t1 + knn[2][2] * t3 + knn[2][3])
				;

			Ft2 = D1* (2.0f * knn[0][1] * t2 + knn[0][2] * t1 + knn[0][4])
				+ D2* (2.0f * knn[1][0] * t2 + knn[1][2] * t3 + knn[1][3])
				;

			Ft3 = D2* (2.0f * knn[1][1] * t3 + knn[1][2] * t2 + knn[1][4])
				+ D3* (2.0f * knn[2][1] * t3 + knn[2][2] * t1 + knn[2][4])
				;

			float t1br = t1 - nn*Ft1;
			float t2br = t2 - nn*Ft2;
			float t3br = t3 - nn*Ft3;
			//----------------------------
			if ((t1br*a1) > 0 && (t1br*a1) < 350 && ABS(t1br*b1) > 0 && ABS(t1br*b1) < 350 && (t1br*c1) > 0 && (t1br*c1) < 350)
			{
				t1b = t1br;
				t2b = t2br;
				t3b = t3br;
			}
			else
			{
				//printf("----------------------------------------------- >>>> 计算最优峰值 溢出报错！！！\n");
				//printf("tnbr:(%f,%f,%f)\n", t1br, t2br, t3br);
				Flag_DiGu = 1;
				break;
			}
			//------ 当梯度变化趋势很小时，认为找到极值点，跳出循环。
			if (t1 == t1b && t2 == t2b && t3 == t3b)//if (ABS(Ft1)<0.003 && ABS(Ft2)<0.003 && ABS(Ft3)<0.003)//
			{
				t1 = t1b;
				t2 = t2b;
				t3 = t3b;
				Flag_DiGu = 1;
				break;
			}

			t1 = t1b;
			t2 = t2b;
			t3 = t3b;
		}
		DiguNum = i;
	}
	//---------------------------------------------------
	if (flag_NewTon == 1)
	{
		printf("\t\t----------------------- >>> 已采用拟牛顿算法 BFGS \n");
	}



	//将递归结果返回全局变量保存
	tN_GlobalS_3N[numc][0] = t1;
	tN_GlobalS_3N[numc][1] = t2;
	tN_GlobalS_3N[numc][2] = t3;


	/* tN_GlobalS_4N[numc][0] = t_min[0];
	tN_GlobalS_4N[numc][1] = t_min[1];
	tN_GlobalS_4N[numc][2] = t_min[2];
	tN_GlobalS_4N[numc][3] = t_min[3];*/

	/* t1=t_min[0];
	t2=t_min[1];
	t3=t_min[2];
	t4=t_min[3];*/

	Energy_GlobalN[numc] = (knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12)*(knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12)
		+ (knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22)*(knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22)
		+ (knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32)*(knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32)
		;

	delete[]knn;
	//delete[]m;
	//---------------------------------------------------
}

/************************************* 自适应迭代递归过程 **********************************/
// DiguNum : 记录递归算法小循环
// DiguCnt : 记录递归算法大循环 N*65535
// Point_Check ：模式选择：算法点数
// ID_Pt：拟合的ID点号 （例子：0,4,5,6）
// ID_Length: 对应的ID点之间长度：
//            对应规则：// 1->2
//						   2->3
//						   1->3
//						   1->4
//						   2->4
//						   3->4
//例子：float E12=16;// 1->2
//	 float E22=16;// 2->3
//	 float E32=32;// 1->3
//	 float E42=32;// 1->4
//	 float E52=16;// 2->4
//	 float E62=64;// 3->4

// numc:四点拟合的组数
//Rude_Step_Control：扰动步长控制旋转
void GL_Build_MPoint_Process(int *ID_Pt,float*ID_Length,int numc,int &Point_Check,int &DiguCnt,int &DiguNum,int Rude_Step_Control)
{
	//GL_Build_YuX_M4Point(ID_Pt, ID_Length, tN_GlobalS_4A, Energy_GlobalA, tStep_nn_G, numc, Point_Check, DiguCnt, DiguNum, 1, Rude_Step_Control);
	if (!First_Fps[numc])//第一帧，拉近牛顿迭代法
	{
		printf("启动第一帧牛顿逼近预处理~~~~#########################\n");
		GL_Build_Steepest_M3Point(ID_Pt, ID_Length, tN_GlobalS_4A, Energy_GlobalA, numc, Point_Check, DiguCnt, DiguNum, 1, Rude_Step_Control);
		tN_GlobalS_4A[numc][3] = (tN_GlobalS_4A[numc][0] + tN_GlobalS_4A[numc][1] + tN_GlobalS_4A[numc][2]) / 3;
		GL_Build_NewTon_M4Point(ID_Pt, ID_Length, tN_GlobalS_4A, Energy_GlobalA, tStep_nn_G, numc, Point_Check, DiguCnt, DiguNum, 1, Rude_Step_Control);
		First_Fps[numc] = 1;
		
	}
	else
	{
		GL_Build_NewTon_M4Point(ID_Pt, ID_Length, tN_GlobalS_4A, Energy_GlobalA, tStep_nn_G, numc, Point_Check, DiguCnt, DiguNum, 1, Rude_Step_Control);
	}
	//GL_Build_Steepest_M3Point(ID_Pt, ID_Length, tN_GlobalS_4A, Energy_GlobalA, numc, Point_Check, DiguCnt, DiguNum, 1, Rude_Step_Control);
	//GL_Build_NewTon_M3Point(ID_Pt, ID_Length, tN_GlobalS_4A, Energy_GlobalA, numc, Point_Check, DiguCnt, DiguNum, 1, Rude_Step_Control);
	//GL_Build_QuasiNewTon_DFP_M3Point(ID_Pt, ID_Length, tN_GlobalS_4A, Energy_GlobalA, numc, Point_Check, DiguCnt, DiguNum, 1, Rude_Step_Control);
	//GL_Build_QuasiNewTon_BFGS_M3Point(ID_Pt, ID_Length, tN_GlobalS_4A, Energy_GlobalA, numc, Point_Check, DiguCnt, DiguNum, 1, Rude_Step_Control);
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
/***************************** 计算能量函数 *****************************/
void GL_Energy()
{
	//-------------------------------- 测试梯度下降算法
#if 0
	int Ces=2;
	if(Ces==0)
	{
		// y=x*x/2-2*x
		// y偏导= x-2
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

		printf("误差 = %f ,最优解 = %f \n",Ee,x);
	}
	if(Ces==1)
	{
		// f(x,y)=(x-7)*(x-7) + 2*(y-4)*(y-4)+3
		// f的x偏导数 = 2*(x-7)/2
		// f的y偏导数 = 4*(y-4)/2
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

		//printf("误差xy = (%f, %f) ,最优解xy = (%f,%f) \n",Eex,Eey,x,y);
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
	if(!Best_Flag && EnableX[0]&&EnableY[0] && EnableX[5]&&EnableY[5] && EnableX[4]&&EnableY[4]// 扫描点 使能判断，满足使能条件 则 进行算法迭代。
	    && EnableX[6]&&EnableY[6] 
		&& EnableX[1]&&EnableY[1] 
		&& EnableX[9] && EnableY[9]
	)
	{
		printf("/****************************************************************************/\n");
		 int DiguNum=0;//记录递归算法小循环
		 int DiguCnt=0;//记录递归算法大循环 N*65535
		 int Point_Check;//模式选择：算法点数
		 int i=0;
		 
		 long start_time=clock();//Cpu测试算法时间 起始：

		 //---------------迭代模式选择
		 Point_Check=4;
		 //
		 //建模用的头显 ID
		 int ID_Pt1[4]={9,1,4,5};
		 int ID_Pt2[4]={9,1,4,6};
		 //建模用的头显 ID之间的长度平方
		 float ID_Length1[6];
		 float ID_Length2[6];

		 GL_Got_Length(ID_Pt1,ID_Length1);
		 GL_Got_Length(ID_Pt2,ID_Length2);
		 //-------------------------------------------------------------------------------------------递归算法 Start
		 //GL_Build_Steepest_M(Point_Check,DiguCnt,DiguNum );
		 //4点拟合
		 if(Point_Check==4)
		 {
			 int numc=0;//四点拟合组数选择

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

			 GL_Build_MPoint_Process(ID_Pt1,ID_Length1,numc,Point_Check,DiguCnt,DiguNum,0);
			 numc=1;//四点拟合组数选择
			 GL_Build_MPoint_Process(ID_Pt2,ID_Length2,numc,Point_Check,DiguCnt,DiguNum,1);
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

			 //返回用于打印显示
			 numc=0;
			 t1_Global=tN_GlobalS_4A[numc][0];
			 t2_Global=tN_GlobalS_4A[numc][1];
			 t3_Global=tN_GlobalS_4A[numc][2];
			 t4_Global=tN_GlobalS_4A[numc][3];
		 }
		 
		 //5点拟合
		 /*if(Point_Check==5)
		 {
			 GL_Build_Steepest_M5Point(Point_Check,DiguCnt,DiguNum );
		 }*/
		 
		 Best_Flag=1;//置标志位
		 //-------------------------------------------------------------------------------------------递归算法 Finsh
		
		 long end_time=clock();//Cpu 算法计时完成
		 float timex=float(end_time-start_time);
		 
		 printf("                       ------<< %d 点估计 >>------ \n",Point_Check);
		 printf("                             <<< 第 %d 帧 >>>\n",Fps_World);
		
		 //--------------------------------------------------
		 /* for(int i=0;i<2;i++)
		  {
		  printf(" %d) 递归步长： %f \n",i+1,tStep_nn_G[i]);
		  }*/
		 
		 //------------------------
		/* float xx=LineRays[0].x*t1_Global+LineRays[0].pt0.x;
		 float yy=LineRays[0].y*t1_Global+LineRays[0].pt0.y;
		 float zz=LineRays[0].z*t1_Global+LineRays[0].pt0.z;
		 float dst_print=GL_Distance(LineRays[0].pt0.x,LineRays[0].pt0.y,LineRays[0].pt0.z,xx,yy,zz);
		 printf("目标到扫描原点距离：%f\n",dst_print);*/
		 //------------------------
		 for(int i=0;i<10;i++)
		 {
			 printf("  %d) dis_minxy: (%f,%f)\n",i+1,DistsX_min[i],DistsY_min[i]);
		 }
		 /* for(int i=0;i<10;i++)
		 {
		 printf("  %d) 二维投影 : (%f,%f)\n",i+1,CV_PointS[i].x,CV_PointS[i].y);
		 }*/
		 
		 FILE *file=fopen(".//logcpu.txt","a+");
		 fprintf(file,"/***************************************/\n");
		 fprintf(file,"CPU递归时间 ： %f \n",timex);
		
		//printf("------ >>>  最小能量 ： %f\n",dst_min);
		//------
		float x1=LineRays[9].x*t1_Global+LineRays[9].pt0.x;
		float y1=LineRays[9].y*t1_Global+LineRays[9].pt0.y;
		float z1=LineRays[9].z*t1_Global+LineRays[9].pt0.z;
		float xp0=HeadPlay_Pt[9].x;
		float yp0=HeadPlay_Pt[9].y;
		float zp0=HeadPlay_Pt[9].z;
		//printf("/***************************************/\n");
		printf("单位：每厘米 为 0.4 \n");
		printf("真实值 : (%f,%f,%f)\n",xp0,yp0,zp0);
		printf("估计值 : (%f,%f,%f)\n",x1,y1,z1);

		printf("上一帧真实值 : (%f,%f,%f)\n",xp0_Old,yp0_Old,zp0_Old);
		printf("上一帧估计值 : (%f,%f,%f)\n",x1_Old,y1_Old,z1_Old);

		printf("真实速度= (%f,%f,%f)\n",(xp0-xp0_Old),(yp0-yp0_Old),(zp0-zp0_Old));
		printf("估计速度= (%f,%f,%f)\n",(x1-x1_Old),(y1-y1_Old),(z1-z1_Old));

		printf("误差值 ：(%f,%f,%f)\n",abs(xp0-x1),abs(yp0-y1),abs(zp0-z1));
		//------
		if(abs(xp0-x1)<2 && abs(yp0-y1)<2 && abs(zp0-z1)<2)
		{
			Cnt_Correct++;
		}
		else
		{
			Cnt_Err++;
		}

		printf("4.5*4.5空间 >>> （ 5cm范围 ）达标与不达标个数： ( %d , %d )\n",Cnt_Correct,Cnt_Err);
		
		//------------------------------------
		//多点显示
		float x_ture1[5],y_ture1[5],z_ture1[5];//真实
		float x_etm1[5],y_etm1[5],z_etm1[5];//估计
		float x_ture2[5],y_ture2[5],z_ture2[5];//真实
		float x_etm2[5],y_etm2[5],z_etm2[5];//估计

		for(int i=0;i<Point_Check;i++)
		{
			//获取估计值
			x_etm1[i]=LineRays[ID_Pt1[i]].x*tN_GlobalS_4A[0][i]+LineRays[ID_Pt1[i]].pt0.x;
			y_etm1[i]=LineRays[ID_Pt1[i]].y*tN_GlobalS_4A[0][i]+LineRays[ID_Pt1[i]].pt0.y;
			z_etm1[i]=LineRays[ID_Pt1[i]].z*tN_GlobalS_4A[0][i]+LineRays[ID_Pt1[i]].pt0.z;
			//
			x_etm2[i]=LineRays[ID_Pt2[i]].x*tN_GlobalS_4A[1][i]+LineRays[ID_Pt2[i]].pt0.x;
			y_etm2[i]=LineRays[ID_Pt2[i]].y*tN_GlobalS_4A[1][i]+LineRays[ID_Pt2[i]].pt0.y;
			z_etm2[i]=LineRays[ID_Pt2[i]].z*tN_GlobalS_4A[1][i]+LineRays[ID_Pt2[i]].pt0.z;
			//获取真实值
			x_ture1[i]=HeadPlay_Pt[ID_Pt1[i]].x;
			y_ture1[i]=HeadPlay_Pt[ID_Pt1[i]].y;
			z_ture1[i]=HeadPlay_Pt[ID_Pt1[i]].z;
			//
			x_ture2[i]=HeadPlay_Pt[ID_Pt2[i]].x;
			y_ture2[i]=HeadPlay_Pt[ID_Pt2[i]].y;
			z_ture2[i]=HeadPlay_Pt[ID_Pt2[i]].z;
		}
	
		//-----------------------------
		printf("-------------------------- 冗余多点信息 ：\n");
		printf("---------------------- Point 1 ----------------------\n");
		FILE *file_Err=fopen(".//ErronPos.txt","a+");
		//-------------------------------------- 1
		for(int i=0;i<Point_Check;i++)
		{
			printf(" %d) 真实值：(%f,%f,%f) \n",i+1,x_ture1[i],y_ture1[i],z_ture1[i]);
			printf("     估计值：(%f,%f,%f) \n",    x_etm1[i],y_etm1[i],z_etm1[i]);
			printf("     向量  ：(%f,%f,%f) \n", LineRays[ID_Pt1[i]].x,LineRays[ID_Pt1[i]].y,LineRays[ID_Pt1[i]].z);
			printf("     误差值：(%f,%f,%f) \n", ABS(x_ture1[i] - x_etm1[i]), ABS(y_ture1[i] - y_etm1[i]), ABS(z_ture1[i] - z_etm1[i]));

			if( ABS(x_ture1[i]-x_etm1[i])<1.0f && ABS(y_ture1[i]-y_etm1[i])<1.0f && ABS(z_ture1[i]-z_etm1[i])<1.0f )
			{
				printf("------------------------------->> Success !!!\n");
				Right_CntBest[0]++;
			}
			else
			{
				printf("------------------------------->> Erron   !!!………………\n");
				Erron_CntBest[0]++;

				//错误情况进行记录用于分析
				fprintf(file_Err,"                       ------<< %d 点估计 >>------ \n",Point_Check);
				fprintf(file_Err,"                             <<< 第 %d 帧 >>>\n",Fps_World);
				fprintf(file_Err,"递归次数：%d*65535 + %d \n",DiguCnt-1,DiguNum);
				fprintf(file_Err,"CPU递归时间 ： %f \n",timex);
//				fprintf(file_Err,"目标到扫描原点距离：%f\n",dst_print);
				fprintf(file_Err,"%d) 错误情况: 点ID:%d\n",Erron_CntBest,i+1);
				fprintf(file_Err,"    真实值：(%f,%f,%f)\n",x_ture1[i],y_ture1[i],z_ture1[i]);
				fprintf(file_Err,"    估计值：(%f,%f,%f)\n",x_etm1[i],y_etm1[i],z_etm1[i]);
				fprintf(file_Err,"    误差值：(%f,%f,%f)\n",ABS(x_ture1[i]-x_etm1[i]),ABS(y_ture1[i]-y_etm1[i]),ABS(z_ture1[i]-z_etm1[i]));
				fprintf(file_Err,"    PRY姿态角：(%d,%d,%d)\n",Pitch_angle,Rool_angle,Yaw_angle);
			}
		}
		
		printf("---------------------- Point 2 ----------------------\n");
		//------------------------------------ 2
		for(int i=0;i<Point_Check;i++)
		{
			printf(" %d) 真实值：(%f,%f,%f) \n",i+1,x_ture2[i],y_ture2[i],z_ture2[i]);
			printf("     估计值：(%f,%f,%f) \n",    x_etm2[i],y_etm2[i],z_etm2[i]);
			printf("     向量  ：(%f,%f,%f) \n", LineRays[ID_Pt2[i]].x,LineRays[ID_Pt2[i]].y,LineRays[ID_Pt2[i]].z);
			printf("     误差值：(%f,%f,%f) \n", ABS(x_ture2[i] - x_etm2[i]), ABS(y_ture2[i] - y_etm2[i]), ABS(z_ture2[i] - z_etm2[i]));
			
			if( ABS(x_ture2[i]-x_etm2[i])<1.0f && ABS(y_ture2[i]-y_etm2[i])<1.0f && ABS(z_ture2[i]-z_etm2[i])<1.0f )
			{
				printf("------------------------------->> Success !!!\n");
				Right_CntBest[1]++;
			}
			else
			{
				printf("------------------------------->> Erron   !!!………………\n");
				Erron_CntBest[1]++;

				//错误情况进行记录用于分析
				
				//				fprintf(file_Err,"目标到扫描原点距离：%f\n",dst_print);
				fprintf(file_Err, "    \n");
				fprintf(file_Err, "    真实值：(%f,%f,%f) 2组\n", x_ture2[i], y_ture2[i], z_ture2[i]);
				fprintf(file_Err, "    估计值：(%f,%f,%f) 2组\n", x_etm2[i], y_etm2[i], z_etm2[i]);
				fprintf(file_Err, "    误差值：(%f,%f,%f) 2组\n", ABS(x_ture2[i] - x_etm2[i]), ABS(y_ture2[i] - y_etm2[i]), ABS(z_ture2[i] - z_etm2[i]));
			}
		}

		fclose(file_Err);
		//显示能量值
		for(int i=0;i<2;i++)
		{
			printf( "%d) 能量值 ： %f\n",i+1,Energy_GlobalA[i]);
		}

		for(int i=0;i<2;i++)
		{
			printf("@:::::::::::::::::: Point%d (2.5cm以内)最优错误计数 ： %d\n",i+1,Erron_CntBest[i]);
			printf("@!!!!!!!!!!!!!!!!!! Point%d (2.5cm以内)最优正确计数 ： %d\n",i+1,Right_CntBest[i]);
			printf("@!!!!!!!!!!!!!!!! Point%d (2.5cm以内)错误计算百分率 ： %f\n", 1 + 1, float(Erron_CntBest[i]) / float(Erron_CntBest[i] + Right_CntBest[i]));
			printf("\n");
		}

		//---------------------
		printf("CPU递归时间 ： %f 毫秒\n", timex);
		printf("帧率        ： %f Fps \n", 1000.0f / timex);
		printf("递归次数：%d*65535 + %d \n", DiguCnt - 1, DiguNum);
		if (timex > 15)
		{
			printf("------------------->> 算法超时 !!! \n");
			Cnt_Cpu_Best++;
		}
		else
		{
			printf("------------------->> 算法OK   !!! \n");
		}
		printf("------------------->> 算法超时次数 ：%d \n", Cnt_Cpu_Best);
		
		//---------------------
		
		fprintf(file,"单位：每厘米 为 0.4 \n");
		fprintf(file,"真实值 : (%f,%f,%f)\n",xp0,yp0,zp0);
		fprintf(file,"估计值 : (%f,%f,%f)\n",x1,y1,z1);

		fprintf(file,"误差值 ：(%f,%f,%f)\n",abs(xp0-x1),abs(yp0-y1),abs(zp0-z1));

		
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
		float x1=LineRays[9].x*t1_Global+LineRays[9].pt0.x;
		float y1=LineRays[9].y*t1_Global+LineRays[9].pt0.y;
		float z1=LineRays[9].z*t1_Global+LineRays[9].pt0.z;

		/*float x2=LineRays[0].x*t2_Global+LineRays[0].pt0.x;
		float y2=LineRays[0].y*t2_Global+LineRays[0].pt0.y;
		float z2=LineRays[0].z*t2_Global+LineRays[0].pt0.z;

		float x3 = LineRays[4].x*t3_Global + LineRays[6].pt0.x;
		float y3 = LineRays[4].y*t3_Global + LineRays[6].pt0.y;
		float z3 = LineRays[4].z*t3_Global + LineRays[6].pt0.z;

		float x4 = LineRays[5].x*t4_Global + LineRays[5].pt0.x;
		float y4 = LineRays[5].y*t4_Global + LineRays[5].pt0.y;
		float z4 = LineRays[5].z*t4_Global + LineRays[5].pt0.z;*/

		

		glPushMatrix();//储存当前视图矩阵
		glLineWidth(1); 
		glColor3f(0.8,0.0,0.8); 
		glTranslatef(x1,y1,z1);
		glutWireSphere(1.6, 20, 20);
		glPopMatrix();//弹出上次保存的位置

		//glPushMatrix();//储存当前视图矩阵
		//glLineWidth(1); 
		//glColor3f(0.1,0.0,0.98); 
		//glTranslatef(x2,y2,z2);
		//glutSolidSphere(0.9, 20, 20);
		//glPopMatrix();//弹出上次保存的位置

		//glPushMatrix();//储存当前视图矩阵
		//glLineWidth(1);
		//glColor3f(0.1, 0.0, 0.98);
		//glTranslatef(x3, y3, z3);
		//glutSolidSphere(0.9, 20, 20);
		//glPopMatrix();//弹出上次保存的位置

		//glPushMatrix();//储存当前视图矩阵
		//glLineWidth(1);
		//glColor3f(0.1, 0.0, 0.98);
		//glTranslatef(x4, y4, z4);
		//glutSolidSphere(0.9, 20, 20);
		//glPopMatrix();//弹出上次保存的位置

		//glLineWidth(2);
		//glBegin(GL_LINES);//绘制居中轨迹
		//glColor3f(0.85f, 0.6f, 0.0f);
		//glVertex3f(x2,y2,z2);
		//glVertex3f(x3,y3,z3);
		//glVertex3f(x2,y2,z2);
		//glVertex3f(x4,y4,z4);
		//
		//glEnd();

		//glPushMatrix();//储存当前视图矩阵
		//glLineWidth(1); 
		//glColor3f(0.0,0.8,0.0); 
		//glTranslatef(x3,y3,z3);
		//glutSolidSphere(0.8, 20, 20);
		//glPopMatrix();//弹出上次保存的位置

		//if(Point_Check==4)
		//{
		//	glPushMatrix();//储存当前视图矩阵
		//	glLineWidth(1); 
		//	glColor3f(0.0,0.8,0.0); 
		//	glTranslatef(x4,y4,z4);
		//	glutSolidSphere(0.8, 20, 20);
		//	glPopMatrix();//弹出上次保存的位置
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
	//glPushMatrix();//储存当前视图矩阵
	//glRotatef(angle,0,1,0);
	//glPopMatrix();//储存当前视图矩阵
	//glEnable(GL_AUTO_NORMAL);
	/*************绘制网格线*************/
#if 0
	glBegin(GL_TRIANGLES);  
	glColor3f(0.92f, 0.08f, 0.02f );
	glVertex3f(0.0f,0.0f,0.0f);  
	glVertex3f(50.0f,0.0f,0.0f);  
	glVertex3f(25.0f,25.0f,0.0f);  

	glVertex3f(-25.0f,0.0f,0.0f);  
	glVertex3f(-75.0f,0.0f,0.0f);  
	glVertex3f(-50.0f,25.0f,0.0f);  
	//结束绘三角形  
	glEnd(); 
#endif
	angle_GL++;
//--------------------------------
	//glPushMatrix();//储存当前视图矩阵
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
	GLB_IMU(gxr_Global,gyr_Global, gzr_Global,1,Ww ) ;
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
	if (Angle_Y > 87)
	{
		//if (Best_Flag==0)
		//GLB_Line_3Point_Quick();
		//Best_Flag = 1;
		GL_Energy();

		//glLineWidth(3);
		//glBegin(GL_LINES);//绘制居中轨迹
		//glColor3f(0.85f, 0.6f, 0.0f);
		//glVertex3f(Edge_HW_x, Edge_ZW_y, 0);
		//glVertex3f(x_Quick[1], y_Quick[1], z_Quick[1]);
		//glEnd();
		////
		//glLineWidth(3);
		//glBegin(GL_LINES);//绘制居中轨迹
		//glColor3f(0.85f, 0.6f, 0.0f);
		//glVertex3f(x_Quick[0], y_Quick[0], z_Quick[0]);
		//glVertex3f(x_Quick[2], y_Quick[2], z_Quick[2]);
		//glEnd();
		////
		//glPushMatrix();
		//glLineWidth(1);
		//glTranslatef(x_Quick[0], y_Quick[0], z_Quick[0]);

		//glRotatef(angle_GL, 0, 1, 0);

		//glColor3f(1.0f, 0.0f, 0.0f);

		//glutWireSphere(1, 15, 15);
		//glPopMatrix();
		////
		//glPushMatrix();
		//glLineWidth(1);
		//glTranslatef(x_Quick[1], y_Quick[1], z_Quick[1]);

		//glRotatef(angle_GL, 0, 1, 0);

		//glColor3f(0.0f, 1.0f, 0.0f);

		//glutWireSphere(1, 15, 15);
		//glPopMatrix();
		////
		//glPushMatrix();
		//glLineWidth(1);
		//glTranslatef(x_Quick[2], y_Quick[2], z_Quick[2]);

		//glRotatef(angle_GL, 0, 1, 0);

		//glColor3f(0.0f, 0.0f, 1.0f);

		//glutWireSphere(1, 15, 15);
		//glPopMatrix();
	}
	
	
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
DWORD _stdcall ThreadProc(LPVOID lpParameter)//线程执行函数
{
	//LastData_Init();
	glutCreateWindow("三维模型"); 
	int num=10;//初始化 目标点的X扫描面 与 Y扫描面 标志
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