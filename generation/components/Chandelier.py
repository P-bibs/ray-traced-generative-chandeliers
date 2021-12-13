from common import SceneComponent
from components.Layers import PolygonLayer, SquareLayer
from components.Chain import TiltedSphereChain
from shapes.primitives import Cube, Cone
from shapes.transblock import TransBlock
from shapes.tree import Tree

class Chandelier(SceneComponent):
    def __init__(self):
        self.layers = [
            # SquareLayer((0, 4, 0), 1),
            # SquareLayer((0, 2, 0), 2),
            PolygonLayer((0, 0, 0),  4, 8, 0.3),
            TiltedSphereChain((-1,0,-1), (1,1,1), 8)
        ] 

    def scene_rep(self):
        
        return TransBlock(Tree(self.layers)).scene_rep()