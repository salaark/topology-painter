#ifndef CreateLSystemCmd_H_
#define CreateLSystemCmd_H_

#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>
#include <maya/MGlobal.h>
#include <maya/MArgDatabase.h>
#include <string>
#include "LSystem.h"

class LSystemCmd : public MPxCommand
{
public:
	const char* g_StepSizeLong = "-stepSize";
	const char* g_StepSize = "-s";
	const char* g_AngleLong = "-angle";
	const char* g_Angle = "-a";
	const char* g_GrammarLong = "-grammar";
	const char* g_Grammar = "-g";
	const char* g_IterationsLong = "-iterations";
	const char* g_Iterations = "-i";

	LSystemCmd();
    virtual ~LSystemCmd();
    static void* creator() { return new LSystemCmd(); }
    MStatus doIt( const MArgList& args );
};

#endif