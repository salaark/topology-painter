curves = {}

def newCurve(newName):
	curves[newName] = {}
	for name in curves:
		curvePos = curveIntersection(name, newName) # compute intersection here
		if name != newName and curvePos:
			curves[name][newName] = curvePos
			curves[newName][name] = curvePos
	for loop in checkLoops():
		makeFace(loop[0], loop[1], loop[2], loop[3]) # make face here

def checkLoops():
	curves = {"curve1": {"curve2":(1,0), "curve4":(0,0)},
			  "curve2": {"curve1":(1,0), "curve3":(1,1)},
			  "curve3": {"curve2":(1,1), "curve4":(0,1)},
			  "curve4": {"curve3":(0,1), "curve1":(0,0)}}
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
				if(set(loop) not in rv):
					rv.append(set(loop))
	return rv

print(checkLoops())