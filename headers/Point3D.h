#pragma once

class Point3D
{
public:
        Point3D();
        Point3D(double x, double y, double z);
        ~Point3D();
        double x() const;
        double y() const;
        double z() const;
        bool operator<(const Point3D &other) const;
        Point3D& operator=(const Point3D& other);
private:
        double mX;
        double mY;
        double mZ;
};

