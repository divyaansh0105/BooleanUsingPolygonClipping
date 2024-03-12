#pragma once


#include <vector>

#include "Point2D.h"
#include "Point3D.h"
#include "Triangulation.h"


class BooleanOperations
{
    public :
             BooleanOperations();
             ~BooleanOperations();
             
             double objectWidth(std::vector<Point3D>& face);
             double minZValue(std::vector<Point3D>& face);
             double maxZValue(std::vector<Point3D>& face);
            
             void get2DPolygons(vector<Point3D>& Clipper, vector<Point3D>& Clipped, double z, vector<Point3D>& answer);
             vector<Point3D> getIntersection(Triangulation& T1, Triangulation& T2, string& filePath1, string& filePath2);
             vector<Point2D> getClippingSurface(std::vector<Point3D> surface);                   
};