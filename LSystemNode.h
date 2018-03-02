#pragma once
#ifndef _LSystemNode_
#define _LSystemNode_

#include <maya/MTime.h>
#include <maya/MFnMesh.h>
#include <maya/MPoint.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatPointArray.h>
#include <maya/MIntArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>

#include <maya/MPxNode.h>
#include <maya/MObject.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MFnMeshData.h>
#include <maya/MIOStream.h>
#include <maya/MGlobal.h>

#include "LSystem.h"
#include "cylinder.h"

class LSystemNode : public MPxNode
{
public:
	LSystemNode() {};
	virtual 		~LSystemNode() {};
	virtual MStatus compute(const MPlug& plug, MDataBlock& data);
	static  void*	creator();
	static  MStatus initialize();

	static MObject  angle;
	static MObject  stepsize;
	static MObject  grammar;
	static MObject	time;
	static MObject	outputMesh;
	static MTypeId	id;
};
#endif
