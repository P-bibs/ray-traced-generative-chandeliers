# ray-traced-generative-chandeliers

This project consists of two components: the scene generator and the renderer

# Generator

This exists in the `generation` subdirectory. The entry point is `main.py` which can be invoked as follows:
```bash
python main.py <output file> <chandelier number>
# Example: python main.py ../scenes/chand.xml 1
```
There are currently 8 different chandeliers to pick from. You can add your own by adding a function to `components/Chandelier.py` and then adding that function to the list of chandeliers in `components/Chandelier.py::Chandelier.__init__()`. There are a variety of components to choose from when creating your chandeliers. They're all listed below, but a better way to get a sense of the api is to look at the preset functions in `Chandelier.py` and read through the comments explaining how different Chandelier presets are made.

## Components
### Layers
These are the layers of the chandelier. You can see examples in `results/components/polygonLayer.png`.

### Chains
These are connecting chains of various shapes (diamond, sphere) and arrangements (constant radius, tapered). 

### Fixtures
There is currently only one fixture: a candle.

### Primitives
You can also interact with scenefile primitives directly. These are in `generation/primitives`. This includes basic shapes like cubes, spheres, and cones as well as transblocks and trees for composing objects.

### Lighting

To add lights to the scene, you can use `context::appendLight()`. This takes either a `context::PointLight` object or a `context::DirectionalLight` object.

### Global data

The global data (lights, coefficients, camera, etc.) is calculated and outputted in `generation/components/Scene.py`. Much of it is read from the global settings object in `generation/settings.py`.

## Rendering

This contains an extended version of the project code. The changes are primarily in `rendering/ray/RayTracer.cpp`. This includes refraction, transparency, bloom, and environment maps.


## Deviations from project plan

We decided to go with parameterized chandlers instead of randomly generated chandeliers, since it gave us more control of the final output and made it easier to get good results. In theory, it would be easy to extend this to randomness by randomly selecting from our list of materials, chains, layers, etc., but it might take a long time to tune heuristics that give good results.