#include "LightManager.h"
#include <math.h>
float LightManager::lx = 0;
float LightManager::ly = 0;
float LightManager::lz = 0;
float LightManager::lightAmbientR = 0;
float LightManager::lightAmbientG = 0;
float LightManager::lightAmbientB = 0;
float LightManager::lightAmbientA = 0;
float LightManager::lightDiffuseR = 0;
float LightManager::lightDiffuseG = 0;
float LightManager::lightDiffuseB = 0;
float LightManager::lightDiffuseA = 0;
float LightManager::lightSpecularR = 0;
float LightManager::lightSpecularG = 0;
float LightManager::lightSpecularB = 0;
float LightManager::lightSpecularA = 0;
float LightManager::lightFWJ = 0;
void LightManager::setLightPosition(float lxIn, float lyIn, float lzIn) {
    lx = lxIn;
    ly = lyIn;
    lz = lzIn;
}
void LightManager::setLightAmbient(float lightAmbientRIn, float lightAmbientGIn, float lightAmbientBIn, float lightAmbientAIn) {
    lightAmbientR = lightAmbientRIn;
    lightAmbientG = lightAmbientGIn;
    lightAmbientB = lightAmbientBIn;
    lightAmbientA = lightAmbientAIn;
}
void LightManager::setLightDiffuse(float lightDiffuseRIn, float lightDiffuseGIn, float lightDiffuseBIn, float lightDiffuseAIn) {
    lightDiffuseR = lightDiffuseRIn;
    lightDiffuseG = lightDiffuseGIn;
    lightDiffuseB = lightDiffuseBIn;
    lightDiffuseA = lightDiffuseAIn;
}
void LightManager::setLightSpecular(float lightSpecularRIn, float lightSpecularGIn, float lightSpecularBIn, float lightSpecularAIn) {
    lightSpecularR = lightSpecularRIn;
    lightSpecularG = lightSpecularGIn;
    lightSpecularB = lightSpecularBIn;
    lightSpecularA = lightSpecularAIn;
}
void LightManager::move() {
    lightFWJ = lightFWJ + 0.5;
    float tempLx = sin(lightFWJ / 180 * 3.14150265) * 150;
    float tempLz = cos(lightFWJ / 180 * 3.14150265) * 150;
    LightManager::setLightPosition(tempLx, 200, tempLz);
    if (lightFWJ >= 360) {
        lightFWJ = 0;
    }
}