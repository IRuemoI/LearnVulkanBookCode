#pragma once
#include "Matrix.h"//导入需要的头文件

class MatrixState3D {
public:
    static float currMatrix[16];// 当前变换矩阵
    static float mProjMatrix[16];// 投影矩阵
    static float mVMatrix[16];// 摄像机矩阵
    static float mMVPMatrix[16];// 总矩阵
    static float vulkanClipMatrix[16];// 剪裁空间矩阵 X不变 Y置反 Z减半
    static float mStack[10][16];// 保护变换矩阵的栈
    static int stackTop;// 栈顶位置
    static void setInitStack();// 初始化矩阵

    static void pushMatrix();// 保护变换矩阵

    static void popMatrix();// 恢复变换矩阵
    static void translate(float x, float y, float z);// 沿x、y、z轴平移

    static void rotate(float angle, float x, float y, float z);// 绕指定轴旋转

    static void scale(float x, float y, float z);// 矩阵缩放

    static void setCamera(float cx, float cy, float cz, float tx, float ty, float tz, float upx, float upy, float upz);// 设置摄像机

    static void setProjectFrustum(float left, float right, float bottom, float top, float near, float far);// 设置透视投影参数

    static void setProjectOrtho(float left, float right, float bottom, float top, float near, float far);//设置正交投影参数

    static float *getFinalMatrix();// 获取最终矩阵

    static float *getMMatrix();// 获取当前变换矩阵
};
