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

template <size_t Rows, size_t Cols, typename T>
struct mat
{
public:
    mat() {}

    static mat<Rows, Cols, T> identity()
    {
        mat<Rows, Cols, T> ret;
        for(size_t i = 0; i < Rows; i++)
        {
            for (size_t j = 0; j < Cols; j++)
            {
                ret[i][j] == (i == j ? 1 : 0);
            }
        }
        return ret;
    }

    vec<Cols, T>& operator[](const size_t i)
    {
        assert(i >= 0 && i < Rows);
        return rows[i];
    }

    const vec<Cols, T>& operator[](const size_t i) const
    {
        assert(i >= 0 && i < Rows);
        return rows[i];
    }

    vec<Rows, T> get_col(const size_t i) const
    {
        assert(i >=0 && i < Cols);
        vec<Rows, T> ret;
        for (size_t j = 0; j < Rows; j++)
        {
            ret[j] = rows[j][i];
        }
        return ret;
    }

    void set_col(size_t i, const vec<Rows, T>& v)
    {
        assert(i >= 0 && i < Cols);
        for (size_t j = 0; j < Rows; j++)
        {
            rows[j][i] = v[j];
        }
    }

    mat<Rows - 1, Cols - 1, T> get_minor(size_t row, size_t col) const
    {
        mat<Rows - 1, Cols - 1, T> ret;
        for (size_t i = 0; i < Rows - 1; i++)
        {
            for (size_t j = 0; j < Cols - 1; j++)
            {
                ret[i][j] = rows[i < row ? i : i + 1][j < col ? j : j + 1];
            }
        }
        return ret;
    }

private:
    vec<Cols, T> rows[Rows];
};


typedef vec<2, float> vec2f;
typedef vec<2, int> vec2i;
typedef vec<3, float> vec3f;
typedef vec<3, int> vec3i;

END_NAMESPACE(SoftRender)

#endif // _GEOMETRY_H_