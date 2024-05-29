#ifndef _SOFT_GL_H_
#define _SOFT_GL_H_

#include "../common/cpplang.h"
#include "../geometry/geometry.h"

BEGIN_NAMESPACE(SoftRender)

extern mat4x4 g_model_mat;
extern mat4x4 g_view_mat;
extern mat4x4 g_projection_mat;

void get_model_matrix(vec3f position, vec3f rotation, vec3f scale);
void get_view_matrix(vec3f eye, vec3f center, vec3f up);
void get_projection_matrix(float fov, float aspect, float zNear, float zFar);

END_NAMESPACE(SoftRender)

#endif // _SOFT_GL_H_