import math
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
        cube = Cube(diffuse=settings.default_diffuse, specular=settings.default_specular, ambient=settings.default_ambient, shininess=settings.default_shininess, reflective=(0.7,0.7,0.7))
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

class OctagonLayer(SceneComponent):
    def __init__(self, center, radius, girth):
        self.center = center
        self.radius = radius
        self.girth = girth
        r = 0.2
        self.chain_types = [
            lambda top: TaperedSphereChain((top[0], top[1] - r / 2, top[2]), 8, r / 2, r / 8),
            lambda top: TaperedSphereChain((top[0], top[1] - r / 2, top[2]), 4, r / 2, r / 4),
        ]

    def scene_rep(self):
        cylinder = Cylinder(diffuse=settings.default_diffuse, specular=settings.default_specular, ambient=settings.default_ambient, shininess=settings.default_shininess, reflective=(0.7,0.7,0.7))
        sphere= Sphere(diffuse=settings.default_diffuse, specular=settings.default_specular, ambient=settings.default_ambient, shininess=settings.default_shininess, reflective=(0.7,0.7,0.7))

        g = self.girth
        r = self.radius

        side_length = self.radius / (1/2 + 1/math.sqrt(2))
        triangle_foot = math.sqrt(side_length ** 2 / 2)
        diagonal_offset = side_length / 2 + triangle_foot / 2


        # One octagon side, of proper length and with joints, collinear with x axis, centered at 0
        octagon_side = Tree([TransBlock(Tree([
                TransBlock(cylinder, scale=(g, side_length, g)),
                TransBlock(sphere, translate=(0, side_length / 2, 0), scale=(g, g, g)),
                TransBlock(sphere, translate=(0, -side_length / 2, 0), scale=(g, g, g)),
        ]), rotate=(0,0,-1,90))])
        # One octagon side, of proper length and without joints, collinear with x axis, centered at 0
        octagon_side_no_joints = Tree([
                TransBlock(cylinder, rotate=(0,0,-1,90), scale=(g, side_length, g))
        ])

        parts = [
            # top_side
            TransBlock(octagon_side, translate=(0,0,-r)),
            # bottom_side
            TransBlock(octagon_side, translate=(0,0,r)),
            # left_side
            TransBlock(octagon_side, translate=(r,0,0), rotate=(0,1,0,90)),
            # right_side
            TransBlock(octagon_side, translate=(-r,0,0), rotate=(0,1,0,90)),
            # top_left_corner
            TransBlock(octagon_side_no_joints, translate=(-diagonal_offset,0,-diagonal_offset), rotate=(0,1,0,45)),
            # top_right_corner
            TransBlock(octagon_side_no_joints, translate=(diagonal_offset,0,-diagonal_offset), rotate=(0,1,0,-45)),
            # bottom_left_corner
            TransBlock(octagon_side_no_joints, translate=(-diagonal_offset,0,diagonal_offset), rotate=(0,1,0,-45)),
            # bottom_right_corner
            TransBlock(octagon_side_no_joints, translate=(diagonal_offset,0,diagonal_offset), rotate=(0,1,0,45)),
        ]

        r = self.girth / 1.5
        short_off = side_length / 2
        long_off = side_length / 2 + triangle_foot
        joints = [
            # top left
            TransBlock(sphere, (0,0,0,0), (-short_off,-g,-long_off), (r, r, r)),
            # top right
            TransBlock(sphere, (0,0,0,0), (short_off,-g,-long_off), (r, r, r)),
            # bottom left
            TransBlock(sphere, (0,0,0,0), (-short_off,-g,long_off), (r, r, r)),
            # bottom right
            TransBlock(sphere, (0,0,0,0), (short_off,-g,long_off), (r, r, r)),
            # left top
            TransBlock(sphere, (0,0,0,0), (-long_off,-g,-short_off), (r, r, r)),
            # left bottom
            TransBlock(sphere, (0,0,0,0), (-long_off,-g,short_off), (r, r, r)),
            # right top
            TransBlock(sphere, (0,0,0,0), (long_off,-g,-short_off), (r, r, r)),
            # right bottom
            TransBlock(sphere, (0,0,0,0), (long_off,-g,short_off), (r, r, r))
        ]

        chains = []
        for i, joint in enumerate(joints):
            chain_type = self.chain_types[i % len(self.chain_types)]
            top = joint.translate
            
            chains.append(chain_type(top))

        full_layer = TransBlock(
            Tree(parts + joints + chains),
            (0,0,0,0),
            self.center,
            (self.radius, self.radius, self.radius)
        )

        return full_layer.scene_rep()