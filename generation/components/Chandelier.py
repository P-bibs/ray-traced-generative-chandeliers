from common import SceneComponent
from components.Layers import SquareLayer, OctagonLayer
from shapes.primitives import Cube, Cone
from shapes.transblock import TransBlock
from shapes.tree import Tree

class Chandelier(SceneComponent):
    def __init__(self):
        self.layers = [
            # SquareLayer((0, 4, 0), 1),
            # SquareLayer((0, 2, 0), 2),
            OctagonLayer((0, 0, 0), 3),
        ] 

    def scene_rep(self):
        
        return TransBlock(Tree(self.layers)).scene_rep()