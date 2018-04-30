import maya.mel as mel
import maya.cmds as cmds
import math

def mergePatches():
	patches = []
	mesh = ""
	surface = "boundaryMerged"

	mel.eval("delete -all -constructionHistory;")
	count = mel.eval('global int $SAGsubdivisions; $temp = $SAGsubdivisions;')
	for o in mel.eval("ls -et transform;"):
	    if o.startswith("boundary"):
	        patches.append(str(o))
	    elif o.startswith("quadMesh"):
	    	mesh = str(o)
	if len(patches) > 0:
		cmds.rename(patches[0], surface)
		if len(patches) > 1:
			# Merge nurbs patches using attachSurface
			cmds.rename(patches[0], surface)
			for p in range(1, len(patches)):
			    surface = str(cmds.attachSurface(surface, patches[p], rpo=True)[0])
			    cmds.delete(patches[p])
		if mesh != "":
			cmds.delete(mesh)
		mel.eval("nurbsToPolygonsPref -f 0 -pc "+str(count)+";")
		cmds.rebuildSurface(surface, po=1, rpo=False, n="quadMeshResult")
		cmds.select("quadMeshResult", r=True)
		mel.eval("dR_conform;")
		mel.eval("dR_DoCmd(\"conform\");")

# mergePatches()

def sewPatches():
	patches = []
	for o in mel.eval("ls -et transform;"):
	    if o.startswith("boundary"):
	        patches.append(str(o))
	if len(patches) > 1:
		# Sew patches by selecting boundary edges and using polySewEdge
		cmds.rename(patches[0], "boundaryMerged0")
		for p in range(1, len(patches)):
		    edge = cmds.polyEvaluate("boundaryMerged"+str(p-1), e=True)
		    cmds.polyUnite("boundaryMerged"+str(p-1), patches[p], n="boundaryMerged"+str(p))
		    cmds.polySelect("boundaryMerged"+str(p), eb=0)
		    cmds.polySelect("boundaryMerged"+str(p), eb=edge, add=True)
		    sewcmd = "polySewEdge -t 1 -tx 1 -ws 1 -ch 1 "
		    for e in mel.eval("ls -sl"):
		    	sewcmd += e+" "
		    sewcmd += ";"
		    print(sewcmd)
		    mel.eval(sewcmd)

def curveInt(curve1, curve2, tol=0.2, num=45):
	for a in range(0, num):
		for b in range(0, num):
			p1 = cmds.pointOnCurve(curve1, pr=a/float(num), p=True)
			p2 = cmds.pointOnCurve(curve2, pr=b/float(num), p=True)
			dist = math.sqrt((p1[0]-p2[0])**2+(p1[1]-p2[1])**2+(p1[2]-p2[2])**2)
			if dist <= tol:
				return [(p1[0]+p2[0])/2.0, (p1[1]+p2[1])/2.0, (p1[2]+p2[2])/2.0]

def newCurve(newName):
	curves[newName] = {}
	for name in curves:
		if name == newName:
			continue
		# compute curve intersection
		curvePos = curveInt(newName, name, tol=0.1)
		if curvePos:
			#curvePos = float(curvePos.split(" ")[0])
			#intPos = cmds.pointOnCurve(newName, pr=curvePos, p=True)
			print("Intersection: "+str(curvePos))
			curves[name][newName] = curvePos
			curves[newName][name] = curvePos

def checkLoops():
	"""
	curves = {"curve1": {"curve2":(1,0), "curve4":(0,0)},
			  "curve2": {"curve1":(1,0), "curve3":(1,1)},
			  "curve3": {"curve2":(1,1), "curve4":(0,1)},
			  "curve4": {"curve3":(0,1), "curve1":(0,0)}}
	"""
	rv = []
	for start in curves:
		loops = {start: [start]}
		stack = [start]
		for i in range(0,4):
			newstack = []
			for curr in stack:
				for insx in curves[curr]:
					if insx not in loops[curr]:
						loops[insx] = loops[curr][:]
						loops[insx].append(insx)
						newstack.append(insx)
			stack = newstack
		for curr in loops:
			if len(loops[curr]) == 4:
				loop = loops[curr]
				if loop[3] in curves[loop[0]] and set(loop) not in [set(r) for r in rv]:
					rv.append(loop)
	return rv

curves = {}
def makePatches():
	objs = mel.eval("ls -et transform;")

	for o in objs:
	    if o[0:5] == "curve":
	        newCurve(o)

	for loop in checkLoops():
		loop = list(loop)
		print("Loop: "+str(loop))
		# Make surface patch
		cmds.boundary(loop[0], loop[1], loop[2], loop[3], order=False)

# makePatches()
