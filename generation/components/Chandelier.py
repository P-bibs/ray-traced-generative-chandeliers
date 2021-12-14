import materials
from common import SceneComponent
from components.Layers import PolygonLayer, SquareLayer
from components.Chain import CylinderChain, TiltedSphereChain
from shapes.primitives import Cube, Cone
from shapes.transblock import TransBlock
from shapes.tree import Tree

def connect_points_with_chains(points1, points2):
    if len(points1) > len(points2):
        points1, points2 = points2, points1

    chains = []
    for i, point in enumerate(points1):
        j = ((i / len(points1)) * len(points2) + 1) % len(points2)
        chains.append(TiltedSphereChain(point, points2[int(j)], 4))

    return chains


class Chandelier(SceneComponent):
    def __init__(self):
        self.layers = [
            # SquareLayer((0, 4, 0), 1),
            # SquareLayer((0, 2, 0), 2),
            PolygonLayer((0, 3, 0),  1, 4, 0.1),
            PolygonLayer((0, 0, 0),  2, 8, 0.15),
        ]
        points1 = self.layers[0].get_attachment_points_world_space()
        points2 = self.layers[1].get_attachment_points_world_space()

        # Generate chains connecting the two layers
        self.chains = connect_points_with_chains(points1, points2)

        self.dangles = [CylinderChain(p2, 2, 0.1) for p2 in points2]

    def scene_rep(self):
        # return TransBlock(Cube(materials.glass), scale=(3,3,3), translate=(0,2,0)).scene_rep()
        return TransBlock(Tree(self.layers + self.chains + self.dangles)).scene_rep()