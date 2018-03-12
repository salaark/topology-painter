#ifndef TopoSketch_H_
#define TopoSketch_H_

#include <string>
#include <vector>
#include <map>
#include "vec.h"

class TopoSketch
{
public:
    TopoSketch();
    ~TopoSketch() {}
i
    // Set/get inputs
    void setDefaultSubdiv(int subdiv);
    int getDefaultSubdiv() const;

protected:
    void reset();
    int mDfltSubdiv;
};

#endif