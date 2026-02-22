#ifndef __FileUtil_H__
#define __FileUtil_H__


#include <string>
#include "TexDataObject.h"

using namespace std;

class FileUtil
{
public:
    static string loadAssetStr(string fname);
    static TexDataObject* loadCommonTexData(string fname);
    static TexDataObject* load_DXT5_BC3_TexData(string fname);
};

#endif
