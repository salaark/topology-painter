#ifndef CreateTopoSketchCmd_H_
#define CreateTopoSketchCmd_H_

#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>
#include <maya/MGlobal.h>
#include <maya/MArgDatabase.h>
#include <string>
#include "TopoSketch.h"

class TopoSketchCmd : public MPxCommand
{
public:
	const char* g_SubdivLong = "-subdiv";
	const char* g_Subdiv = "-s";

	TopoSketchCmd();
    virtual ~TopoSketchCmd();
    static void* creator() { return new TopoSketchCmd(); }
    MStatus doIt( const MArgList& args );
};

#endif