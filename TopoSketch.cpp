#include "TopoSketch.h"    
#include <fstream>
#include <stack>

#pragma warning(disable : 4244)
#pragma warning(disable : 4290)
#include "matrix.h"

#define Rad2Deg 57.295779513082320876798154814105
#define Deg2Rad 0.017453292519943295769236907684886

TopoSketch::TopoSketch() : mDfltSubdiv(3)
{
}

void TopoSketch::setDefaultSubdiv(int subdiv)
{
    mDfltSubdiv = subdiv;
}

int TopoSketch::getDefaultSubdiv() const
{
    return mDfltSubdiv;
}

void TopoSketch::reset()
{
}
