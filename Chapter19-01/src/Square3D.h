#ifndef Square3D_H
#define Square3D_H

#include <string>
#include "ObjObject.h"
#include "FileUtil.h"
#include "MyVulkanManager.h"
using namespace std;

class Square3D
{
public:
    static ObjObject* create(string objname, string texturename);
};

#endif
