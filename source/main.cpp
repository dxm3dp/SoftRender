#include "common/cpplang.h"
#include "common/Logger.h"
#include "geometry/geometry.h"
#include "model/model.h"
#include "model/tgaimage.h"
#include "shader/shader.h"

using namespace SoftRender;

// 1.视口变换的应用时机问题。

int main(int argc, char** argv)
{
    std::cout << "Hello, World!" << std::endl;

    std::string filePath = "./models/diablo3/diablo3_pose.obj";
    SoftRender::Model model(filePath.c_str());

    Shader shader;
    std::vector<vec4f> clip_pos(3);
    for(int i = 0; i < model.nfaces(); i++)
    {
        for(int j = 0; j < 3; j++)
        {
            clip_pos[j] = shader.vert(model, i, j);
        }
        triangle_rasterization(clip_pos);
    }

    return 0;
}