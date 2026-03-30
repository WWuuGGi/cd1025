#ifndef __MATH_H__
#define __MATH_H__

void skew(const float w[3], float W[3][3]);
void mat_transpose(const float R[3][3], float RT[3][3]);
void matmul_33_33(const float A[3][3], const float B[3][3], float C[3][3]);
void matmul_33_31(const float A[3][3], const float x[3], float y[3]);
void matmul_33_34(const float A[3][3], const float B[3][4], float C[3][4]);
void matmul_34_41(const float A[3][4], const float x[4], float y[3]);
void matmul_44_43(const float A[4][4], const float B[4][3], float C[4][3]);
void matmul_43_31(const float A[4][3], const float x[3], float y[4]);
void quat_error(const float p[4], const float q[4], float e[3]);
void quat2rot(const float q[4], float R[3][3]);

#endif
