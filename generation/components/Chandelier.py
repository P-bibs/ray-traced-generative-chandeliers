from common import SceneComponent
from components.Layers import PolygonLayer, SquareLayer
from components.Chain import CylinderChain, TiltedSphereChain
from shapes.primitives import Cube, Cone
from shapes.transblock import TransBlock
from shapes.tree import Tree

class Chandelier(SceneComponent):
    def __init__(self):
        self.layers = [
            # SquareLayer((0, 4, 0), 1),
            # SquareLayer((0, 2, 0), 2),
            PolygonLayer((0, 3, 0),  1, 5, 0.3),
            PolygonLayer((0, 0, 0),  2, 5, 0.4),
        ]
        points1 = self.layers[0].get_attachment_points_world_space()
        points2 = self.layers[1].get_attachment_points_world_space()
        self.chains = [ TiltedSphereChain(p1, p2, 8) for (p1, p2) in zip(points1, points2) ]
        self.dangles = [CylinderChain(p2, 2, 0.1) for p2 in points2]

    def scene_rep(self):
        
        return TransBlock(Tree(self.layers + self.chains + self.dangles)).scene_rep()