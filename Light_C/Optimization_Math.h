#pragma once
#ifndef __OPTINIZATION_MATH__
#define __OPTIMIZATION_MATH__
#include "Param_GL.h"

/*****************************构建模型方程系数（3点）*****************************/
void GLB_Knn(float Knn[][3], float a1, float b1, float c1, GL_Vector pt1, float a2, float b2, float c2, GL_Vector pt2, int num)
{
	Knn[num][0] = a1*a1 + b1*b1 + c1*c1;
	Knn[num][1] = a2*a2 + b2*b2 + c2*c2;
	Knn[num][2] = -2.0f*(a1*a2 + b1*b2 + c1*c2);
}
/*****************************构建模型方程系数（）*****************************/
void GLB_Knn2(float Knn[][6], float a1, float b1, float c1, GL_Vector pt1, float a2, float b2, float c2, GL_Vector pt2, int num)
{
	Knn[num][0] = a1*a1 + b1*b1 + c1*c1; // t1*t1
	Knn[num][1] = a2*a2 + b2*b2 + c2*c2; // t2*t2
	Knn[num][2] = -2.0f*(a1*a2 + b1*b2 + c1*c2); // t1*t2
	Knn[num][3] = 2.0f*((pt1.x - pt2.x)*a1 + (pt1.y - pt2.y)*b1 + (pt1.z - pt2.z)*c1); // t1
	Knn[num][4] = -2.0f*((pt1.x - pt2.x)*a2 + (pt1.y - pt2.y)*b2 + (pt1.z - pt2.z)*c2); // t2
	Knn[num][5] = (pt1.x - pt2.x)*(pt1.x - pt2.x) + (pt1.y - pt2.y)*(pt1.y - pt2.y) + (pt1.z - pt2.z)*(pt1.z - pt2.z);// 常数
}

/*****************************获得三维点之间距离*****************************/
float GL_Distance(float x1, float y1, float z1, float x2, float y2, float z2)
{
	float dis = 0;
	dis = (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + (z1 - z2)*(z1 - z2);
	dis = sqrt(dis);
	return dis;
}

/************求12*12行列式***********/
//用1）迭代思想，迭代到1*1行列式，2）以及代数余子式计算行列式的思想
// n:为矩阵阶数
float Fun12(int n, float a[][12])
{
	float b[12][12] = { { 0 } }; /*定义数组b并初始化*/
	int i = 0, j = 0;//i，j为行与列
	float sum = 0; /*，sum为行列式的值*/
	int x = 0, c = 0, p = 0; /*用x判断加与减，c,p为中间变量*/

	if (n == 1)
		return a[0][0];

	for (i = 0; i < n; i++) /*此处大循环实现将余子式存入数组b中*/
	{
		for (c = 0; c < n - 1; c++)
		{
			for (j = 0; j < n - 1; j++)
			{
				if (c < i){ /*借助c判断每行的移动方法*/
					p = 0; /*当p=0时，行列式只向左移，即消去对应的第一列的数*/
				}
				else{ /*否则行列式左移后再上移*/
					p = 1;
				}
				b[c][j] = a[c + p][j + 1];
			}
		}

		if (i % 2 == 0){ /*i+j（此时j=0，故只考虑i）为偶数，加法预算*/
			x = 1;
		}
		else{ /*i+j为奇数，减法运算*/
			x = (-1);
		}
		sum += a[i][0] * Fun12(n - 1, b) * x; /*计算行列式的值*/
	}

	return sum; /*将值返回*/
}

/************求5*5行列式***********/
//用1）迭代思想，迭代到1*1行列式，2）以及代数余子式计算行列式的思想
// n:为矩阵阶数
float Fun5(int n, float a[][5])
{
	float b[5][5] = { { 0 } }; /*定义数组b并初始化*/
	int i = 0, j = 0;//i，j为行与列
	float sum = 0; /*，sum为行列式的值*/
	int x = 0, c = 0, p = 0; /*用x判断加与减，c,p为中间变量*/

	if (n == 1)
		return a[0][0];

	for (i = 0; i < n; i++) /*此处大循环实现将余子式存入数组b中*/
	{
		for (c = 0; c < n - 1; c++)
		{
			for (j = 0; j < n - 1; j++)
			{
				if (c < i){ /*借助c判断每行的移动方法*/
					p = 0; /*当p=0时，行列式只向左移，即消去对应的第一列的数*/
				}
				else{ /*否则行列式左移后再上移*/
					p = 1;
				}
				b[c][j] = a[c + p][j + 1];
			}
		}

		if (i % 2 == 0){ /*i+j（此时j=0，故只考虑i）为偶数，加法预算*/
			x = 1;
		}
		else{ /*i+j为奇数，减法运算*/
			x = (-1);
		}
		sum += a[i][0] * Fun5(n - 1, b) * x; /*计算行列式的值*/
	}

	return sum; /*将值返回*/
}
/************求4*4行列式***********/
//用1）迭代思想，迭代到1*1行列式，2）以及代数余子式计算行列式的思想
// n:为矩阵阶数
float Fun4(int n, float a[][4])
{
	float b[4][4] = { { 0 } }; /*定义数组b并初始化*/
	int i = 0, j = 0;//i，j为行与列
	float sum = 0; /*，sum为行列式的值*/
	int x = 0, c = 0, p = 0; /*用x判断加与减，c,p为中间变量*/

	if (n == 1)
		return a[0][0];

	for (i = 0; i < n; i++) /*此处大循环实现将余子式存入数组b中*/
	{
		for (c = 0; c < n - 1; c++)
		{
			for (j = 0; j < n - 1; j++)
			{
				if (c < i){ /*借助c判断每行的移动方法*/
					p = 0; /*当p=0时，行列式只向左移，即消去对应的第一列的数*/
				}
				else{ /*否则行列式左移后再上移*/
					p = 1;
				}
				b[c][j] = a[c + p][j + 1];
			}
		}

		if (i % 2 == 0){ /*i+j（此时j=0，故只考虑i）为偶数，加法预算*/
			x = 1;
		}
		else{ /*i+j为奇数，减法运算*/
			x = (-1);
		}
		sum += a[i][0] * Fun4(n - 1, b) * x; /*计算行列式的值*/
	}

	return sum; /*将值返回*/
}
/************求3*3行列式***********/
//用1）迭代思想，迭代到1*1行列式，2）以及代数余子式计算行列式的思想
// n:为矩阵阶数
float Fun3(int n, float a[][3])
{
	float b[3][3] = { { 0 } }; /*定义数组b并初始化*/
	int i = 0, j = 0;//i，j为行与列
	float sum = 0; /*，sum为行列式的值*/
	int x = 0, c = 0, p = 0; /*用x判断加与减，c,p为中间变量*/

	if (n == 1)
		return a[0][0];

	for (i = 0; i < n; i++) /*此处大循环实现将余子式存入数组b中*/
	{
		for (c = 0; c < n - 1; c++)
		{
			for (j = 0; j < n - 1; j++)
			{
				if (c < i){ /*借助c判断每行的移动方法*/
					p = 0; /*当p=0时，行列式只向左移，即消去对应的第一列的数*/
				}
				else{ /*否则行列式左移后再上移*/
					p = 1;
				}
				b[c][j] = a[c + p][j + 1];
			}
		}

		if (i % 2 == 0){ /*i+j（此时j=0，故只考虑i）为偶数，加法预算*/
			x = 1;
		}
		else{ /*i+j为奇数，减法运算*/
			x = (-1);
		}
		sum += a[i][0] * Fun3(n - 1, b) * x; /*计算行列式的值*/
	}

	return sum; /*将值返回*/
}

/************************************/
void Fun12_Test()
{
	printf("\t12*12 行列式求解测试\n\n");
	float a[5][5];
	//
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (i == j)
				a[i][j] = 10;
			else
				a[i][j] = 0;
		}
	}
	//
	float A = Fun5(5, a);
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			printf(" %f ,", a[i][j]);
		}
		printf("\n");
	}
	//
	printf("\n\t行列式的值： %f\n\n", A);
}

/***************************求逆矩阵********************************/
//r 逆矩阵
//m 原矩阵
//A 原矩阵行列式

