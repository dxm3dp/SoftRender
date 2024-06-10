#include "shader.h"

BEGIN_NAMESPACE(SoftRender)

vec4f Shader::vert(Model *model, int iface, int nthvert)
{
    vec4f gl_vertex = g_viewport_mat * g_projection_mat * g_view_mat * g_model_mat * embed<4>(model->vert(iface, nthvert));
    varying_uv.set_col(nthvert, model->uv(iface, nthvert));
    return gl_vertex;
}

void Shader::frag(Model *model, vec3f bc, TGAColor &color)
{
    // 1. 通过重心坐标计算当前片元的uv坐标，用于纹理采样。
    // 2. 对diffuse、nomral贴图进行采样。
    // 3. 使用光照模型，计算光照。

    vec2f uv = varying_uv * bc;
    TGAColor diffuse = model->diffuse(uv);
    vec3f n = proj<3>(uniform_MIT * embed<4>(model->normal(uv))).normalize();
    vec3f l(1, 1, 0);
    l = proj<3>(uniform_M * embed<4>(l)).normalize();
    vec3f r = (n * ( n * l * 2.f) - l).normalize();
    float diff = std::max(0.f, n * l);
    for(int i = 0; i < 3; i++)
    {
        diffuse[i] = std::min<float>(20 + diffuse[i] * diff, 255);
    }
    color = diffuse;
}

END_NAMESPACE(SoftRender)
