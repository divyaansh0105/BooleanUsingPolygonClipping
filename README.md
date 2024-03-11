# BooleanUsingPolygonClipping
 
BooleanUsingPolygonClipping is a project aimed at finding the common portion between multiple STL files using the Sutherland-Hodgeman polygon clipping algorithm.
 
## Overview
 
The project takes multiple STL files from the user and identifies the common portion between them. It achieves this by first reading the triangle data from the STL files and storing them in a triangulated format. Then, the STL files are divided into multiple surfaces, and these surfaces are converted into a 2D form by considering only the x and y axes, as the z-axis values change when dividing the surface.
 
## Algorithm
 
The Sutherland-Hodgeman polygon clipping algorithm is applied to the 2D representations of the surfaces. This algorithm checks if two polygons intersect and finds the common intersecting vertices. These common intersecting vertices are then stored in a vector.
 
## Implementation
 
The project is implemented in C++ and utilizes OpenGL for rendering the common intersecting portion.
 
## Approach
 
1. **Input STL Files**: The user provides STL files representing the 3D objects.

2. **Triangulation**: The project reads the triangle data from the STL files and triangulates the objects.

3. **Surface Division**: The STL files are divided into multiple surfaces, taking into account the changes along the z-axis.

4. **2D Conversion**: Each surface is converted into a 2D representation, considering only the x and y axes.

5. **Polygon Clipping**: The Sutherland-Hodgeman polygon clipping algorithm is applied to the 2D representations to identify intersecting polygons.

6. **Intersection Calculation**: The common intersecting vertices are determined, and the shared portion between the objects is extracted.

7. **Rendering**: Finally, the common intersecting portion is rendered using OpenGL for visualization.
 
## User Interface
 
The user interface consists of the following elements:
 
1. **Load STL Files Buttons**: Two buttons are provided to load the STL files representing the two objects.
 
2. **Checkbox Options**:

   - **Left Portion (First STL)**: Checkbox to visualize the left portion of the first STL file.

   - **Right Portion (Second STL)**: Checkbox to visualize the right portion of the second STL file.

   - **Common Portion**: Checkbox to visualize the common portion between the two STL files.
 
## Usage
 
To use the project:

1. Provide the STL files of the objects you want to analyze.

2. Run the program.

3. The common intersecting portion will be identified and rendered using OpenGL.
 
## UI ScreenShot 

## Output ScreenShot