void Ivin(float r[][4],float m[][4],float A)
{
	//
	float mr[3][3];
	mr[0][0] = m[1][1]; mr[0][1] = m[1][2]; mr[0][2] = m[1][3];
	mr[1][0] = m[2][1]; mr[1][1] = m[2][2]; mr[1][2] = m[2][3];
	mr[2][0] = m[3][1]; mr[2][1] = m[3][2]; mr[2][2] = m[3][3];

	r[0][0] = Fun3(3, mr);

	mr[0][0] = m[1][0]; mr[0][1] = m[1][2]; mr[0][2] = m[1][3];
	mr[1][0] = m[2][0]; mr[1][1] = m[2][2]; mr[1][2] = m[2][3];
	mr[2][0] = m[3][0]; mr[2][1] = m[3][2]; mr[2][2] = m[3][3];

	r[1][0] = -Fun3(3, mr);

	mr[0][0] = m[1][0]; mr[0][1] = m[1][1]; mr[0][2] = m[1][3];
	mr[1][0] = m[2][0]; mr[1][1] = m[2][1]; mr[1][2] = m[2][3];
	mr[2][0] = m[3][0]; mr[2][1] = m[3][1]; mr[2][2] = m[3][3];

	r[2][0] = Fun3(3, mr);

	mr[0][0] = m[1][0]; mr[0][1] = m[1][1]; mr[0][2] = m[1][2];
	mr[1][0] = m[2][0]; mr[1][1] = m[2][1]; mr[1][2] = m[2][2];
	mr[2][0] = m[3][0]; mr[2][1] = m[3][1]; mr[2][2] = m[3][2];

	r[3][0] = -Fun3(3, mr);

	//
	mr[0][0] = m[0][1]; mr[0][1] = m[0][2]; mr[0][2] = m[0][3];
	mr[1][0] = m[2][1]; mr[1][1] = m[2][2]; mr[1][2] = m[2][3];
	mr[2][0] = m[3][1]; mr[2][1] = m[3][2]; mr[2][2] = m[3][3];

	r[0][1] = -Fun3(3, mr);

	mr[0][0] = m[0][0]; mr[0][1] = m[0][2]; mr[0][2] = m[0][3];
	mr[1][0] = m[2][0]; mr[1][1] = m[2][2]; mr[1][2] = m[2][3];
	mr[2][0] = m[3][0]; mr[2][1] = m[3][2]; mr[2][2] = m[3][3];

	r[1][1] = Fun3(3, mr);

	mr[0][0] = m[0][0]; mr[0][1] = m[0][1]; mr[0][2] = m[0][3];
	mr[1][0] = m[2][0]; mr[1][1] = m[2][1]; mr[1][2] = m[2][3];
	mr[2][0] = m[3][0]; mr[2][1] = m[3][1]; mr[2][2] = m[3][3];

	r[2][1] = -Fun3(3, mr);

	mr[0][0] = m[0][0]; mr[0][1] = m[0][1]; mr[0][2] = m[0][2];
	mr[1][0] = m[2][0]; mr[1][1] = m[2][1]; mr[1][2] = m[2][2];
	mr[2][0] = m[3][0]; mr[2][1] = m[3][1]; mr[2][2] = m[3][2];

	r[3][1] = Fun3(3, mr);

	//
	mr[0][0] = m[0][1]; mr[0][1] = m[0][2]; mr[0][2] = m[0][3];
	mr[1][0] = m[1][1]; mr[1][1] = m[1][2]; mr[1][2] = m[1][3];
	mr[2][0] = m[3][1]; mr[2][1] = m[3][2]; mr[2][2] = m[3][3];

	r[0][2] = Fun3(3, mr);

	mr[0][0] = m[0][0]; mr[0][1] = m[0][2]; mr[0][2] = m[0][3];
	mr[1][0] = m[1][0]; mr[1][1] = m[1][2]; mr[1][2] = m[1][3];
	mr[2][0] = m[3][0]; mr[2][1] = m[3][2]; mr[2][2] = m[3][3];

	r[1][2] = -Fun3(3, mr);

	mr[0][0] = m[0][0]; mr[0][1] = m[0][1]; mr[0][2] = m[0][3];
	mr[1][0] = m[1][0]; mr[1][1] = m[1][1]; mr[1][2] = m[1][3];
	mr[2][0] = m[3][0]; mr[2][1] = m[3][1]; mr[2][2] = m[3][3];

	r[2][2] = Fun3(3, mr);

	mr[0][0] = m[0][0]; mr[0][1] = m[0][1]; mr[0][2] = m[0][2];
	mr[1][0] = m[1][0]; mr[1][1] = m[1][1]; mr[1][2] = m[1][2];
	mr[2][0] = m[3][0]; mr[2][1] = m[3][1]; mr[2][2] = m[3][2];

	r[3][2] = -Fun3(3, mr);

	//
	mr[0][0] = m[0][1]; mr[0][1] = m[0][2]; mr[0][2] = m[0][3];
	mr[1][0] = m[1][1]; mr[1][1] = m[1][2]; mr[1][2] = m[1][3];
	mr[2][0] = m[2][1]; mr[2][1] = m[2][2]; mr[2][2] = m[2][3];

	r[0][3] = -Fun3(3, mr);

	mr[0][0] = m[0][0]; mr[0][1] = m[0][2]; mr[0][2] = m[0][3];
	mr[1][0] = m[1][0]; mr[1][1] = m[1][2]; mr[1][2] = m[1][3];
	mr[2][0] = m[2][0]; mr[2][1] = m[2][2]; mr[2][2] = m[2][3];

	r[1][3] = Fun3(3, mr);

	mr[0][0] = m[0][0]; mr[0][1] = m[0][1]; mr[0][2] = m[0][3];
	mr[1][0] = m[1][0]; mr[1][1] = m[1][1]; mr[1][2] = m[1][3];
	mr[2][0] = m[2][0]; mr[2][1] = m[2][1]; mr[2][2] = m[2][3];

	r[2][3] = -Fun3(3, mr);

	mr[0][0] = m[0][0]; mr[0][1] = m[0][1]; mr[0][2] = m[0][2];
	mr[1][0] = m[1][0]; mr[1][1] = m[1][1]; mr[1][2] = m[1][2];
	mr[2][0] = m[2][0]; mr[2][1] = m[2][1]; mr[2][2] = m[2][2];

	r[3][3] = Fun3(3, mr);

	for (int ii = 0; ii < 4; ii++)
	{
		for (int jj = 0; jj < 4; jj++)
		{
			r[ii][jj] /= A;
		}
	}

}
/******************************************* 求伴随矩阵元素 ******************************************/
//求4*4矩阵的伴随矩阵元素
void Companion_Matrix(float m[][4],float mr[3][3],int k0,int m0)
{
	int mm = 0;
	int kk = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i != k0 && j != m0)
			{
				mr[kk][mm] = m[i][j];
				if (mm < 2)
				{
					mm++;
				}
				else
				{
					mm = 0;
					kk++;
				}
			}
		}
	}
}
/******************************* 求逆矩阵（模块化） *****************************/
//r 逆矩阵
//m 原矩阵
//A 原矩阵行列式
void Ivin4_Auto(float r[][4], float m[][4], float A)
{
	//
	printf("函数化: 求逆矩阵\n");
	float mr[3][3];
	// r[0][0]
	
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Companion_Matrix(m, mr, i, j);//求伴随矩阵元素
			//求逆
			r[j][i] = Fun3(3, mr)/A;//注意伴随矩阵的转置关系
			if ((i + j) % 2 != 0)//判断代数余子式的符号
			{
				r[j][i] = -r[j][i];
			}
		}
	}
	
	//r[0][0] = Fun3(3, mr);
	//r[1][0] = -Fun3(3, mr);
	//r[2][0] = Fun3(3, mr);
	//r[3][0] = -Fun3(3, mr);
	//r[0][1] = -Fun3(3, mr);
	//r[1][1] = Fun3(3, mr);
	//r[2][1] = -Fun3(3, mr);
	//r[3][1] = Fun3(3, mr);
	//r[0][2] = Fun3(3, mr);
	//r[1][2] = -Fun3(3, mr);
	//r[2][2] = Fun3(3, mr);
	//r[3][2] = -Fun3(3, mr);
	//r[0][3] = -Fun3(3, mr);
	//r[1][3] = Fun3(3, mr);
	//r[2][3] = -Fun3(3, mr);
	//r[3][3] = Fun3(3, mr);
	//for (int ii = 0; ii < 4; ii++)
	//{
	//	for (int jj = 0; jj < 4; jj++)
	//	{
	//		r[ii][jj] /= A;
	//	}
	//}
}
/*****************************4点 牛顿，再梯度，再拟牛顿******************************/
void GL_Build_NewTon_M4Point(int *ID_Pt, float*ID_Length, float tN_GlobalS_4N[][4], float* Energy_GlobalN, float *tStep_nn_GX, int numc,
	int &Point_Check, int &DiguCnt, int &DiguNum, bool Flag_Step, int Rude_Step_Control)
{
	Point_Check = 4;//模式选择：算法点数
	float t1 = tN_GlobalS_4N[numc][0];
	float t2 = tN_GlobalS_4N[numc][1];
	float t3 = tN_GlobalS_4N[numc][2];
	float t4 = tN_GlobalS_4N[numc][3];
	//-------------------
	float Ft1, Ft2, Ft3, Ft4;
	float t1b = 170, t2b = 170, t3b = 170, t4b = 170;;

	//射线向量
	float a1 = LineRays[ID_Pt[0]].x; float b1 = LineRays[ID_Pt[0]].y; float c1 = LineRays[ID_Pt[0]].z;
	float a2 = LineRays[ID_Pt[1]].x; float b2 = LineRays[ID_Pt[1]].y; float c2 = LineRays[ID_Pt[1]].z;
	float a3 = LineRays[ID_Pt[2]].x; float b3 = LineRays[ID_Pt[2]].y; float c3 = LineRays[ID_Pt[2]].z;
	float a4 = LineRays[ID_Pt[3]].x; float b4 = LineRays[ID_Pt[3]].y; float c4 = LineRays[ID_Pt[3]].z;
	// 
	GL_Vector pt0[4];
	pt0[0] = LineRays[ID_Pt[0]].pt0;
	pt0[1] = LineRays[ID_Pt[1]].pt0;
	pt0[2] = LineRays[ID_Pt[2]].pt0;
	pt0[3] = LineRays[ID_Pt[3]].pt0;
	//
	float x = a1*t1;
	float y = b1*t1;
	float z = c1*t1;
	float track_dst = GL_Distance(0, 0, 0, x, y, z);

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
	float E12 = ID_Length[0];//16;// 1->2
	float E22 = ID_Length[1];//16;// 2->3
	float E32 = ID_Length[2];//32;// 1->3
	float E42 = ID_Length[3];//32;// 1->4
	float E52 = ID_Length[4];//16;// 2->4
	float E62 = ID_Length[5];//64;// 3->4
	//------构建模型系数

#if 0
	float knn[10][3];
	GLB_Knn(knn, a1, b1, c1, a2, b2, c2, 0);
	GLB_Knn(knn, a2, b2, c2, a3, b3, c3, 1);
	GLB_Knn(knn, a1, b1, c1, a3, b3, c3, 2);

	GLB_Knn(knn, a1, b1, c1, a4, b4, c4, 3);
	GLB_Knn(knn, a2, b2, c2, a4, b4, c4, 4);
	GLB_Knn(knn, a3, b3, c3, a4, b4, c4, 5);
	nn = 0.00268531;
#else
	float knn[10][6];
	GLB_Knn2(knn, a1, b1, c1, pt0[0], a2, b2, c2, pt0[1], 0);
	GLB_Knn2(knn, a2, b2, c2, pt0[1], a3, b3, c3, pt0[2], 1);
	GLB_Knn2(knn, a1, b1, c1, pt0[0], a3, b3, c3, pt0[2], 2);

	GLB_Knn2(knn, a1, b1, c1, pt0[0], a4, b4, c4, pt0[3], 3);
	GLB_Knn2(knn, a2, b2, c2, pt0[1], a4, b4, c4, pt0[3], 4);
	GLB_Knn2(knn, a3, b3, c3, pt0[2], a4, b4, c4, pt0[3], 5);

#endif

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

	//-------------------雅克比矩阵 4*4 (前3行各为一个方程等式，后3行合起来为一个方程等式)
	// 1*4 
	// (knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2+knn[0][3]*t1+knn[0][4]*t2+knn[0][5]-E12)* (2*knn[0][0]*t1+knn[0][2]*t2+knn[0][3])
	// (knn[0][0]*t1*t1+knn[0][1]*t2*t2+knn[0][2]*t1*t2+knn[0][3]*t1+knn[0][4]*t2+knn[0][5]-E12)* (2*knn[0][1]*t2+knn[0][2]*t1+knn[0][4])
	// 0
	// 0

	// 2*4
	// 0
	// (knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3+knn[1][3]*t2+knn[1][4]*t3+knn[1][5]-E22)* (2*knn[1][0]*t2+knn[1][2]*t3+knn[1][3])
	// (knn[1][0]*t2*t2+knn[1][1]*t3*t3+knn[1][2]*t2*t3+knn[1][3]*t2+knn[1][4]*t3+knn[1][5]-E22)* (2*knn[1][1]*t3+knn[1][2]*t2+knn[1][4])
	// 0

	// 3*4
	// (knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3+knn[2][3]*t1+knn[2][4]*t3+knn[2][5]-E32)* (2*knn[2][0]*t1+knn[2][2]*t3+knn[2][3])
	// 0
	// (knn[2][0]*t1*t1+knn[2][1]*t3*t3+knn[2][2]*t1*t3+knn[2][3]*t1+knn[2][4]*t3+knn[2][5]-E32)* (2*knn[2][1]*t3+knn[2][2]*t1+knn[2][4])
	// 0

	// 4*4
	// (knn[3][0]*t1*t1+knn[3][1]*t4*t4+knn[3][2]*t1*t4+knn[3][3]*t1+knn[3][4]*t4+knn[3][5]-E42)* (2*knn[3][0]*t1+knn[3][2]*t4+knn[3][3])
	// (knn[4][0]*t2*t2+knn[4][1]*t4*t4+knn[4][2]*t2*t4+knn[4][3]*t2+knn[4][4]*t4+knn[4][5]-E52)* (2*knn[4][0]*t2+knn[4][2]*t4+knn[4][3])
	// (knn[5][0]*t3*t3+knn[5][1]*t4*t4+knn[5][2]*t3*t4+knn[5][3]*t3+knn[5][4]*t4+knn[5][5]-E62)* (2*knn[5][0]*t3+knn[5][2]*t4+knn[5][3])
	//   (knn[3][0]*t1*t1+knn[3][1]*t4*t4+knn[3][2]*t1*t4+knn[3][3]*t1+knn[3][4]*t4+knn[3][5]-E42)* (2*knn[3][1]*t4+knn[3][2]*t1+knn[3][4])
	//  +(knn[4][0]*t2*t2+knn[4][1]*t4*t4+knn[4][2]*t2*t4+knn[4][3]*t2+knn[4][4]*t4+knn[4][5]-E52)* (2*knn[4][1]*t4+knn[4][2]*t2+knn[4][4])
	//  +(knn[5][0]*t3*t3+knn[5][1]*t4*t4+knn[5][2]*t3*t4+knn[5][3]*t3+knn[5][4]*t4+knn[5][5]-E62)* (2*knn[5][1]*t4+knn[5][2]*t3+knn[5][4])

	//---------------------------------------------------------------------------------------------------------------------
	//当多组合时，DiguNum 与 DiguCnt必须清零。

	bool Flag_DiGu = 0;//递归限制控制表示符号
	bool Flag_TiDu = 0;//梯度使用标志位

	//--------------------------------------------------------------------------- 牛顿迭代法
#if 1
	float m[4][4];
	Flag_DiGu = 0;
	DiguCnt = 0;//递归算法大循环
	DiguNum = 0;//递归算法小循环计数
	int Cnt_Newton = 0;//牛顿迭代法 使用计数器
	//为了防止NewTon不收敛所以,将上一帧的值tn保存在tn_Newton
	float t1_Newton = t1;
	float t2_Newton = t2;
	float t3_Newton = t3;
	float t4_Newton = t4;
	while (!Flag_DiGu)
	{
		/*if(DiguCnt>15)nn=0.000023;*/
		if (DiguCnt>0)break;
		DiguCnt++;
		DiguNum = 0;

		int i = 0;

		for (i = 0; i<618; i++)
		{
				//-------------------雅克比矩阵 4*4 (前3行各为一个方程等式，后3行合起来为一个方程等式)
				// 1*4 
				m[0][0] = (knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12)* (2 * knn[0][0] * t1 + knn[0][2] * t2 + knn[0][3]);
				m[0][1] = (knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12)* (2 * knn[0][1] * t2 + knn[0][2] * t1 + knn[0][4]);
				m[0][2] = 0;
				m[0][3] = 0;

				// 2*4
				m[1][0] = 0;
				m[1][1] = (knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22)* (2 * knn[1][0] * t2 + knn[1][2] * t3 + knn[1][3]);
				m[1][2] = (knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22)* (2 * knn[1][1] * t3 + knn[1][2] * t2 + knn[1][4]);
				m[1][3] = 0;

				// 3*4
				m[2][0] = (knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32)* (2 * knn[2][0] * t1 + knn[2][2] * t3 + knn[2][3]);
				m[2][1] = 0;
				m[2][2] = (knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32)* (2 * knn[2][1] * t3 + knn[2][2] * t1 + knn[2][4]);
				m[2][3] = 0;

				// 4*4
				m[3][0] = (knn[3][0] * t1*t1 + knn[3][1] * t4*t4 + knn[3][2] * t1*t4 + knn[3][3] * t1 + knn[3][4] * t4 + knn[3][5] - E42)* (2 * knn[3][0] * t1 + knn[3][2] * t4 + knn[3][3]);
				m[3][1] = (knn[4][0] * t2*t2 + knn[4][1] * t4*t4 + knn[4][2] * t2*t4 + knn[4][3] * t2 + knn[4][4] * t4 + knn[4][5] - E52)* (2 * knn[4][0] * t2 + knn[4][2] * t4 + knn[4][3]);
				m[3][2] = (knn[5][0] * t3*t3 + knn[5][1] * t4*t4 + knn[5][2] * t3*t4 + knn[5][3] * t3 + knn[5][4] * t4 + knn[5][5] - E62)* (2 * knn[5][0] * t3 + knn[5][2] * t4 + knn[5][3]);
				m[3][3] = (knn[3][0] * t1*t1 + knn[3][1] * t4*t4 + knn[3][2] * t1*t4 + knn[3][3] * t1 + knn[3][4] * t4 + knn[3][5] - E42)* (2 * knn[3][1] * t4 + knn[3][2] * t1 + knn[3][4])
					+ (knn[4][0] * t2*t2 + knn[4][1] * t4*t4 + knn[4][2] * t2*t4 + knn[4][3] * t2 + knn[4][4] * t4 + knn[4][5] - E52)* (2 * knn[4][1] * t4 + knn[4][2] * t2 + knn[4][4])
					+ (knn[5][0] * t3*t3 + knn[5][1] * t4*t4 + knn[5][2] * t3*t4 + knn[5][3] * t3 + knn[5][4] * t4 + knn[5][5] - E62)* (2 * knn[5][1] * t4 + knn[5][2] * t3 + knn[5][4])
					;

				float A=Fun4(4, m);

				float r[4][4];

				if (A != 0)
				{
					//Ivin(r, m, A);// 求矩阵m的逆矩阵r。A为矩阵m行列式的值
					Ivin4_Auto(r, m, A);// 函数化求伴随矩阵，求矩阵m的逆矩阵r。A为矩阵m行列式的值
					Cnt_Newton++;
				}
				else
				{
					printf("\t\t ---------------------------->>> 雅可比 奇异！！！\n");
					Flag_DiGu = 1;
					break;
				}
				//----------------------------------------------------

			float bb1 = (knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12)*(knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12);
			float bb2 = (knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22)*(knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22);
			float bb3 = (knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32)*(knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32);

			float bb4 = (knn[3][0] * t1*t1 + knn[3][1] * t4*t4 + knn[3][2] * t1*t4 + knn[3][3] * t1 + knn[3][4] * t4 + knn[3][5] - E42)*(knn[3][0] * t1*t1 + knn[3][1] * t4*t4 + knn[3][2] * t1*t4 + knn[3][3] * t1 + knn[3][4] * t4 + knn[3][5] - E42)
				+ (knn[4][0] * t2*t2 + knn[4][1] * t4*t4 + knn[4][2] * t2*t4 + knn[4][3] * t2 + knn[4][4] * t4 + knn[4][5] - E52)*(knn[4][0] * t2*t2 + knn[4][1] * t4*t4 + knn[4][2] * t2*t4 + knn[4][3] * t2 + knn[4][4] * t4 + knn[4][5] - E52)
				+ (knn[5][0] * t3*t3 + knn[5][1] * t4*t4 + knn[5][2] * t3*t4 + knn[5][3] * t3 + knn[5][4] * t4 + knn[5][5] - E62)*(knn[5][0] * t3*t3 + knn[5][1] * t4*t4 + knn[5][2] * t3*t4 + knn[5][3] * t3 + knn[5][4] * t4 + knn[5][5] - E62)
				;

			float E1 = -(bb1*r[0][0] + bb2*r[0][1] + bb3*r[0][2] + bb4*r[0][3]);
			float E2 = -(bb1*r[1][0] + bb2*r[1][1] + bb3*r[1][2] + bb4*r[1][3]);
			float E3 = -(bb1*r[2][0] + bb2*r[2][1] + bb3*r[2][2] + bb4*r[2][3]);
			float E4 = -(bb1*r[3][0] + bb2*r[3][1] + bb3*r[3][2] + bb4*r[3][3]);

			float t1br = t1 + E1;
			float t2br = t2 + E2;
			float t3br = t3 + E3;
			float t4br = t4 + E4;

			//if ((t1br*a1) > 0 && (t1br*c1) > 0 )
			//if ((t1br*a1) > 0  && ABS(t1br*b1) > 0  && (t1br*c1) > 0 )
			{
				t1 = t1br;
				t2 = t2br;
				t3 = t3br;
				t4 = t4br;
			}
		/*	else
			{
				printf("\t\t ---------------------------->>> 最优解溢出！！！\n");
				Flag_DiGu = 1;
				break;
			}*/

			if (E1 == 0 && E2 == 0 && E3 == 0 && E4==0)// && (ABS(t1-t1b)<0.005 && ABS(t2-t2b)<0.005 && ABS(t3-t3b)<0.005 && ABS(t4-t4b)<0.005)) 
			{
				printf("\t\t ---------------------------->>> 已寻到最优解！！！\n");
				Flag_DiGu = 1;
				break;
			}
		}
		DiguNum = i;
	}

	if (Cnt_Newton > 0)
	{
		printf("\t\t ---------------------------->>> 已使用牛顿迭代法！！！(%d)\n", DiguNum);
	}

	//判断牛顿迭代是否收敛
	if ((t1*a1) > 0 &&(t1*a1)<200 && (t1*c1) > 0&&(t1*c1)<200)
	{
		printf("------->>> 牛顿迭代收敛  ！Yes \n");
	}
	else
	{
		printf("------->>> 牛顿迭代不收敛！NO  \n");
		t1 = t1_Newton;
		t2 = t2_Newton;
		t3 = t3_Newton;
		t4 = t4_Newton;
	}

	
#endif
	//------------------------------------------------------------------------------------------------------------  1 次梯度下降法
	//在小区域内扰动
#if 1
	/*t1 += float(rand() % 101 - 50) / 50.0f *2.3f;
	t2 += float(rand() % 101 - 50) / 50.0f *2.3f;
	t3 += float(rand() % 101 - 50) / 50.0f *2.3f;
	t4 += float(rand() % 101 - 50) / 50.0f *2.3f;*/
	Flag_TiDu = 0;//梯度使用标志位
	float nn = 0.002; // 递归步长 适中（不能太大也不能太小）。
	Flag_DiGu = 0;
	DiguNum = 0;

	//为了防止梯度下降法不收敛所以,将上一帧的值tn保存在tn_TiDu
	float t1_TiDu = t1;
	float t2_TiDu = t2;
	float t3_TiDu = t3;
	float t4_TiDu = t4;
	while (!Flag_DiGu)
	{
		int i = 0;
		for (i = 0; i<22999; i++)
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
			
			//-----------------------------------------------------------------------------------------------------------
			t1b = t1 - nn*Ft1;
			t2b = t2 - nn*Ft2;
			t3b = t3 - nn*Ft3;
			t4b = t4 - nn*Ft4;

			if (Ft1 == 0 && Ft2 == 0 && Ft3 == 0 && Ft4 == 0)//if (ABS(Ft1)<0.005 && ABS(Ft2)<0.005 && ABS(Ft3)<0.005 && ABS(Ft4)<0.005)//// && (ABS(t1-t1b)<0.005 && ABS(t2-t2b)<0.005 && ABS(t3-t3b)<0.005 && ABS(t4-t4b)<0.005)) 
			{
				printf("\t\t----------------------- >>> 梯度下降法，   寻到最优值 @@@  \n");
				Flag_DiGu = 1;
				break;
			}
			//--------------------

			//if ((t1b*a1) >= 0 && (t1b*a1) < 200 && (t1b*c1) >= 0 && (t1b*c1) < 200)
			{
				t1 = t1b;
				t2 = t2b;
				t3 = t3b;
				t4 = t4b;
				Flag_TiDu = 1;
			}
			/*else
			{
				printf("输出（%f, %f ,%f）\n", t1b*a1, t1b*b1, t1b*c1);
				Flag_DiGu = 1;
				break;
			}*/
			
		}
		Flag_DiGu = 1;
		DiguNum = i;
	}

	//------------------------------------------
	if (Flag_TiDu == 1)
	{
		printf("\t\t----------------------- >>> 已采用梯度下降算法(%d)  \n", DiguNum);
	}

	//判断牛顿迭代是否收敛
	if ((t1*a1) > 0 && (t1*a1) < 200 && (t1*c1) > 0 && (t1*c1) < 200)
	{
		printf("------->>> 梯度下降法收敛  ****** Yes \n");
	}
	else
	{
		printf("------->>> 梯度下降法不收敛****** NO  \n");
		t1 = t1_TiDu;
		t2 = t2_TiDu;
		t3 = t3_TiDu;
		t4 = t4_TiDu;
	}
