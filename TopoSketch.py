import maya.mel as mel
import maya.cmds as cmds

def newCurve(newName):
	curves[newName] = {}
	for name in curves:
		if name == newName:
			continue
		# compute curve intersection
		curvePos = cmds.curveIntersect(newName, name, tol=0.1)
		if curvePos:
			curvePos = float(curvePos.split(" ")[0])
			intPos = cmds.pointOnCurve(newName, pr=curvePos, p=True)
			print("Intersection: "+str(intPos))
			curves[name][newName] = intPos
			curves[newName][name] = intPos

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

objs = mel.eval("ls -et transform;")

curves = {}

for o in objs:
    if o[0:5] == "curve":
        newCurve(o)

for loop in checkLoops():
	loop = list(loop)
	print("Loop: "+str(loop))
	# Make surface patch
	cmds.squareSurface(loop[0], loop[1], loop[2], loop[3])
