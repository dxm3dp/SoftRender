#ifndef _SHADER_H_
#define _SHADER_H_

#include "../pipeline/softgl.h"
#include "../model/model.h"

BEGIN_NAMESPACE(SoftRender)

struct Shader : public IShader
{
    virtual vec4f vert(Model& model, int iface, int nthvert);
    virtual void frag(vec3f bc);
};

END_NAMESPACE(SoftRender)

#endif // _SHADER_H_
