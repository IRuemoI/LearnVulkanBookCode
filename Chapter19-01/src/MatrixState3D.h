#ifndef MatrixState_hpp
#define MatrixState_hpp


class MatrixState3D
{
public:
    static float currMatrix[16];
    static float mProjMatrix[16]; //Í¶Ó°¾ØÕó
    static float mVMatrix[16]; //ÉãÏñ»ú¾ØÕó
    static float mMVPMatrix[16]; //×Ü¾ØÕó
    static float vulkanClipMatrix[16]; //¼ô²Ã¿Õ¼ä¾ØÕó X²»±ä YÖÃ·´ Z¼õ°ë
    static float mStack[10][16]; //±£»¤±ä»»¾ØÕóµÄÕ»
    static int stackTop; //Õ»¶¥Î»ÖÃ

    static float cx, cy, cz; //ÉãÏñ»úÎ»ÖÃ

    static void setInitStack(); //³õÊ¼»¯¾ØÕó

    static void pushMatrix(); //±£»¤±ä»»¾ØÕó

    static void popMatrix(); //»Ö¸´±ä»»¾ØÕó

    static void translate(float x, float y, float z); //ÑØx¡¢y¡¢zÖáÆ½ÒÆ

    static void rotate(float angle, float x, float y, float z); //ÈÆÖ¸¶¨ÖáÐý×ª

    static void scale(float x, float y, float z); //¾ØÕóËõ·Å
    static void setCamera //ÉèÖÃÉãÏñ»ú
    (
        float cx,
        float cy,
        float cz,
        float tx,
        float ty,
        float tz,
        float upx,
        float upy,
        float upz
    );

    static void setProjectFrustum //ÉèÖÃÍ¸ÊÓÍ¶Ó°²ÎÊý
    (
        float left,
        float right,
        float bottom,
        float top,
        float near,
        float far
    );

    static float* getFinalMatrix(); //»ñÈ¡×îÖÕ¾ØÕó

    static float* getMMatrix(); //»ñÈ¡µ±Ç°±ä»»¾ØÕó
};


#endif