#endif

	//------------------------------------------------------------------------拟牛顿 BFGS
#if 1
	/*t1 += float(rand() % 101 - 50) / 50.0f *0.3f;
	t2 += float(rand() % 101 - 50) / 50.0f *0.3f;
	t3 += float(rand() % 101 - 50) / 50.0f *0.3f;
	t4 += float(rand() % 101 - 50) / 50.0f *0.3f;*/

	DiguNum = 0;
	DiguCnt = 0;
	Flag_DiGu = 0;//递归运算运行标识符
	//H0=单位矩阵，H0矩阵初始化
	float H11 = 1, H12 = 0, H13 = 0, H14 = 0;
	float H21 = 0, H22 = 1, H23 = 0, H24 = 0;
	float H31 = 0, H32 = 0, H33 = 1, H34 = 0;
	float H41 = 0, H42 = 0, H43 = 0, H44 = 1;

	bool Flag_QuasiNewTon = 0;
	while (!Flag_DiGu)
	{
		if (DiguCnt >= 1)break;//递归次数大循环 限制
		DiguNum = 0;
		DiguCnt++;
		int i = 0;

		for (i = 0; i < 100; i++)
		{
			// :求偏导数
			Ft1 = (knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12) * (2 * knn[0][0] * t1 + knn[0][2] * t2 + knn[0][3])
				+ (knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32) * (2 * knn[2][0] * t1 + knn[2][2] * t3 + knn[2][3])
				+ (knn[3][0] * t1*t1 + knn[3][1] * t4*t4 + knn[3][2] * t1*t4 + knn[3][3] * t1 + knn[3][4] * t4 + knn[3][5] - E42) * (2 * knn[3][0] * t1 + knn[3][2] * t4 + knn[3][3])
				;

			Ft2 = (knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12) * (2 * knn[0][1] * t2 + knn[0][2] * t1 + knn[0][4])
				+ (knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22) * (2 * knn[1][0] * t2 + knn[1][2] * t3 + knn[1][3])
				+ (knn[4][0] * t2*t2 + knn[4][1] * t4*t4 + knn[4][2] * t2*t4 + knn[4][3] * t2 + knn[4][4] * t4 + knn[4][5] - E52) * (2 * knn[4][0] * t2 + knn[4][2] * t4 + knn[4][3])
				;

			Ft3 = (knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22) * (2 * knn[1][1] * t3 + knn[1][2] * t2 + knn[1][4])
				+ (knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32) * (2 * knn[2][1] * t3 + knn[2][2] * t1 + knn[2][4])
				+ (knn[5][0] * t3*t3 + knn[5][1] * t4*t4 + knn[5][2] * t3*t4 + knn[5][3] * t3 + knn[5][4] * t4 + knn[5][5] - E62) * (2 * knn[5][0] * t3 + knn[5][2] * t4 + knn[5][3])
				;

			Ft4 = (knn[3][0] * t1*t1 + knn[3][1] * t4*t4 + knn[3][2] * t1*t4 + knn[3][3] * t1 + knn[3][4] * t4 + knn[3][5] - E42) * (2 * knn[3][1] * t4 + knn[3][2] * t1 + knn[3][4])
				+ (knn[4][0] * t2*t2 + knn[4][1] * t4*t4 + knn[4][2] * t2*t4 + knn[4][3] * t2 + knn[4][4] * t4 + knn[4][5] - E52) * (2 * knn[4][1] * t4 + knn[4][2] * t2 + knn[4][4])
				+ (knn[5][0] * t3*t3 + knn[5][1] * t4*t4 + knn[5][2] * t3*t4 + knn[5][3] * t3 + knn[5][4] * t4 + knn[5][5] - E62) * (2 * knn[5][1] * t4 + knn[5][2] * t3 + knn[5][4])
				;

			float Pt1 = -(H11*Ft1 + H12*Ft2 + H13*Ft3 + H14*Ft4);
			float Pt2 = -(H21*Ft1 + H22*Ft2 + H23*Ft3 + H24*Ft4);
			float Pt3 = -(H31*Ft1 + H32*Ft2 + H33*Ft3 + H34*Ft4);
			float Pt4 = -(H41*Ft1 + H42*Ft2 + H43*Ft3 + H44*Ft4);

			//所求未知数变化步长
			float step = 100.0f;
			float stepr = 100.10f;

			float EnergyNewton = (knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12)*(knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12)
				+ (knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22)*(knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22)
				+ (knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32)*(knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32)

				+ (knn[3][0] * t1*t1 + knn[3][1] * t4*t4 + knn[3][2] * t1*t4 + knn[3][3] * t1 + knn[3][4] * t4 + knn[3][5] - E42)*(knn[3][0] * t1*t1 + knn[3][1] * t4*t4 + knn[3][2] * t1*t4 + knn[3][3] * t1 + knn[3][4] * t4 + knn[3][5] - E42)
				+ (knn[4][0] * t2*t2 + knn[4][1] * t4*t4 + knn[4][2] * t2*t4 + knn[4][3] * t2 + knn[4][4] * t4 + knn[4][5] - E52)*(knn[4][0] * t2*t2 + knn[4][1] * t4*t4 + knn[4][2] * t2*t4 + knn[4][3] * t2 + knn[4][4] * t4 + knn[4][5] - E52)
				+ (knn[5][0] * t3*t3 + knn[5][1] * t4*t4 + knn[5][2] * t3*t4 + knn[5][3] * t3 + knn[5][4] * t4 + knn[5][5] - E62)*(knn[5][0] * t3*t3 + knn[5][1] * t4*t4 + knn[5][2] * t3*t4 + knn[5][3] * t3 + knn[5][4] * t4 + knn[5][5] - E62)
				;
			bool Flag_Step = 0;
			for (int ii = 0; ii < 1000; ii++)//线性搜索步长
			{
				float t1r = t1 + stepr*Pt1;
				float t2r = t2 + stepr*Pt2;
				float t3r = t3 + stepr*Pt3;
				float t4r = t4 + stepr*Pt4;

				float EnergyNewtonR = (knn[0][0] * t1r*t1r + knn[0][1] * t2r*t2r + knn[0][2] * t1r*t2r + knn[0][3] * t1r + knn[0][4] * t2r + knn[0][5] - E12)*(knn[0][0] * t1r*t1r + knn[0][1] * t2r*t2r + knn[0][2] * t1r*t2r + knn[0][3] * t1r + knn[0][4] * t2r + knn[0][5] - E12)
					+ (knn[1][0] * t2r*t2r + knn[1][1] * t3r*t3r + knn[1][2] * t2r*t3r + knn[1][3] * t2r + knn[1][4] * t3r + knn[1][5] - E22)*(knn[1][0] * t2r*t2r + knn[1][1] * t3r*t3r + knn[1][2] * t2r*t3r + knn[1][3] * t2r + knn[1][4] * t3r + knn[1][5] - E22)
					+ (knn[2][0] * t1r*t1r + knn[2][1] * t3r*t3r + knn[2][2] * t1r*t3r + knn[2][3] * t1r + knn[2][4] * t3r + knn[2][5] - E32)*(knn[2][0] * t1r*t1r + knn[2][1] * t3r*t3r + knn[2][2] * t1r*t3r + knn[2][3] * t1r + knn[2][4] * t3r + knn[2][5] - E32)

					+ (knn[3][0] * t1r*t1r + knn[3][1] * t4r*t4r + knn[3][2] * t1r*t4r + knn[3][3] * t1r + knn[3][4] * t4r + knn[3][5] - E42)*(knn[3][0] * t1r*t1r + knn[3][1] * t4r*t4r + knn[3][2] * t1r*t4r + knn[3][3] * t1r + knn[3][4] * t4r + knn[3][5] - E42)
					+ (knn[4][0] * t2r*t2r + knn[4][1] * t4r*t4r + knn[4][2] * t2r*t4r + knn[4][3] * t2r + knn[4][4] * t4r + knn[4][5] - E52)*(knn[4][0] * t2r*t2r + knn[4][1] * t4r*t4r + knn[4][2] * t2r*t4r + knn[4][3] * t2r + knn[4][4] * t4r + knn[4][5] - E52)
					+ (knn[5][0] * t3r*t3r + knn[5][1] * t4r*t4r + knn[5][2] * t3r*t4r + knn[5][3] * t3r + knn[5][4] * t4r + knn[5][5] - E62)*(knn[5][0] * t3r*t3r + knn[5][1] * t4r*t4r + knn[5][2] * t3r*t4r + knn[5][3] * t3r + knn[5][4] * t4r + knn[5][5] - E62)
					;
				if (EnergyNewtonR < EnergyNewton)
				{
					step = stepr;
					Flag_Step = 1;
					//break;
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
				printf("\t\t----------------------- >>> BFGS 线性搜索无合适步长退出！  \n");
				break;
			}
			//----------------------------

			// 更新未知数
			t1b = t1 + step*Pt1;
			t2b = t2 + step*Pt2;
			t3b = t3 + step*Pt3;
			t4b = t4 + step*Pt4;
			// 求新未知数 新偏导数
			float Ft1b = (knn[0][0] * t1b*t1b + knn[0][1] * t2b*t2b + knn[0][2] * t1b*t2b + knn[0][3] * t1b + knn[0][4] * t2b + knn[0][5] - E12) * (2 * knn[0][0] * t1b + knn[0][2] * t2b + knn[0][3])
				+ (knn[2][0] * t1b*t1b + knn[2][1] * t3b*t3b + knn[2][2] * t1b*t3b + knn[2][3] * t1b + knn[2][4] * t3b + knn[2][5] - E32) * (2 * knn[2][0] * t1b + knn[2][2] * t3b + knn[2][3])
				+ (knn[3][0] * t1b*t1b + knn[3][1] * t4b*t4b + knn[3][2] * t1b*t4b + knn[3][3] * t1b + knn[3][4] * t4b + knn[3][5] - E42) * (2 * knn[3][0] * t1b + knn[3][2] * t4b + knn[3][3])
				;

			float Ft2b = (knn[0][0] * t1b*t1b + knn[0][1] * t2b*t2b + knn[0][2] * t1b*t2b + knn[0][3] * t1b + knn[0][4] * t2b + knn[0][5] - E12) * (2 * knn[0][1] * t2b + knn[0][2] * t1b + knn[0][4])
				+ (knn[1][0] * t2b*t2b + knn[1][1] * t3b*t3b + knn[1][2] * t2b*t3b + knn[1][3] * t2b + knn[1][4] * t3b + knn[1][5] - E22) * (2 * knn[1][0] * t2b + knn[1][2] * t3b + knn[1][3])
				+ (knn[4][0] * t2b*t2b + knn[4][1] * t4b*t4b + knn[4][2] * t2b*t4b + knn[4][3] * t2b + knn[4][4] * t4b + knn[4][5] - E52) * (2 * knn[4][0] * t2b + knn[4][2] * t4b + knn[4][3])
				;

			float Ft3b = (knn[1][0] * t2b*t2b + knn[1][1] * t3b*t3b + knn[1][2] * t2b*t3b + knn[1][3] * t2b + knn[1][4] * t3b + knn[1][5] - E22) * (2 * knn[1][1] * t3b + knn[1][2] * t2b + knn[1][4])
				+ (knn[2][0] * t1b*t1b + knn[2][1] * t3b*t3b + knn[2][2] * t1b*t3b + knn[2][3] * t1b + knn[2][4] * t3b + knn[2][5] - E32) * (2 * knn[2][1] * t3b + knn[2][2] * t1b + knn[2][4])
				+ (knn[5][0] * t3b*t3b + knn[5][1] * t4b*t4b + knn[5][2] * t3b*t4b + knn[5][3] * t3b + knn[5][4] * t4b + knn[5][5] - E62) * (2 * knn[5][0] * t3b + knn[5][2] * t4b + knn[5][3])
				;

			float Ft4b = (knn[3][0] * t1b*t1b + knn[3][1] * t4b*t4b + knn[3][2] * t1b*t4b + knn[3][3] * t1b + knn[3][4] * t4b + knn[3][5] - E42) * (2 * knn[3][1] * t4b + knn[3][2] * t1b + knn[3][4])
				+ (knn[4][0] * t2b*t2b + knn[4][1] * t4b*t4b + knn[4][2] * t2b*t4b + knn[4][3] * t2b + knn[4][4] * t4b + knn[4][5] - E52) * (2 * knn[4][1] * t4b + knn[4][2] * t2b + knn[4][4])
				+ (knn[5][0] * t3b*t3b + knn[5][1] * t4b*t4b + knn[5][2] * t3b*t4b + knn[5][3] * t3b + knn[5][4] * t4b + knn[5][5] - E62) * (2 * knn[5][1] * t4b + knn[5][2] * t3b + knn[5][4])
				;

			float sx1 = t1b - t1; //未知数差值，当未知数差值都为0找到极小值
			float sx2 = t2b - t2;
			float sx3 = t3b - t3;
			float sx4 = t4b - t4;

			float yx1 = Ft1b - Ft1;//梯度差值，当梯度差值都为0找到极小值
			float yx2 = Ft2b - Ft2;
			float yx3 = Ft3b - Ft3;
			float yx4 = Ft4b - Ft4;

			//计算 H0矩阵相关参数
			float C = (sx1*H11 + sx2*H21 + sx3*H31 + sx4*H41)*sx1
				    + (sx1*H12 + sx2*H22 + sx3*H32 + sx4*H42)*sx2
				    + (sx1*H13 + sx2*H23 + sx3*H33 + sx4*H43)*sx3
				    + (sx1*H14 + sx2*H24 + sx3*H34 + sx4*H44)*sx4
				;

			float E = yx1*sx1 + yx2*sx2 + yx3*sx3 + yx4*sx4;

			float D11 = yx1*yx1, D12 = yx1*yx2, D13 = yx1*yx3, D14 = yx1*yx4;
			float D21 = yx1*yx2, D22 = yx2*yx2, D23 = yx2*yx3, D24 = yx2*yx4;
			float D31 = yx1*yx3, D32 = yx2*yx3, D33 = yx3*yx3, D34 = yx3*yx4;
			float D41 = yx1*yx4, D42 = yx2*yx4, D43 = yx3*yx4, D44 = yx4*yx4;

			//计算B矩阵
			//矩阵中间缓存
			float BB11 = (H11*sx1 + H12*sx2 + H13*sx3 + H14*sx4)*sx1; float BB12 = (H11*sx1 + H12*sx2 + H13*sx3 + H14*sx4)*sx2; float BB13 = (H11*sx1 + H12*sx2 + H13*sx3 + H14*sx4)*sx3; float BB14 = (H11*sx1 + H12*sx2 + H13*sx3 + H14*sx4)*sx4;
			float BB21 = (H21*sx1 + H22*sx2 + H23*sx3 + H24*sx4)*sx1; float BB22 = (H21*sx1 + H22*sx2 + H23*sx3 + H24*sx4)*sx2; float BB23 = (H21*sx1 + H22*sx2 + H23*sx3 + H24*sx4)*sx3; float BB24 = (H21*sx1 + H22*sx2 + H23*sx3 + H24*sx4)*sx4;
			float BB31 = (H31*sx1 + H32*sx2 + H33*sx3 + H34*sx4)*sx1; float BB32 = (H31*sx1 + H32*sx2 + H33*sx3 + H34*sx4)*sx2; float BB33 = (H31*sx1 + H32*sx2 + H33*sx3 + H34*sx4)*sx3; float BB34 = (H31*sx1 + H32*sx2 + H33*sx3 + H34*sx4)*sx4;
			float BB41 = (H41*sx1 + H42*sx2 + H43*sx3 + H44*sx4)*sx1; float BB42 = (H41*sx1 + H42*sx2 + H43*sx3 + H44*sx4)*sx2; float BB43 = (H41*sx1 + H42*sx2 + H43*sx3 + H44*sx4)*sx3; float BB44 = (H41*sx1 + H42*sx2 + H43*sx3 + H44*sx4)*sx4;
			//
			float B11 = BB11*H11 + BB12*H21 + BB13*H31 + BB14*H41;
			float B12 = BB11*H12 + BB12*H22 + BB13*H32 + BB14*H42;
			float B13 = BB11*H13 + BB12*H23 + BB13*H33 + BB14*H43;
			float B14 = BB11*H14 + BB12*H24 + BB13*H34 + BB14*H44;

			float B21 = BB21*H11 + BB22*H21 + BB23*H31 + BB24*H41;
			float B22 = BB21*H12 + BB22*H22 + BB23*H32 + BB24*H42;
			float B23 = BB21*H13 + BB22*H23 + BB23*H33 + BB24*H43;
			float B24 = BB21*H14 + BB22*H24 + BB23*H34 + BB24*H44;

			float B31 = BB31*H11 + BB32*H21 + BB33*H31 + BB34*H41;
			float B32 = BB31*H12 + BB32*H22 + BB33*H32 + BB34*H42;
			float B33 = BB31*H13 + BB32*H23 + BB33*H33 + BB34*H43;
			float B34 = BB31*H14 + BB32*H24 + BB33*H34 + BB34*H44;

			float B41 = BB41*H11 + BB42*H21 + BB43*H31 + BB44*H41;
			float B42 = BB41*H12 + BB42*H22 + BB43*H32 + BB44*H42;
			float B43 = BB41*H13 + BB42*H23 + BB43*H33 + BB44*H43;
			float B44 = BB41*H14 + BB42*H24 + BB43*H34 + BB44*H44;

			//
			float H11b, H12b, H13b, H14b;
			float H21b, H22b, H23b, H24b;
			float H31b, H32b, H33b, H34b;
			float H41b, H42b, H43b, H44b;

			if (C != 0)        //Hesse阵更新
			{
				if (E > 0)
				{
					H11b = H11 - B11 / C + D11 / E;
					H12b = H12 - B12 / C + D12 / E;
					H13b = H13 - B13 / C + D13 / E;
					H14b = H14 - B14 / C + D14 / E;

					H21b = H21 - B21 / C + D21 / E;
					H22b = H22 - B22 / C + D22 / E;
					H23b = H23 - B23 / C + D23 / E;
					H24b = H24 - B24 / C + D24 / E;

					H31b = H31 - B31 / C + D31 / E;
					H32b = H32 - B32 / C + D32 / E;
					H33b = H33 - B33 / C + D33 / E;
					H34b = H34 - B34 / C + D34 / E;

					H41b = H41 - B41 / C + D41 / E;
					H42b = H42 - B42 / C + D42 / E;
					H43b = H43 - B43 / C + D43 / E;
					H44b = H44 - B44 / C + D44 / E;
				}
				else
				{
					H11b = H11;
					H12b = H12;
					H13b = H13;
					H14b = H14;

					H21b = H21;
					H22b = H22;
					H23b = H23;
					H24b = H24;

					H31b = H31;
					H32b = H32;
					H33b = H33;
					H34b = H34;

					H41b = H41;
					H42b = H42;
					H43b = H43;
					H44b = H44;
				}
			}
			else
			{
				H11b = H11;
				H12b = H12;
				H13b = H13;
				H14b = H14;

				H21b = H21;
				H22b = H22;
				H23b = H23;
				H24b = H24;

				H31b = H31;
				H32b = H32;
				H33b = H33;
				H34b = H34;

				H41b = H41;
				H42b = H42;
				H43b = H43;
				H44b = H44;
			
				//Flag_DiGu = 1;
				//break;
			}
			//找到最优结果退出
			if (C == 0 && E == 0)
			{
				Flag_DiGu = 1;
				printf("\t\t --------------------->>> Quasi 变化量为0，获得最优解！！！\n");
				break;
			}
			//迭代
			if ((t1b*a1) > 0 && (t1b*a1) < 300 && ABS(t1b*b1) > 0 && ABS(t1b*b1) < 300 && (t1b*c1) > 0 && (t1b*c1) < 300)
			{
				t1 = t1b;
				t2 = t2b;
				t3 = t3b;
				t4 = t4b;

				H11 = H11b;
				H12 = H12b;
				H13 = H13b;
				H14 = H14b;

				H21 = H21b;
				H22 = H22b;
				H23 = H23b;
				H24 = H24b;

				H31 = H31b;
				H32 = H32b;
				H33 = H33b;
				H34 = H34b;

				H41 = H41b;
				H42 = H42b;
				H43 = H43b;
				H44 = H44b;

				Flag_QuasiNewTon = 1;
			}
			else
			{
				printf("\t\t ---------------------------->>> Quasi 最优解溢出！！！\n");
				Flag_DiGu = 1;
				break;
			}
		}

		DiguNum = i;
	}

	//------------------------------------------
	if (Flag_QuasiNewTon == 1)
	{
		printf("\t\t----------------------- >>> 已采用拟牛顿算法 BFGS (%d)\n", DiguNum);
	}

//
////------------------------------------------------------------------------拟牛顿 DFP
///*t1 += float(rand() % 101 - 50) / 50.0f *0.3f;
//t2 += float(rand() % 101 - 50) / 50.0f *0.3f;
//t3 += float(rand() % 101 - 50) / 50.0f *0.3f;
//t4 += float(rand() % 101 - 50) / 50.0f *0.3f;*/
//
//DiguNum = 0;
//DiguCnt = 0;
//Flag_DiGu = 0;//递归运算运行标识符
////H0=单位矩阵，H0矩阵初始化
//float H11 = 1, H12 = 0, H13 = 0, H14 = 0;
//float H21 = 0, H22 = 1, H23 = 0, H24 = 0;
//float H31 = 0, H32 = 0, H33 = 1, H34 = 0;
//float H41 = 0, H42 = 0, H43 = 0, H44 = 1;
//
//bool Flag_QuasiNewTon = 0;
//while (!Flag_DiGu)
//{
//	if (DiguCnt >= 1)break;//递归次数大循环 限制
//	DiguNum = 0;
//	DiguCnt++;
//	int i = 0;
//
//	for (i = 0; i < 200; i++)
//	{
//		// :求偏导数
//		Ft1 = (knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12) * (2 * knn[0][0] * t1 + knn[0][2] * t2 + knn[0][3])
//			+ (knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32) * (2 * knn[2][0] * t1 + knn[2][2] * t3 + knn[2][3])
//			+ (knn[3][0] * t1*t1 + knn[3][1] * t4*t4 + knn[3][2] * t1*t4 + knn[3][3] * t1 + knn[3][4] * t4 + knn[3][5] - E42) * (2 * knn[3][0] * t1 + knn[3][2] * t4 + knn[3][3])
//			;
//
//		Ft2 = (knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12) * (2 * knn[0][1] * t2 + knn[0][2] * t1 + knn[0][4])
//			+ (knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22) * (2 * knn[1][0] * t2 + knn[1][2] * t3 + knn[1][3])
//			+ (knn[4][0] * t2*t2 + knn[4][1] * t4*t4 + knn[4][2] * t2*t4 + knn[4][3] * t2 + knn[4][4] * t4 + knn[4][5] - E52) * (2 * knn[4][0] * t2 + knn[4][2] * t4 + knn[4][3])
//			;
//
//		Ft3 = (knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22) * (2 * knn[1][1] * t3 + knn[1][2] * t2 + knn[1][4])
//			+ (knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32) * (2 * knn[2][1] * t3 + knn[2][2] * t1 + knn[2][4])
//			+ (knn[5][0] * t3*t3 + knn[5][1] * t4*t4 + knn[5][2] * t3*t4 + knn[5][3] * t3 + knn[5][4] * t4 + knn[5][5] - E62) * (2 * knn[5][0] * t3 + knn[5][2] * t4 + knn[5][3])
//			;
//
//		Ft4 = (knn[3][0] * t1*t1 + knn[3][1] * t4*t4 + knn[3][2] * t1*t4 + knn[3][3] * t1 + knn[3][4] * t4 + knn[3][5] - E42) * (2 * knn[3][1] * t4 + knn[3][2] * t1 + knn[3][4])
//			+ (knn[4][0] * t2*t2 + knn[4][1] * t4*t4 + knn[4][2] * t2*t4 + knn[4][3] * t2 + knn[4][4] * t4 + knn[4][5] - E52) * (2 * knn[4][1] * t4 + knn[4][2] * t2 + knn[4][4])
//			+ (knn[5][0] * t3*t3 + knn[5][1] * t4*t4 + knn[5][2] * t3*t4 + knn[5][3] * t3 + knn[5][4] * t4 + knn[5][5] - E62) * (2 * knn[5][1] * t4 + knn[5][2] * t3 + knn[5][4])
//			;
//
//		float Pt1 = -(H11*Ft1 + H12*Ft2 + H13*Ft3 + H14*Ft4);
//		float Pt2 = -(H21*Ft1 + H22*Ft2 + H23*Ft3 + H24*Ft4);
//		float Pt3 = -(H31*Ft1 + H32*Ft2 + H33*Ft3 + H34*Ft4);
//		float Pt4 = -(H41*Ft1 + H42*Ft2 + H43*Ft3 + H44*Ft4);
//
//		//所求未知数变化步长
//		float step = 10.0f;
//		float stepr = 10.10f;
//
//		float EnergyNewton = (knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12)*(knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12)
//			+ (knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22)*(knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22)
//			+ (knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32)*(knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32)
//
//			+ (knn[3][0] * t1*t1 + knn[3][1] * t4*t4 + knn[3][2] * t1*t4 + knn[3][3] * t1 + knn[3][4] * t4 + knn[3][5] - E42)*(knn[3][0] * t1*t1 + knn[3][1] * t4*t4 + knn[3][2] * t1*t4 + knn[3][3] * t1 + knn[3][4] * t4 + knn[3][5] - E42)
//			+ (knn[4][0] * t2*t2 + knn[4][1] * t4*t4 + knn[4][2] * t2*t4 + knn[4][3] * t2 + knn[4][4] * t4 + knn[4][5] - E52)*(knn[4][0] * t2*t2 + knn[4][1] * t4*t4 + knn[4][2] * t2*t4 + knn[4][3] * t2 + knn[4][4] * t4 + knn[4][5] - E52)
//			+ (knn[5][0] * t3*t3 + knn[5][1] * t4*t4 + knn[5][2] * t3*t4 + knn[5][3] * t3 + knn[5][4] * t4 + knn[5][5] - E62)*(knn[5][0] * t3*t3 + knn[5][1] * t4*t4 + knn[5][2] * t3*t4 + knn[5][3] * t3 + knn[5][4] * t4 + knn[5][5] - E62)
//			;
//		bool Flag_Step = 0;
//		for (int ii = 0; ii < 1000; ii++)//线性搜索步长
//		{
//			float t1r = t1 + stepr*Pt1;
//			float t2r = t2 + stepr*Pt2;
//			float t3r = t3 + stepr*Pt3;
//			float t4r = t4 + stepr*Pt4;
//
//			float EnergyNewtonR = (knn[0][0] * t1r*t1r + knn[0][1] * t2r*t2r + knn[0][2] * t1r*t2r + knn[0][3] * t1r + knn[0][4] * t2r + knn[0][5] - E12)*(knn[0][0] * t1r*t1r + knn[0][1] * t2r*t2r + knn[0][2] * t1r*t2r + knn[0][3] * t1r + knn[0][4] * t2r + knn[0][5] - E12)
//				+ (knn[1][0] * t2r*t2r + knn[1][1] * t3r*t3r + knn[1][2] * t2r*t3r + knn[1][3] * t2r + knn[1][4] * t3r + knn[1][5] - E22)*(knn[1][0] * t2r*t2r + knn[1][1] * t3r*t3r + knn[1][2] * t2r*t3r + knn[1][3] * t2r + knn[1][4] * t3r + knn[1][5] - E22)
//				+ (knn[2][0] * t1r*t1r + knn[2][1] * t3r*t3r + knn[2][2] * t1r*t3r + knn[2][3] * t1r + knn[2][4] * t3r + knn[2][5] - E32)*(knn[2][0] * t1r*t1r + knn[2][1] * t3r*t3r + knn[2][2] * t1r*t3r + knn[2][3] * t1r + knn[2][4] * t3r + knn[2][5] - E32)
//
//				+ (knn[3][0] * t1r*t1r + knn[3][1] * t4r*t4r + knn[3][2] * t1r*t4r + knn[3][3] * t1r + knn[3][4] * t4r + knn[3][5] - E42)*(knn[3][0] * t1r*t1r + knn[3][1] * t4r*t4r + knn[3][2] * t1r*t4r + knn[3][3] * t1r + knn[3][4] * t4r + knn[3][5] - E42)
//				+ (knn[4][0] * t2r*t2r + knn[4][1] * t4r*t4r + knn[4][2] * t2r*t4r + knn[4][3] * t2r + knn[4][4] * t4r + knn[4][5] - E52)*(knn[4][0] * t2r*t2r + knn[4][1] * t4r*t4r + knn[4][2] * t2r*t4r + knn[4][3] * t2r + knn[4][4] * t4r + knn[4][5] - E52)
//				+ (knn[5][0] * t3r*t3r + knn[5][1] * t4r*t4r + knn[5][2] * t3r*t4r + knn[5][3] * t3r + knn[5][4] * t4r + knn[5][5] - E62)*(knn[5][0] * t3r*t3r + knn[5][1] * t4r*t4r + knn[5][2] * t3r*t4r + knn[5][3] * t3r + knn[5][4] * t4r + knn[5][5] - E62)
//				;
//			if (EnergyNewtonR < EnergyNewton)
//			{
//				step = stepr;
//				Flag_Step = 1;
//				break;
//			}
//			else
//			{
//				stepr *= 0.99f;
//				//stepr *= 0.618f;
//			}
//		}
//		//
//		if (!Flag_Step)
//		{
//			Flag_DiGu = 1;
//			printf("\t\t----------------------- >>> DFP 线性搜索无合适步长退出！  \n");
//			break;
//		}
//		//----------------------------
//
//		// 更新未知数
//		t1b = t1 + step*Pt1;
//		t2b = t2 + step*Pt2;
//		t3b = t3 + step*Pt3;
//		t4b = t4 + step*Pt4;
//		// 求新未知数 新偏导数
//		float Ft1b = (knn[0][0] * t1b*t1b + knn[0][1] * t2b*t2b + knn[0][2] * t1b*t2b + knn[0][3] * t1b + knn[0][4] * t2b + knn[0][5] - E12) * (2 * knn[0][0] * t1b + knn[0][2] * t2b + knn[0][3])
//			+ (knn[2][0] * t1b*t1b + knn[2][1] * t3b*t3b + knn[2][2] * t1b*t3b + knn[2][3] * t1b + knn[2][4] * t3b + knn[2][5] - E32) * (2 * knn[2][0] * t1b + knn[2][2] * t3b + knn[2][3])
//			+ (knn[3][0] * t1b*t1b + knn[3][1] * t4b*t4b + knn[3][2] * t1b*t4b + knn[3][3] * t1b + knn[3][4] * t4b + knn[3][5] - E42) * (2 * knn[3][0] * t1b + knn[3][2] * t4b + knn[3][3])
//			;
//
//		float Ft2b = (knn[0][0] * t1b*t1b + knn[0][1] * t2b*t2b + knn[0][2] * t1b*t2b + knn[0][3] * t1b + knn[0][4] * t2b + knn[0][5] - E12) * (2 * knn[0][1] * t2b + knn[0][2] * t1b + knn[0][4])
//			+ (knn[1][0] * t2b*t2b + knn[1][1] * t3b*t3b + knn[1][2] * t2b*t3b + knn[1][3] * t2b + knn[1][4] * t3b + knn[1][5] - E22) * (2 * knn[1][0] * t2b + knn[1][2] * t3b + knn[1][3])
//			+ (knn[4][0] * t2b*t2b + knn[4][1] * t4b*t4b + knn[4][2] * t2b*t4b + knn[4][3] * t2b + knn[4][4] * t4b + knn[4][5] - E52) * (2 * knn[4][0] * t2b + knn[4][2] * t4b + knn[4][3])
//			;
//
//		float Ft3b = (knn[1][0] * t2b*t2b + knn[1][1] * t3b*t3b + knn[1][2] * t2b*t3b + knn[1][3] * t2b + knn[1][4] * t3b + knn[1][5] - E22) * (2 * knn[1][1] * t3b + knn[1][2] * t2b + knn[1][4])
//			+ (knn[2][0] * t1b*t1b + knn[2][1] * t3b*t3b + knn[2][2] * t1b*t3b + knn[2][3] * t1b + knn[2][4] * t3b + knn[2][5] - E32) * (2 * knn[2][1] * t3b + knn[2][2] * t1b + knn[2][4])
//			+ (knn[5][0] * t3b*t3b + knn[5][1] * t4b*t4b + knn[5][2] * t3b*t4b + knn[5][3] * t3b + knn[5][4] * t4b + knn[5][5] - E62) * (2 * knn[5][0] * t3b + knn[5][2] * t4b + knn[5][3])
//			;
//
//		float Ft4b = (knn[3][0] * t1b*t1b + knn[3][1] * t4b*t4b + knn[3][2] * t1b*t4b + knn[3][3] * t1b + knn[3][4] * t4b + knn[3][5] - E42) * (2 * knn[3][1] * t4b + knn[3][2] * t1b + knn[3][4])
//			+ (knn[4][0] * t2b*t2b + knn[4][1] * t4b*t4b + knn[4][2] * t2b*t4b + knn[4][3] * t2b + knn[4][4] * t4b + knn[4][5] - E52) * (2 * knn[4][1] * t4b + knn[4][2] * t2b + knn[4][4])
//			+ (knn[5][0] * t3b*t3b + knn[5][1] * t4b*t4b + knn[5][2] * t3b*t4b + knn[5][3] * t3b + knn[5][4] * t4b + knn[5][5] - E62) * (2 * knn[5][1] * t4b + knn[5][2] * t3b + knn[5][4])
//			;
//
//		float sx1 = t1b - t1; //未知数差值，当未知数差值都为0找到极小值
//		float sx2 = t2b - t2;
//		float sx3 = t3b - t3;
//		float sx4 = t4b - t4;
//
//		float yx1 = Ft1b - Ft1;//梯度差值，当梯度差值都为0找到极小值
//		float yx2 = Ft2b - Ft2;
//		float yx3 = Ft3b - Ft3;
//		float yx4 = Ft4b - Ft4;
//
//		//计算 H0矩阵相关参数
//		float C = (yx1*H11 + yx2*H21 + yx3*H31 + yx4*H14)*yx1
//			+ (yx1*H12 + yx2*H22 + yx3*H32 + yx4*H42)*yx2
//			+ (yx1*H13 + yx2*H23 + yx3*H33 + yx4*H43)*yx3
//			+ (yx1*H14 + yx2*H24 + yx3*H34 + yx4*H44)*yx4
//			;
//
//		float E = yx1*sx1 + yx2*sx2 + yx3*sx3 + yx4*sx4;
//
//		float D11 = sx1*sx1, D12 = sx1*sx2, D13 = sx1*sx3, D14 = sx1*sx4;
//		float D21 = sx1*sx2, D22 = sx2*sx2, D23 = sx2*sx3, D24 = sx2*sx4;
//		float D31 = sx1*sx3, D32 = sx2*sx3, D33 = sx3*sx3, D34 = sx3*sx4;
//		float D41 = sx1*sx3, D42 = sx2*sx4, D43 = sx3*sx4, D44 = sx4*sx4;
//
//		//计算B矩阵
//		//矩阵中间缓存
//		float BB11 = (H11*yx1 + H12*yx2 + H13*yx3 + H14*yx4)*yx1; float BB12 = (H11*yx1 + H12*yx2 + H13*yx3 + H14*yx4)*yx2; float BB13 = (H11*yx1 + H12*yx2 + H13*yx3 + H14*yx4)*yx3; float BB14 = (H11*yx1 + H12*yx2 + H13*yx3 + H14*yx4)*yx4;
//		float BB21 = (H21*yx1 + H22*yx2 + H23*yx3 + H24*yx4)*yx1; float BB22 = (H21*yx1 + H22*yx2 + H23*yx3 + H24*yx4)*yx2; float BB23 = (H21*yx1 + H22*yx2 + H23*yx3 + H24*yx4)*yx3; float BB24 = (H21*yx1 + H22*yx2 + H23*yx3 + H24*yx4)*yx4;
//		float BB31 = (H31*yx1 + H32*yx2 + H33*yx3 + H34*yx4)*yx1; float BB32 = (H31*yx1 + H32*yx2 + H33*yx3 + H34*yx4)*yx2; float BB33 = (H31*yx1 + H32*yx2 + H33*yx3 + H34*yx4)*yx3; float BB34 = (H31*yx1 + H32*yx2 + H33*yx3 + H34*yx4)*yx4;
//		float BB41 = (H41*yx1 + H42*yx2 + H43*yx3 + H44*yx4)*yx1; float BB42 = (H41*yx1 + H42*yx2 + H43*yx3 + H44*yx4)*yx2; float BB43 = (H41*yx1 + H42*yx2 + H43*yx3 + H44*yx4)*yx3; float BB44 = (H41*yx1 + H42*yx2 + H43*yx3 + H44*yx4)*yx4;
//		//
//		float B11 = BB11*H11 + BB12*H21 + BB13*H31 + BB14*H41;
//		float B12 = BB11*H12 + BB12*H22 + BB13*H32 + BB14*H42;
//		float B13 = BB11*H13 + BB12*H23 + BB13*H33 + BB14*H43;
//		float B14 = BB11*H14 + BB12*H24 + BB13*H34 + BB14*H44;
//
//		float B21 = BB21*H11 + BB22*H21 + BB23*H31 + BB24*H41;
//		float B22 = BB21*H12 + BB22*H22 + BB23*H32 + BB24*H42;
//		float B23 = BB21*H13 + BB22*H23 + BB23*H33 + BB24*H43;
//		float B24 = BB21*H14 + BB22*H24 + BB23*H34 + BB24*H44;
//
//		float B31 = BB31*H11 + BB32*H21 + BB33*H31 + BB34*H41;
//		float B32 = BB31*H12 + BB32*H22 + BB33*H32 + BB34*H42;
//		float B33 = BB31*H13 + BB32*H23 + BB33*H33 + BB34*H43;
//		float B34 = BB31*H14 + BB32*H24 + BB33*H34 + BB34*H44;
//
//		float B41 = BB41*H11 + BB42*H21 + BB43*H31 + BB44*H41;
//		float B42 = BB41*H12 + BB42*H22 + BB43*H32 + BB44*H42;
//		float B43 = BB41*H13 + BB42*H23 + BB43*H33 + BB44*H43;
//		float B44 = BB41*H14 + BB42*H24 + BB43*H34 + BB44*H44;
//
//		//
//		float H11b, H12b, H13b, H14b;
//		float H21b, H22b, H23b, H24b;
//		float H31b, H32b, H33b, H34b;
//		float H41b, H42b, H43b, H44b;
//
//		if (C != 0)        //Hesse阵更新
//		{
//			if (E > 0)
//			{
//				H11b = H11 - B11 / C + D11 / E;
//				H12b = H12 - B12 / C + D12 / E;
//				H13b = H13 - B13 / C + D13 / E;
//				H14b = H14 - B14 / C + D14 / E;
//
//				H21b = H21 - B21 / C + D21 / E;
//				H22b = H22 - B22 / C + D22 / E;
//				H23b = H23 - B23 / C + D23 / E;
//				H24b = H24 - B24 / C + D24 / E;
//
//				H31b = H31 - B31 / C + D31 / E;
//				H32b = H32 - B32 / C + D32 / E;
//				H33b = H33 - B33 / C + D33 / E;
//				H34b = H34 - B34 / C + D34 / E;
//
//				H41b = H41 - B41 / C + D41 / E;
//				H42b = H42 - B42 / C + D42 / E;
//				H43b = H43 - B43 / C + D43 / E;
//				H44b = H44 - B44 / C + D44 / E;
//			}
//			else
//			{
//				H11b = H11;
//				H12b = H12;
//				H13b = H13;
//				H14b = H14;
//
//				H21b = H21;
//				H22b = H22;
//				H23b = H23;
//				H24b = H24;
//
//				H31b = H31;
//				H32b = H32;
//				H33b = H33;
//				H34b = H34;
//
//				H41b = H41;
//				H42b = H42;
//				H43b = H43;
//				H44b = H44;
//			}
//		}
//		else
//		{
//			H11b = H11;
//			H12b = H12;
//			H13b = H13;
//			H14b = H14;
//
//			H21b = H21;
//			H22b = H22;
//			H23b = H23;
//			H24b = H24;
//
//			H31b = H31;
//			H32b = H32;
//			H33b = H33;
//			H34b = H34;
//
//			H41b = H41;
//			H42b = H42;
//			H43b = H43;
//			H44b = H44;
//
//			//Flag_DiGu = 1;
//			//break;
//		}
//		//找到最优结果退出
//		if (C == 0 && E == 0)
//		{
//			Flag_DiGu = 1;
//			printf("\t\t --------------------->>> Quasi 变化量为0，获得最优解！！！\n");
//			break;
//		}
//		//迭代
//		if ((t1b*a1) > 0 && (t1b*a1) < 360 && ABS(t1b*b1) > 0 && ABS(t1b*b1) < 360 && (t1b*c1) > 0 && (t1b*c1) < 360)
//		{
//			t1 = t1b;
//			t2 = t2b;
//			t3 = t3b;
//			t4 = t4b;
//
//			H11 = H11b;
//			H12 = H12b;
//			H13 = H13b;
//			H14 = H14b;
//
//			H21 = H21b;
//			H22 = H22b;
//			H23 = H23b;
//			H24 = H24b;
//
//			H31 = H31b;
//			H32 = H32b;
//			H33 = H33b;
//			H34 = H34b;
//
//			H31 = H31b;
//			H32 = H32b;
//			H33 = H33b;
//			H34 = H34b;
//
//			H41 = H41b;
//			H42 = H42b;
//			H43 = H43b;
//			H44 = H44b;
//
//			Flag_QuasiNewTon = 1;
//		}
//		else
//		{
//			printf("\t\t ---------------------------->>> Quasi 最优解溢出！！！\n");
//			Flag_DiGu = 1;
//			break;
//		}
//	}
//
//	DiguNum = i;
//}
//
////------------------------------------------
//if (Flag_QuasiNewTon == 1)
//{
//	printf("\t\t----------------------- >>> 已采用拟牛顿算法 DFP (%d)\n", DiguNum);
//}
#endif

	//------------------------------------------------------------------------------------------------------------  2 次梯度下降法
	//在小区域内扰动
#if 0
	/*t1 += float(rand() % 101 - 50) / 50.0f *2.3f;
	t2 += float(rand() % 101 - 50) / 50.0f *2.3f;
	t3 += float(rand() % 101 - 50) / 50.0f *2.3f;
	t4 += float(rand() % 101 - 50) / 50.0f *2.3f;*/
	bool Flag_TiDu = 0;//梯度使用标志位
	nn = 0.0019;
	Flag_DiGu = 0;
	DiguNum = 0;
	while (!Flag_DiGu)
	{
		int i = 0;
		for (i = 0; i<21999; i++)
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

			//-----------------------------------------------------------------------------------------------------------
			t1b = t1 - nn*Ft1;
			t2b = t2 - nn*Ft2;
			t3b = t3 - nn*Ft3;
			t4b = t4 - nn*Ft4;

			if (Ft1 == 0 && Ft2 == 0 && Ft3 == 0 && Ft4 == 0)//if (ABS(Ft1)<0.005 && ABS(Ft2)<0.005 && ABS(Ft3)<0.005 && ABS(Ft4)<0.005)//// && (ABS(t1-t1b)<0.005 && ABS(t2-t2b)<0.005 && ABS(t3-t3b)<0.005 && ABS(t4-t4b)<0.005)) 
			{
				printf("\t\t----------------------- >>> 梯度下降法，   寻到最优值 @@@  \n");
				Flag_DiGu = 1;
				break;
			}
			//--------------------

			if ((t1b*a1) > 0 && (t1b*a1) < 360 && ABS(t1b*b1) > 0 && ABS(t1b*b1) < 360 && (t1b*c1) > 0 && (t1b*c1) < 360)
			{
				t1 = t1b;
				t2 = t2b;
				t3 = t3b;
				t4 = t4b;
				Flag_TiDu = 1;
			}
			else
			{
				Flag_DiGu = 1;
				break;
			}

		}
		Flag_DiGu = 1;
		DiguNum = i;
	}

	//------------------------------------------
	if (Flag_TiDu == 1)
	{
		printf("\t\t----------------------- >>> 已采用梯度下降算法(%d)  \n", DiguNum);
	}
