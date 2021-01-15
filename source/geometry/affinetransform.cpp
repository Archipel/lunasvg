#include "affinetransform.h"
#include "point.h"
#include "rect.h"

#include <cmath>

namespace lunasvg {

AffineTransform::AffineTransform()
{
    m_matrix[0] = 1.0; m_matrix[1] = 0.0;
    m_matrix[2] = 0.0; m_matrix[3] = 1.0;
    m_matrix[4] = 0.0; m_matrix[5] = 0.0;
}

AffineTransform::AffineTransform(const AffineTransform& transform)
{
   const double* m = transform.m_matrix;
   m_matrix[0] = m[0]; m_matrix[1] = m[1];
   m_matrix[2] = m[2]; m_matrix[3] = m[3];
   m_matrix[4] = m[4]; m_matrix[5] = m[5];
}

/**
 * [ m00 m01 m02 ]    [ 0   2   4 ]
 * [ m10 m11 m12 ] => [ 1   3   5 ]
 * [ 0   0   1   ]    [ /   /   / ]
 */
AffineTransform::AffineTransform(double m00, double m10, double m01, double m11, double m02, double m12)
{
    m_matrix[0] = m00; m_matrix[1] = m10;
    m_matrix[2] = m01; m_matrix[3] = m11;
    m_matrix[4] = m02; m_matrix[5] = m12;
}

bool AffineTransform::isIdentity() const
{
    return (m_matrix[0] == 1.0 && m_matrix[1] == 0.0 &&
            m_matrix[2] == 0.0 && m_matrix[3] == 1.0 &&
            m_matrix[4] == 0.0 && m_matrix[5] == 0.0);
}

void AffineTransform::setMatrix(double m00, double m10, double m01, double m11, double m02, double m12)
{
    m_matrix[0] = m00; m_matrix[1] = m10;
    m_matrix[2] = m01; m_matrix[3] = m11;
    m_matrix[4] = m02; m_matrix[5] = m12;
}

void AffineTransform::makeIdentity()
{
    m_matrix[0] = 1.0; m_matrix[1] = 0.0;
    m_matrix[2] = 0.0; m_matrix[3] = 1.0;
    m_matrix[4] = 0.0; m_matrix[5] = 0.0;
}

AffineTransform AffineTransform::inverted() const
{
    const double* m = m_matrix;
    double det = (m[0] * m[3] - m[1] * m[2]);
    if(det == 0.0)
        return AffineTransform();

    double inv_det = 1.0 / det;
    double m00 = m[0] * inv_det;
    double m10 = m[1] * inv_det;
    double m01 = m[2] * inv_det;
    double m11 = m[3] * inv_det;
    double m02 = (m[2] *  m[5] - m[3] * m[4]) * inv_det;
    double m12 = (m[1] *  m[4] - m[0] * m[5]) * inv_det;

    return AffineTransform(m11, -m10,
                           -m01, m00,
                           m02, m12);
}

/**
 * [ m00 m01 m02 ]    [ 0   2   4 ]
 * [ m10 m11 m12 ] => [ 1   3   5 ]
 * [ 0   0   1   ]    [ /   /   / ]
 */
	
AffineTransform AffineTransform::operator*(const AffineTransform& previousTransform) const
{
    const double* B = m_matrix;
    const double* A = previousTransform.m_matrix;

    double m00 = B[0] * A[0] + B[2] * A[1];
    double m10 = B[1] * A[0] + B[3] * A[1];
    double m01 = B[0] * A[2] + B[2] * A[3];
    double m11 = B[1] * A[2] + B[3] * A[3];
    double m02 = B[0] * A[4] + B[2] * A[5] + B[4];
    double m12 = B[1] * A[4] + B[3] * A[5] + B[5];

	
    return AffineTransform(m00, m10,
                           m01, m11,
                           m02, m12);
}

AffineTransform& AffineTransform::operator*=(const AffineTransform& nextTransform)
{
    return leftMultiply(nextTransform);
}

AffineTransform& AffineTransform::leftMultiply(const AffineTransform& nextTransform)
{
    *this = nextTransform * *this;
    return *this;
}

AffineTransform& AffineTransform::rightMultiply(const AffineTransform& previousTransform)
{
    *this = *this * previousTransform;
    return *this;
}

AffineTransform AffineTransform::fromRotate(double radians)
{
    double c = std::cos(radians);
    double s = std::sin(radians);

    return AffineTransform(c, s,
                           -s, c,
                           0, 0);
}

AffineTransform AffineTransform::fromRotate(double radians, double cx, double cy)
{
    double c = std::cos(radians);
    double s = std::sin(radians);

    double x = cx * (1 - c) + cy * s;
    double y = cy * (1 - c) - cx * s;

    return AffineTransform(c, s,
                           -s, c,
                           x, y);
}

AffineTransform AffineTransform::fromScale(double sx, double sy)
{
    return AffineTransform(sx, 0,
                           0, sy,
                           0, 0);
}

AffineTransform AffineTransform::fromShear(double shx, double shy)
{
    double x = std::tan(shx);
    double y = std::tan(shy);

    return AffineTransform(1, y,
                           x, 1,
                           0, 0);
}

AffineTransform AffineTransform::fromTranslate(double tx, double ty)
{
    return AffineTransform(1, 0,
                           0, 1,
                           tx, ty);
}

AffineTransform& AffineTransform::rotate(double radians)
{
    return leftMultiply(fromRotate(radians));
}

AffineTransform& AffineTransform::rotate(double radians, double cx, double cy)
{
    return leftMultiply(fromRotate(radians, cx, cy));
}

AffineTransform& AffineTransform::scale(double sx, double sy)
{
    return leftMultiply(fromScale(sx, sy));
}

AffineTransform& AffineTransform::shear(double shx, double shy)
{
    return leftMultiply(fromShear(shx, shy));
}

AffineTransform& AffineTransform::translate(double cx, double cy)
{
    return leftMultiply(fromTranslate(cx, cy));
}

void AffineTransform::map(double x, double y, double& _x, double& _y) const
{
    _x = x * m_matrix[0] + y * m_matrix[2] + m_matrix[4];
    _y = x * m_matrix[1] + y * m_matrix[3] + m_matrix[5];
}

void AffineTransform::map(const double* src, double* dst, int size) const
{
    for(int i = 0;i < size;i+=2)
        map(src[i], src[i+1], dst[i], dst[i+1]);
}

void AffineTransform::map(const Point* src, Point* dst, int size) const
{
    for(int i = 0;i < size;i++)
        map(src[i].x, src[i].y, dst[i].x, dst[i].y);
}

Point AffineTransform::mapPoint(const Point& point) const
{
    Point p;
    map(&point, &p, 1);
    return p;
}

Rect AffineTransform::mapRect(const Rect& rect) const
{
    Point p[4];
    p[0].x = rect.x;
    p[0].y = rect.y;
    p[1].x = rect.x + rect.width;
    p[1].y = rect.y;
    p[2].x = rect.x + rect.width;
    p[2].y = rect.y + rect.height;
    p[3].x = rect.x;
    p[3].y = rect.y + rect.height;

    map(p, p, 4);

    double l = p[0].x;
    double t = p[0].y;
    double r = p[0].x;
    double b = p[0].y;

    for(int i = 0;i < 4;i++)
    {
        if(p[i].x < l) l = p[i].x;
        if(p[i].x > r) r = p[i].x;
        if(p[i].y < t) t = p[i].y;
        if(p[i].y > b) b = p[i].y;
    }

    return Rect(l, t, r-l, b-t);
}

} // namespace lunasvg
