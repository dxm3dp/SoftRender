#include "tgaimage.h"
#include <cstring>
#include "../common/Logger.h"

BEGIN_NAMESPACE(SoftRender)

TGAImage::TGAImage() : data(nullptr), width(0), height(0), bytespp(0) {}

TGAImage::TGAImage(int w, int h, int bpp) : data(nullptr), width(w), height(h), bytespp(bpp)
{
    int nbytes = width * height * bytespp;
    data = new unsigned char[nbytes];
    memset(data, 0, nbytes);
}

TGAImage::TGAImage(const TGAImage & image) : data(nullptr), width(image.width), height(image.height), bytespp(image.bytespp)
{
    int nbytes = width * height * bytespp;
    data = new unsigned char[nbytes];
    memcpy(data, image.data, nbytes);
}

TGAImage::~TGAImage()
{
    if (data)
    {
        delete[] data;
    }
}

TGAImage& TGAImage::operator=(const TGAImage & image)
{
    if (this != &image)
    {
        if (data)
        {
            delete[] data;
        }
        width = image.width;
        height = image.height;
        bytespp = image.bytespp;
        int nbytes = width * height * bytespp;
        data = new unsigned char[nbytes];
        memcpy(data, image.data, nbytes);
    }
    return *this;
}

bool TGAImage::read_tga_file(const char * filename)
{
    if (data)
    {
        delete[] data;
    }

    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open())
    {
        LOGE("Cannot open file: %s", filename);
        in.close();
        return false;
    }

    // 读取头部信息

    TGA_Header header;
    in.read((char *)&header, sizeof(header));
    if (!in.good())
    {
        LOGE("An error occured while reading the header");
        in.close();
        return false;
    }
    width = header.width;
    height = header.height;
    bytespp = header.bitsperpixel >> 3;
    if (width <= 0 || height <= 0 || (bytespp != GRAYSCALE && bytespp != RGB && bytespp != RGBA))
    {
        LOGE("Bad width or height or bytespp");
        in.close();
        return false;
    }
    // 读取数据
    int nbytes = width * height * bytespp;
    data = new unsigned char[nbytes];
    if ( 2 == header.datatypecode || 3 == header.datatypecode)      // no compression
    {
        in.read((char *)data, nbytes);
        if (!in.good())
        {
            LOGE("An error occured while reading the data");
            in.close();
            delete [] data;
            return false;
        }
    }
    else if (10 == header.datatypecode || 11 == header.datatypecode)// RLE compression
    {
        // 加载 rle 数据
    }
    else
    {
        LOGE("Unknown file format %d", header.datatypecode);
        in.close();
        return false;
    }

    return true;
}

bool TGAImage::load_rle_data(std::ifstream & in)
{
    // TODO: 实现 RLE 数据加载
    unsigned long pixelcount = width * height;
    unsigned long currentpixel = 0;
    unsigned long currentbyte = 0;
    TGAColor colorbuffer;
    do
    {
        unsigned char chunkheader = 0;
        chunkheader = in.get();
        if (!in.good())
        {
            LOGE("an error occured while reading the data");
            return false;
        }
        if (chunkheader < 128)
        {
            chunkheader++;
            for (int i = 0; i < chunkheader; i++)
            {
                in.read((char *)colorbuffer.bgra, bytespp);
                if (!in.good())
                {
                    LOGE("an error occured while reading the header");
                    return false;
                }
                for(int t = 0; t < bytespp; t++)
                {
                    data[currentbyte++] = colorbuffer.bgra[t];
                }
                currentpixel++;
                if (currentpixel > pixelcount)
                {
                    LOGE("Too many pixels read");
                    return false;
                }
            }
        }
        else
        {
            chunkheader -= 127;
            in.read((char*)colorbuffer.bgra, bytespp);
            if (!in.good())
            {
                LOGE("an error occured while reading the header");
                return false;
            }

        }
    }
    while (currentpixel < pixelcount);
    return true;
}

END_NAMESPACE(SoftRender)