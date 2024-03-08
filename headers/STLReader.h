#pragma once

#include <map>
#include <string>
#include <vector>
#include "Triangulation.h"

class STLReader
{
public:
    // Function to read data from an STL file and populate a Triangulation object
    void readSTL(std::string &filePath, Triangulation &triangulation);
    void Coordinates(std::string &filePath, Triangulation &triangulation, vector<Point3D> &Points);
};
