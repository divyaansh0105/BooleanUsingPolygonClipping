#pragma once
#include <vector>

#include "Point2D.h"


class SutherLand
{
	public :

		void clipPolygonAgainstFixed(std::vector<Point2D>& subjectSurface, std::vector<Point2D>& clipperSurface,std::vector<Point2D>& clippedSurface);
		Point2D intersection(Point2D& p1, Point2D& p2, Point2D& q1, Point2D& q2);

};