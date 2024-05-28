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
        if (!load_rle_data(in))
        {
            LOGE("an error occured while reading the data");
            in.close();
            delete [] data;
            return false;
        }
    }
    else // unknown file format
    {
        LOGE("Unknown file format %d", header.datatypecode);
        in.close();
        delete [] data;
        return false;
    }

    if (!(header.imagedescriptor & 0x20))
    {
        flip_vertically();
    }
    if (header.imagedescriptor & 0x10)
    {
        flip_horizontally();
    }
    LOGI("%d x %d / %d", width, height, bytespp * 8);
    in.close();
    return true;
}

bool TGAImage::flip_vertically()
{
    if (nullptr == data)
        return false;

    unsigned long bytes_per_line = width * bytespp;
    unsigned char * line = new unsigned char[bytes_per_line];
    int half = height >> 1;
    for(int i = 0; i < half; i++)
    {
        unsigned char * l1 = data + i * bytes_per_line;
        unsigned char * l2 = data + (height - 1 - i) * bytes_per_line;
        memmove((void *)line, (void *)l1, bytes_per_line);
        memmove((void *)l1, (void *)l2, bytes_per_line);
        memmove((void *)l2, (void *)line, bytes_per_line);
    }
    delete [] line;
    return true;
}

bool TGAImage::flip_horizontally()
{
    if (nullptr == data)
        return false;

    int half = width >> 1;
    for(int i = 0; i < half; i++)
    {
        for(int j = 0; j < height; j++)
        {
            TGAColor c1 = get(i, j);
            TGAColor c2 = get(width - 1 - i, j);
            set(i, j, c2);
            set(width - 1 - i, j, c1);
        }
    }
    return true;
}

TGAColor TGAImage::get(int x, int y) const
{
    if (!data || x < 0 || y < 0 || x >= width || y >= height)
    {
        return TGAColor();
    }
    return TGAColor(data + (x + y * width) * bytespp, bytespp);
}

bool TGAImage::set(int x, int y, const TGAColor& c)
{
    if (!data || x < 0 || y < 0 || x >= width || y >= height)
    {
        return false;
    }
    memcpy((void*)(data + (x + y * width) * bytespp), (void *)c.bgra, bytespp);
    return true;
}

bool TGAImage::load_rle_data(std::ifstream & in)
{
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
            for(int i = 0; i < chunkheader; i++)
            {
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
    }
    while (currentpixel < pixelcount);
    return true;
}

END_NAMESPACE(SoftRender)