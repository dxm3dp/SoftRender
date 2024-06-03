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
        assert(i < DIM);
        return data[i];
    }

    const T& operator[](const size_t i) const
    {
        assert(i < DIM);
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

    float norm() const
    {
        return std::sqrt(x * x + y * y);
    }

    vec<2, T> &normalize()
    {
        return *this = (*this) * (1 / norm());
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

    float norm() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    vec<3, T> &normalize()
    {
        return *this = (*this) * (1 / norm());
    }

    T x, y, z;
};

// ---------------------------------------------------------------------------

template <size_t Dim, typename T>
T operator*(const vec<Dim, T> &lhs, const vec<Dim, T> &rhs)
{
    T ret = T();
    for(size_t i = 0; i < Dim; i++)
    {
        ret += lhs[i] * rhs[i];
    }
    return ret;
}

template <size_t Dim, typename T>
vec<Dim, T> operator+(const vec<Dim, T> &lhs, const vec<Dim, T> &rhs)
{
    vec<Dim, T> ret;
    for(size_t i = 0; i < Dim; i++)
    {
        ret[i] = lhs[i] + rhs[i];
    }
    return ret;
}

template <size_t Dim, typename T>
vec<Dim, T> operator-(const vec<Dim, T> &lhs, const vec<Dim, T> &rhs)
{
    vec<Dim, T> ret;
    for(size_t i = 0; i < Dim; i++)
    {
        ret[i] = lhs[i] - rhs[i];
    }
    return ret;
}

template <size_t Dim, typename T, typename U>
vec<Dim, T> operator*(const vec<Dim, T> &lhs, const U &rhs)
{
    vec<Dim, T> ret;
    for(size_t i = 0; i < Dim; i++)
    {
        ret[i] = lhs[i] * rhs;
    }
    return ret;
}

template <size_t Dim, typename T, typename U>
vec<Dim, T> operator/(const vec<Dim, T> &lhs, const U &rhs)
{
    vec<Dim, T> ret;
    for(size_t i = 0; i < Dim; i++)
    {
        ret[i] = lhs[i] / rhs;
    }
    return ret;
}

template <size_t Len, size_t Dim, typename T>
vec<Len, T> embed(const vec<Dim, T> &v, T fill = 1)
{
    vec<Len, T> ret;
    for(size_t i = 0; i < Len; i++)
    {
        ret[i] = i < Dim ? v[i] : fill;
    }
    return ret;
}

template <size_t Len, size_t Dim, typename T>
vec<Len, T> proj(const vec<Dim, T> &v)
{
    vec<Len, T> ret;
    for(size_t i = 0; i < Len; i++)
    {
        ret[i] = v[i];
    }
    return ret;
}

template <typename T>
vec<3, T> cross(const vec<3, T> &v1, const vec<3, T> &v2)
{
    vec<3, T> ret;
    ret[0] = v1.y * v2.z - v1.z * v2.y;
    ret[1] = v1.z * v2.x - v1.x * v2.z;
    ret[2] = v1.x * v2.y - v1.y * v2.x;
    return ret;
}

template <size_t Dim, typename T>
std::ostream& operator<<(std::ostream &out, const vec<Dim, T> &v)
{
    for(size_t i = 0; i < Dim; i++)
    {
        out << v[i] << " ";
    }
    return out;
}

// ---------------------------------------------------------------------------

template <size_t Rows, size_t Cols, typename T>
struct mat;

template <size_t Dim, typename T>
struct determinant
{
    static T det(const mat<Dim, Dim, T> &src)
    {
        T ret;
        for(size_t i = 0; i < Dim; i++)
        {
            ret += src[0][i] * src.cofactor(0, i);
        }
        return ret;
    }
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
                ret[i][j] = (i == j ? 1 : 0);
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

    T det() const
    {
        return determinant<Cols, T>::det(*this);
    }

    T cofactor(size_t row, size_t col) const
    {
        return get_minor(row, col).det() * (((row + col) % 2) ? -1 : 1);
    }

    mat<Rows, Cols, T> adjusgate() const
    {
        mat<Rows, Cols, T> ret;
        for(size_t i = 0; i < Rows; i++)
        {
            for (size_t j = 0; j < Cols; j++)
            {
                ret[i][j] = cofactor(i, j);
            }
        }
        return ret;
    }

    mat<Rows, Cols, T> invert_transpose() const
    {
        mat<Rows, Cols, T> ret = adjusgate();
        T det = ret[0] * rows[0];
        return ret / det; // 对这里的结果还有疑问
    }

    mat<Cols, Rows, T> transpose() const
    {
        mat<Cols, Rows, T> ret;
        for(size_t i = 0; i < Cols; i++)
        {
            ret[i] = this->get_col(i);
        }
        return ret;
    }

private:
    vec<Cols, T> rows[Rows];
};

// ---------------------------------------------------------------------------

template <size_t Rows, size_t Cols, typename T>
vec<Rows, T> operator*(const mat<Rows, Cols, T> &m, const vec<Cols, T> &v)
{
    vec<Rows, T> ret;
    for(size_t i = 0; i < Rows; i++)
    {
        ret[i] = m[i] * v;
    }
    return ret;
}

template <size_t R1, size_t C1, size_t C2, typename T>
mat<R1, C2, T> operator*(const mat<R1, C1, T> &m1, const mat<C1, C2, T> &m2)
{
    mat<R1, C2, T> ret;
    for(size_t i = 0; i < R1; i++)
    {
        for(size_t j = 0; j < C2; j++)
        {
            ret[i][j] = m1[i] * m2.get_col(j);
        }
    }
    return ret;
}

template <size_t Rows, size_t Cols, typename T>
mat<Rows, Cols, T> operator*(const mat<Rows, Cols, T> &m, const T &factor)
{
    mat<Rows, Cols, T> ret;
    for (size_t i = 0; i < Rows; i++)
    {
        ret[i] = m[i] * factor;
    }
    return ret;
}

template <size_t Rows, size_t Cols, typename T>
mat<Rows, Cols, T> operator/(const mat<Rows, Cols, T> &m, const T &div)
{
    mat<Rows, Cols, T> ret;
    for (size_t i = 0; i < Rows; i++)
    {
        for (size_t j = 0; j < Cols; j++)
        {
            ret[i][j] = m[i][j] / div;
        }
    }
    return ret;
}

template <size_t Rows, size_t Cols, typename T>
std::ostream& operator<<(std::ostream &out, const mat<Rows, Cols, T> &m)
{
    for(size_t i = 0; i < Rows; i++)
    {
        out << m[i] << std::endl;
    }
    return out;
}

// ---------------------------------------------------------------------------

typedef vec<2, float> vec2f;
typedef vec<2, int> vec2i;
typedef vec<3, float> vec3f;
typedef vec<3, int> vec3i;
typedef vec<4, float> vec4f;
typedef mat<3, 3, float> mat3x3;
typedef mat<4, 4, float> mat4x4;

END_NAMESPACE(SoftRender)

#endif // _GEOMETRY_H_