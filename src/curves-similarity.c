/*
 ============================================================================
 Name        : c_test.c
 Author      : 陈华友
 Version     :
 Copyright   : xbm
 Description : Curve Similarity in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <string.h>
#define PI 3.141592



typedef struct { //定义点
	float x;
	float y;
}Point;

typedef struct { //定义曲线
	Point *data;
	int length;
}Curve;

typedef struct { //定义矩阵
	float *data;
	int m;
	int n;
}Matrix;


float min(float n1, float n2, float n3) {
	float min = n1;
	if (min > n2) min = n2;
	if (min > n3) min = n3;
	return min;
}

float max(float a, float b) {
	return a>b?a:b;
}

float Matrix_get(Matrix *p, int i, int j) { //获取矩阵对应下标的值
	return p->data[i*(p->n)+j];
}

void Matrix_set(Matrix *p, int i, int j, float value) { //设置矩阵对应下标的值
	p->data[i*(p->n)+j] = value;
}

float euc_dist(Point *p1, Point *p2) {
	float x1 = p1->x;
	float y1 = p1->y;
	float x2 = p2->x;
	float y2 = p2->y;
	float ans = sqrt(pow((x1-x2),2)+pow((y1-y2),2));
	return ans;
}

float _c(Matrix *pArr, int i, int j, Curve *P, Curve *Q) {
	float caij = Matrix_get(pArr, i, j);
	if (caij > -1) {
		return caij;
	}else if (i==0 && j==0) {
        caij = euc_dist(P->data,Q->data);
		Matrix_set(pArr,i,j,caij);
	}else if (i>0 && j==0) {
        caij = max(_c(pArr,i-1,0,P,Q),euc_dist(P->data+i,Q->data));
		Matrix_set(pArr,i,j,caij);
	}else if (i==0 && j>0) {
        caij = max(_c(pArr,0,j-1,P,Q),euc_dist(P->data,Q->data+j));
		Matrix_set(pArr,i,j,caij);
	}else if (i>0 && j>0) {
        caij = max(min(_c(pArr,i-1,j,P,Q),_c(pArr,i-1,j-1,P,Q),_c(pArr,i,j-1,P,Q)),euc_dist(P->data+i,Q->data+j));
		Matrix_set(pArr,i,j,caij);
	} else {
        caij = FLT_MAX;
		Matrix_set(pArr,i,j,caij);
	}
	return caij;
}

float frechetDist(Curve *P, Curve *Q) {
	Matrix ca;
	ca.m = P->length;
	ca.n = Q->length;
	ca.data = (float *)malloc(ca.m*ca.n*sizeof(float));
    memset(ca.data,-1,ca.m*ca.n*sizeof(float));
	return _c(&ca,P->length-1,Q->length-1,P,Q);
}


int main(void) {
    int SIZE = 2*(PI)/0.01;
	//构造两条曲线
	Curve P;
	P.data = (Point *)malloc(SIZE*sizeof(Point));
	P.length = SIZE;

	Curve Q;
	Q.data = (Point *)malloc(SIZE*sizeof(Point));
	Q.length = SIZE;

	float x = 0.0;
	int i = 0;
	while (x < 2*PI) {
		float y = sin(x);
		Point p = {x,y};
		Point q = {x,y+1};
		P.data[i] = p;
		Q.data[i] = q;
		x += 0.01;
		i ++;
	}
	float ans = frechetDist(&P,&Q);

	printf("amount of point is : %d\n", SIZE); /* prints !!!Hello World!!! */
	printf("the frechet distance is : %f",ans);
	return EXIT_SUCCESS;
}
