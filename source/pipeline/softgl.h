#ifndef _SOFT_GL_H_
#define _SOFT_GL_H_

#include "../common/cpplang.h"
#include "../geometry/geometry.h"
#include "../model/model.h"

BEGIN_NAMESPACE(SoftRender)

extern mat4x4 g_model_mat;
extern mat4x4 g_view_mat;
extern mat4x4 g_projection_mat;
extern mat4x4 g_viewport_mat;

const float depth = 2000.f;

void get_model_matrix(vec3f position, vec3f rotation, vec3f scale);
void get_view_matrix(vec3f eye, vec3f center, vec3f up);
void get_projection_matrix(float fov, float aspect, float zNear, float zFar);
void get_projection_matrix(float coeff = 0.f);
void get_viewport_matrix(int x, int y, int width, int height);

struct IShader
{
    virtual vec4f vert(Model &model, int iface, int nthvert) = 0;
    virtual void frag(vec3f bc, TGAColor &color) = 0;
};

void triangle_rasterization(std::vector<vec4f> screenPos, TGAImage &framebuffer, float *zbuffer, IShader &shader);

vec3f barycentric(vec2f A, vec2f B, vec2f C, vec2f P);
vec3f barycentricV2(vec2f A, vec2f B, vec2f C, vec2f P);

END_NAMESPACE(SoftRender)

#endif // _SOFT_GL_H_