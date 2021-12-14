import math
import materials
from common import SceneComponent
from shapes.transblock import TransBlock
from shapes.primitives import Cube, Cylinder, Sphere
from shapes.tree import Tree
from settings import settings
from components.Chain import DiamondChain,  SphereChain, TaperedChain, TaperedSphereChain

class SquareLayer(SceneComponent):
    def __init__(self, center, radius):
        self.center = center
        self.radius = radius


    def scene_rep(self):
        cube = Cube(materials.gold)
        top_side = TransBlock(cube, (0,0,0,0), (0,0,-1), (2.5, 0.5, 0.5))
        bottom_side = TransBlock(cube, (0,0,0,0), (0,0,1), (2.5, 0.5, 0.5))
        left_side = TransBlock(cube, (0,0,0,0), (-1,0,0), (0.5, 0.5, 2.5))
        right_side = TransBlock(cube, (0,0,0,0), (1,0,0), (0.5, 0.5, 2.5))

        full_layer = TransBlock(
            Tree([top_side, bottom_side, left_side, right_side]),
            (0,0,0,0),
            self.center,
            (self.radius, self.radius, self.radius)
        )

        return full_layer.scene_rep()

class PolygonLayer(SceneComponent):
    def __init__(self, center, radius, sides, girth):
        self.center = center
        self.radius = radius
        self.sides = sides
        self.girth = girth
        r = self.girth
        self.chain_types = [
            lambda top: TaperedSphereChain((top[0], top[1] - r / 2, top[2]), 8, r / 2, r / 8),
            lambda top: TaperedSphereChain((top[0], top[1] - r / 2, top[2]), 4, r / 2, r / 4),
        ]

    def get_attachment_points_world_space(self):
        points = []
        theta = math.pi * 2 / self.sides
        for i in range(self.sides):
            angle = 2 * math.pi * i / self.sides + theta / 2
            x = math.cos(angle) * self.radius
            y = 0
            z = math.sin(angle) * self.radius
            points.append((x + self.center[0], y + self.center[1], z + self.center[2]))

        return points

    def scene_rep(self):
        cylinder = Cylinder(materials.silver)
        sphere= Sphere(materials.silver)


        theta = math.pi * 2 / self.sides
        edge_offset = self.radius * math.cos(theta / 2) 
        side_length = 2 * self.radius * math.sin(theta / 2)

        side_no_joints =  Tree([TransBlock(cylinder, rotate=(0,0,-1,90), scale=(self.girth, side_length, self.girth))])


        sides = []
        for i in range(self.sides):
            angle = 2 * math.pi * i / self.sides
            x = math.cos(angle) * edge_offset
            y = 0
            z = math.sin(angle) * edge_offset
            sides.append(TransBlock(side_no_joints, translate=(x,y,z), rotate=(0,-1,0, math.degrees(angle) + 90)))

        joints = []
        for i in range(self.sides):
            angle = 2 * math.pi * i / self.sides + theta / 2
            x = math.cos(angle) * self.radius
            y = 0
            z = math.sin(angle) * self.radius
            joints.append(TransBlock(sphere, translate=(x,y,z), scale=(self.girth, self.girth, self.girth)))

        chains = []
        # for i, joint in enumerate(joints):
        #     chain_type = self.chain_types[i % len(self.chain_types)]
        #     top = joint.translate
            
        #     chains.append(chain_type(top))

        full_layer = TransBlock(
            Tree(sides + joints + chains),
            (0,0,0,0),
            self.center,
            (1,1,1) # (self.radius, self.radius, self.radius)
        )

        return full_layer.scene_rep()
