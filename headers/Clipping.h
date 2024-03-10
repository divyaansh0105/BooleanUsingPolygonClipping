#pragma once


#include <vector>

#include "Point3D.h"
#include "Triangulation.h"
#include "Point2D.h"

class Clipping
{
    public :
             Clipping();
             ~Clipping();
             
             double SurfacWidth(std::vector<Point3D>& face);
             double minzval(std::vector<Point3D>& face);
             double maxzval(std::vector<Point3D>& face);
             vector<Point3D> clip(Triangulation& T1, Triangulation& T2, string& filePath1, string& filePath2);
             void sutherland(vector<Point3D>& Polygon1,vector<Point3D>& Polygon2,double z,vector<Point3D>& answer);
             vector<Point2D> getClippingSurface(std::vector<Point3D> surface);                   
};