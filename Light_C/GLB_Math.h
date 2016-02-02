#pragma once
#ifndef __GLB_MAT_H__
#define __GLB_MAT_H__
#include <glut.h>
#include <windows.h>
#include <math.h>
#include "Param_GL.h"

int Compass_Yaw_Data[8]={0};

/*********************************************************************/
//����ֱ�߶�Ӧ Normals Vector1(ax,ay,az) X Vector2 (bx,by,bz) 
//ƽ��1�� ax*X+ay*Y+az*Z+Da=0
//ƽ��2�� bx*X+by*Y+bz*Z+Db=0
GL_Line GLB_Line(float ax,float ay,float az,float Da,float bx,float by,float bz,float Db)
{
	GL_Line v;
	v.x=0;
	v.y=0;
	v.z=0;

	float N_X=(ay*bz-az*by);
	float N_Y=(az*bx-ax*bz);
	float N_Z=(ax*by-ay*bx);

	float norm=N_X*N_X + N_Y*N_Y + N_Z*N_Z;
	norm = sqrt(norm);

	if(norm!=0)
	{
		v.x=N_X/norm;
		v.y=N_Y/norm;
		v.z=N_Z/norm;
#if 0
		if((ax*by-ay*bx)!=0)
		{
			v.pt0.x=(ay*Db-by*Da)/(ax*by-ay*bx);
		}
		else
		{
			v.pt0.x=0;
		}


		if((ay*bx-ax*by)!=0)
		{
			v.pt0.y=(ax*Db-bx*Da)/(ay*bx-ax*by);
		}
		else
		{
			v.pt0.y=0;
		}
	
		v.pt0.z=0;
#else
		v.pt0.x=Edge_HW_x;
		v.pt0.y=Edge_ZW_y;
		v.pt0.z=0;
#endif
		return v;
	}

	return v;
}
/*********************************************************************/
//�����Ӧ Normals Vector1 X Vector2  
GL_Vector GLB_CHAJI(float ax,float ay,float az,float bx,float by,float bz)
{
	GL_Vector v;
	v.x=0;
	v.y=0;
	v.z=0;

	float N_X=(ay*bz-az*by);
	float N_Y=(az*bx-ax*bz);
	float N_Z=(ax*by-ay*bx);

	float norm=N_X*N_X + N_Y*N_Y + N_Z*N_Z;
	norm = sqrt(norm);
	
	if(norm!=0)
	{
		v.x=N_X/norm;
		v.y=N_Y/norm;
		v.z=N_Z/norm;
		return v;
	}

	return v;
}

/************************************��ά�����н�*********************************/
float GLB_Angle(float a1,float a2,float a3,float b1,float b2,float b3)
{
	float angle=0;
	float a=a1*b1+a2*b2+a3*b3;

	float b=a1*a1+a2*a2+a3*a3;
	b=sqrt(b);

	float c=b1*b1+b2*b2+b3*b3;
	c=sqrt(c);

	if(b*c!=0)
	{
		float angle_r=a/(b*c);
		angle=acos(angle_r);

		return angle;
	}
	return angle;
}
/************************************��ά������������ת*********************************/
GL_Point GLB_Point(float old_x,float old_y,float old_z,float angle,float x,float y,float z)
{
	GL_Point New_Pt;
	float new_x,new_y,new_z;

	//angle=3.1415926*20/180;
	float c=cos(angle);
	float s=sin(angle);

	new_x = (x*x*(1-c)+c) * old_x + (x*y*(1-c)-z*s) * old_y + (x*z*(1-c)+y*s) * old_z;

	new_y = (y*x*(1-c)+z*s) * old_x + (y*y*(1-c)+c) * old_y + (y*z*(1-c)-x*s) * old_z;

	new_z = (x*z*(1-c)-y*s) * old_x + (y*z*(1-c)+x*s) * old_y + (z*z*(1-c)+c) * old_z;

	New_Pt.x=new_x;
	New_Pt.y=new_y;
	New_Pt.z=new_z;

	return New_Pt;
}


