from common import SceneComponent
from shapes.primitives import Cube, Cone
from shapes.transblock import TransBlock

class Chandelier(SceneComponent):
    def __init__(self):
        self.body = TransBlock(Cone(diffuse=(1,1,1)), (0, 0, 0, 0), (-1.3, 1.2, 0), (1, 1, 1))

    def scene_rep(self):
        return self.body.scene_rep()