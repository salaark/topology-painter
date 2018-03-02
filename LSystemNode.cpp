#include "LSystemNode.h"

#define McheckErr(stat,msg)			\
	if ( MS::kSuccess != stat ) {	\
		cerr << msg;				\
		return MS::kFailure;		\
	}

MObject LSystemNode::angle;
MObject LSystemNode::stepsize;
MObject LSystemNode::grammar;
MObject LSystemNode::time;
MObject LSystemNode::outputMesh;
MTypeId LSystemNode::id(0x80000);

void* LSystemNode::creator()
{
	return new LSystemNode;
}

MStatus LSystemNode::initialize()
{
	MFnUnitAttribute unitAttr;
	MFnTypedAttribute typedAttr;
	MFnNumericAttribute numAttr;

	MStatus returnStatus;

	LSystemNode::angle = numAttr.create("angle", "a",
		MFnNumericData::kFloat, 90.0, &returnStatus);
	McheckErr(returnStatus, "ERROR creating LSystemNode angle attribute\n");

	LSystemNode::stepsize = numAttr.create("stepsize", "s",
		MFnNumericData::kFloat, 1.0, &returnStatus);
	McheckErr(returnStatus, "ERROR creating LSystemNode stepsize attribute\n");

	LSystemNode::grammar = typedAttr.create("grammar", "g",
		MFnData::kString, &returnStatus);
	McheckErr(returnStatus, "ERROR creating LSystemNode grammar attribute\n");

	LSystemNode::time = unitAttr.create("time", "tm",
		MFnUnitAttribute::kTime, 0.0, &returnStatus);
	McheckErr(returnStatus, "ERROR creating LSystemNode time attribute\n");

	LSystemNode::outputMesh = typedAttr.create("outputMesh", "out",
		MFnData::kMesh, &returnStatus);
	McheckErr(returnStatus, "ERROR creating LSystemNode output attribute\n");
	typedAttr.setStorable(false);

	returnStatus = addAttribute(LSystemNode::angle);
	McheckErr(returnStatus, "ERROR adding angle attribute\n");

	returnStatus = addAttribute(LSystemNode::stepsize);
	McheckErr(returnStatus, "ERROR adding stepsize attribute\n");

	returnStatus = addAttribute(LSystemNode::grammar);
	McheckErr(returnStatus, "ERROR adding grammar attribute\n");

	returnStatus = addAttribute(LSystemNode::time);
	McheckErr(returnStatus, "ERROR adding time attribute\n");

	returnStatus = addAttribute(LSystemNode::outputMesh);
	McheckErr(returnStatus, "ERROR adding outputMesh attribute\n");

	returnStatus = attributeAffects(LSystemNode::angle,
		LSystemNode::outputMesh);
	McheckErr(returnStatus, "ERROR in attributeAffects\n");

	returnStatus = attributeAffects(LSystemNode::stepsize,
		LSystemNode::outputMesh);
	McheckErr(returnStatus, "ERROR in attributeAffects\n");

	returnStatus = attributeAffects(LSystemNode::grammar,
		LSystemNode::outputMesh);
	McheckErr(returnStatus, "ERROR in attributeAffects\n");

	returnStatus = attributeAffects(LSystemNode::time,
		LSystemNode::outputMesh);
	McheckErr(returnStatus, "ERROR in attributeAffects\n");

	return MS::kSuccess;
}

MStatus LSystemNode::compute(const MPlug& plug, MDataBlock& data)

{
	MStatus returnStatus;

	if (plug == outputMesh) {
		/* Get time */
		MDataHandle timeData = data.inputValue(time, &returnStatus);
		McheckErr(returnStatus, "Error getting time data handle\n");
		MTime time = timeData.asTime();

		/* Get output object */

		MDataHandle outputHandle = data.outputValue(outputMesh, &returnStatus);
		McheckErr(returnStatus, "ERROR getting polygon data handle\n");

		MFnMeshData dataCreator;
		MObject newOutputData = dataCreator.create(&returnStatus);
		McheckErr(returnStatus, "ERROR creating outputData");

		std::string grammarStr = data.inputValue(grammar, &returnStatus).asString().asChar();
		float angleFloat = data.inputValue(angle, &returnStatus).asFloat();
		float stepsizeFloat = data.inputValue(stepsize, &returnStatus).asFloat();

		LSystem lsystem;
		lsystem.loadProgram(grammarStr);
		lsystem.setDefaultAngle(angleFloat);
		lsystem.setDefaultStep(stepsizeFloat);
		std::vector<LSystem::Branch> branches;
		lsystem.process(3, branches);

		MFnMesh meshFS;
		MPointArray points;
		MIntArray faceCounts;
		MIntArray faceConnects;

		for (int i = 0; i < branches.size(); i++) {
			LSystem::Branch& branch = branches.at(i);
			CylinderMesh cylinder(MPoint(branch.first[0], branch.first[1], branch.first[2]),
				MPoint(branch.second[0], branch.second[1], branch.second[2]));
			cylinder.appendToMesh(points, faceCounts, faceConnects);
		}

		MObject newMesh = meshFS.create(points.length(), faceCounts.length(),
			points, faceCounts, faceConnects, newOutputData, &returnStatus);
		outputHandle.set(newOutputData);
		data.setClean(plug);
		McheckErr(returnStatus, "ERROR creating geometry\n");

		// message in scriptor editor
		MGlobal::displayInfo(("LSystemNode Created With Face Count="+std::to_string(faceCounts.length())).c_str());
	}
	else
		return MS::kUnknownParameter;

	return MS::kSuccess;
}
