#include "TriangleData.h"
#include <vector>

float *TriangleData::vdata;// 数据数组首地址指针
int TriangleData::dataByteCount;// 数据所占总字节数量
int TriangleData::vCount;// 顶点数量
void TriangleData::genVertexData() {// 顶点数据生成方法
    vCount = 3;// 顶点数量
    dataByteCount = vCount * 6 * sizeof(float);// 数据所占内存总字节数
    vdata = new float[vCount * 6]{// 数据数组
                                  0, 75, 0, 1, 0, 0,// 每一行前三个是顶点坐标
                                  -45, 0, 0, 0, 1, 0,// 每一行后三个是颜色RGB值
                                  45, 0, 0, 0, 0, 1};
}