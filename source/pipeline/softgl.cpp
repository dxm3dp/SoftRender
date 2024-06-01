#include "softgl.h"
#include "../geometry/geometry.h"
#include <limits>

BEGIN_NAMESPACE(SoftRender)

mat4x4 g_model_mat;
mat4x4 g_view_mat;
mat4x4 g_projection_mat;
mat4x4 g_viewport_mat;

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

void get_viewport_matrix(int x, int y, int width, int height)
{
    mat4x4 viewport_mat = mat4x4::identity();
    viewport_mat[0][0] = width / 2;
    viewport_mat[1][1] = height / 2;
    viewport_mat[0][3] = x + width / 2;
    viewport_mat[1][3] = y + height / 2;
    //viewport_mat[2][3] = 1; // ?

    g_viewport_mat = viewport_mat;
}

void triangle_rasterization(std::vector<vec4f> clipPos, TGAImage &framebuffer, float *zbuffer, IShader &shader)
{
    vec2f bboxmax{-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max()};
    vec2f bboxmin{std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
    for(int i = 0; i < 3; i++)
    {
        if (bboxmax[0] < clipPos[i][0])
            bboxmax[0] = std::max(0.f, clipPos[i][0]);
        if (bboxmax[1] < clipPos[i][1])
            bboxmax[1] = std::max(0.f, clipPos[i][1]);

        if (bboxmin[0] > clipPos[i][0])
            bboxmin[0] = std::min(0.f, clipPos[i][0]);
        if (bboxmin[1] > clipPos[i][1])
            bboxmin[1] = std::min(0.f, clipPos[i][1]);
    }

    vec2i p;
    for(p.x = bboxmin[0]; p.x < bboxmax[0]; p.x++)
    {
        for (p.y = bboxmin[1]; p.y < bboxmax[1]; p.y++)
        {
            vec3f bc = barycentric(
                proj<2>(clipPos[0]/clipPos[0][3]),
                proj<2>(clipPos[1]/clipPos[1][3]),
                proj<2>(clipPos[2]/clipPos[2][3]), p);

            float z = clipPos[0][2] * bc[0] + clipPos[1][2] * bc[1] + clipPos[2][2] * bc[2];
            float w = clipPos[0][3] * bc[0] + clipPos[1][3] * bc[1] + clipPos[2][3] * bc[2];
            float frag_depth = z / w;

            if (bc.x < 0 || bc.y < 0 || bc.z < 0)
                continue;
            if (zbuffer[p.y * framebuffer.get_width() + p.x] > frag_depth)
                continue;

            shader.frag(bc);
        }
    }
}

vec3f barycentric(vec2f A, vec2f B, vec2f C, vec2f P)
{
    vec3f s[2];
    for(int i = 0; i < 2; i++)
    {
        s[i][0] = C[i] - A[i];
        s[i][1] = B[i] - A[i];
        s[i][2] = A[i] - P[i];
    }
    vec3f u = cross(s[0], s[1]);
    // if u[2] is 0, than triangle ABC is degenerate
    if (std::abs(u[2] > 1e-2))
        return vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);

    return vec3f(-1.f, 1.f, 1.f);
}

END_NAMESPACE(SoftRender)