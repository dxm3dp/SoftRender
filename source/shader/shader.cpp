#include "shader.h"

BEGIN_NAMESPACE(SoftRender)

vec4f Shader::vert(Model &model, int iface, int nthvert)
{
    vec4f gl_vertex = g_projection_mat * g_view_mat * g_model_mat * embed<4>(model.vert(iface, nthvert));
    return gl_vertex;
}

void Shader::frag(vec3f bc)
{
    // TODO 实现片元着色器
}

END_NAMESPACE(SoftRender)
