#include "MatrixState3D.h"
#include <math.h>
float MatrixState3D::vulkanClipMatrix[16] = {1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0.5, 0, 0, 0, 0.5, 1.0};
float MatrixState3D::mProjMatrix[16];
float MatrixState3D::mVMatrix[16];
float MatrixState3D::cx;
float MatrixState3D::cy;
float MatrixState3D::cz;
void MatrixState3D::setInitStack(float *currMatrix) {
    Matrix::setIdentityM(currMatrix, 0);
}


void MatrixState3D::translate(float *currMatrix, float x, float y, float z) {
    Matrix::translateM(currMatrix, 0, x, y, z);
}

void MatrixState3D::rotate(float *currMatrix, float angle, float x, float y, float z) {
    Matrix::rotateM(currMatrix, 0, angle, x, y, z);
}

void MatrixState3D::scale(float *currMatrix, float x, float y, float z) {
    Matrix::scaleM(currMatrix, 0, x, y, z);
}

void MatrixState3D::setCamera(float cx, float cy, float cz, float tx, float ty, float tz, float upx, float upy, float upz) {
    MatrixState3D::cx = cx;
    MatrixState3D::cy = cy;
    MatrixState3D::cz = cz;
    Matrix::setLookAtM(mVMatrix, 0, cx, cy, cz, tx, ty, tz, upx, upy, upz);
}

void MatrixState3D::setProjectFrustum(float left, float right, float bottom, float top, float near, float far) {
    Matrix::frustumM(mProjMatrix, 0, left, right, bottom, top, near, far);
}

void MatrixState3D::getFinalMatrix(float *mMVPMatrix, float *currMatrix) {
    Matrix::multiplyMM(mMVPMatrix, 0, mVMatrix, 0, currMatrix, 0);
    Matrix::multiplyMM(mMVPMatrix, 0, mProjMatrix, 0, mMVPMatrix, 0);
    Matrix::multiplyMM(mMVPMatrix, 0, vulkanClipMatrix, 0, mMVPMatrix, 0);
}
