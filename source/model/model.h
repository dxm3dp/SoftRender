#ifndef _MODEL_H_
#define _MODEL_H_

#include "../common/cpplang.h"
#include "../geometry/geometry.h"

BEGIN_NAMESPACE(SoftRender)

class Model
{
public:
    Model(const char * filename);

private:
    std::vector<vec3f> vertices;
    std::vector<vec2f> texcoords;
    std::vector<vec3f> normals;
    std::vector<std::vector<vec3i>> faces;
};

END_NAMESPACE(SoftRender)

#endif // _MODEL_H_