#include "shader.h"

BEGIN_NAMESPACE(SoftRender)

vec4f Shader::vert(Model &model, int iface, int nthvert)
{
    vec4f gl_vertex = g_projection_mat * g_view_mat * g_model_mat * embed<4>(model.vert(iface, nthvert));
    return gl_vertex;
}

END_NAMESPACE(SoftRender)
