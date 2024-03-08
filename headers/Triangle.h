#pragma once
#include "Point3D.h"


    class Triangle {
public:
    Triangle(int v1, int v2, int v3,int normalIndex);
    ~Triangle();
    const int v1() const;
    const int v2() const;
    const int v3() const;
    const int normalIndex() const;
private:
    int mV1;
    int mV2;
    int mV3;
    int mNormalIndex;
};
