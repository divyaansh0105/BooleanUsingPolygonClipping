#include "stdafx.h"

#include <vector>
#include "ClippingAlgorithm.h"

// Sutherland-Hodgman algorithm for polygon clipping against a fixed polygon

void  ClippingAlgorithm::clip(std::vector<Point2D>& subjectPolygon, std::vector<Point2D>& clipPolygon, std::vector<Point2D>& answer)
{
    std::vector<Point2D> output = subjectPolygon;
    for (int i = 0; i < clipPolygon.size(); ++i)
    {
        int k = (i + 1) % clipPolygon.size();
        std::vector<Point2D> input = output;
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

    for (auto it : output)
    {
        answer.push_back(it);

    }
}


// Helper function to calculate the intersection point of two lines
Point2D ClippingAlgorithm::intersection(Point2D& p1, Point2D& p2, Point2D& q1, Point2D& q2)
{
    double x1 = p1.x(), y1 = p1.y();
    double x2 = p2.x(), y2 = p2.y();
    double x3 = q1.x(), y3 = q1.y();
    double x4 = q2.x(), y4 = q2.y();

    double denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    double x = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / denom;
    double y = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / denom;

    Point2D point(x, y);
    return point;
}


