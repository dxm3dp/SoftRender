#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include "../common/cpplang.h"

BEGIN_NAMESPACE(SoftRender)

template <size_t DIM, typename T>
struct vec
{
    vec()
    {
        for (size_t i = 0; i < DIM; i++)
        {
            data[i] = T();
        }
    }

    T& operator[](const size_t i)
    {
        static_assert(i < DIM); // 这里为何可以用静态断言？
        return data[i];
    }

    const T& operator[](const size_t i) const
    {
        static_assert(i < DIM);
        return data[i];
    }

private:
    T data[DIM];
};

template <typename T>
struct vec<2, T>
{
    vec() : x(T()), y(T()) {}
    vec(T x, T y) : x(x), y(y) {}
    template <typename U>
    vec<2, T>(const vec<2, U>& v);

    T& operator[](const size_t i)
    {
        assert(i < 2);
        return i == 0 ? x : y;
    }

    const T& operator[](const size_t i) const
    {
        assert(i < 2);
        return i == 0 ? x : y;
    }

    T x, y;
};

template <typename T>
struct vec<3, T>
{
    vec() : x(T()), y(T()), z(T()) {}
    vec(T x, T y, T z) : x(x), y(y), z(z) {}
    template<typename U>
    vec<3, T>(const vec<3, U>& v);

    T& operator[](const size_t i)
    {
        assert(i < 3);
        return i == 0 ? x : (i == 1 ? y : z);
    }

    const T& operator[](const size_t i) const
    {
        assert(i < 3);
        return i == 0 ? x : (i == 1 ? y : z);
    }

    T x, y, z;
};

END_NAMESPACE(SoftRender)

#endif // _GEOMETRY_H_