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
}

END_NAMESPACE(SoftRender)