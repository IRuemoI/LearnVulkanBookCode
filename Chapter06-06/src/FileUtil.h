#pragma once
#include "TexDataObject.h"
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;
class FileUtil {
public:
    static string loadAssetStr(string filename);
    static TexDataObject *loadCommonTexData(const string &filename);
};
