#include "LoadUtil.h"
#include "FileUtil.h"
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vulkan/vulkan.h>
using namespace std;

size_t splitString(const string& strSrc, const string& strDelims, vector<string>& strDest)
{
    string delims = strDelims;
    string STR;
    if (delims.empty()) delims = " **";
    string::size_type pos = 0;
    string::size_type LEN = strSrc.size();
    while (pos < LEN)
    {
        STR = "";
        while ((delims.find(strSrc[pos]) != std::string::npos) && (pos < LEN))
        {
            ++pos;
        }
        if (pos == LEN)
        {
            return strDest.size();
        }
        while ((delims.find(strSrc[pos]) == std::string::npos) && (pos < LEN))
        {
            STR += strSrc[pos++];
        }
        if (!STR.empty())
        {
            strDest.push_back(STR);
        }
    }
    return strDest.size();
}

bool tryParseDouble(const char* s, const char* s_end, double* result)
{
    if (s >= s_end)
    {
        return false;
    }
    double mantissa = 0.0;
    int exponent = 0;
    char sign = '+';
    char exp_sign = '+';
    char const* curr = s;
    int read = 0;
    bool end_not_reached = false;
    if (*curr == '+' || *curr == '-')
    {
        sign = *curr;
        curr++;
    }
    else if (isdigit(*curr))
    {
        /* Pass through. */
    }
    else
    {
        goto fail;
    }
    while ((end_not_reached = (curr != s_end)) && isdigit(*curr))
    {
        mantissa *= 10;
        mantissa += static_cast<int>(*curr - 0x30);
        curr++;
        read++;
    }
    if (read == 0)
        goto fail;
    if (!end_not_reached)
        goto assemble;
    if (*curr == '.')
    {
        curr++;
        read = 1;
        while ((end_not_reached = (curr != s_end)) && isdigit(*curr))
        {
            mantissa += static_cast<int>(*curr - 0x30) * pow(10.0, -read);
            read++;
            curr++;
        }
    }
    else if (*curr == 'e' || *curr == 'E')
    {
    }
    else
    {
        goto assemble;
    }
    if (!end_not_reached)
        goto assemble;
    if (*curr == 'e' || *curr == 'E')
    {
        curr++;
        if ((end_not_reached = (curr != s_end)) && (*curr == '+' || *curr == '-'))
        {
            exp_sign = *curr;
            curr++;
        }
        else if (isdigit(*curr))
        {
            /* Pass through. */
        }
        else
        {
            goto fail;
        }
        read = 0;
        while ((end_not_reached = (curr != s_end)) && isdigit(*curr))
        {
            exponent *= 10;
            exponent += static_cast<int>(*curr - 0x30);
            curr++;
            read++;
        }
        exponent *= (exp_sign == '+' ? 1 : -1);
        if (read == 0)
            goto fail;
    }
assemble:
    *result = (sign == '+' ? 1 : -1) * ldexp(mantissa * pow(5.0, exponent), exponent);
    return true;
fail:
    return false;
}

float parseFloat(const char* token)
{
    token += strspn(token, " \t");
#ifdef TINY_OBJ_LOADER_OLD_FLOAT_PARSER
    float f = atof(token);
#else
    const char* end = token + strcspn(token, " \t\r");
    double val = 0.0;
    tryParseDouble(token, end, &val);
    float f = static_cast<float>(val);
#endif
    return f;
}

int parseInt(const char* token)
{
    token += strspn(token, " \t");
    int i = atoi(token);
    return i;
}

