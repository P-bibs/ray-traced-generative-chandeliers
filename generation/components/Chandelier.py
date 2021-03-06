import sys
from settings import settings
import materials
from common import SceneComponent
from components.Layers import PolygonLayer
from components.Chain import CylinderChain, DiamondChain, TaperedSphereChain, TiltedSphereChain, DemiCurve, UnevenDemiCurve
from shapes.primitives import Cube
from shapes.transblock import TransBlock
from shapes.tree import Tree
from components.Fixtures import Candle
import context
from context import DirectionalLight, PointLight

def connect_points_with_chains(points1, points2, chain_maker):
    """
    Given two sets of points and a function that takes in two points and makes a chain
    between them, returns a list of chains connecting the two sets of points. Let `n`
    be the size of the smaller set of points. There will be `n` total chains
    """
    if len(points1) > len(points2):
        points1, points2 = points2, points1

    chains = []
    for i, point in enumerate(points1):
        j = ((i / len(points1)) * len(points2) + 1) % len(points2)
        chains.append(chain_maker(point, points2[int(j)]))

    return chains

def chandelier1():
    settings.camera_position = [0, 1, -10]
    settings.camera_focus = [0, 0.1, 1]

    # Three layers. The top and bottom have radius 1 and the middle has radius 2.
    # They increase in subdivisions as you go downward
    layers = [
        PolygonLayer(materials.silver, (0, 3, 0),  1, 5, 0.1),
        PolygonLayer(materials.silver, (0, 2, 0),  2, 15, 0.15),
        PolygonLayer(materials.silver, (0, 1, 0),  1, 30, 0.1),
    ]
    # Get the coordinates of the vertexes of the layers, so we can attach points
    points1 = layers[0].get_attachment_points_world_space()
    points2 = layers[1].get_attachment_points_world_space()
    points3 = layers[2].get_attachment_points_world_space()

    # Generate chains connecting layer1<->layer2 and layer2<->layer3
    chains = connect_points_with_chains(points1, points2,  lambda pt1, pt2: TiltedSphereChain(materials.silver, pt1, pt2, 12))
    chains = chains + connect_points_with_chains(points2, points3, lambda pt1, pt2: TiltedSphereChain(materials.silver, pt1, pt2, 12))

    # Add narrow glass cylinders hanging from the vertices of layers 2 and 3
    dangles = [CylinderChain(materials.glass, p2, 2, 0.1) for p2 in points2]
    dangles = dangles + [CylinderChain(materials.glass, p3, 2, 0.1) for p3 in points3]

    # Add a top chain that connects to each vertex of the top layer
    for point in points1:
        chains.append(
            TiltedSphereChain(materials.bronze, point, (0, 5, 0), 12)
        )
    chains.append(
        TiltedSphereChain(materials.bronze, (0, 5, 0), (0, 7, 0), 10)
    )

    # Add candles at all vertices of the middle layer
    fixtures = []
    for point in points2:
        fixtures.append(Candle(materials.silver, point, 0.5))

    return layers + chains + dangles + fixtures

def chandelier2():
    # Three vertically stacked layers, increasing in radius and subdivisions
    # as you move downward
    layer1 = PolygonLayer(materials.silver, (0, 3, 0),  1, 5, 0.05)
    layer2 = PolygonLayer(materials.silver, (0, 2, 0),  2, 15, 0.1)
    layer3 = PolygonLayer(materials.silver, (0, 1, 0),  2.5, 30, 0.05)

    points1 = layer1.get_attachment_points_world_space()
    points2 = layer2.get_attachment_points_world_space()
    points3 = layer3.get_attachment_points_world_space()

    # Generate curves connecting layer1<->layer2 and sphere chain connecting layer2<->layer3
    chains = []
    chains += connect_points_with_chains(points1, points2, lambda pt1, pt2: UnevenDemiCurve(materials.silver, pt1, pt2, 6, 0.075, 0.2))
    chains += connect_points_with_chains(points2, points3, lambda pt1, pt2: TiltedSphereChain(materials.glass, pt1, pt2, 12))

    # Add hanging glass cylinders to layer 3
    dangling_chains = []
    for point in points3:
        dangling_chains.append(CylinderChain(materials.glass, point, 2, 0.1))

    # Add candles to layer 2
    candles = []
    for point in points2:
        candles.append(Candle(materials.gold, point, 0.25))

    # Add a top chain that connects to each vertex of the top layer
    top_chain = []
    for point in points1:
        top_chain.append(
            TiltedSphereChain(materials.glass, point, (0, 4, 0), 12)
        )
    top_chain.append(
        TiltedSphereChain(materials.glass, (0, 4, 0), (0, 7, 0), 20)
    )

    return [layer1, layer2, layer3] + chains + dangling_chains + candles + top_chain

