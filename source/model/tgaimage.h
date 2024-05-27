#ifndef _TGA_IMAGE_H_
#define _TGA_IMAGE_H_

#include "../common/cpplang.h"

BEGIN_NAMESPACE(SoftRender)

#pragma pack(push, 1)
struct TGA_Header
{
    char idlength;          // ID 长度（1字节）
    char colormaptype;      // 调色板类型（1字节）
    char datatypecode;      // 数据类型码（1字节）
    short colormaporigin;   // 调色板起始索引（2字节，小端序）
    short colormaplength;   // 调色板长度（2字节，小端序）
    char colormapdepth;     // 调色板深度（1字节）
    short x_origin;         // X 起始坐标（2字节，小端序）
    short y_origin;         // Y 起始坐标（2字节，小端序）
    short width;            // 宽度（2字节，小端序）
    short height;           // 高度（2字节，小端序）
    char bitsperpixel;      // 每像素位数（1字节）
    char imagedescriptor;   // 图像描述符（1字节）
};
#pragma pack(pop)

// 小端序与大端序是两种不同的字节序。
// 小端序系统，多字节数据的最低有效字节存储在最低内存地址。多数现代个人计算机，包括x86和x86-64架构的PC，
// 以及ARM架构的设备都是小端序。
// 大端序系统，多字节数据的最高有效字节存储在最低内存地址。
// 在处理跨平台数据交换时（如文件读写、网络通信），需要特别注意字节序的问题，可能需要进行字节序的转换。

struct TGAColor
{
    TGAColor() : bgra{0, 0, 0, 0}, bytespp(1) {}
    TGAColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : bgra{b, g, r, a}, bytespp(4) {}
    TGAColor(unsigned char v) : bgra{v, 0, 0, 0}, bytespp(1) {}
    TGAColor(const unsigned char * p, unsigned char bpp) : bgra{0, 0, 0, 0}, bytespp(bpp)
    {
        for (int i = 0; i < (int)bpp; i++)
        {
            bgra[i] = p[i];
        }
        for (int i = (int)bpp; i < 4; i++)
        {
            bgra[i] = 0;
        }
    }

    unsigned char& operator[](const int i)
    {
        return bgra[i];
    }

    TGAColor operator*(float intensity) const
    {
        TGAColor res = *this;
        intensity = intensity > 1.0f ? 1.0f : (intensity < 0.0f ? 0.0f : intensity);
        for (int i = 0; i < 4; i++)
        {
            res.bgra[i] *= intensity;
        }
        return res;
    }

    unsigned char bgra[4];
    unsigned char bytespp;
};

class TGAImage
{
public:
    TGAImage();
    TGAImage(int w, int h, int bpp);
    TGAImage(const TGAImage& img);
    ~TGAImage();

private:
    unsigned char* data;
    int width;
    int height;
    int bytespp;
};

END_NAMESPACE(SoftRender)

#endif // _TGA_IMAGE_H_