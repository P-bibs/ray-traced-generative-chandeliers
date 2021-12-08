from common import SceneComponent
from shapes.transblock import TransBlock
from shapes.primitives import Cube, Cylinder, Sphere
from shapes.tree import Tree
from settings import settings

class SquareLayer(SceneComponent):
    def __init__(self, center, radius):
        self.center = center
        self.radius = radius


    def scene_rep(self):
        cube = Cube(diffuse=settings.default_diffuse, specular=settings.default_specular, ambient=settings.default_ambient, shininess=settings.default_shininess)
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
    def __init__(self, center, radius):
        self.center = center
        self.radius = radius

    def scene_rep(self):
        cylinder = Cylinder(diffuse=settings.default_diffuse, specular=settings.default_specular, ambient=settings.default_ambient, shininess=settings.default_shininess)

        rotation = (1, 0, 0, 90)

        # square root of 2 divided by 2
        s2d2 = 0.70710678118654757

        parts = [
            # top_side
            TransBlock(cylinder, (0,0,1,90), (0,0,-1), (0.5, 1, 0.5)),
            # bottom_side
            TransBlock(cylinder, (0,0,1,90), (0,0,1), (0.5, 1, 0.5)),
            # left_side
            TransBlock(cylinder, (1,0,0,90), (-1,0,0), (0.5, 1, 0.5)),
            # right_side
            TransBlock(cylinder, (1,0,0,90), (1,0,0), (0.5, 1, 0.5)),
            # top_left_corner
            TransBlock(cylinder, (1,0,1,90), (-s2d2,0,-s2d2), (0.5, 1, 0.5)),
            # top_right_corner
            TransBlock(cylinder, (1,0,-1,90), (s2d2,0,-s2d2), (0.5, 1, 0.5)),
            # bottom_left_corner
            TransBlock(cylinder, (1,0,-1,90), (-s2d2,0,s2d2), (0.5, 1, 0.5)),
            # bottom_right_corner
            TransBlock(cylinder, (1,0,1,90), (s2d2,0,s2d2), (0.5, 1, 0.5))
        ]

        sphere= Sphere(diffuse=settings.default_diffuse, specular=settings.default_specular, ambient=settings.default_ambient, shininess=settings.default_shininess)
        r = 0.6
        short_off = 0.35
        long_off = 1
        joints = [
            # top left
            TransBlock(sphere, (0,0,0,0), (-short_off,0,-long_off), (r, r, r)),
            # top right
            TransBlock(sphere, (0,0,0,0), (short_off,0,-long_off), (r, r, r)),
            # bottom left
            TransBlock(sphere, (0,0,0,0), (-short_off,0,long_off), (r, r, r)),
            # bottom right
            TransBlock(sphere, (0,0,0,0), (short_off,0,long_off), (r, r, r)),
            # left top
            TransBlock(sphere, (0,0,0,0), (-long_off,0,-short_off), (r, r, r)),
            # left bottom
            TransBlock(sphere, (0,0,0,0), (-long_off,0,short_off), (r, r, r)),
            # right top
            TransBlock(sphere, (0,0,0,0), (long_off,0,-short_off), (r, r, r)),
            # right bottom
            TransBlock(sphere, (0,0,0,0), (long_off,0,short_off), (r, r, r))
        ]

        full_layer = TransBlock(
            Tree(parts + joints),
            (0,0,0,0),
            self.center,
            (self.radius, self.radius, self.radius)
        )

        return full_layer.scene_rep()