from shapes.primitives import Sphere
from common import SceneComponent
from settings import settings
from shapes.transblock import TransBlock
from shapes.tree import Tree


def SimpleChain(top, count, radius):
    return TaperedChain(top, count, radius, radius)

class TaperedChain(SceneComponent):
    def __init__(self, top, count, start_radius, end_radius):
        self.top = top
        self.count = count
        self.start_radius = start_radius
        self.end_radius = end_radius

    def scene_rep(self):
        sphere = Sphere(
            diffuse=settings.default_diffuse,
            ambient=settings.default_ambient,
            specular=settings.default_specular,
            shininess=settings.default_shininess,
            refraction=settings.default_refraction,
        )

        links = []
        current_y_location = 0
        for i in range(self.count):
            radius_delta = self.end_radius - self.start_radius
            interped_radius = self.start_radius + radius_delta * i / (self.count - 1)

            current_y_location -= interped_radius
            links.append(
                TransBlock(
                    sphere,
                    translate=(0, current_y_location, 0),
                    scale=(interped_radius, interped_radius, interped_radius),
                )
            )
            current_y_location -= interped_radius

        return TransBlock(
            Tree(links), translate=(self.top[0], self.top[1] - self.start_radius, self.top[2])
        ).scene_rep()