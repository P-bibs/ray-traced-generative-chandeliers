import materials
from common import SceneComponent
from components.Layers import PolygonLayer, SquareLayer
from components.Chain import CylinderChain, TiltedSphereChain, DemiCurve, UnevenDemiCurve
from shapes.primitives import Cube, Cone
from shapes.transblock import TransBlock
from shapes.tree import Tree
from components.Fixtures import Candle

def connect_points_with_chains(points1, points2):
    if len(points1) > len(points2):
        points1, points2 = points2, points1

    chains = []
    for i, point in enumerate(points1):
        j = ((i / len(points1)) * len(points2) + 1) % len(points2)
        chains.append(TiltedSphereChain(materials.silver, point, points2[int(j)], 12))

    return chains

def chandelier1():
    layers = [
        PolygonLayer(materials.silver, (0, 3, 0),  1, 5, 0.1),
        PolygonLayer(materials.silver, (0, 2, 0),  2, 15, 0.15),
        PolygonLayer(materials.silver, (0, 1, 0),  1, 30, 0.1),
    ]
    points1 = layers[0].get_attachment_points_world_space()
    points2 = layers[1].get_attachment_points_world_space()
    points3 = layers[2].get_attachment_points_world_space()

    # Generate chains connecting the two layers
    chains = connect_points_with_chains(points1, points2)
    chains = chains + connect_points_with_chains(points2, points3)

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

def axes():
    return [
            TransBlock(Cube(materials.bronze), scale=(10,0.07,0.07)),
            TransBlock(Cube(materials.silver), scale=(0.07,0.07,10), translate=(0,0,3)),
            TransBlock(Cube(materials.glass), scale=(0.07,10,0.07)),
        ]

class Chandelier(SceneComponent):
    def __init__(self):
        self.components = [
            UnevenDemiCurve(materials.silver, (0, 1, 0), (1, 0, 1), 8, 0.1, 0.5),
        ]
            

    def scene_rep(self):
        return TransBlock(Tree(self.components)).scene_rep()