#endif
	//------------------------------------------------------------------------------------------------------------------
	//将递归结果返回全局变量保存
	tN_GlobalS_4N[numc][0] = t1;
	tN_GlobalS_4N[numc][1] = t2;
	tN_GlobalS_4N[numc][2] = t3;
	tN_GlobalS_4N[numc][3] = t4;

	Energy_GlobalN[numc] = (knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12)*(knn[0][0] * t1*t1 + knn[0][1] * t2*t2 + knn[0][2] * t1*t2 + knn[0][3] * t1 + knn[0][4] * t2 + knn[0][5] - E12)
		+ (knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22)*(knn[1][0] * t2*t2 + knn[1][1] * t3*t3 + knn[1][2] * t2*t3 + knn[1][3] * t2 + knn[1][4] * t3 + knn[1][5] - E22)
		+ (knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32)*(knn[2][0] * t1*t1 + knn[2][1] * t3*t3 + knn[2][2] * t1*t3 + knn[2][3] * t1 + knn[2][4] * t3 + knn[2][5] - E32)

		+ (knn[3][0] * t1*t1 + knn[3][1] * t4*t4 + knn[3][2] * t1*t4 + knn[3][3] * t1 + knn[3][4] * t4 + knn[3][5] - E42)*(knn[3][0] * t1*t1 + knn[3][1] * t4*t4 + knn[3][2] * t1*t4 + knn[3][3] * t1 + knn[3][4] * t4 + knn[3][5] - E42)
		+ (knn[4][0] * t2*t2 + knn[4][1] * t4*t4 + knn[4][2] * t2*t4 + knn[4][3] * t2 + knn[4][4] * t4 + knn[4][5] - E52)*(knn[4][0] * t2*t2 + knn[4][1] * t4*t4 + knn[4][2] * t2*t4 + knn[4][3] * t2 + knn[4][4] * t4 + knn[4][5] - E52)
		+ (knn[5][0] * t3*t3 + knn[5][1] * t4*t4 + knn[5][2] * t3*t4 + knn[5][3] * t3 + knn[5][4] * t4 + knn[5][5] - E62)*(knn[5][0] * t3*t3 + knn[5][1] * t4*t4 + knn[5][2] * t3*t4 + knn[5][3] * t3 + knn[5][4] * t4 + knn[5][5] - E62)
		;
	printf(">递归结果(t1,t2,t3,t4):(%f ,%f ,%f ,%f)\n",t1,t2,t3,t4);
	printf("\n");
}

