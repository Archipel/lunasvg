#ifndef AFFINETRANSFORM_H
#define AFFINETRANSFORM_H

namespace lunasvg {

class Point;
class Rect;

class AffineTransform
{
public:
    AffineTransform();
    AffineTransform(const AffineTransform& transform);
	
	/**
	 * [ m00 m01 m02 ]
	 * [ m10 m11 m12 ]
	 * [ 0   0   1   ]
	 */
    AffineTransform(double m00, double m10, double m01, double m11, double m02, double m12);
    
	/**
	 * [ m00 m01 m02 ]
	 * [ m10 m11 m12 ]
	 * [ 0   0   1   ]
	 */
    void setMatrix(double m00, double m10, double m01, double m11, double m02, double m12);
	
    void makeIdentity();
    AffineTransform inverted() const;

    AffineTransform operator*(const AffineTransform& previousTransform) const;
    AffineTransform& operator*=(const AffineTransform& nextTransform);

    AffineTransform& leftMultiply(const AffineTransform& nextTransform);
    AffineTransform& rightMultiply(const AffineTransform& previousTransform);
    AffineTransform& rotate(double radians);
    AffineTransform& rotate(double radians, double cx, double cy);
    AffineTransform& scale(double sx, double sy);
    AffineTransform& shear(double shx, double shy);
    AffineTransform& translate(double cx, double cy);

    static AffineTransform fromRotate(double radians);
    static AffineTransform fromRotate(double radians, double cx, double cy);
    static AffineTransform fromScale(double sx, double sy);
    static AffineTransform fromShear(double shx, double shy);
    static AffineTransform fromTranslate(double tx, double ty);

    void map(double x, double y, double& _x, double& _y) const;
    void map(const double* src, double* dst, int size) const;
    void map(const Point* src, Point* dst, int size) const;
    Point mapPoint(const Point& point) const;
    Rect mapRect(const Rect& rect) const;

    bool isIdentity() const;
    double determinant() const;
    const double* getMatrix() const;

private:
    double m_matrix[2*3];
};

inline const double* AffineTransform::getMatrix() const
{
   return m_matrix;
}

} // namespace lunasvg

#endif //AFFINETRANSFORM_H
