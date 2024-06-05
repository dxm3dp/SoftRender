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

    g_view_mat = trans * rot; // First perform the linear transformation, then the translate transformation.
}

void get_projection_matrix(float fov, float aspect, float zNear, float zFar)
{
    fov = fov * M_PI / 180.f;
    float fax = 1 / std::tan( fov * 0.5f);
    mat4x4 projection_mat = mat4x4::identity();
    projection_mat[0][0] = fax / aspect;
    projection_mat[1][1] = fax;
    projection_mat[2][2] = -(zNear + zFar) / (zFar - zNear);
    projection_mat[2][3] = -2.f * zNear * zFar / (zFar - zNear);
    projection_mat[3][2] = -1.f;
    projection_mat[3][3] = 0.f;

    g_projection_mat = projection_mat;
}

void get_projection_matrix(float coeff)
{
    mat4x4 projection_mat = mat4x4::identity();
    projection_mat[3][2] = coeff;

    g_projection_mat = projection_mat;
}

void get_viewport_matrix(int x, int y, int width, int height)
{
    // NDC -> Screen
    mat4x4 viewport_mat = mat4x4::identity();
    viewport_mat[0][0] = width / 2;     // width scale
    viewport_mat[1][1] = height / 2;    // height scale
    viewport_mat[2][2] = depth / 2;     // depth scale
    viewport_mat[0][3] = x + width / 2; // min x
    viewport_mat[1][3] = y + height / 2;// min y
    viewport_mat[2][3] = depth / 2;     // min z

    g_viewport_mat = viewport_mat;
}

void triangle_rasterization(Model *model, std::vector<vec4f> screenPos, TGAImage &framebuffer, float *zbuffer, IShader &shader)
{
    vec2f bboxmin{std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
    vec2f bboxmax{-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max()};
    vec2f clamp{(float)(framebuffer.get_width() - 1), (float)(framebuffer.get_height() - 1)};
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            bboxmin[j] = std::max(0.f, std::min(bboxmin[j], screenPos[i][j]/screenPos[i][3]));
            bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], screenPos[i][j]/screenPos[i][3]));
        }
    }

    vec2i p;
    for(p.x = bboxmin.x; p.x <= bboxmax.x; p.x++)
    {
        for (p.y = bboxmin.y; p.y <= bboxmax.y; p.y++)
        {
            vec3f bc = barycentric(
                proj<2>(screenPos[0]/screenPos[0][3]),
                proj<2>(screenPos[1]/screenPos[1][3]),
                proj<2>(screenPos[2]/screenPos[2][3]), p);
            //vec3f bc = barycentricV2(
                //proj<2>(screenPos[0]/screenPos[0][3]),
                //proj<2>(screenPos[1]/screenPos[1][3]),
                //proj<2>(screenPos[2]/screenPos[2][3]), p);

            // correct barycentric coordnates
            float z = screenPos[0][2] * bc[0] + screenPos[1][2] * bc[1] + screenPos[2][2] * bc[2];
            float w = screenPos[0][3] * bc[0] + screenPos[1][3] * bc[1] + screenPos[2][3] * bc[2];
            int frag_depth = z / w;

            if (bc.x < 0 || bc.y < 0 || bc.z < 0)
                continue;
            if (zbuffer[p.x + p.y * framebuffer.get_width()] > frag_depth)
                continue;

            TGAColor color;
            shader.frag(model, bc, color);
            zbuffer[p.x + p.y * framebuffer.get_width()] = frag_depth;
            framebuffer.set(p.x, p.y, color);
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
    if (std::abs(u[2]) > 1e-2)
        return vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);

    return vec3f(-1.f, 1.f, 1.f);
}

vec3f barycentricV2(vec2f A, vec2f B, vec2f C, vec2f P)
{
    vec3f ret;
    ret.x = (P.x * (B.y - C.y) + (C.x - B.x) * P.y + B.x * C.y - C.x * B.y) / (A.x * (B.y - C.y) + (C.x - B.x) * A.y + B.x * C.y - C.x * B.y) ;
    ret.y = (P.x * (C.y - A.y) + (A.x - C.x) * P.y + C.x * A.y - A.x * C.y) / (B.x * (C.y - A.y) + (A.x - C.x) * B.y + C.x * A.y - A.x * C.y) ;
    ret.z = (P.x * (A.y - B.y) + (B.x - A.x) * P.y + A.x * B.y - B.x * A.y) / (C.x * (A.y - B.y) + (B.x - A.x) * C.y + A.x * B.y - B.x * A.y) ;

    return ret;
}

END_NAMESPACE(SoftRender)