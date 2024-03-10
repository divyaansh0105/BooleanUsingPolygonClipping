#pragma once
 
class Point2D 
{
public:

 
    // Constructor
    Point2D(float x, float y) : mX(x), mY(y) {}
 
    // Comparison operators
    bool operator<(const Point2D& other) const ;
    bool operator==(const Point2D& other) const ;
    
    //user defined
    void setX(double x);
    void setY(double y);
    double x();
    double y();

private :     
           double mX;
           double mY;
};
 