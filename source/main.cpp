#include "common/cpplang.h"
#include "common/Logger.h"
#include "geometry/geometry.h"
#include "model/model.h"
#include "model/tgaimage.h"
#include "shader/shader.h"

using namespace SoftRender;

// 1.视口变换的应用时机问题。

const int width = 800;
const int height = 800;

vec3f eye(0, 1, 4);
vec3f center(0, 0, 0);
vec3f up(0, 1, 0);

int main(int argc, char** argv)
{
    //std::string filePath = "F:/Work/GitHub/dxm3dp/SoftRender/models/diablo3/diablo3_pose.obj";
    std::string filePath = "F:/Work/GitHub/dxm3dp/SoftRender/models/triangle/sj.obj";
    SoftRender::Model model(filePath.c_str());

    get_model_matrix(vec3f(0, 0, -1), vec3f(90, 0, 0), vec3f(1, 1, 1));
    get_view_matrix(eye, center, up);
    get_projection_matrix(60.f, 1.f, 0.1f, 100.f);
    //get_projection_matrix(-1.f / (eye - center).norm());
    get_viewport_matrix(width / 8, height / 8, width * 3 / 4, height * 3 / 4);

    TGAImage framebuffer{width, height, TGAImage::RGB};

    float *zbuffer = new float[width * height];
    for(int i = 0; i < width * height; i++)
    {
        zbuffer[i] = -std::numeric_limits<float>::max();
    }
    Shader shader;
    std::vector<vec4f> clip_pos(3);
    for(int i = 0; i < model.nfaces(); i++)
    {
        for(int j = 0; j < 3; j++)
        {
            clip_pos[j] = shader.vert(model, i, j);
        }
        triangle_rasterization(clip_pos, framebuffer, zbuffer, shader);
    }
    framebuffer.flip_vertically();
    framebuffer.write_tga_file("framebuffer.tga", false);

    delete[] zbuffer;

    return 0;
}