def chandelier3():
    """cheaper version of chandelier 2"""
    settings.camera_position = (0, 0, 0)
    settings.camera_focus = (0, 0, 0)

    layer1 = PolygonLayer(materials.silver, (0, 3, 0),  1, 3, 0.05)
    layer2 = PolygonLayer(materials.silver, (0, 2, 0),  2, 6, 0.1)

    points1 = layer1.get_attachment_points_world_space()
    points2 = layer2.get_attachment_points_world_space()

    chains = []
    chains += connect_points_with_chains(points1, points2, lambda pt1, pt2: UnevenDemiCurve(materials.silver, pt1, pt2, 3, 0.075, 0.2))


    candles = []
    for point in points2:
        candles.append(Candle(materials.gold, point, 0.4))

    top_chain = []
    for point in points1:
        top_chain.append(
            TiltedSphereChain(materials.glass, point, (0, 4, 0), 6)
        )
    top_chain.append(
        TiltedSphereChain(materials.glass, (0, 4, 0), (0, 7, 0), 10)
    )

    return [layer1, layer2] + chains + candles + top_chain

def chandelier4():
    # A vertical sphere chain that descends downward. From the bottom chain, four curves
    # extend out radially. At the end of each curve is a candle
    return [
            DemiCurve(materials.silver, (0, 0, 0), (1, 0, 1), 8, 0.1),
            DemiCurve(materials.silver, (0, 0, 0), (1, 0, -1), 8, 0.1),
            DemiCurve(materials.silver, (0, 0, 0), (-1, 0, 1), 8, 0.1),
            DemiCurve(materials.silver, (0, 0, 0), (-1, 0, -1), 8, 0.1),
            Candle(materials.silver, (1, 0, 1), 0.5),
            Candle(materials.silver, (1, 0, -1), 0.5),
            Candle(materials.silver, (-1, 0, 1), 0.5),
            Candle(materials.silver, (-1, 0, -1), 0.5),
            TiltedSphereChain(materials.glass, (0, 0, 0), (0, 5, 0), 24),
        ]

def chandelier5():
    # Three vertically stacked layers of constant radius, but more subdivisions on the middle layer
    layer1 = PolygonLayer(materials.silver, (0, 3, 0),  0.5, 6,  0.05)
    layer2 = PolygonLayer(materials.silver, (0, 2, 0),  0.5, 8,  0.05)
    layer3 = PolygonLayer(materials.silver, (0, 0, 0),  0.5, 6, 0.05)

    points1 = layer1.get_attachment_points_world_space()
    points2 = layer2.get_attachment_points_world_space()
    points3 = layer3.get_attachment_points_world_space()

    # A copy of the vertex points of the second layer, but extended outward radially
    points2_extended = [ (pt[0] * 5, pt[1], pt[2] * 5) for pt in points2 ]

    # Connect a downward swooping curve from each point of layer 2 to layer 2 extended
    chains = []
    chains += connect_points_with_chains(points2, points2_extended, lambda pt1, pt2: DemiCurve(materials.silver, pt1, pt2, 6, 0.075))

    # Add a dangling diamon chain from the bottom of the ends of these curves
    dangling_chains = []
    for point in points2_extended:
        dangling_chains.append(DiamondChain(materials.glass, point, 6, 0.3, 0.05))

    # Place candles on the end of all the curves that we extended from layer 2
    candles = []
    for point in points2_extended:
        candles.append(Candle(materials.gold, point, 0.4))

    # Add a glass sphere chain running through the center of each layer from top to bottom.
    top_chain = [
        TiltedSphereChain(materials.glass, (0, 7, 0), (0, 0, 0), 30)
    ]
    
    # Add a sphere chains that connect from the main top_chain to each of the layers
    connecting_chains = []
    for point in points1:
        connecting_chains.append(
            TiltedSphereChain(materials.glass, point, (0, 3.4, 0), 6)
        )
    for point in points3:
        connecting_chains.append(
            TiltedSphereChain(materials.glass, point, (0, 0.4, 0), 6)
        )


    return [layer1, layer2, layer3] + chains + dangling_chains + candles + top_chain + connecting_chains

