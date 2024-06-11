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
    vec2f uv = varying_uv * bc;
    TGAColor diffuse = model->diffuse(uv);
    vec3f n = proj<3>(uniform_MIT * embed<4>(model->normal(uv))).normalize();
    vec3f light(1, 1, 0);
    light = proj<3>(uniform_M * embed<4>(light)).normalize();
    vec3f r = (n * ( n * light * 2.f) - light).normalize();
    float spec = pow(std::max(r.z, 0.f), model->specular(uv));
    float diff = std::max(0.f, n * light);
    for(int i = 0; i < 3; i++)
    {
        color[i] = std::min<float>(20 + diffuse[i] * (1.2 * diff + 0.6 * spec) , 255);
    }
}

END_NAMESPACE(SoftRender)
