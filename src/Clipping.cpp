#include "stdafx.h"
#include <cmath>
#include <iostream>
#include <set>

#include "Point3D.h"
#include "Clipping.h"
#include "STLReader.h"
#include "../headers/STLWriter.h"

using namespace std;

Clipping::Clipping()
{

}

Clipping::~Clipping()
{
    
}

double Clipping::SurfacWidth(std::vector<Point3D>& face)
{  
    double minWidth = INT_MAX;
    double maxWidth = INT_MIN;  
    for(auto it:face)
    {
        if(it.z()>maxWidth)
        {
            maxWidth = it.z();
        }
        else if(it.z() < minWidth)
        {
            minWidth = it.z();
        }
    }
    if(maxWidth == minWidth)
    {
        return maxWidth;
    }
    else
    {
        return (abs(maxWidth) + abs(minWidth));
    }
}

double Clipping::maxzval(std::vector<Point3D>& face)
{
    double maxWidth = INT_MIN;  
    for(auto it:face)
    {
        if(it.z()>maxWidth)
        {
            maxWidth = it.z();
        }      
    }
    return maxWidth;
}

double Clipping::minzval(std::vector<Point3D>& face)
{
    double minWidth = INT_MAX;  
    for(auto it:face)
    {
        if(it.z()<minWidth)
        {
            minWidth = it.z();
        }      
    }
    return minWidth;
}

bool isequal(Point3D P1 , Point3D P2)
{  
    bool result ;
 
    result = ((P1.x()==P2.x()) && (P1.y()==P2.y()) && (P1.z()==P2.z()));
   
    return result;
} 
 
void  getPolygonSurface(Triangulation& triangulation,vector<Point3D>& surfacePoint)
{
    std::vector<Point3D>& points = triangulation.uniquePoints();
    std::vector<Triangle>& triangles = triangulation.triangles();
    std::vector<Point3D>& normals = triangulation.uniqueNormals();
   
    int targetNormalIndex ;
   
    // Find unique normal indices
    for (const Triangle& triangle : triangles)
    {   
        Point3D checkNormal;
        checkNormal = normals[triangle.normalIndex()];
        if(checkNormal.z() !=0)
        {
           targetNormalIndex = triangle.normalIndex() ;
        }
    }

    std::vector<int> triangleIndices;
    std::vector<Point3D> seenIndices;
    int count = 0 ;
    for (const Triangle& triangle : triangles)
    {  
        int normalIndex = triangle.normalIndex();
        if(targetNormalIndex == normalIndex  )
        { 
         if(seenIndices.size()==0)
         {  
           
            seenIndices.push_back(points[triangle.v1()]);
            seenIndices.push_back(points[triangle.v2()]);
            seenIndices.push_back(points[triangle.v3()]);        
         }
         else
         {   
           vector<Point3D> trianglePoints;
           Point3D newPoint1  = points[triangle.v1()];
           Point3D newPoint2  = points[triangle.v2()];
           Point3D newPoint3  = points[triangle.v3()];
           trianglePoints.push_back(newPoint1);
           trianglePoints.push_back(newPoint2);
           trianglePoints.push_back(newPoint3);
         
            for(auto it : trianglePoints)
            {
                bool flag = false ;
                for(auto jt : seenIndices)
                {
                    if(isequal(it,jt))
                    {
                      flag = true ;
                      break;    
                    }
                    else
                    {
                       
                    }
                }
 
                if(flag==false)
                {
                   seenIndices.push_back(it);
                       
                }
               
            }
         }
                         
        }
    }
   for(auto element : seenIndices)
    {
        surfacePoint.push_back(element);
    }
}

 vector<Point2D> Clipping:: getClippingSurface(vector<Point3D> surface)
  {
    std::vector<Point2D>currentSurface;
    for(auto it : surface)
    {  
        Point2D  newpoint(it.x(),it.y());
        currentSurface.push_back(newpoint);
    }
    return currentSurface;
  }

 
