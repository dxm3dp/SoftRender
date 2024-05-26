#include "geometry.h"

BEGIN_NAMESPACE(SoftRender)

template <>
template <>
vec<2, int>::vec(const vec<2, float>& v) : x(v.x), y(v.y) {}

template <>
template <>
vec<2, float>::vec(const vec<2, int>& v) : x(v.x + .5f), y(v.y + .5f) {}

END_NAMESPACE(SoftRender)