#include "math.h"
#include "stdint.h"

// 计算反对称矩阵
void skew(const float w[3], float W[3][3])
{
    W[0][0] = 0;      W[0][1] = -w[2]; W[0][2] = w[1];
    W[1][0] = w[2];   W[1][1] = 0;     W[1][2] = -w[0];
    W[2][0] = -w[1];  W[2][1] = w[0];  W[2][2] = 0;
}

// 计算矩阵转置
void mat_transpose(const float R[3][3], float RT[3][3])
{
    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            RT[i][j] = R[j][i];
        }
    }
}

// 3x3矩阵乘法
void matmul_33_33(const float A[3][3], const float B[3][3], float C[3][3])
{
    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            C[i][j] = 0;
            for (uint8_t k = 0; k < 3; k++) {   
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// 3x3矩阵和3x1向量乘法
void matmul_33_31(const float A[3][3], const float x[3], float y[3])
{
    for (uint8_t i = 0; i < 3; i++) {
        y[i] = 0;
        for (uint8_t j = 0; j < 3; j++) {
            y[i] += A[i][j] * x[j];
        }
    }
}

// 3x3矩阵和3x4矩阵乘法
void matmul_33_34(const float A[3][3], const float B[3][4], float C[3][4])
{
    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            C[i][j] = 0;
            for (uint8_t k = 0; k < 3; k++) {   
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// 3x4矩阵和4x1向量乘法
void matmul_34_41(const float A[3][4], const float x[4], float y[3])
{
    for (uint8_t i = 0; i < 3; i++) {
        y[i] = 0;
        for (uint8_t j = 0; j < 4; j++) {
            y[i] += A[i][j] * x[j];
        }
    }
}

// 4x4矩阵和4x3矩阵乘法
void matmul_44_43(const float A[4][4], const float B[4][3], float C[4][3])
{
    for (uint8_t i = 0; i < 4; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            C[i][j] = 0;
            for (uint8_t k = 0; k < 4; k++) {   
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// 4x3矩阵和3x1向量乘法
void matmul_43_31(const float A[4][3], const float x[3], float y[4])
{
    for (uint8_t i = 0; i < 4; i++) {
        y[i] = 0;
        for (uint8_t j = 0; j < 3; j++) {
            y[i] += A[i][j] * x[j];
        }
    }
}

// 四元数误差
void quat_error(const float qd[4], const float q[4], float e[3])
{
    float qd_inv[4];
    qd_inv[0] = qd[0];
    qd_inv[1] = -qd[1];
    qd_inv[2] = -qd[2];
    qd_inv[3] = -qd[3];

    e[1] = qd_inv[0]*q[1] + qd_inv[1]*q[0] + qd_inv[2]*q[3] - qd_inv[3]*q[2];
    e[2] = qd_inv[0]*q[2] - qd_inv[1]*q[3] + qd_inv[2]*q[0] + qd_inv[3]*q[1];
    e[3] = qd_inv[0]*q[3] + qd_inv[1]*q[2] - qd_inv[2]*q[1] + qd_inv[3]*q[0];
}

// 由四元数计算旋转矩阵
void quat2rot(const float q[4], float R[3][3])
{
    R[0][0] = 1 - 2*q[2]*q[2] - 2*q[3]*q[3];
    R[0][1] = 2*q[1]*q[2] - 2*q[0]*q[3];
    R[0][2] = 2*q[1]*q[3] + 2*q[0]*q[2];
    R[1][0] = 2*q[1]*q[2] + 2*q[0]*q[3];
    R[1][1] = 1 - 2*q[1]*q[1] - 2*q[3]*q[3];
    R[1][2] = 2*q[2]*q[3] - 2*q[0]*q[1];
    R[2][0] = 2*q[1]*q[3] - 2*q[0]*q[2];
    R[2][1] = 2*q[2]*q[3] + 2*q[0]*q[1];
    R[2][2] = 1 - 2*q[1]*q[1] - 2*q[2]*q[2];
}