//------------------------------------------------------------------------------
//C1函数：
// (a1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0) - c1*(C11*X1+C12*Y1+C13*Z1+Tx-X0))
// (b1*(C11*X1+C12*Y1+C13*Z1+Tx-X0) - a1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0))

// C11偏导数=(a1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0) - c1*(C11*X1+C12*Y1+C13*Z1+Tx-X0))*(-c1*X1)
//          +(b1*(C11*X1+C12*Y1+C13*Z1+Tx-X0) - a1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0))*(b1*X1)
//------------------------------------------------------------------------------
// C12偏导数=(a1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0) - c1*(C11*X1+C12*Y1+C13*Z1+Tx-X0))*(-c1*Y1)
//          +(b1*(C11*X1+C12*Y1+C13*Z1+Tx-X0) - a1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0))*(b1*Y1)
//------------------------------------------------------------------------------
// C13偏导数=(a1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0) - c1*(C11*X1+C12*Y1+C13*Z1+Tx-X0))*(-c1*Z1)
//          +(b1*(C11*X1+C12*Y1+C13*Z1+Tx-X0) - a1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0))*(b1*Z1)

void GLB_Ft_C1N(float &FC11,float &FC12,float &FC13,float Xn,float Yn,float Zn,float X0,float Y0,float Z0,float an,float bn,float cn)
{
	float FC11r = (an*(C31*Xn + C32*Yn + C33*Zn + Tz - Z0) - cn*(C11*Xn + C12*Yn + C13*Zn + Tx - X0))*(-cn*Xn)
		        + (bn*(C11*Xn + C12*Yn + C13*Zn + Tx - X0) - an*(C21*Xn + C22*Yn + C23*Zn + Ty - Y0))*(bn*Xn);
	//------------------------------------------------------------------------------
	float FC12r = (an*(C31*Xn + C32*Yn + C33*Zn + Tz - Z0) - cn*(C11*Xn + C12*Yn + C13*Zn + Tx - X0))*(-cn*Yn)
		        + (bn*(C11*Xn + C12*Yn + C13*Zn + Tx - X0) - an*(C21*Xn + C22*Yn + C23*Zn + Ty - Y0))*(bn*Yn);
	//------------------------------------------------------------------------------
	float FC13r = (an*(C31*Xn + C32*Yn + C33*Zn + Tz - Z0) - cn*(C11*Xn + C12*Yn + C13*Zn + Tx - X0))*(-cn*Zn)
		        + (bn*(C11*Xn + C12*Yn + C13*Zn + Tx - X0) - an*(C21*Xn + C22*Yn + C23*Zn + Ty - Y0))*(bn*Zn);

	FC11 += FC11r;
	FC12 += FC12r;
	FC13 += FC13r;
}

