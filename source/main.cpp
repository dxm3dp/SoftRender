#include "common/cpplang.h"
#include "common/Logger.h"
#include "geometry/geometry.h"
#include "model/model.h"
#include "model/tgaimage.h"
#include "shader/shader.h"

using namespace SoftRender;

const int width = 800;
const int height = 800;

Model *model = nullptr;
vec3f model_position(0.f, 0.f, -2.f);
vec3f model_rotation(0.f, 0.f, 0.f);
vec3f model_scale(1.f, 1.f, 1.f);
vec3f eye(0.f, 0.f, 4.f);
vec3f center(0.f, 0.f, 0.f);
vec3f up(0.f, 1.f, 0.f);
float fovy(60.f);
float aspect = (float)width / (float)height;
float near = 0.1f;
float far = 100.f;

int main(int argc, char** argv)
{
    std::string filePath = "F:/Work/GitHub/dxm3dp/SoftRender/models/diablo3/diablo3_pose.obj";
    //std::string filePath = "F:/Work/GitHub/dxm3dp/SoftRender/models/AfricanHead/african_head.obj";
    //std::string filePath = "F:/Work/GitHub/dxm3dp/SoftRender/models/triangle/sj.obj";
    //std::string filePath = "F:/Work/GitHub/dxm3dp/SoftRender/models/brickwall/brickwall.obj";
    model = new Model(filePath.c_str());

    set_model_matrix(model_position, model_rotation, model_scale);
    set_view_matrix(eye, center, up);
    set_projection_matrix(fovy, aspect, near, far);
    //set_projection_matrix(-1.f / (eye - center).norm());
    set_viewport_matrix(0, 0, width, height);

    TGAImage framebuffer{width, height, TGAImage::RGB};
    float *zbuffer = new float[width * height];
    for(int i = 0; i < width * height; i++)
    {
        zbuffer[i] = -std::numeric_limits<float>::max();
    }

    Shader shader{g_model_mat, g_model_mat.invert_transpose()};
    std::vector<vec4f> screen_pos(3);
    for(int i = 0; i < model->nfaces(); i++)
    {
        for(int j = 0; j < 3; j++)
        {
            screen_pos[j] = shader.vert(model, i, j);
        }
        triangle_rasterization(model, screen_pos, framebuffer, zbuffer, shader);
    }
    framebuffer.flip_vertically();
    framebuffer.write_tga_file("framebuffer.tga", false);

    delete model;
    delete[] zbuffer;

    return 0;
}