import materials
from common import SceneComponent
from shapes.transblock import TransBlock
from context import PointLight
from shapes.primitives import Cylinder, Sphere, Cone
from shapes.tree import Tree
import context


class Candle(SceneComponent):
    def __init__(self, material, base, size):
        self.material = material
        self.base = base
        self.size = size
        self.color = (1, 0.7, 0)

        self.stand_height = 0.5
        self.base_height = 0.5
        self.candle_height = 1
        self.flame_height = 0.25

        self.stand_radius = 0.25
        self.base_radius = 0.5
        self.candle_radius = 0.15
        self.flame_radius = 0.15

    def scene_rep(self):
        stand_translate = self.stand_height / 2
        base_translate = stand_translate + self.base_height / 2
        candle_translate = (
            base_translate + self.base_height / 2 + self.candle_height / 2
        )
        flame_translate = (
            candle_translate + self.candle_height / 2 + self.flame_height / 2
        )

        parts = [
            # stand
            TransBlock(
                Cylinder(self.material),
                translate=(0, stand_translate, 0),
                scale=(self.stand_radius, self.stand_height, self.stand_radius),
            ),
            # base
            TransBlock(
                Cone(self.material),
                translate=(0, base_translate, 0),
                scale=(self.base_radius, self.base_height, self.base_radius),
                rotate=(1, 0, 0, 180),
            ),
            # candle
            TransBlock(
                Cylinder(materials.wax),
                translate=(0, candle_translate, 0),
                scale=(self.candle_radius, self.candle_height, self.candle_radius),
            ),
            # Flame
            TransBlock(
                Sphere(materials.glass),
                translate=(0, flame_translate, 0),
                scale=(self.flame_radius, self.flame_height, self.flame_radius),
            ),
        ]

        context.append_light(
            PointLight(
                context.get_id(),
                self.color,
                (self.base[0], self.base[1] + flame_translate, self.base[2]),
            )
        )

        return TransBlock(
            Tree(parts),
            scale=(self.size, self.size, self.size),
            translate=(self.base[0], self.base[1], self.base[2]),
        ).scene_rep()
