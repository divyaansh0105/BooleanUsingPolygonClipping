#include "stdafx.h"
#include <cmath>
#include <iostream>
#include <set>

#include "BooleanOperations.h"
#include "Point3D.h"
#include "STLReader.h"
#include "STLWriter.h"
#include "SutherLand.h"

using namespace std;

BooleanOperations::BooleanOperations()
{

}

BooleanOperations::~BooleanOperations()
{
    
}

double BooleanOperations::objectWidth(std::vector<Point3D>& face)
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

double BooleanOperations::maxZValue(std::vector<Point3D>& face)
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

double BooleanOperations::minZValue(std::vector<Point3D>& face)
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

bool BooleanOperations:: isEqual(Point3D P1 , Point3D P2)
{  
    bool result ;
 
    result = ((P1.x()==P2.x()) && (P1.y()==P2.y()) && (P1.z()==P2.z()));
   
    return result;
} 
 
void BooleanOperations::getPolygonSurface(Triangulation& triangulation,vector<Point3D>& surfacePoint)
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
        
        //if the normal is on z axis then extract the normal to get 2d surfaces
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
        //check for the target noraml
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
                    if(isEqual(it,jt))
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

 vector<Point2D> BooleanOperations:: getClippingSurface(vector<Point3D> surface)
  {
    std::vector<Point2D>currentSurface;
    for(auto it : surface)
    {  
        Point2D  newpoint(it.x(),it.y());
        currentSurface.push_back(newpoint);
    }
    return currentSurface;
  }


 

void BooleanOperations::get2DPolygons(vector<Point3D>& Clipper,vector<Point3D>& Clipped,double z,vector<Point3D>& answer )
{   
   //Creating vector of 2d points for polygon clipping
    vector<Point2D>clipperSurface;
    vector<Point2D>subjectSurface;
    vector<Point2D> clippedSurface;
  
    clipperSurface = getClippingSurface(Clipper);
    subjectSurface = getClippingSurface(Clipped);
  
    //call for suther land algorithm
    SutherLand sutherland; 
    sutherland.clipPolygonAgainstFixed(subjectSurface, clipperSurface, clippedSurface);

    //pushing the clipped polygon output into answer array
    for(auto it:clippedSurface)
    {
        Point3D tempPoint(it.x(),it.y(),z);
        answer.push_back(tempPoint);
    }
}

vector<Point3D> BooleanOperations::getIntersection(Triangulation& T1, Triangulation& T2, string& filePath1, string& filePath2)
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
    startT1 = minZValue(pointsT1); endT1 = maxZValue(pointsT1);
    startT2 = minZValue(pointsT2); endT2 = maxZValue(pointsT2);

    double sliceinterval = objectWidth(pointsT1)/10000;
  
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
                    get2DPolygons(clipper,clipped,z,answer);   
                }           
        }
    }
//  Clipped output
return answer;


}