/************************************************************************************************/
//C2函数：
// (c1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0) - b1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0))
// (b1*(C11*X1+C12*Y1+C13*Z1+Tx-X0) - a1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0))

// C21偏导数=(c1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0) - b1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0))*(c1*X1)
//          +(b1*(C11*X1+C12*Y1+C13*Z1+Tx-X0) - a1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0))*(-a1*X1)
//------------------------------------------------------------------------------
// C22偏导数=(c1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0) - b1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0))*(c1*Y1)
//          +(b1*(C11*X1+C12*Y1+C13*Z1+Tx-X0) - a1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0))*(-a1*Y1)
//------------------------------------------------------------------------------
// C23偏导数=(c1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0) - b1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0))*(c1*Z1)
//          +(b1*(C11*X1+C12*Y1+C13*Z1+Tx-X0) - a1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0))*(-a1*Z1)

void GLB_Ft_C2N(float &FC21, float &FC22, float &FC23, float Xn, float Yn, float Zn, float X0, float Y0, float Z0, float an, float bn, float cn)
{
	float FC21r = (cn*(C21*Xn + C22*Yn + C23*Zn + Ty - Y0) - bn*(C31*Xn + C32*Yn + C33*Zn + Tz - Z0))*(cn*Xn)
		        + (bn*(C11*Xn + C12*Yn + C13*Zn + Tx - X0) - an*(C21*Xn + C22*Yn + C23*Zn + Ty - Y0))*(-an*Xn);
	//------------------------------------------------------------------------------
	float FC22r = (cn*(C21*Xn + C22*Yn + C23*Zn + Ty - Y0) - bn*(C31*Xn + C32*Yn + C33*Zn + Tz - Z0))*(cn*Yn)
		        + (bn*(C11*Xn + C12*Yn + C13*Zn + Tx - X0) - an*(C21*Xn + C22*Yn + C23*Zn + Ty - Y0))*(-an*Yn);
	//------------------------------------------------------------------------------
	float FC23r = (cn*(C21*Xn + C22*Yn + C23*Zn + Ty - Y0) - bn*(C31*Xn + C32*Yn + C33*Zn + Tz - Z0))*(cn*Zn)
		        + (bn*(C11*Xn + C12*Yn + C13*Zn + Tx - X0) - an*(C21*Xn + C22*Yn + C23*Zn + Ty - Y0))*(-an*Zn);

	FC21 += FC21r;
	FC22 += FC22r;
	FC23 += FC23r;
}
/************************************************************************************************/
//C3函数：
// (c1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0) - b1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0))
// (a1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0) - c1*(C11*X1+C12*Y1+C13*Z1+Tx-X0))

