#include "stdafx.h"

#include "../headers/STLWriter.h"
#include "../headers/Triangle.h"
#include <fstream>
#include <stdexcept>
#include <iostream>

using namespace std;

void STLWriter::writeSTLToFile(std::string& filePath, Triangulation& triangulationObj)
{
    std::ofstream outFile(filePath);

    if (!outFile.is_open())
    {
        cout << "Error while opening the file for writing." << endl;
    }

    // Write triangles to the STL file
    vector<Point3D>& points = triangulationObj.uniquePoints();
    vector<Triangle>& triangles = triangulationObj.triangles();

    for (const Triangle& triangle : triangles)
    {
        // Write each vertex of the triangle to the STL file
        outFile << points[triangle.v1()].x() << " " << points[triangle.v1()].y() << " " << points[triangle.v1()].z() << endl;
        outFile << points[triangle.v2()].x() << " " << points[triangle.v2()].y() << " " << points[triangle.v2()].z() << endl;
        outFile << points[triangle.v3()].x() << " " << points[triangle.v3()].y() << " " << points[triangle.v3()].z() << endl;
    }
    cout << "\nData written successfully" << endl;
    // Close the STL file
    outFile.close();
}

void STLWriter::writevector(vector<Point3D>& answer, Triangulation& triangulationObj)
{
    // Write triangles to the STL file
    vector<Point3D>& points = triangulationObj.uniquePoints();
    vector<Triangle>& triangles = triangulationObj.triangles();

    for (const Triangle& triangle : triangles)
    {
        // Write each vertex of the triangle to answer
        answer.push_back(points[triangle.v1()]);
        answer.push_back(points[triangle.v2()]);
        answer.push_back(points[triangle.v3()]);
    }
    cout << "\nData written successfully" << endl;
    // Close the STL file
    cout << "\nansd=" << answer.size();
}