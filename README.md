# Dynamic Nested Cages

This repository contains code for the project *Dynamic Nested Cages* for the course [Geometry Processing](https://github.com/alecjacobson/geometry-processing-csc2520). See [alecjacobson/nested_cages](https://github.com/alecjacobson/nested_cages) for the original algorithm and [simeonkr/nested_cages](https://github.com/simeonkr/nested_cages) for a port that uses a later version of [libigl](http://libigl.github.io/libigl/). Make sure to clone as follows:

	git clone --recursive https://github.com/simeonkr/dynamic_nested_cages.git


## Compile

This code has been mainly tested on Linux. Compiling is done using cmake:

    mkdir build
    cd build
    cmake ..
    make


## Run

From within the `build` directory issue:

    ./gen_cage <method> <mode> <mesh_basename> <initial_cage> <frame_num> [reference_cage]

This will draw `<mesh_basename>_0000.obj` and `<mesh_basename>_<frame_num>.obj` in the viewer, as well as the initial cage, the generated cage, and an optional reference cage for the same frame (which can provided for testing purposes). The generated cage will also be saved as `<mesh_basename>_<frame_num>_gen_cage.obj`.

For example, if the directory `../data/horse-gallop` contains an animation sequence `mesh_0000.obj, mesh_0001.obj, ...` and `../data/cages/horse-gallop_0000_1.obj` is a cage for the 0th frame, then running
 
	./gen_cage mvc i ../data/horse-gallop/mesh ../data/cages/horse-gallop_0000_1.obj 10

will generate a cage `../data/horse-gallop/mesh_0010_gen_cage.obj` for the 10th (0-indexed) frame. 

The currently supported `<method>`s are `s` (algorithm based on per-vertex normals), and `mvc` (Mean value coordinates).

The `<mode>` is either `d` (direct propagation) or `i` (incremental propagation).

To interact with the viewer, use the up and down keys to select a mesh; the viewer commands can be used on the selected mesh (pressing `t`, for example, will hide the faces of the selected mesh).

The code can be used as a library: an interface is provided through the header `extrap_cage.h`.

## Dependencies

The only dependencies are stl, eigen, [libigl](http://libigl.github.io/libigl/) and
the dependencies of `igl::opengl::glfw::Viewer` (the viewer is not required if using code as a library).

libigl can be installed using git via:

    git clone https://github.com/libigl/libigl.git
    cd libigl/
    git submodule update --init --recursive
    cd ..

