#include "softgl.h"
#include "../geometry/geometry.h"

BEGIN_NAMESPACE(SoftRender)

mat4x4 g_model_mat;
mat4x4 g_view_mat;
mat4x4 g_projection_mat;

void get_model_matrix(vec3f position, vec3f rotation, vec3f scale)
{
    // translate
    mat4x4 translate_mat = mat4x4::identity();
    translate_mat[0][3] = position.x;
    translate_mat[1][3] = position.y;
    translate_mat[2][3] = position.z;

    // rotate
    float angle;
    mat4x4 x_rotate_mat = mat4x4::identity();
    angle = rotation.x * M_PI / 180.0f;
    x_rotate_mat[1][1] = std::cos(angle);
    x_rotate_mat[1][2] = -std::sin(angle);
    x_rotate_mat[2][1] = std::sin(angle);
    x_rotate_mat[2][2] = std::cos(angle);
    mat4x4 y_rotate_mat = mat4x4::identity();
    angle = rotation.y * M_PI / 180.0f;
    y_rotate_mat[0][0] = std::cos(angle);
    y_rotate_mat[0][2] = std::sin(angle);
    y_rotate_mat[2][0] = -std::sin(angle);
    y_rotate_mat[2][2] = std::cos(angle);
    mat4x4 z_rotate_mat = mat4x4::identity();
    angle = rotation.z * M_PI / 180.0f;
    z_rotate_mat[0][0] = std::cos(angle);
    z_rotate_mat[0][2] = -std::sin(angle);
    z_rotate_mat[2][0] = std::sin(angle);
    z_rotate_mat[2][2] = std::cos(angle);

    // scale
    mat4x4 scale_mat = mat4x4::identity();
    scale_mat[0][0] = scale.x;
    scale_mat[1][1] = scale.y;
    scale_mat[2][2] = scale.z;

    g_model_mat = translate_mat * x_rotate_mat * y_rotate_mat * z_rotate_mat * scale_mat;
}

void get_view_matrix(vec3f eye, vec3f center, vec3f up)
{
    vec3f z = (eye - center).normalize();
    vec3f x = cross(up, z).normalize();
    vec3f y = cross(z, x).normalize();

    mat4x4 rot = mat4x4::identity();
    mat4x4 trans = mat4x4::identity();
    for(size_t i = 0; i < 3; i++)
    {
        rot[0][i] = x[i];
        rot[1][i] = y[i];
        rot[2][i] = z[i];
        trans[i][3] = -center[i];
    }

    g_view_mat = rot * trans;
}

void get_projection_matrix(float fov, float aspect, float zNear, float zFar)
{
    fov = fov * M_PI / 180.f;
    float fax = 1 / std::tan( fov / 2.f);
    mat4x4 projection_mat = mat4x4::identity();
    projection_mat[0][0] = fax / aspect;
    projection_mat[1][1] = fax;
    projection_mat[2][2] = -(zNear + zFar) / (zFar - zNear);
    projection_mat[2][3] = -2.f * zNear * zFar / (zFar - zNear);
    projection_mat[3][2] = -1.f;

    g_projection_mat = projection_mat;
}

END_NAMESPACE(SoftRender)