def chandelier6():
    """
    lots of handing glass cylinders
    """
    context.append_light(
        PointLight(context.get_id(), (.224, .157, .55), (0, 3, 1))
    )

    # Stacked vertical layers of decreasing radius going downward
    layer1 = PolygonLayer(materials.silver, (0, 3, 0),  1.5, 48,  0.05)
    layer2 = PolygonLayer(materials.silver, (0, 2, 0),  1.0, 32,  0.05)
    layer3 = PolygonLayer(materials.silver, (0, 1, 0),  0.5, 16,  0.05)

    points1 = layer1.get_attachment_points_world_space()
    points2 = layer2.get_attachment_points_world_space()
    points3 = layer3.get_attachment_points_world_space()

    # Hang glass cylinders from each of them
    dangling_chains = []
    for point in points1:
        dangling_chains.append(
            CylinderChain(materials.glass, point, 1, 0.1)
        )
    for point in points2:
        dangling_chains.append(
            CylinderChain(materials.glass, point, 1, 0.1)
        )
    for point in points3:
        dangling_chains.append(
            CylinderChain(materials.glass, point, 1, 0.1)
        )

    top_chain = [
        TiltedSphereChain(materials.glass, (0, 7, 0), (0, 0, 0), 30)
    ]
    
 
    return [layer1, layer2, layer3] +  dangling_chains +  top_chain

def chandelier7():
    """
    Chandelier 2 but with red/blue lights and alternate background
    """
    settings.environment_map = "../assets/basement/"
    
    context.append_light(
        DirectionalLight(context.get_id(), (0.5, 0.5, 0.5), (-0.5, 1, 0))
    )

    layer1 = PolygonLayer(materials.silver, (0, 3, 0),  1, 5, 0.05)
    layer2 = PolygonLayer(materials.silver, (0, 2, 0),  2, 15, 0.1)
    layer3 = PolygonLayer(materials.silver, (0, 1, 0),  2.5, 30, 0.05)

    points1 = layer1.get_attachment_points_world_space()
    points2 = layer2.get_attachment_points_world_space()
    points3 = layer3.get_attachment_points_world_space()

    chains = []
    chains += connect_points_with_chains(points1, points2, lambda pt1, pt2: UnevenDemiCurve(materials.silver, pt1, pt2, 6, 0.075, 0.2))
    chains += connect_points_with_chains(points2, points3, lambda pt1, pt2: TiltedSphereChain(materials.glass, pt1, pt2, 12))

    dangling_chains = []
    for point in points3:
        dangling_chains.append(CylinderChain(materials.glass, point, 2, 0.1))

    candles = []
    for i, point in enumerate(points2):
        if i % 2 == 0:
            candles.append(Candle(materials.red, point, 0.25, color=(1, 0, 0), light=materials.red_light))
        else:
            candles.append(Candle(materials.blue, point, 0.25, color=(0,0,1), light=materials.blue_light))

    top_chain = []
    for point in points1:
        top_chain.append(
            TiltedSphereChain(materials.glass, point, (0, 4, 0), 12)
        )
    top_chain.append(
        TiltedSphereChain(materials.glass, (0, 4, 0), (0, 7, 0), 20)
    )

    return [layer1, layer2, layer3] + chains + dangling_chains + candles + top_chain

