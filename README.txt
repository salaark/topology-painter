Load the plugin and click TopoSketch->Create Quad Patches. Draw strokes corresponding to patch boundary edges. Select 4 adjacent curves and click Generate Quad Patch to generate each patch (which are automatically merged). Adjust the subdivision level using the slider. If desired, click Attach to Parent Mesh to have the patch merged into the mesh.


Final MEL Script: New.mel
Final C++ Plugin: TopoSketch.mll
Final Python Script: TopoSketch.py
The above files are in TopoSketch/Debug

Load Plugin by loading TopoSketch.mll

Note: If the TopoSketch.py is not loaded automatically(patches cannot be created), run the contents of TopoSketch.py in the Python Script Editor in Maya.		