// C31偏导数=(c1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0) - b1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0))*(-b1*X1)
//          +(a1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0) - c1*(C11*X1+C12*Y1+C13*Z1+Tx-X0))*(a1*X1)
//------------------------------------------------------------------------------
// C32偏导数=(c1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0) - b1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0))*(-b1*Y1)
//          +(a1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0) - c1*(C11*X1+C12*Y1+C13*Z1+Tx-X0))*(a1*Y1)
//------------------------------------------------------------------------------
// C33偏导数=(c1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0) - b1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0))*(-b1*Z1)
//          +(a1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0) - c1*(C11*X1+C12*Y1+C13*Z1+Tx-X0))*(a1*Z1)

void GLB_Ft_C3N(float &FC31, float &FC32, float &FC33, float Xn, float Yn, float Zn, float X0, float Y0, float Z0, float an, float bn, float cn)
{
	float FC31r = (cn*(C21*Xn + C22*Yn + C23*Zn + Ty - Y0) - bn*(C31*Xn + C32*Yn + C33*Zn + Tz - Z0))*(-bn*Xn)
		        + (an*(C31*Xn + C32*Yn + C33*Zn + Tz - Z0) - cn*(C11*Xn + C12*Yn + C13*Zn + Tx - X0))*(an*Xn);
	//------------------------------------------------------------------------------
	float FC32r = (cn*(C21*Xn + C22*Yn + C23*Zn + Ty - Y0) - bn*(C31*Xn + C32*Yn + C33*Zn + Tz - Z0))*(-bn*Yn)
		        + (an*(C31*Xn + C32*Yn + C33*Zn + Tz - Z0) - cn*(C11*Xn + C12*Yn + C13*Zn + Tx - X0))*(an*Yn);
	//------------------------------------------------------------------------------
	float FC33r = (cn*(C21*Xn + C22*Yn + C23*Zn + Ty - Y0) - bn*(C31*Xn + C32*Yn + C33*Zn + Tz - Z0))*(-bn*Zn)
		        + (an*(C31*Xn + C32*Yn + C33*Zn + Tz - Z0) - cn*(C11*Xn + C12*Yn + C13*Zn + Tx - X0))*(an*Zn);

	FC31 += FC31r;
	FC32 += FC32r;
	FC33 += FC33r;
}

/************************************************************************************************/
//Tx函数：
// (a1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0) - c1*(C11*X1+C12*Y1+C13*Z1+Tx-X0))
// (b1*(C11*X1+C12*Y1+C13*Z1+Tx-X0) - a1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0))

// Tx偏导数=(a1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0) - c1*(C11*X1+C12*Y1+C13*Z1+Tx-X0))*(-c1)
//         +(b1*(C11*X1+C12*Y1+C13*Z1+Tx-X0) - a1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0))*(b1)

/************************************************************************************************/
//Ty函数：
// (c1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0) - b1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0))
// (b1*(C11*X1+C12*Y1+C13*Z1+Tx-X0) - a1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0))

// Ty偏导数=(c1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0) - b1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0))*(c1)
//         +(b1*(C11*X1+C12*Y1+C13*Z1+Tx-X0) - a1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0))*(-a1)

/************************************************************************************************/
//Tz函数：
// (c1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0) - b1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0))
// (a1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0) - c1*(C11*X1+C12*Y1+C13*Z1+Tx-X0))

// Tz偏导数=(c1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0) - b1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0))*(-b1)
//         +(a1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0) - c1*(C11*X1+C12*Y1+C13*Z1+Tx-X0))*(a1)

void GLB_Ft_Txyz(float &FTxn, float &FTyn, float &FTzn, float Xn, float Yn, float Zn, float X0, float Y0, float Z0, float an, float bn, float cn)
{
	float FTxr = (an*(C31*Xn + C32*Yn + C33*Zn + Tz - Z0) - cn*(C11*Xn + C12*Yn + C13*Zn + Tx - X0))*(-cn)
		       + (bn*(C11*Xn + C12*Yn + C13*Zn + Tx - X0) - an*(C21*Xn + C22*Yn + C23*Zn + Ty - Y0))*(bn);

	float FTyr = (cn*(C21*Xn + C22*Yn + C23*Zn + Ty - Y0) - bn*(C31*Xn + C32*Yn + C33*Zn + Tz - Z0))*(cn)
		       +(bn*(C11*Xn + C12*Yn + C13*Zn + Tx - X0) - an*(C21*Xn + C22*Yn + C23*Zn + Ty - Y0))*(-an);

	float FTzr = (cn*(C21*Xn + C22*Yn + C23*Zn + Ty - Y0) - bn*(C31*Xn + C32*Yn + C33*Zn + Tz - Z0))*(-bn)
		       + (an*(C31*Xn + C32*Yn + C33*Zn + Tz - Z0) - cn*(C11*Xn + C12*Yn + C13*Zn + Tx - X0))*(an);

	FTxn += FTxr;
	FTyn += FTyr;
	FTzn += FTzr;
}

