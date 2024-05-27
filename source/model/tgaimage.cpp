#include "tgaimage.h"
#include <cstring>

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

END_NAMESPACE(SoftRender)