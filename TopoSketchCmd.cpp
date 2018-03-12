#include "TopoSketchCmd.h"

#include <list>
TopoSketchCmd::TopoSketchCmd() : MPxCommand()
{
}

TopoSketchCmd::~TopoSketchCmd() 
{
}

MStatus TopoSketchCmd::doIt( const MArgList& args )
{
	// TopoSketchCmd -s 3

	// default values for flag command arguments
	int subdiv = 3;
	MStatus stat;

	// create and use parser
	MSyntax syn;
	syn.addFlag(g_Subdiv, g_SubdivLong, MSyntax::kDouble);

	MArgDatabase parser(syn, args, &stat);
	if (stat != MS::kSuccess) {
		MGlobal::displayError("TopoSketchCmd: Invalid Syntax!");
		return stat;
	}
	
	if (parser.isFlagSet(g_Subdiv))
		parser.getFlagArgument(g_Subdiv, 0, subdiv);
	
	TopoSketch toposk;
	toposk.setDefaultSubdiv(subdiv);

	std::pair<vec3, vec3> branch = std::pair<vec3, vec3>(vec3(0,0,0),vec3(1,1,1));
	MString curve = ("curve -d 1 -p " + std::to_string(branch.first[0]) + " " + std::to_string(branch.first[1]) + " "
		+ std::to_string(branch.first[2]) + " -p " + std::to_string(branch.second[0]) + " " + std::to_string(branch.second[1]) + " "
		+ std::to_string(branch.second[2]) + " -k 0 -k 1 -name curve1;").c_str();
	MString select = "select -r nurbsCircle1 curve1;";
	MString extrude = "extrude -ch true -rn false -po 1 -et 2 -ucp 1 -fpt 1 -upn 1 -rotation 0 -scale 1 -rsp 1 nurbsCircle1 curve1;";

	// execute commands
	MGlobal::executeCommand(curve);
	MGlobal::executeCommand(select);
	MGlobal::executeCommand(extrude);

	// message in scriptor editor
	MGlobal::displayInfo("TopoSketchCmd Complete!");

    return MStatus::kSuccess;
}

