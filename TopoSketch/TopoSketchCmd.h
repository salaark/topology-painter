#pragma once

#ifndef CreateTopoSketch_H_
#define CreateTopoSketch_H_

#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>
#include <maya/MGlobal.h>
#include <maya/MArgDatabase.h>
#include <string>


class TopoSketch : public MPxCommand
{
public:
	

	
	
	static void* creator() { return new TopoSketch(); }
	MStatus doIt(const MArgList& args);
};

#endif