#pragma once
#include <vector>
#include <Point3D.h>
#include <Triangulation.h>
class Surface {
public:
	bool isEqual(Point3D P1, Point3D P2);
	void getPolygonSurface(Triangulation& triangulation, std::vector<Point3D>& surfacePoint);
};