#include "CircleData.h"
#include <cmath>
#include <vector>

const double PI = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170680;

float *CircleData::vdata;
int CircleData::dataByteCount;
int CircleData::vCount;


uint16_t *CircleData::idata;
int CircleData::indexByteCount;
int CircleData::iCount;


double CircleData::toRadians(double d) {
    double e = d * PI / 180;
    return e;
}
void CircleData::genVertexData() {
    int n = 10;//切割的份数
    vCount = n + 2;//顶点数量
    dataByteCount = vCount * 6 * sizeof(float);//顶点数据所占总字节数
    vdata = new float[vCount * 6];//顶点数据数组
    float angdegSpan = 360.0f / n;//每份度数
    int count = 0;//辅助索引
    vdata[count++] = 0;
    vdata[count++] = 0;
    vdata[count++] = 0;//第一个顶点的坐标
    vdata[count++] = 1;
    vdata[count++] = 1;
    vdata[count++] = 1;//第一个顶点的颜色
    for (float angdeg = 0; ceil(angdeg) <= 360; angdeg += angdegSpan) {//循环生成周围其他顶点的坐标
        double angrad = toRadians(angdeg);//当前弧度
        vdata[count++] = (float) (-30 * sin(angrad));//顶点x 坐标
        vdata[count++] = (float) (30 * cos(angrad));//顶点y 坐标
        vdata[count++] = 0;//顶点z 坐标
        vdata[count++] = 0;
        vdata[count++] = 1;
        vdata[count++] = 0;//顶点颜色
    }
    iCount = 12;//索引数量
    indexByteCount = iCount * sizeof(uint16_t);//索引数据所占总字节数
    idata = new uint16_t[iCount]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1};//索引数据数组
}