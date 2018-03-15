#include <maya/MPxCommand.h>
#include <maya/MGlobal.h>
#include <maya/MFnPlugin.h>
#include"TopoSketchCmd.h"



MStatus initializePlugin(MObject obj) {
	
	
	char buffer[2048];
	MFnPlugin pluginFn(obj, "TopoSketchC", "1.0");
	sprintf_s(buffer, 2048, "source \"%s/StartStopTopo.mel\";", (pluginFn.loadPath()).asChar());

	MGlobal::executeCommand(buffer, true);

	MStatus stat;
	stat = pluginFn.registerCommand("TopoSketch", TopoSketch::creator);
	if (!stat)
		stat.perror("registerCommand failed");
	return stat;
}
	MStatus uninitializePlugin(MObject obj)
	{
		MFnPlugin pluginFn(obj);
		MStatus stat; stat = pluginFn.deregisterCommand("TopoSketch");
		if (!stat)
			stat.perror("deregisterCommand failed");
		return stat;
	}
