#ifndef _SHADER_H_
#define _SHADER_H_

#include "../pipeline/softgl.h"
#include "../model/model.h"

BEGIN_NAMESPACE(SoftRender)

struct Shader : public IShader
{
    Shader(mat4x4 M, mat4x4 MIT) : uniform_M(M), uniform_MIT(MIT) { }

    virtual vec4f vert(Model *model, int iface, int nthvert);
    virtual void frag(Model *model, vec3f bc, TGAColor &color);

    mat<2, 3, float> varying_uv; // triangle uv coordinate, written by vertex shader, read by fragment shader.
    mat4x4 uniform_M;
    mat4x4 uniform_MIT;
};

END_NAMESPACE(SoftRender)

#endif // _SHADER_H_