/***************************************** �ų���ת *****************************************/
int GLB_Angle_Compass_Yaw(float mx,float my,float mz,float Rr,int PitchN,int RollN,int YawN)
{
	float p=-3.1415926*float(PitchN)/180;
	float r=-3.1415926*float(RollN)/180;
	float norm =mx*mx+my*my+mz*mz;
	norm=sqrt(norm);

	if(norm!=0)
	{
		mx/=norm;
		my/=norm;
		mz/=norm;
	}
	

	float Hx=mx*cos(p)+my*sin(r)*sin(p)-mz*cos(r)*sin(p);
	float Hy=my*cos(r)+mz*sin(r);

	//float Hx=HM_x*cos(Pitch)+HM_y*sin(Rool)*sin(Pitch)-HM_z*cos(Rool)*sin(Pitch);
	//float Hy=HM_y*cos(Rool)+HM_z*sin(Rool);


	float anglef=0;
	anglef=atan2(Hx,Hy);

	int Angle=180.0*anglef/3.1415926;

	if(Angle<=0)Angle=360+Angle;

	
	return Angle;
}

/***********************��������**************************/
//������� ����Ԫ��
//������� : ��Ԫ������
//��ʽ�鿴�� �����ĵ� ���������������ƽ�ģ -��̬����˵��
GL_Quater Conjugate_Q(GL_Quater QN)
{
	GL_Quater QN_C;//����
	float norm = sqrt(QN.q0*QN.q0+QN.q1*QN.q1+QN.q2*QN.q2+QN.q3*QN.q3);//��ģ

	QN_C.q0= QN.q0;
	QN_C.q1=-QN.q1;
	QN_C.q2=-QN.q2;
	QN_C.q3=-QN.q3;

	GL_Quater QN_I;//����

	QN_I.q0=QN_C.q0/norm;
	QN_I.q1=QN_C.q1/norm;
	QN_I.q2=QN_C.q2/norm;
	QN_I.q3=QN_C.q3/norm;

	return QN_I;
}

/***********************��Ԫ�����**************************/
//��ʽ�鿴�� �����ĵ� ���������������ƽ�ģ -��̬����˵��
//���������������Ԫ��
//���������������Ԫ���ĳ˻�
GL_Quater MUL_Q(GL_Quater Q1,GL_Quater Q2)
{
	CvMat* A1 = cvCreateMat(4,4,CV_32FC1);//����4*4��Ԫ������
	CvMat* A2 = cvCreateMat(4,1,CV_32FC1);//����4*1��Ԫ������
	CvMat* MM = cvCreateMat(4,1,CV_32FC1);//����4*1��Ԫ������

	float lm=Q1.q0;
	float p1=Q1.q1;
	float p2=Q1.q2;
	float p3=Q1.q3;
	//------4*4����ֵ
	cvmSet(A1,0,0,lm);
	cvmSet(A1,0,1,-p1);
	cvmSet(A1,0,2,-p2);
	cvmSet(A1,0,3,-p3);

	cvmSet(A1,1,0,p1);
	cvmSet(A1,1,1,lm);
	cvmSet(A1,1,2,-p3);
	cvmSet(A1,1,3,p2);

	cvmSet(A1,2,0,p2);
	cvmSet(A1,2,1,p3);
	cvmSet(A1,2,2,lm);
	cvmSet(A1,2,3,-p1);

	cvmSet(A1,3,0,p3);
	cvmSet(A1,3,1,-p2);
	cvmSet(A1,3,2,p1);
	cvmSet(A1,3,3,lm);
	//------4*1����ֵ

	cvmSet(A2,0,0,Q2.q0);
	cvmSet(A2,1,0,Q2.q1);
	cvmSet(A2,2,0,Q2.q2);
	cvmSet(A2,3,0,Q2.q3);
	//------

	cvMatMulAdd( A1, A2, 0, MM );//�������

	GL_Quater QN;
	//�����Ԫ�� ��ֵ
	QN.q0=cvmGet(MM,0,0);
	QN.q1=cvmGet(MM,1,0);
	QN.q2=cvmGet(MM,2,0);
	QN.q3=cvmGet(MM,3,0);
	//�ͷž��󻺴�
	cvReleaseMat(&A1);
	cvReleaseMat(&A2);
	cvReleaseMat(&MM);

	return QN;//������� ��Ԫ��
}

