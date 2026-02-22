#include "Square3D.h"
#include "LoadUtil.h"
#include "MyVulkanManager.h"

ObjObject* Square3D::create(string objname, string texturename)
{
    ObjObject* tempOO = LoadUtil::loadFromFile(objname, MyVulkanManager::device, MyVulkanManager::memoryroperties);
    tempOO->texturename = texturename;
    return tempOO;
}
