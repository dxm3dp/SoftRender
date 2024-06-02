#include "shader.h"

BEGIN_NAMESPACE(SoftRender)

vec4f Shader::vert(Model &model, int iface, int nthvert)
{
    vec4f gl_vertex = g_viewport_mat * g_projection_mat * g_view_mat * embed<4>(model.vert(iface, nthvert));
    //vec4f gl_vertex = g_viewport_mat * g_projection_mat * g_view_mat * g_model_mat * embed<4>(model.vert(iface, nthvert));
    return gl_vertex;
}

void Shader::frag(vec3f bc, TGAColor &color)
{
    // 待用纯色完成图像绘制后，再回来实现这部分。
    // 1. 通过重心坐标计算当前片元的uv坐标，用于纹理采样。
    // 2. 对diffuse、nomral贴图进行采样。
    // 3. 使用光照模型，计算光照。

    color[0] = 230;
    color[1] = 230;
    color[2] = 2;
    color[3] = 2;
}

END_NAMESPACE(SoftRender)
