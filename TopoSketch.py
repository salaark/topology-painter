import maya.mel as mel
import maya.cmds as cmds
import math

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

objs = mel.eval("ls -et transform;")

curves = {}

for o in objs:
    if o[0:5] == "curve":
        newCurve(o)

for loop in checkLoops():
	loop = list(loop)
	print("Loop: "+str(loop))
	# Make surface patch
	cmds.boundary(loop[0], loop[1], loop[2], loop[3], order=False)
