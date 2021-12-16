from settings import settings
import materials
from common import SceneComponent
from components.Layers import PolygonLayer, SquareLayer
from components.Chain import CylinderChain, DiamondChain, SphereChain, TaperedChain, TaperedSphereChain, TiltedSphereChain, DemiCurve, UnevenDemiCurve
from shapes.primitives import Cube, Cone
from shapes.transblock import TransBlock
from shapes.tree import Tree
from components.Fixtures import Candle
import context
from context import DirectionalLight, PointLight

def connect_points_with_chains(points1, points2, chain_maker):
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

    layers = [
        PolygonLayer(materials.silver, (0, 3, 0),  1, 5, 0.1),
        PolygonLayer(materials.silver, (0, 2, 0),  2, 15, 0.15),
        PolygonLayer(materials.silver, (0, 1, 0),  1, 30, 0.1),
    ]
    points1 = layers[0].get_attachment_points_world_space()
    points2 = layers[1].get_attachment_points_world_space()
    points3 = layers[2].get_attachment_points_world_space()

    # Generate chains connecting the two layers
    chains = connect_points_with_chains(points1, points2,  lambda pt1, pt2: TiltedSphereChain(materials.silver, pt1, pt2, 12))
    chains = chains + connect_points_with_chains(points2, points3, lambda pt1, pt2: TiltedSphereChain(materials.silver, pt1, pt2, 12))

    dangles = [CylinderChain(materials.glass, p2, 2, 0.1) for p2 in points2]
    dangles = dangles + [CylinderChain(materials.glass, p3, 2, 0.1) for p3 in points3]

    for point in points1:
        chains.append(
            TiltedSphereChain(materials.bronze, point, (0, 5, 0), 12)
        )
    chains.append(
        TiltedSphereChain(materials.bronze, (0, 5, 0), (0, 7, 0), 10)
    )

    fixtures = []
    for point in points2:
        fixtures.append(Candle(materials.silver, point, 0.5))

    return layers + chains + dangles + fixtures

def chandelier2():
    
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
    for point in points2:
        candles.append(Candle(materials.gold, point, 0.25))

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
    # layer3 = PolygonLayer(materials.silver, (0, 1, 0),  2.5, 18, 0.05)

    points1 = layer1.get_attachment_points_world_space()
    points2 = layer2.get_attachment_points_world_space()
    # points3 = layer3.get_attachment_points_world_space()

    chains = []
    chains += connect_points_with_chains(points1, points2, lambda pt1, pt2: UnevenDemiCurve(materials.silver, pt1, pt2, 3, 0.075, 0.2))
    # chains += connect_points_with_chains(points2, points3, lambda pt1, pt2: TiltedSphereChain(materials.glass, pt1, pt2, 6))

    dangling_chains = []
    # for point in points3:
    #     dangling_chains.append(CylinderChain(materials.glass, point, 2, 0.1))

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

    return [layer1, layer2] + chains + dangling_chains + candles + top_chain

def chandelier4():
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
    layer1 = PolygonLayer(materials.silver, (0, 3, 0),  0.5, 6,  0.05)
    layer2 = PolygonLayer(materials.silver, (0, 2, 0),  0.5, 8,  0.05)
    layer3 = PolygonLayer(materials.silver, (0, 0, 0),  0.5, 6, 0.05)

    points1 = layer1.get_attachment_points_world_space()
    points2 = layer2.get_attachment_points_world_space()
    points3 = layer3.get_attachment_points_world_space()

    points2_extended = [ (pt[0] * 5, pt[1], pt[2] * 5) for pt in points2 ]

    chains = []
    chains += connect_points_with_chains(points2, points2_extended, lambda pt1, pt2: DemiCurve(materials.silver, pt1, pt2, 6, 0.075))

    dangling_chains = []
    for point in points2_extended:
        dangling_chains.append(DiamondChain(materials.glass, point, 6, 0.3, 0.05))

    candles = []
    for point in points2_extended:
        candles.append(Candle(materials.gold, point, 0.4))

    top_chain = [
        TiltedSphereChain(materials.glass, (0, 7, 0), (0, 0, 0), 30)
    ]
    
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

    layer1 = PolygonLayer(materials.silver, (0, 3, 0),  1.5, 48,  0.05)
    layer2 = PolygonLayer(materials.silver, (0, 2, 0),  1.0, 32,  0.05)
    layer3 = PolygonLayer(materials.silver, (0, 1, 0),  0.5, 16,  0.05)

    points1 = layer1.get_attachment_points_world_space()
    points2 = layer2.get_attachment_points_world_space()
    points3 = layer3.get_attachment_points_world_space()

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
    
    connecting_chains = []
    # for point in points3:
    #     connecting_chains.append(
    #         TiltedSphereChain(materials.glass, point, (0, 0.4, 0), 6)
    #     )

 
    return [layer1, layer2, layer3] +  dangling_chains +  top_chain + connecting_chains

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

def axes():
    return [
            TransBlock(Cube(materials.bronze), scale=(10,0.07,0.07)),
            TransBlock(Cube(materials.silver), scale=(0.07,0.07,10), translate=(0,0,3)),
            TransBlock(Cube(materials.glass), scale=(0.07,10,0.07)),
        ]

def demoSetup():
    context.append_light(
        context.DirectionalLight(context.get_id(), (1,1,1), (-1,-1,-0.2) )
    )
    settings.camera_position = (0, 0.5, 10)
    settings.camera_focus = (0, 0, -0.1)

def polygonLayerDemo():
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
        self.components = chandelier7()

    def scene_rep(self):
        
        return TransBlock(Tree(self.components)).scene_rep()