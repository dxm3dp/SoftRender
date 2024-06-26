#include "model.h"
#include "../common/Logger.h"

BEGIN_NAMESPACE(SoftRender)

Model::Model(const char * filename)
{
    std::ifstream in(filename, std::ios::in);
    if (!in.is_open())
    {
        LOGE("Cannot open file: %s", filename);
        return;
    }

    std::string line;
    while (!in.eof())
    {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v "))       // vertex
        {
            iss >> trash;
            vec3f v;
            for(int i = 0; i < 3; i++)
            {
                iss >> v[i];
            }
            vertices.emplace_back(v);
        }
        else if (!line.compare(0, 3, "vt ")) // texture
        {
            iss >> trash >> trash;
            vec2f vt;
            for(int i = 0; i < 2; i++)
            {
                iss >> vt[i];
            }
            texcoords.emplace_back(vt);
        }
        else if (!line.compare(0, 3, "vn ")) // normal
        {
            iss >> trash >> trash;
            vec3f vn;
            for(int i = 0; i < 3; i++)
            {
                iss >> vn[i];
            }
            normals.emplace_back(vn);
        }
        else if (!line.compare(0, 2, "f "))  // face
        {
            iss >> trash;
            std::vector<vec3i> f;
            vec3i tmp;
            while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2])
            {
                for (int i = 0; i < 3; i++)
                {
                    tmp[i]--; // in wavefront obj all indices start at 1, not zero
                }
                f.emplace_back(tmp);
            }
            faces.emplace_back(f);
        }
    }
    LOGI("vertices: %d, texcoords: %d, normals: %d, faces: %d", vertices.size(), texcoords.size(), normals.size(), faces.size());
    load_texture(filename, "_diffuse.tga", diffuse_tex);
    load_texture(filename, "_nm_tangent.tga", normal_tex);
    load_texture(filename, "_spec.tga", specular_tex);
}

int Model::nfaces() const
{
    return (int)faces.size();
}

int Model::nverts() const
{
    return (int)vertices.size();
}

vec3f Model::vert(int idx) const
{
    return vertices[idx];
}

vec3f Model::vert(int iface, int nthvert) const
{
    return vertices[faces[iface][nthvert][0]];
}

vec2f Model::uv(int iface, int nthvert) const
{
    return texcoords[faces[iface][nthvert][1]];
}

vec3f Model::normal(int iface, int nthvert)
{
    int idx = faces[iface][nthvert][2];
    return normals[idx].normalize();
}

std::vector<vec3i> Model::face(int idx) const
{
    return faces[idx];
}

TGAColor Model::diffuse(vec2f uv) const
{
    vec2i uvi{(int)(uv[0] * diffuse_tex.get_width()), (int)(uv[1] * diffuse_tex.get_height())};
    return diffuse_tex.get(uvi.x, uvi.y);
}

vec3f Model::normal(vec2f uv) const
{
    vec2i uvi{(int)(uv[0] * normal_tex.get_width()), (int)(uv[1] * normal_tex.get_height())};
    TGAColor color = normal_tex.get(uvi.x, uvi.y);
    vec3f ret;
    for(int i = 0; i < 3; i++)
    {
        ret[i] = color[i] / 255.f * 2.f - 1.f;
    }
    return ret;
}

float Model::specular(vec2f uvf) const
{
    vec2i uv(uvf[0] * specular_tex.get_width(), uvf[1] * specular_tex.get_height());

    return specular_tex.get(uv[0], uv[1])[0]/1.f;
}

void Model::load_texture(const char * filename, const char * suffix, TGAImage & tex)
{
    std::string texfile(filename);
    size_t dot = texfile.find_last_of(".");
    if (dot != std::string::npos)
    {
        texfile = texfile.substr(0, dot) + std::string(suffix);
        bool ret = tex.read_tga_file(texfile.c_str());
        LOGI("texture file: %s, %s", texfile.c_str(), ret ? "loaded" : "failed");
        tex.flip_vertically(); // 为什么要做这个操作？
    }
}

END_NAMESPACE(SoftRender)