/********************************** 方向余弦建模寻优 *********************************/
void GL_Build_YuX_M4Point(int *ID_Pt, float*ID_Length, float tN_GlobalS_4N[][4], float* Energy_GlobalN, float *tStep_nn_GX, int numc,
	int &Point_Check, int &DiguCnt, int &DiguNum, bool Flag_Step, int Rude_Step_Control)
{
	//当多组合时，DiguNum 与 DiguCnt必须清零。
	DiguNum = 0;
	DiguCnt = 0;

	//Point_Check = 4;//模式选择：算法点数
	//float t1 = tN_GlobalS_4N[numc][0];
	//float t2 = tN_GlobalS_4N[numc][1];
	//float t3 = tN_GlobalS_4N[numc][2];
	//float t4 = tN_GlobalS_4N[numc][3];
	//-------------------
	//偏导
	float FC11 , FC12 , FC13 ;
	float FC21 , FC22 , FC23 ;
	float FC31 , FC32 , FC33 ;

	float FTx , FTy , FTz ;

	//缓存初始化
	float C11b = C11, C12b = C12, C13b = C13;
	float C21b = C21, C22b = C22, C23b = C23;
	float C31b = C31, C32b = C32, C33b = C33;

	float Txb = Tx, Tyb = Ty, Tzb = Tz;

	//射线向量
	float a1 = LineRays[ID_Pt[0]].x; float b1 = LineRays[ID_Pt[0]].y; float c1 = LineRays[ID_Pt[0]].z;
	float a2 = LineRays[ID_Pt[1]].x; float b2 = LineRays[ID_Pt[1]].y; float c2 = LineRays[ID_Pt[1]].z;
	float a3 = LineRays[ID_Pt[2]].x; float b3 = LineRays[ID_Pt[2]].y; float c3 = LineRays[ID_Pt[2]].z;
	float a4 = LineRays[ID_Pt[3]].x; float b4 = LineRays[ID_Pt[3]].y; float c4 = LineRays[ID_Pt[3]].z;
	// 
	GL_Vector pt0[4];
	pt0[0] = LineRays[ID_Pt[0]].pt0;
	pt0[1] = LineRays[ID_Pt[1]].pt0;
	pt0[2] = LineRays[ID_Pt[2]].pt0;
	pt0[3] = LineRays[ID_Pt[3]].pt0;

	//多点射线同时过一个共同的点 （X0,Y0,Z0）
	float X0 = LineRays[ID_Pt[0]].pt0.x;
	float Y0 = LineRays[ID_Pt[0]].pt0.y;
	float Z0 = LineRays[ID_Pt[0]].pt0.z;
	//模型各点相对于旋转中心的相对坐标
	float X1 = HeadPlay_PtB[ID_Pt[0]].x;
	float Y1 = HeadPlay_PtB[ID_Pt[0]].y;
	float Z1 = HeadPlay_PtB[ID_Pt[0]].z;

	float X2 = HeadPlay_PtB[ID_Pt[1]].x;
	float Y2 = HeadPlay_PtB[ID_Pt[1]].y;
	float Z2 = HeadPlay_PtB[ID_Pt[1]].z;

	float X3 = HeadPlay_PtB[ID_Pt[2]].x;
	float Y3 = HeadPlay_PtB[ID_Pt[2]].y;
	float Z3 = HeadPlay_PtB[ID_Pt[2]].z;

	float X4 = HeadPlay_PtB[ID_Pt[3]].x;
	float Y4 = HeadPlay_PtB[ID_Pt[3]].y;
	float Z4 = HeadPlay_PtB[ID_Pt[3]].z;
	/**********************************************************/
	// Pt1
	// (c1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0) - b1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0))
	// (a1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0) - c1*(C11*X1+C12*Y1+C13*Z1+Tx-X0))
	// (b1*(C11*X1+C12*Y1+C13*Z1+Tx-X0) - a1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0))

	// Pt2
	// (c2*(C21*X2+C22*Y2+C23*Z2+Ty-Y0) - b2*(C31*X2+C32*Y2+C33*Z2+Tz-Z0))
	// (a2*(C31*X2+C32*Y2+C33*Z2+Tz-Z0) - c2*(C11*X2+C12*Y2+C13*Z2+Tx-X0))
	// (b2*(C11*X2+C12*Y2+C13*Z2+Tx-X0) - a2*(C21*X2+C22*Y2+C23*Z2+Ty-Y0))

	// Pt3
	// (c3*(C21*X3+C22*Y3+C23*Z3+Ty-Y0) - b3*(C31*X3+C32*Y3+C33*Z3+Tz-Z0))
	// (a3*(C31*X3+C32*Y3+C33*Z3+Tz-Z0) - c3*(C11*X3+C12*Y3+C13*Z3+Tx-X0))
	// (b3*(C11*X3+C12*Y3+C13*Z3+Tx-X0) - a3*(C21*X3+C22*Y3+C23*Z3+Ty-Y0))

	// Pt4
	// (c4*(C21*X4+C22*Y4+C23*Z4+Ty-Y0) - b4*(C31*X4+C32*Y4+C33*Z4+Tz-Z0))
	// (a4*(C31*X4+C32*Y4+C33*Z4+Tz-Z0) - c4*(C11*X4+C12*Y4+C13*Z4+Tx-X0))
	// (b4*(C11*X4+C12*Y4+C13*Z4+Tx-X0) - a4*(C21*X4+C22*Y4+C23*Z4+Ty-Y0))
	//------------------------------------------------------------------------------
	// 相关参数
	// n=(1,2,3,4)
	// Pt.x=Xn;
	// Pt.y=Yn;
	// Pt.z=Zn;
	//------------------------------------------------------------------------------
	//C1函数：
	// (a1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0) - c1*(C11*X1+C12*Y1+C13*Z1+Tx-X0))
	// (b1*(C11*X1+C12*Y1+C13*Z1+Tx-X0) - a1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0))

	// C11偏导数=(a1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0) - c1*(C11*X1+C12*Y1+C13*Z1+Tx-X0))*(-c1*X1)
	//          +(b1*(C11*X1+C12*Y1+C13*Z1+Tx-X0) - a1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0))*(b1*X1)
	//------------------------------------------------------------------------------
	// C12偏导数=(a1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0) - c1*(C11*X1+C12*Y1+C13*Z1+Tx-X0))*(-c1*Y1)
	//          +(b1*(C11*X1+C12*Y1+C13*Z1+Tx-X0) - a1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0))*(b1*Y1)
	//------------------------------------------------------------------------------
	// C13偏导数=(a1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0) - c1*(C11*X1+C12*Y1+C13*Z1+Tx-X0))*(-c1*Z1)
	//          +(b1*(C11*X1+C12*Y1+C13*Z1+Tx-X0) - a1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0))*(b1*Z1)

	/************************************************************************************************/
	//C2函数：
	// (c1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0) - b1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0))
	// (b1*(C11*X1+C12*Y1+C13*Z1+Tx-X0) - a1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0))

	// C21偏导数=(c1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0) - b1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0))*(c1*X1)
	//          +(b1*(C11*X1+C12*Y1+C13*Z1+Tx-X0) - a1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0))*(-a1*X1)
	//------------------------------------------------------------------------------
	// C22偏导数=(c1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0) - b1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0))*(c1*Y1)
	//          +(b1*(C11*X1+C12*Y1+C13*Z1+Tx-X0) - a1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0))*(-a1*Y1)
	//------------------------------------------------------------------------------
	// C23偏导数=(c1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0) - b1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0))*(c1*Z1)
	//          +(b1*(C11*X1+C12*Y1+C13*Z1+Tx-X0) - a1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0))*(-a1*Z1)

	/************************************************************************************************/
	//C3函数：
	// (c1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0) - b1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0))
	// (a1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0) - c1*(C11*X1+C12*Y1+C13*Z1+Tx-X0))

	// C31偏导数=(c1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0) - b1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0))*(-b1*X1)
	//          +(a1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0) - c1*(C11*X1+C12*Y1+C13*Z1+Tx-X0))*(a1*X1)
	//------------------------------------------------------------------------------
	// C32偏导数=(c1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0) - b1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0))*(-b1*Y1)
	//          +(a1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0) - c1*(C11*X1+C12*Y1+C13*Z1+Tx-X0))*(a1*Y1)
	//------------------------------------------------------------------------------
	// C33偏导数=(c1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0) - b1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0))*(-b1*Z1)
	//          +(a1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0) - c1*(C11*X1+C12*Y1+C13*Z1+Tx-X0))*(a1*Z1)

	/************************************************************************************************/
	//Tx函数：
	// (a1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0) - c1*(C11*X1+C12*Y1+C13*Z1+Tx-X0))
	// (b1*(C11*X1+C12*Y1+C13*Z1+Tx-X0) - a1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0))
	
	// Tx偏导数=(a1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0) - c1*(C11*X1+C12*Y1+C13*Z1+Tx-X0))*(-c1)
	//         +(b1*(C11*X1+C12*Y1+C13*Z1+Tx-X0) - a1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0))*(b1)

	/************************************************************************************************/
	//Ty函数：
	// (c1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0) - b1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0))
	// (b1*(C11*X1+C12*Y1+C13*Z1+Tx-X0) - a1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0))

	// Ty偏导数=(c1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0) - b1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0))*(c1)
	//         +(b1*(C11*X1+C12*Y1+C13*Z1+Tx-X0) - a1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0))*(-a1)

	/************************************************************************************************/
	//Tz函数：
	// (c1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0) - b1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0))
	// (a1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0) - c1*(C11*X1+C12*Y1+C13*Z1+Tx-X0))

	// Tz偏导数=(c1*(C21*X1+C22*Y1+C23*Z1+Ty-Y0) - b1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0))*(-b1)
	//         +(a1*(C31*X1+C32*Y1+C33*Z1+Tz-Z0) - c1*(C11*X1+C12*Y1+C13*Z1+Tx-X0))*(a1)

	Fps_Track_Start = 1;//显示追踪点标志位
	float nn = 0.000115;// 递归步长 适中（不能太大也不能太小）。
	//-------------------------------------------------------------
	printf("\t ----------------------------- >>> 使用方向余弦整体建模!!!\n");
	bool Flag_DiGu = 0;
	while (!Flag_DiGu)
	{
		/*if(DiguCnt>15)nn=0.000023;*/
		if (DiguCnt>65)break;
		DiguNum = 0;

		int i = 0;

		for (i = 0; i<65535; i++)
		{
			//偏导初始化化
			FC11 = 0; FC12 = 0; FC13 = 0;
			FC21 = 0; FC22 = 0; FC23 = 0;
			FC31 = 0; FC32 = 0; FC33 = 0;

			FTx = 0; FTy = 0; FTz = 0;

			GLB_Ft_C1N(FC11, FC12, FC13, X1, Y1, Z1, X0, Y0, Z0, a1, b1, c1);
			GLB_Ft_C2N(FC21, FC22, FC23, X1, Y1, Z1, X0, Y0, Z0, a1, b1, c1);
			GLB_Ft_C3N(FC31, FC32, FC33, X1, Y1, Z1, X0, Y0, Z0, a1, b1, c1);
			GLB_Ft_Txyz(FTx, FTy, FTz,   X1, Y1, Z1, X0, Y0, Z0, a1, b1, c1);

			GLB_Ft_C1N(FC11, FC12, FC13, X2, Y2, Z2, X0, Y0, Z0, a2, b2, c2);
			GLB_Ft_C2N(FC21, FC22, FC23, X2, Y2, Z2, X0, Y0, Z0, a2, b2, c2);
			GLB_Ft_C3N(FC31, FC32, FC33, X2, Y2, Z2, X0, Y0, Z0, a2, b2, c2);
			GLB_Ft_Txyz(FTx, FTy, FTz,   X2, Y2, Z2, X0, Y0, Z0, a2, b2, c2);

			GLB_Ft_C1N(FC11, FC12, FC13, X3, Y3, Z3, X0, Y0, Z0, a3, b3, c3);
			GLB_Ft_C2N(FC21, FC22, FC23, X3, Y3, Z3, X0, Y0, Z0, a3, b3, c3);
			GLB_Ft_C3N(FC31, FC32, FC33, X3, Y3, Z3, X0, Y0, Z0, a3, b3, c3);
			GLB_Ft_Txyz(FTx, FTy, FTz,   X3, Y3, Z3, X0, Y0, Z0, a3, b3, c3);

			GLB_Ft_C1N(FC11, FC12, FC13, X4, Y4, Z4, X0, Y0, Z0, a4, b4, c4);
			GLB_Ft_C2N(FC21, FC22, FC23, X4, Y4, Z4, X0, Y0, Z0, a4, b4, c4);
			GLB_Ft_C3N(FC31, FC32, FC33, X4, Y4, Z4, X0, Y0, Z0, a4, b4, c4);
			GLB_Ft_Txyz(FTx, FTy, FTz,   X4, Y4, Z4, X0, Y0, Z0, a4, b4, c4);
			//-----------------------------------------------------------------------------------------------------------
			C11b = C11 - nn*FC11;
			C12b = C12 - nn*FC12;
			C13b = C13 - nn*FC13;

			C21b = C21 - nn*FC21;
			C22b = C22 - nn*FC22;
			C23b = C23 - nn*FC23;

			C31b = C31 - nn*FC31;
			C32b = C32 - nn*FC32;
			C33b = C33 - nn*FC33;

			Txb = Tx - nn*FTx;
			Tyb = Ty - nn*FTy;
			Tzb = Tz - nn*FTz;

			//if (Txb > 0 && Txb < 600 && ABS(Tyb) < 200 && Tzb > 0 && Tzb < 600)
			{
				
				//-------- 正交
				float C11z = C11b, C12z = C12b, C13z = C13b;
				float C21z = C21b, C22z = C22b, C23z = C23b;

				float norm1 = C11z*C11z + C12z*C12z + C13z*C13z;
				float A1 = C11b*C21b + C12b*C22b + C13b*C23b;

				if (norm1 != 0)
				{
					C21z = C21b - A1*C11z / norm1;
					C22z = C22b - A1*C12z / norm1;
					C23z = C23b - A1*C13z / norm1;
				}
				
				float norm2 = C21z*C21z + C22z*C22z + C23z*C23z;

				float C31z, C32z, C33z;
				if (norm2 != 0 && norm1 != 0)
				{
					float A2 = C31b*C11z + C32b*C12z + C33b*C13z;
					float A3 = C31b*C21z + C32b*C22z + C33b*C23z;

					C31z = C31b - A2*C11z / norm1 - A3*C21z / norm2;
					C32z = C32b - A2*C12z / norm1 - A3*C22z / norm2;
					C33z = C33b - A2*C13z / norm1 - A3*C23z / norm2;

					//-------------------单位化
					float norm = C11z*C11z + C12z*C12z + C13z*C13z;
					norm = sqrt(norm);
					if (norm != 0)
					{
						C11z = C11z / norm;
						C12z = C12z / norm;
						C13z = C13z / norm;
					}

					norm = C21z*C21z + C22z*C22z + C23z*C23z;
					norm = sqrt(norm);
					if (norm != 0)
					{
						C21z = C21z / norm;
						C22z = C22z / norm;
						C23z = C23z / norm;
					}

					norm = C31z*C31z + C32z*C32z + C33z*C33z;
					norm = sqrt(norm);
					if (norm != 0)
					{
						C31z = C31z / norm;
						C32z = C32z / norm;
						C33z = C33z / norm;
					}
					//---------------
					C11b = C11z;
					C12b = C12z;
					C13b = C13z;

					C21b = C21z;
					C22b = C22z;
					C23b = C23z;

					C31b = C31z;
					C32b = C32z;
					C33b = C33z;
				}

				C11 = C11b;
				C12 = C12b;
				C13 = C13b;

				C21 = C21b;
				C22 = C22b;
				C23 = C23b;

				C31 = C31b;
				C32 = C32b;
				C33 = C33b;

				


				Tx = Txb;
				Ty = Tyb;
				Tz = Tzb;
			}

			if (ABS(FC11)<=0.00 && ABS(FC12)<=0.00 && ABS(FC13)<=0.00 && ABS(FTx)<=0.00 &&
				ABS(FC21)<=0.00 && ABS(FC22)<=0.00 && ABS(FC23)<=0.00 && ABS(FTy)<=0.00 &&
				ABS(FC31)<=0.00 && ABS(FC32)<=0.00 && ABS(FC33)<=0.00 && ABS(FTz)<=0.00
				)
			{
				
				Flag_DiGu = 1;
				printf("\t ----------------------------- >>>方向余弦整体建模,寻找到最优解退出!!!\n");
				break;
			}

		}
		DiguCnt++;
		DiguNum = i;
	}

	//--------------------------------------------------------------------------
	//计算真值
	float XW1 = C11*X1 + C12*Y1 + C13*Z1 + Tx;
	float YW1 = C21*X1 + C22*Y1 + C23*Z1 + Ty;
	float ZW1 = C31*X1 + C32*Y1 + C33*Z1 + Tz;

	float XW2 = C11*X2 + C12*Y2 + C13*Z2 + Tx;
	float YW2 = C21*X2 + C22*Y2 + C23*Z2 + Ty;
	float ZW2 = C31*X2 + C32*Y2 + C33*Z2 + Tz;

	float XW3 = C11*X3 + C12*Y3 + C13*Z3 + Tx;
	float YW3 = C21*X3 + C22*Y3 + C23*Z3 + Ty;
	float ZW3 = C31*X3 + C32*Y3 + C33*Z3 + Tz;

	float XW4 = C11*X4 + C12*Y4 + C13*Z4 + Tx;
	float YW4 = C21*X4 + C22*Y4 + C23*Z4 + Ty;
	float ZW4 = C31*X4 + C32*Y4 + C33*Z4 + Tz;

	if (a1!=0&&a2!=0&&a3!=0&&a4!=0)
	{
		float t1 = (XW1 - X0) / a1;
		float t2 = (XW2 - X0) / a2;
		float t3 = (XW3 - X0) / a3;
		float t4 = (XW4 - X0) / a4;

		tN_GlobalS_4N[numc][0] = t1;
		tN_GlobalS_4N[numc][1] = t2;
		tN_GlobalS_4N[numc][2] = t3;
		tN_GlobalS_4N[numc][3] = t4;

		//
		// Pt1
		Energy_GlobalN[numc] = (c1*(C21*X1 + C22*Y1 + C23*Z1 + Ty - Y0) - b1*(C31*X1 + C32*Y1 + C33*Z1 + Tz - Z0))*(c1*(C21*X1 + C22*Y1 + C23*Z1 + Ty - Y0) - b1*(C31*X1 + C32*Y1 + C33*Z1 + Tz - Z0))
			+ (a1*(C31*X1 + C32*Y1 + C33*Z1 + Tz - Z0) - c1*(C11*X1 + C12*Y1 + C13*Z1 + Tx - X0))*(a1*(C31*X1 + C32*Y1 + C33*Z1 + Tz - Z0) - c1*(C11*X1 + C12*Y1 + C13*Z1 + Tx - X0))
			+ (b1*(C11*X1 + C12*Y1 + C13*Z1 + Tx - X0) - a1*(C21*X1 + C22*Y1 + C23*Z1 + Ty - Y0))*(b1*(C11*X1 + C12*Y1 + C13*Z1 + Tx - X0) - a1*(C21*X1 + C22*Y1 + C23*Z1 + Ty - Y0))

			+ (c2*(C21*X2 + C22*Y2 + C23*Z2 + Ty - Y0) - b2*(C31*X2 + C32*Y2 + C33*Z2 + Tz - Z0))*(c2*(C21*X2 + C22*Y2 + C23*Z2 + Ty - Y0) - b2*(C31*X2 + C32*Y2 + C33*Z2 + Tz - Z0))
			+ (a2*(C31*X2 + C32*Y2 + C33*Z2 + Tz - Z0) - c2*(C11*X2 + C12*Y2 + C13*Z2 + Tx - X0))*(a2*(C31*X2 + C32*Y2 + C33*Z2 + Tz - Z0) - c2*(C11*X2 + C12*Y2 + C13*Z2 + Tx - X0))
			+ (b2*(C11*X2 + C12*Y2 + C13*Z2 + Tx - X0) - a2*(C21*X2 + C22*Y2 + C23*Z2 + Ty - Y0))*(b2*(C11*X2 + C12*Y2 + C13*Z2 + Tx - X0) - a2*(C21*X2 + C22*Y2 + C23*Z2 + Ty - Y0))

			+ (c3*(C21*X3 + C22*Y3 + C23*Z3 + Ty - Y0) - b3*(C31*X3 + C32*Y3 + C33*Z3 + Tz - Z0))*(c3*(C21*X3 + C22*Y3 + C23*Z3 + Ty - Y0) - b3*(C31*X3 + C32*Y3 + C33*Z3 + Tz - Z0))
			+ (a3*(C31*X3 + C32*Y3 + C33*Z3 + Tz - Z0) - c3*(C11*X3 + C12*Y3 + C13*Z3 + Tx - X0))*(a3*(C31*X3 + C32*Y3 + C33*Z3 + Tz - Z0) - c3*(C11*X3 + C12*Y3 + C13*Z3 + Tx - X0))
			+ (b3*(C11*X3 + C12*Y3 + C13*Z3 + Tx - X0) - a3*(C21*X3 + C22*Y3 + C23*Z3 + Ty - Y0))*(b3*(C11*X3 + C12*Y3 + C13*Z3 + Tx - X0) - a3*(C21*X3 + C22*Y3 + C23*Z3 + Ty - Y0))

			+ (c4*(C21*X4 + C22*Y4 + C23*Z4 + Ty - Y0) - b4*(C31*X4 + C32*Y4 + C33*Z4 + Tz - Z0))*(c4*(C21*X4 + C22*Y4 + C23*Z4 + Ty - Y0) - b4*(C31*X4 + C32*Y4 + C33*Z4 + Tz - Z0))
			+ (a4*(C31*X4 + C32*Y4 + C33*Z4 + Tz - Z0) - c4*(C11*X4 + C12*Y4 + C13*Z4 + Tx - X0))*(a4*(C31*X4 + C32*Y4 + C33*Z4 + Tz - Z0) - c4*(C11*X4 + C12*Y4 + C13*Z4 + Tx - X0))
			+ (b4*(C11*X4 + C12*Y4 + C13*Z4 + Tx - X0) - a4*(C21*X4 + C22*Y4 + C23*Z4 + Ty - Y0))*(b4*(C11*X4 + C12*Y4 + C13*Z4 + Tx - X0) - a4*(C21*X4 + C22*Y4 + C23*Z4 + Ty - Y0))
			;
		printf("\t ----------------------------- >>> 使用方向余弦整体,保存运算结果!!!\n");

	}
	

	//将递归结果返回全局变量保存
	
}

#endif // !__OPTINIZATION_MATH__
