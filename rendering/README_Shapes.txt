# Shapes

## Design Decisions
Each shape is constructed of a number of `Surface`s, where Surface is an interface for defining 2d surfaces that make up the 3d shapes. `Surface` has only one require method: `getVerticesAndNormals`. This method returns a list of vertices and normals in the format that `OpenGLShape` expects. This means that in `Cube`, `Cylinder`, etc., we can just instantiate a bunch of shapes and then call their `getVerticesAndNormals` methods to get a bunch of lists of floats, then append those lists together and pass them to `OpenGLShape`.

Here's a rundown of different surfaces:
`IsocelesTriangle`: A triangle that is tesselated from base to tip, with two axes that can be subdivided (doesn't actually have to be isoceles)
`Square`: two isoceles triangles paired together to make a quadrangle (doesn't actually have to be a square)
`SimpleTriangle`: similar to `IsocelesTriangle`, but only subdivided along one axis. Used as the sides of the cylinder.
`SimpleRectangle`: a rectangle that can be subdivided along one axis similar to `SimpleTriangle`. Used as the sides of the cylinder.
`Disc`: a bunch of `SimpleTriangles` placed side-by-side radially to make a disc. Used as the base of the cone and cylinder

## Formatting
I'm aware that some of the formatting choices are odd (e.g.: commas to start the line in member initializer lists). I used a clang-format style preset, so these are deliberate (albeit wacky) choices.

## Known bugs
None

## Extra Credit
None