def chandelier8():
    """
    Lots of bendies :)
    """

    # Add a light to spice things up
    context.append_light(
        DirectionalLight(context.get_id(), (0.5, 0.5, 0.5), (-0.5, 1, 0))
    )

    # Center chain running top to bottom through y axis
    top_chain = [
        TiltedSphereChain(materials.glass, (0, 7, 0), (0, 3, 0), 20)
    ]

    # Make some layers that we won'd actuall render so that we can get coordinates
    # That are distributes in a circle perpendicular to the main chain
    dummy_layer1 = PolygonLayer(materials.silver, (0, 3, 0),  2, 7, 0.05)
    dummy_layer2 = PolygonLayer(materials.silver, (0, 3, 0),  4, 7, 0.05)
    dummy_attach_points1 = dummy_layer1.get_attachment_points_world_space()
    dummy_attach_points2 = dummy_layer2.get_attachment_points_world_space()

    # Connect U bends from the center chain out to the dummy layers
    arms = []
    for pt1, pt2 in zip(dummy_attach_points1, dummy_attach_points2):
        # s bends
        arms.append(
            DemiCurve(materials.silver, (0, 3, 0), pt1, 10, 0.075)
        )
        arms.append(
            TransBlock(Tree([DemiCurve(materials.silver, pt1, pt2, 10, 0.075)]), rotate=(1,0,0,180), translate=(0,6,0))
        )

    # Add satellite layers, candles, and chains to the ends of the arms
    chains = []
    candles = []
    satellites = []
    for pt in dummy_attach_points2:
        satellite = PolygonLayer(materials.silver, (pt[0], pt[1] - 0.5, pt[2]), 0.5, 6, 0.05)

        satellites.append(satellite)
        for pt2 in satellite.get_attachment_points_world_space():
            chains.append(
                TiltedSphereChain(materials.glass, pt, pt2, 6)
            )
            candles.append(Candle(materials.gold, pt2, 0.4, color=(0.1, 0.07, 0)))

    return top_chain + arms + satellites + candles + chains

def axes():
    """
    Axes scene file for debugging
    """
    return [
            TransBlock(Cube(materials.bronze), scale=(10,0.07,0.07)),
            TransBlock(Cube(materials.silver), scale=(0.07,0.07,10), translate=(0,0,3)),
            TransBlock(Cube(materials.glass), scale=(0.07,10,0.07)),
        ]

def demoSetup():
    """
    settings/context set up for simple scenes for demos
    """
    context.append_light(
        context.DirectionalLight(context.get_id(), (1,1,1), (-1,-1,-0.2) )
    )
    settings.camera_position = (0, 0.5, 10)
    settings.camera_focus = (0, 0, -0.1)

def polygonLayerDemo():
    """
    Demo different components
    """
    demoSetup()
    settings.environment_map = "../assets/niagara/"

    layers = [
        PolygonLayer(materials.silver, (0, 1, 0),  1, 3, 0.05),
        PolygonLayer(materials.gold, (0, 0, 0),    2, 6, 0.1),
        PolygonLayer(materials.glass, (0, -1, 0),  2.5, 18, 0.3),
    ]

    chain = [
        DiamondChain(materials.gold, (-1, 1, 0), 6, 0.3, 0.05),
        TaperedSphereChain(materials.silver, (1, 5, -2), 12, 0.6, 0.05),
    ]

    fixtures = [
        Candle(materials.gold, (-2, -1, 0), 0.4),
        Candle(materials.gold, (0, -1, 0), 1),
        Candle(materials.gold, (2, -1, 0), 2),
    ]

    return fixtures

class Chandelier(SceneComponent):
    def __init__(self):
        chandeliers = [
            chandelier1,
            chandelier2,
            chandelier3,
            chandelier4,
            chandelier5,
            chandelier6,
            chandelier7,
            chandelier8,
        ]
        index = int(sys.argv[2])

        self.components = chandeliers[index - 1]()

    def scene_rep(self):
        
        return TransBlock(Tree(self.components)).scene_rep()