//���Ϊ4Ԫ��
/************************ ��Ԫ����ʼ�� ***********************/
//	��ڲ�����
// 	acx  x���������ٶ�
// 	acy  y���������ٶ�
// 	acz  z���������ٶ�
//      a    ��ʼ��Ϊ 0

//	���ȫ�֣���Ԫ��

void Element_4_int(float acx ,float acy ,float acz, float a)
{
	float rot_xx= atan2(acy,acz);//���� ��λ
	float rot_yy= -asin(acx/G0);

	float rot_zz= a;
	//****** ��Ԫ����ʼ�� ******//
	q0 = cos(rot_xx/2)*cos(rot_yy/2)*cos(rot_zz/2)+sin(rot_xx/2)*sin(rot_yy/2)*sin(rot_zz/2);

	q1 = sin(rot_xx/2)*cos(rot_yy/2)*cos(rot_zz/2)-cos(rot_xx/2)*sin(rot_yy/2)*sin(rot_zz/2);

	q2 = cos(rot_xx/2)*sin(rot_yy/2)*cos(rot_zz/2)+sin(rot_xx/2)*cos(rot_yy/2)*sin(rot_zz/2);

	q3 = cos(rot_xx/2)*cos(rot_yy/2)*sin(rot_zz/2)-sin(rot_xx/2)*sin(rot_yy/2)*cos(rot_zz/2);

}

/***********************************************   ��Ԫ�����º���   *********************************************/
//	��ڲ����� 
//	gxr x����ٶȣ��ȣ�
//	gyr y����ٶȣ��ȣ�
//	gzr z����ٶȣ��ȣ�

//	halfT ����ʱ��

//	���ȫ�֣���Ԫ�����Ƕ� �����ȣ�

void GLB_IMU(int gxr, int gyr, int gzr, float halfT ) 
{
	float gx=float(gxr)*CV_PI/180.0f;
	float gy=float(gyr)*CV_PI/180.0f;
	float gz=float(gzr)*CV_PI/180.0f;
        // ������Ԫ��
        float q0r = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
        float q1r = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
        float q2r = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
        float q3r = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

		q0=q0r;
		q1=q1r;
		q2=q2r;
		q3=q3r;
  
	//��Ԫ���淶��
       float norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	   if(norm!=0)
	   {
		   q0 = q0 / norm;
		   q1 = q1 / norm;
		   q2 = q2 / norm;
		   q3 = q3 / norm;
	   }

	   Ww=acos(q0)*2*180/CV_PI;//---- �ں� ��ƫ�� q0 ��Ϊ qq0

	//���·������Ҿ���
	float t11=q0*q0+q1*q1-q2*q2-q3*q3;
	float t12=2.0*(q1*q2+q0*q3);
	float t13=2.0*(q1*q3-q0*q2);
	float t21=2.0*(q1*q2-q0*q3);
	float t22=q0*q0-q1*q1+q2*q2-q3*q3;
	float t23=2.0*(q2*q3+q0*q1);
	float t31=2.0*(q1*q3+q0*q2);
	float t32=2.0*(q2*q3-q0*q1);
	float t33=q0*q0-q1*q1-q2*q2+q3*q3;

	////------ ƫ���ǡ���Yaw,�����ǡ���Pitch,�����ǡ���Rool  �����ȣ�
	Rool = atan2(t23,t33);
	Pitch = -asin(t13);

	Yaw = atan2(t12,t11);

	if (Yaw < 0) Yaw += 360/180*3.14159;
	
	//--- ����תΪ�Ƕ�
	Rool_angle = Rool/CV_PI*180;
	Pitch_angle = Pitch/CV_PI*180;
	Yaw_angle = Yaw/CV_PI*180;
	

	
}

/************************************************************************/
//AB��cos�н�=������AC*����AB��/��AC��ģ*AB��ģ��
float GL_COS_Angle(GL_Vector A,GL_Vector B)
{
	float a=A.x*B.x+A.y*B.y+A.z*B.z;
	float b1=A.x*A.x+A.y*A.y+A.z*A.z;
	float b2=B.x*B.x+B.y*B.y+B.z*B.z;
	float b=sqrt(b1*b2);

	float angle_f=0;
	if(b!=0)
	{
		angle_f=acos(a/b);
	}
	return angle_f;
}
#endif