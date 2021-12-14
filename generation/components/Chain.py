from shapes.primitives import Cube, Sphere
from common import SceneComponent
from settings import settings
from shapes.transblock import TransBlock
from shapes.tree import Tree
import materials


class TiltedSphereChain(SceneComponent):
    def __init__(self, start, end, count):
        self.start = start
        self.end = end
        self.count = count

    def scene_rep(self):
        sphere = Sphere(materials.bronze)
        start_to_end = (
            self.end[0] - self.start[0],
            self.end[1] - self.start[1],
            self.end[2] - self.start[2],
        )

        distance = (
            start_to_end[0] ** 2 + start_to_end[1] ** 2 + start_to_end[2] ** 2
        ) ** 0.5
        radius = distance / (2 * self.count)

        spheres = []
        for i in range(self.count):
            progress = (i + 0.5) / self.count
            interped = (
                self.start[0] + start_to_end[0] * progress,
                self.start[1] + start_to_end[1] * progress,
                self.start[2] + start_to_end[2] * progress,
            )
            spheres.append(
                TransBlock(sphere, translate=(interped), scale=(radius, radius, radius))
            )

        return TransBlock(Tree(spheres)).scene_rep()


class CylinderChain(SceneComponent):
    def __init__(self, top, length, radius):
        self.top = top
        self.length = length
        self.radius = radius

    def scene_rep(self):
        return TransBlock(
            Cube(materials.glass),
            translate=(self.top[0], self.top[1] - self.length / 2, self.top[2]),
            rotate=(1, 0, 0, 00),
            scale=(self.radius, self.length, self.radius),
        ).scene_rep()


def SphereChain(top, count, radius):
    sphere = Sphere(materials.silver)
    return TaperedChain(sphere, top, count, radius, radius)


def TaperedSphereChain(top, count, start_radius, end_radius):
    sphere = Sphere(materials.silver)
    return TaperedChain(sphere, top, count, start_radius, end_radius)


def DiamondChain(top, count, radius):
    diamond = Tree(
        [
            TransBlock(
                Cube(materials.gold),
                rotate=(-1, -1, 0, 45),
            )
        ]
    )
    return TaperedChain(diamond, top, count, radius / 2, radius / 2)


class TaperedChain(SceneComponent):
    def __init__(self, shape, top, count, start_radius, end_radius):
        self.shape = shape
        self.top = top
        self.count = count
        self.start_radius = start_radius
        self.end_radius = end_radius

    def scene_rep(self):

        links = []
        current_y_location = 0
        for i in range(self.count):
            radius_delta = self.end_radius - self.start_radius
            interped_radius = self.start_radius + radius_delta * i / (self.count - 1)

            current_y_location -= interped_radius
            links.append(
                TransBlock(
                    self.shape,
                    translate=(0, current_y_location, 0),
                    scale=(interped_radius, interped_radius, interped_radius),
                )
            )
            current_y_location -= interped_radius

        return TransBlock(
            Tree(links),
            translate=(self.top[0], self.top[1] - self.start_radius, self.top[2]),
        ).scene_rep()
