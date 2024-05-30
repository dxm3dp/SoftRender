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

    // TODO 光栅化处理

    return 0;
}