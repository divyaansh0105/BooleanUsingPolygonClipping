#include "../headers/Point2D.h"
 
// No need to define constructor since it's already defined inline in the header
 
// Comparison operators
bool Point2D::operator<(const Point2D& other) const 
{
    // Compare X coordinates first, then Y coordinates if X coordinates are equal
    return (mX != other.mX) ? (mX < other.mX) : (mY < other.mY);
}
 
bool Point2D::operator==(const Point2D& other) const 
{
    // Check if X and Y coordinates are equal
    return (mX == other.mX) && (mY == other.mY);
}


 void Point2D::setX(double x)
 {
    this->mX = x ;
 }
void Point2D::setY(double y)
{
    this->mY = y;
}
    
double Point2D::x()
{
    return mX;
}

double Point2D::y()
{
    return mY;
}