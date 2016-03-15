/**************************��ز���************************/
#pragma once
#ifndef __PARAM_GL__
#define __PARAM_GL__
#include <cv.h>
#include <opencv2/opencv.hpp>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include <glut.h>
#include <math.h>

#define ABS(x)((x)>0?(x):-(x))
#define STEPNN 0.000051f
#define Scan_Step 0.0101f //���� Motor���˶�����

//Motor ���� �˶�
float Edge_HW_x = -90.0f;
float Edge_HW_y = 81.0f;
float Edge_HW_z = 0.0f;
//Motor ���� �˶�
float Edge_ZW_x = -88.0f;
float Edge_ZW_y = 83.0f;
float Edge_ZW_z = -0.0f;

//float Edge_ZW_x = -90.0f;
//float Edge_ZW_y = 81.0f;
//float Edge_ZW_z = -0.0f;

float x_Quick[3],y_Quick[3],z_Quick[3];

int rot_gl_look=0;

typedef struct _GL_Vector_
{
	float x;
	float y;
	float z;
}GL_Vector;

typedef struct _GL_Line_
{
	float x;
	float y;
	float z;
	GL_Vector pt0; 
}GL_Line;

GL_Vector Plan_XNorml,Plan_YNorml;// X/Yɨ�� ƽ�� ������
GL_Line LineRays[10];//ͷ�Ե�� ����
typedef struct _GL_Plan_
{
	float A;
	float B;
	float C;
	float D;
}GL_Plan;

GL_Plan GL_PlanX,GL_PlanY; // X/Yɨ�� ƽ��

GL_Plan GL_PlanXS[10],GL_PlanYS[10];//ͷ�Ե�� �������� Xƽ�桢Yƽ��

typedef struct _GL_Point_
{
	float x;
	float y;
	float z;

}GL_Point;

GL_Point HeadPlay_Pt[10];//ͷ������
GL_Point HeadPlay_PtB[10];//ͷ����� ���꣬�㷨��ģʱʹ�á�
bool EnableX[10];//Xɨ�� ʹ��
bool EnableY[10];//Yɨ�� ʹ��

GL_Point CV_PointS[10];//ƽ��ӳ������

typedef struct _GL_Quater_ //��Ԫ������ �ṹ��
{
	float q0;
	float q1;
	float q2;
	float q3;
}GL_Quater;

//-------------------------------------------------------
float q0 = 1, q1 = 0, q2 = 0, q3 = 0;        // ��Ԫ��
float Ww=0;

float qB0 = 1, qB1 = 0, qB2 = 0, qB3 = 0;        // ���� ��Ԫ��
float WwB = 0;

// ƫ���ǡ���Yaw,�����ǡ���Pitch,�����ǡ���Rool
float Pitch;//����
float Rool ;
float Yaw  ;

short Pitch_angle;//�Ƕ�
short Rool_angle ;
short Yaw_angle  ;

float PitchB;//���� ����
float RoolB;
float YawB;

short Pitch_angleB=-90;//����ŷ���ǽǶ�
short Rool_angleB=0;
short Yaw_angleB=0;

float G0=9.8;//������ʼ��

//-------------------------------------------------------- ����������
float xp0_Old=0,yp0_Old=0,zp0_Old=0;// ǰһ֡ ��ʵ����
float x1_Old=0,y1_Old=0,z1_Old=0;   // ǰһ֡ ��������

GL_Point GL_SamplePt[500];//500 ֡ ���������
GL_Point GL_Sample_AvgPt; //��������� ��ֵ
GL_Point GL_Sample_VarianzPt;//������

int Erron_CntBest[6]={0,0,0,0,0,0};//�������
int Right_CntBest[6]={0,0,0,0,0,0};//�������
//-------------------------------------------------------- FPS Tracking Start
bool Fps_Track_Start=0;

float Step_nn_Global;
int Cnt_Cpu_Best=0;//�㷨��ʱ�ܴ���

//-------------------------------------- ��ת���� 12δ֪����ģ,��ת����9����ƽ��3��
float C11 = 1, C12 = 0, C13 = 0;
float C21 = 0, C22 = 1, C23 = 0;
float C31 = 0, C32 = 0, C33 = 1;

float Tx = 90, Ty = 50, Tz = 90;

bool First_Fps[6] = {0,0,0,0,0,0};//��һ֡�ӳ�Ԥ����

#endif