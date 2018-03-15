Sagar-

The MEL Command to run the plugin is "TopoSketch".
The MEL Script "StartStopTopo.mel" when loaded, creates two global procedures "startTopoSketch", and "stopTopoSketch".

startTopoSketch - 

This first checks the number of objects in the scene selected by the user.
If only one object is selected, this procedure then makes that mesh live, and starts the CV Curve Tool operation to enable drawing curves on the mesh.
Note: It is suggested you enable wireframe mode when doing this.

If more or no objects are selected, it throws an error.

stopTopoSketch - stops the topo sketch operation. 

This first checks if any objects are selected. If they are, it deselects all the objects and then invokes Make Not Live. 

Once the Script is Loaded, type "startTopoSketch" in the MEL command line or script editor to start sketching on the live mesh.

When you're done sketching, type "stopTopoSketch" in the MEL Command Line or Script Editor to stop sketching, deselect all objects, and make the mesh Not Live. 
