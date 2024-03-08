#include "../headers/Triangle.h"


Triangle::Triangle(int v1, int v2, int v3,int normalIndex) : mV1(v1), mV2(v2), mV3(v3) ,mNormalIndex(normalIndex){}

Triangle::~Triangle()
{
}
const int Triangle::v1() const
{
    return mV1;
}

const int Triangle::v2() const
{
    return mV2;
}

const int Triangle::v3() const
{
    return mV3;
}

const int Triangle::normalIndex() const
{
    return mNormalIndex;
}