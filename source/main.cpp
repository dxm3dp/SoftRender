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

    // TODO 实现渲染管线中的空间变换部分

    return 0;
}