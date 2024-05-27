#include "common/cpplang.h"
#include "common/Logger.h"
#include "geometry/geometry.h"
#include "model/model.h"
#include "model/tgaimage.h"


int main(int argc, char** argv)
{
    std::cout << "Hello, World!" << std::endl;

    std::string filePath = "./models/diablo3/diablo3_pose.obj";
    SoftRender::Model model(filePath.c_str());

    SoftRender::TGAImage image1(1280, 720, 4);
    SoftRender::TGAImage image2(image1);

    return 0;
}