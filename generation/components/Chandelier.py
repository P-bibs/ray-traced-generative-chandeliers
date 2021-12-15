import materials
from common import SceneComponent
from components.Layers import PolygonLayer, SquareLayer
from components.Chain import CylinderChain, TiltedSphereChain
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
        chains.append(TiltedSphereChain(point, points2[int(j)], 12))

    return chains

def chandelier1():
    layers = [
        PolygonLayer((0, 3, 0),  1, 5, 0.1),
        PolygonLayer((0, 2, 0),  2, 15, 0.15),
        PolygonLayer((0, 1, 0),  1, 30, 0.1),
    ]
    points1 = layers[0].get_attachment_points_world_space()
    points2 = layers[1].get_attachment_points_world_space()
    points3 = layers[2].get_attachment_points_world_space()

    # Generate chains connecting the two layers
    chains = connect_points_with_chains(points1, points2)
    chains = chains + connect_points_with_chains(points2, points3)

    dangles = [CylinderChain(p2, 2, 0.1) for p2 in points2]
    dangles = dangles + [CylinderChain(p3, 2, 0.1) for p3 in points3]

    for point in points1:
        chains.append(
            TiltedSphereChain(point, (0, 5, 0), 12)
        )
    chains.append(
        TiltedSphereChain((0, 5, 0), (0, 7, 0), 10)
    )

    fixtures = []
    for point in points2:
        fixtures.append(Candle(point, 0.5))

    return layers + chains + dangles + fixtures
    


class Chandelier(SceneComponent):
    def __init__(self):
        self.components = chandelier1()

    def scene_rep(self):
        return TransBlock(Tree(self.components)).scene_rep()