ObjObject* LoadUtil::loadFromFile(const string& vname, VkDevice& device,
                                  VkPhysicalDeviceMemoryProperties& memoryroperties)
{
    printf("[LoadUtil: 开始加载文件 %s]\n", vname.c_str());
    ObjObject* lo;
    vector<float> alv;
    vector<float> alvResult;
    vector<float> alt;
    vector<float> altResult;
    vector<float> aln;
    vector<float> alnResult;
    std::string resultStr = FileUtil::loadAssetStr(vname);
    printf("[LoadUtil: 文件读取完成，大小 %zu 字符]\n", resultStr.length());
    vector<string> lines;
    splitString(resultStr, "\n", lines);
    vector<string> splitStrs;
    vector<string> splitStrsF;
    string tempContents;
    for (int i = 0; i < lines.size(); i++)
    {
        tempContents = lines[i];
        if (tempContents.compare("") == 0)
        {
            continue;
        }
        // 跳过注释行（以#开头）
        if (tempContents.length() > 0 && tempContents[0] == '#')
        {
            continue;
        }
        string delims = "[ ]+";
        splitStrs.clear();
        splitString(tempContents, delims, splitStrs);

        // 每100行输出一次进度
        if (i % 100 == 0)
        {
            printf("[LoadUtil: 处理第%d行，共%zu行]\n", i, lines.size());
        }

        if (splitStrs[0] == "v")
        {
            alv.push_back(parseFloat(splitStrs[1].c_str()));
            alv.push_back(parseFloat(splitStrs[2].c_str()));
            alv.push_back(parseFloat(splitStrs[3].c_str()));
        }
        else if (splitStrs[0] == "vt")
        {
            alt.push_back(parseFloat(splitStrs[1].c_str()));
            alt.push_back(1 - parseFloat(splitStrs[2].c_str()));
        }
        else if (splitStrs[0] == "vn")
        {
            aln.push_back(parseFloat(splitStrs[1].c_str()));
            aln.push_back(parseFloat(splitStrs[2].c_str()));
            aln.push_back(parseFloat(splitStrs[3].c_str()));
        }
        else if (splitStrs[0] == "f")
        {
            // 每10个face输出一次进度
            static int faceCount = 0;
            faceCount++;
            if (faceCount % 10 == 0)
            {
                printf("[LoadUtil: 处理第%d个face]\n", faceCount);
            }

            // 确保有足够的顶点数据（至少4个部分：f + 3个顶点）
            if (splitStrs.size() < 4)
            {
                printf("[LoadUtil: face数据不足4个部分，跳过]\n");
                continue;
            }

            int index[3];
            string delimsF = "/";

            // 处理第一个顶点
            splitStrsF.clear();
            splitString(splitStrs[1].c_str(), delimsF, splitStrsF);
            if (splitStrsF.size() < 3)
            {
                printf("[LoadUtil: 第一个顶点数据不足3个部分，跳过]\n");
                continue;
            }
            index[0] = parseInt(splitStrsF[0].c_str()) - 1;
            if (index[0] * 3 + 2 >= alv.size())
            {
                printf("[LoadUtil: 顶点索引越界 %d >= %d，跳过]\n", index[0], (int)alv.size() / 3);
                continue;
            }
            alvResult.push_back(alv[3 * index[0]]);
            alvResult.push_back(alv[3 * index[0] + 1]);
            alvResult.push_back(alv[3 * index[0] + 2]);

            int indexTex = parseInt(splitStrsF[1].c_str()) - 1;
            if (indexTex * 2 + 1 >= alt.size())
            {
                printf("[LoadUtil: 纹理索引越界 %d >= %d，跳过]\n", indexTex, (int)alt.size() / 2);
                continue;
            }
            altResult.push_back(alt[indexTex * 2]);
            altResult.push_back(alt[indexTex * 2 + 1]);

            int indexN = parseInt(splitStrsF[2].c_str()) - 1;
            if (indexN * 3 + 2 >= aln.size())
            {
                printf("[LoadUtil: 法向量索引越界 %d >= %d，跳过]\n", indexN, (int)aln.size() / 3);
                continue;
            }
            alnResult.push_back(aln[3 * indexN]);
            alnResult.push_back(aln[3 * indexN + 1]);
            alnResult.push_back(aln[3 * indexN + 2]);

            // 处理第二个顶点
            splitStrsF.clear();
            splitString(splitStrs[2].c_str(), delimsF, splitStrsF);
            if (splitStrsF.size() < 3)
            {
                printf("[LoadUtil: 第二个顶点数据不足3个部分，跳过]\n");
                continue;
            }
            index[1] = parseInt(splitStrsF[0].c_str()) - 1;
            if (index[1] * 3 + 2 >= alv.size())
            {
                printf("[LoadUtil: 顶点索引越界 %d >= %d，跳过]\n", index[1], (int)alv.size() / 3);
                continue;
            }
            alvResult.push_back(alv[3 * index[1]]);
            alvResult.push_back(alv[3 * index[1] + 1]);
            alvResult.push_back(alv[3 * index[1] + 2]);

            indexTex = parseInt(splitStrsF[1].c_str()) - 1;
            if (indexTex * 2 + 1 >= alt.size())
            {
                printf("[LoadUtil: 纹理索引越界 %d >= %d，跳过]\n", indexTex, (int)alt.size() / 2);
                continue;
            }
            altResult.push_back(alt[indexTex * 2]);
            altResult.push_back(alt[indexTex * 2 + 1]);

            indexN = parseInt(splitStrsF[2].c_str()) - 1;
            if (indexN * 3 + 2 >= aln.size())
            {
                printf("[LoadUtil: 法向量索引越界 %d >= %d，跳过]\n", indexN, (int)aln.size() / 3);
                continue;
            }
            alnResult.push_back(aln[3 * indexN]);
            alnResult.push_back(aln[3 * indexN + 1]);
            alnResult.push_back(aln[3 * indexN + 2]);

            // 处理第三个顶点
            splitStrsF.clear();
            splitString(splitStrs[3].c_str(), delimsF, splitStrsF);
            if (splitStrsF.size() < 3)
            {
                printf("[LoadUtil: 第三个顶点数据不足3个部分，跳过]\n");
                continue;
            }
            index[2] = parseInt(splitStrsF[0].c_str()) - 1;
            if (index[2] * 3 + 2 >= alv.size())
            {
                printf("[LoadUtil: 顶点索引越界 %d >= %d，跳过]\n", index[2], (int)alv.size() / 3);
                continue;
            }
            alvResult.push_back(alv[3 * index[2]]);
            alvResult.push_back(alv[3 * index[2] + 1]);
            alvResult.push_back(alv[3 * index[2] + 2]);

            indexTex = parseInt(splitStrsF[1].c_str()) - 1;
            if (indexTex * 2 + 1 >= alt.size())
            {
                printf("[LoadUtil: 纹理索引越界 %d >= %d，跳过]\n", indexTex, (int)alt.size() / 2);
                continue;
            }
            altResult.push_back(alt[indexTex * 2]);
            altResult.push_back(alt[indexTex * 2 + 1]);

            indexN = parseInt(splitStrsF[2].c_str()) - 1;
            if (indexN * 3 + 2 >= aln.size())
            {
                printf("[LoadUtil: 法向量索引越界 %d >= %d，跳过]\n", indexN, (int)aln.size() / 3);
                continue;
            }
            alnResult.push_back(aln[3 * indexN]);
            alnResult.push_back(aln[3 * indexN + 1]);
            alnResult.push_back(aln[3 * indexN + 2]);
        }
        splitStrs.clear();
    }
    printf("[LoadUtil: 数据解析完成，顶点数=%zu，纹理数=%zu，法向量数=%zu\n", alvResult.size() / 3, altResult.size() / 2,
           alnResult.size() / 3);
    int vCount = (int)alvResult.size() / 3;
    int dataByteCount = vCount * 8 * sizeof(float);
    float* vdataIn = new float[vCount * 8];
    int indexTemp = 0;
    for (int i = 0; i < vCount; i++)
    {
        vdataIn[indexTemp++] = alvResult[i * 3 + 0];
        vdataIn[indexTemp++] = alvResult[i * 3 + 1];
        vdataIn[indexTemp++] = alvResult[i * 3 + 2];
        vdataIn[indexTemp++] = altResult[i * 2 + 0];
        vdataIn[indexTemp++] = altResult[i * 2 + 1];
        vdataIn[indexTemp++] = alnResult[i * 3 + 0];
        vdataIn[indexTemp++] = alnResult[i * 3 + 1];
        vdataIn[indexTemp++] = alnResult[i * 3 + 2];
    }
    printf("[LoadUtil: 创建ObjObject，顶点数=%d\n", vCount);
    lo = new ObjObject(vdataIn, dataByteCount, vCount, device, memoryroperties);
    printf("[LoadUtil: loadFromFile完成 %s]\n", vname.c_str());
    return lo;
}
