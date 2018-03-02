#include "LSystemCmd.h"

#include <list>
LSystemCmd::LSystemCmd() : MPxCommand()
{
}

LSystemCmd::~LSystemCmd() 
{
}

MStatus LSystemCmd::doIt( const MArgList& args )
{
	// LSystemCmd -s 1.0 -a 90.0 -g "snow" -i 5

	// default values for flag command arguments
	double stepsize = 1.0f;
	double angle = 90.0f;
	MString grammar = "snow";
	int iterations = 3;
	MStatus stat;

	// create and use parser
	MSyntax syn;
	syn.addFlag(g_StepSize, g_StepSizeLong, MSyntax::kDouble);
	syn.addFlag(g_Angle, g_AngleLong, MSyntax::kDouble);
	syn.addFlag(g_Grammar, g_GrammarLong, MSyntax::kString);
	syn.addFlag(g_Iterations, g_IterationsLong, MSyntax::kLong);

	MArgDatabase parser(syn, args, &stat);
	if (stat != MS::kSuccess) {
		MGlobal::displayError("LSystemCmd: Invalid Syntax!");
		return stat;
	}
	
	if (parser.isFlagSet(g_StepSize))
		parser.getFlagArgument(g_StepSize, 0, stepsize);

	if (parser.isFlagSet(g_Angle))
		parser.getFlagArgument(g_Angle, 0, angle);

	if (parser.isFlagSet(g_Grammar))
		parser.getFlagArgument(g_Grammar, 0, grammar);

	if (parser.isFlagSet(g_Iterations))
		parser.getFlagArgument(g_Iterations, 0, iterations);
	
	LSystem lsystem;
	lsystem.loadProgramFromString(grammar.asChar());
	lsystem.setDefaultAngle(angle);
	lsystem.setDefaultStep(stepsize);
	std::vector<LSystem::Branch> branches;
	lsystem.process(iterations, branches);

	for (int i = 0; i < branches.size(); i++) {
		// create commands
		LSystem::Branch branch = branches.at(i);
		MString curve = ("curve -d 1 -p " + std::to_string(branch.first[0]) + " " + std::to_string(branch.first[1]) + " "
			+ std::to_string(branch.first[2]) + " -p " + std::to_string(branch.second[0]) + " " + std::to_string(branch.second[1]) + " "
			+ std::to_string(branch.second[2]) + " -k 0 -k 1 -name curve1;").c_str();
		MString select = "select -r nurbsCircle1 curve1;";
		MString extrude = "extrude -ch true -rn false -po 1 -et 2 -ucp 1 -fpt 1 -upn 1 -rotation 0 -scale 1 -rsp 1 nurbsCircle1 curve1;";

		// execute commands
		MGlobal::executeCommand(curve);
		MGlobal::executeCommand(select);
		MGlobal::executeCommand(extrude);
	}

	// message in scriptor editor
	MGlobal::displayInfo("LSystemCmd Complete!");

    return MStatus::kSuccess;
}

