#pragma once
typedef enum TexTypeENUM {
    COMMON = 0,//普通类型的纹理
    RGBA8_BC3_DXT5 = 1,//压缩纹理
} BNTexType;

class TexDataObject {
public:
    int width;
    int height;
    int dataByteCount;
    unsigned char *data;
    BNTexType type;

    TexDataObject(int width, int height, unsigned char *data, int dataByteCount, BNTexType type);
    ~TexDataObject();
};
