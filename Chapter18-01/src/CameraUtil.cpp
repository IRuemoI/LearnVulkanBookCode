#include "CameraUtil.h"
#include "MatrixState3D.h"
#include <math.h>
float CameraUtil::degree = 0;
float CameraUtil::yj = 30;
float CameraUtil::camera9Para[9];
float CameraUtil::tx = 0;
float CameraUtil::ty = 0;
float CameraUtil::tz = 0;
void CameraUtil::calCamera() {
    float cy = float(sin(yj * 3.1415926535898 / 180) * CAMERA_R + ty);
    float cxz = float(cos(yj * 3.1415926535898 / 180) * CAMERA_R);
    float cx = float(sin(degree * 3.1415926535898 / 180) * cxz + tx);
    float cz = float(cos(degree * 3.1415926535898 / 180) * cxz + tz);
    float upY = float(cos(yj * 3.1415926535898 / 180));
    float upXZ = float(sin(yj * 3.1415926535898 / 180));
    float upX = float(-upXZ * sin(degree * 3.1415926535898 / 180));
    float upZ = float(-upXZ * cos(degree * 3.1415926535898 / 180));
    camera9Para[0] = cx;
    camera9Para[1] = cy;
    camera9Para[2] = cz;
    camera9Para[3] = tx;
    camera9Para[4] = ty;
    camera9Para[5] = tz;
    camera9Para[6] = upX;
    camera9Para[7] = upY;
    camera9Para[8] = upZ;
}
void CameraUtil::calCamera(float yjSpan, float cxSpan) {
    yj = yj + yjSpan;
    if (yj > 90) {
        yj = 90;
    }
    if (yj < 0) {
        yj = 0;
    }
    degree = degree + cxSpan;
    if (degree >= 360) {
        degree = degree - 360;
    } else if (degree <= 0) {
        degree = degree + 360;
    }
    calCamera();
}
void CameraUtil::flushCameraToMatrix() {
    MatrixState3D::setCamera(
            camera9Para[0], camera9Para[1], camera9Para[2],
            camera9Para[3], camera9Para[4], camera9Para[5],
            camera9Para[6], camera9Para[7], camera9Para[8]);
}