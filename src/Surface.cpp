#include <stdafx.h>
#include "Surface.h"


bool Surface::isEqual(Point3D P1, Point3D P2)
{
    bool result;

    result = ((P1.x() == P2.x()) && (P1.y() == P2.y()) && (P1.z() == P2.z()));

    return result;
}

void Surface::getPolygonSurface(Triangulation& triangulation, vector<Point3D>& surfacePoint)
{
    std::vector<Point3D>& points = triangulation.uniquePoints();
    std::vector<Triangle>& triangles = triangulation.triangles();
    std::vector<Point3D>& normals = triangulation.uniqueNormals();

    int targetNormalIndex;

    // Find unique normal indices
    for (const Triangle& triangle : triangles)
    {
        Point3D checkNormal;
        checkNormal = normals[triangle.normalIndex()];

        //if the normal is on z axis then extract the normal to get 2d surfaces
        if (checkNormal.z() != 0)
        {
            targetNormalIndex = triangle.normalIndex();
        }
    }

    std::vector<int> triangleIndices;
    std::vector<Point3D> seenIndices;
    int count = 0;
    for (const Triangle& triangle : triangles)
    {
        //check for the target noraml
        int normalIndex = triangle.normalIndex();
        if (targetNormalIndex == normalIndex)
        {
            if (seenIndices.size() == 0)
            {

                seenIndices.push_back(points[triangle.v1()]);
                seenIndices.push_back(points[triangle.v2()]);
                seenIndices.push_back(points[triangle.v3()]);
            }
            else
            {
                vector<Point3D> trianglePoints;
                Point3D newPoint1 = points[triangle.v1()];
                Point3D newPoint2 = points[triangle.v2()];
                Point3D newPoint3 = points[triangle.v3()];
                trianglePoints.push_back(newPoint1);
                trianglePoints.push_back(newPoint2);
                trianglePoints.push_back(newPoint3);

                for (auto it : trianglePoints)
                {
                    bool flag = false;
                    for (auto jt : seenIndices)
                    {
                        if (isEqual(it, jt))
                        {
                            flag = true;
                            break;
                        }
                        else
                        {

                        }
                    }

                    if (flag == false)
                    {
                        seenIndices.push_back(it);

                    }

                }
            }

        }
    }
    for (auto element : seenIndices)
    {
        surfacePoint.push_back(element);
    }
}