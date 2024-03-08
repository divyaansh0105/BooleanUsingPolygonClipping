// STLWriter.h
#pragma once
#include <string>
#include <vector>
#include "Triangle.h"
#include "Triangulation.h"


    class STLWriter
    {
    public:
        // Function to write geometry data to an STL file
        void writeSTLToFile(std::string &filePath, Triangulation &triangulationObj);
    };
