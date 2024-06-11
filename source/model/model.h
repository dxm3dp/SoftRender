#ifndef _MODEL_H_
#define _MODEL_H_

#include "../common/cpplang.h"
#include "../geometry/geometry.h"
#include "tgaimage.h"

BEGIN_NAMESPACE(SoftRender)

class Model
{
public:
    Model(const char * filename);

public:
    int nfaces() const;
    int nverts() const;
    vec3f vert(int idx) const;
    vec3f vert(int iface, int nthvert) const;
    vec2f uv(int iface, int nthvert) const;
    vec3f normal(int iface, int nthvert);
    std::vector<vec3i> face(int idx) const;
    TGAColor diffuse(vec2f uv) const;
    vec3f normal(vec2f uv) const;
    float specular(vec2f uv) const;

private:
    void load_texture(const char * filename, const char * suffix, TGAImage & image);

private:
    std::vector<vec3f> vertices;
    std::vector<vec2f> texcoords;
    std::vector<vec3f> normals;
    std::vector<std::vector<vec3i>> faces;

    TGAImage diffuse_tex;
    TGAImage normal_tex;
    TGAImage specular_tex;
};

END_NAMESPACE(SoftRender)

#endif // _MODEL_H_