// Helper function to calculate the intersection point of two lines
Point2D intersection( Point2D& p1,  Point2D& p2,  Point2D& q1,  Point2D& q2)
{  
    double x1 = p1.x(), y1 = p1.y();
    double x2 = p2.x(), y2 = p2.y();
    double x3 = q1.x(), y3 = q1.y();
    double x4 = q2.x(), y4 = q2.y();
 
    double denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    double x = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / denom;
    double y = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / denom;
    
    Point2D point(x,y);
    return point;
}
 
// Sutherland-Hodgman algorithm for polygon clipping against a fixed polygon
void clipPolygonAgainstFixed(vector<Point2D>& subjectPolygon,  vector<Point2D>& clipPolygon,vector<Point2D>& answer) 
{   
    
    vector<Point2D> output = subjectPolygon;
    for (int i = 0; i < clipPolygon.size(); ++i) 
    {
        int k = (i + 1) % clipPolygon.size();
        vector<Point2D> input = output;
        output.clear();
 
        for (int j = 0; j < input.size(); ++j) 
        {
            Point2D p1 = input[j];
            Point2D p2 = input[(j + 1) % input.size()];
           
            // Check if p1 is inside or outside the clip polygon
            bool p1Inside = (p1.x() - clipPolygon[i].x()) * (clipPolygon[k].y() - clipPolygon[i].y()) -
                            (p1.y() - clipPolygon[i].y()) * (clipPolygon[k].x() - clipPolygon[i].x()) <= 0;
            if (p1Inside) 
            {
                output.push_back(p1);
            }
 
            // Check for intersection and add the intersection point
            if ((p1.x() - clipPolygon[i].x()) * (p2.x() - clipPolygon[i].x()) < 0 ||
                (p1.y() - clipPolygon[i].y()) * (p2.y() - clipPolygon[i].y()) < 0) 
            {   
                Point2D intersectionPoint = intersection(clipPolygon[i], clipPolygon[k], p1, p2);
                output.push_back(intersectionPoint);
            }
        }
    }

   for(auto it : output)
   {
    answer.push_back(it);
    
   }
}

void Clipping::sutherland(vector<Point3D>& Clipper,vector<Point3D>& Clipped,double z,vector<Point3D>& answer  )
{   
   //Creating vector of 2d points for polygon clipping
    vector<Point2D>clipperSurface;
    vector<Point2D>subjectSurface;
    vector<Point2D> clippedSurface;
  
    clipperSurface = getClippingSurface(Clipper);
    subjectSurface = getClippingSurface(Clipped);
  
    clipPolygonAgainstFixed(subjectSurface, clipperSurface,clippedSurface);

    //pushing the clipped polygon output into answer array
    for(auto it:clippedSurface)
    {
        Point3D tempPoint(it.x(),it.y(),z);
        answer.push_back(tempPoint);
    }
}

vector<Point3D> Clipping::clip(Triangulation& T1, Triangulation& T2, string& filePath1, string& filePath2)
{   
   STLReader Reader;
   
   Reader.readSTL(filePath1, T1);
   Reader.readSTL(filePath2, T2);

    vector<Point3D> pointsT1;
    vector<Point3D> pointsT2;

    //storing coordinates of both the stl files
    Reader.Coordinates(filePath1, T1 , pointsT1);
    Reader.Coordinates(filePath2, T2 , pointsT2);

    // storing the start and end points of both stl files based on z axis 
    double startT1,endT1,startT2,endT2;
    startT1 = minzval(pointsT1); endT1 = maxzval(pointsT1);
    startT2 = minzval(pointsT2); endT2 = maxzval(pointsT2);


    double sliceinterval = SurfacWidth(pointsT1)/100;
  
    //Creating a vector of resultant surfaces
    vector<Point3D> answer;
  
    int count = 0 ;
    for(double i  = startT1 ; i <=endT1 ; i = i+sliceinterval )
    { 
        for(double j = startT2 ; j<=endT2 ; j = j+sliceinterval)
        {
                if(i==j)
                {
                    //if both slices are in same plane then clip 
                    vector<Point3D> clipper ;
                    vector<Point3D> clipped ;
                    getPolygonSurface(T1,clipper);
                    getPolygonSurface(T2,clipped);          
                    double z = i ;
                    sutherland(clipper,clipped,z,answer);   
                }           
        }
    }
//  Clipped output
return answer;


}