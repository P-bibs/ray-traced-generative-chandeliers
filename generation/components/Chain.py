from shapes.primitives import Cube, Sphere, Cylinder
from common import SceneComponent
from settings import settings
from shapes.transblock import TransBlock
from shapes.tree import Tree
import math

class UnevenDemiCurve(SceneComponent):
    """
    Like a DemiCurve, but the start and end points can be at different y values. Cylinders
    will be used to make up the distance, so the end result looks something like the following:
    
  start
    x
    x
    x
    x
    x         end
    x          x
    x          x 
     x        x  
        x  x   
    """
    def __init__(self, material, start, end, sides, girth, drop):
        self.material = material
        self.start = start
        self.end = end
        self.sides = sides
        self.girth = girth
        self.drop = drop

    def scene_rep(self):
        y_value_of_schwoop = min(self.start[1], self.end[1]) - self.drop
        schwoop_start = (self.start[0], y_value_of_schwoop, self.start[2])
        schwoop_end = (self.end[0], y_value_of_schwoop, self.end[2])

        start_y_delta = self.start[1] - y_value_of_schwoop
        end_y_delta = self.end[1] - y_value_of_schwoop

        start_y_translate = self.start[1] - start_y_delta / 2
        end_y_translate = self.end[1] - end_y_delta / 2

        cylinder = Cylinder(self.material)
        components = [
            DemiCurve(self.material, schwoop_start, schwoop_end, self.sides, self.girth),
            TransBlock(cylinder, translate=(self.start[0], start_y_translate, self.start[2]), scale=(self.girth, start_y_delta, self.girth)),
            TransBlock(cylinder, translate=(self.end[0], end_y_translate, self.end[2]), scale=(self.girth, end_y_delta, self.girth)),
        ]

        return TransBlock(Tree(components)).scene_rep()

class DemiCurve(SceneComponent):
    """
    Creates a half circle between two points. The circle is discretized to a a fixed number
    of sides. The sides are cylinder with spherical joints. The firth specifies how thick 
    the cylinders are. Looks like the following:
    x          x 
     x        x  
        x  x     
    """
    def __init__(self, material, start, end, sides, girth):
        if (start[1] != end[1]):
            raise ValueError("Start and end points must be on the same y level")

        self.material = material
        self.start = start
        self.end = end
        self.sides = sides
        self.girth = girth

    def scene_rep(self):
        radius = (((self.end[0] - self.start[0]) ** 2 + (self.end[2] - self.start[2]) ** 2) ** 0.5 ) / 2
        center = ((self.start[0] + self.end[0]) / 2, self.start[1], (self.start[2] + self.end[2]) / 2)
        
        cylinder = Cylinder(self.material)
        sphere= Sphere(self.material)


        theta = math.pi / self.sides
        edge_offset = radius * math.cos(theta / 2) 
        side_length = 2 * radius * math.sin(theta / 2)

        side_no_joints =  Tree([TransBlock(cylinder, scale=(self.girth, side_length, self.girth))])


        sides = []
        for i in range(self.sides):
            angle = math.pi * i / self.sides + theta / 2
            x = math.cos(angle) * edge_offset
            z = 0
            y = -math.sin(angle) * edge_offset
            sides.append(TransBlock(side_no_joints, translate=(x,y,z), rotate=(0,0,-1, math.degrees(angle) )))

        joints = []
        for i in range(self.sides + 1):
            angle = math.pi * i / self.sides
            x = math.cos(angle) * radius
            z = 0
            y = -math.sin(angle) * radius
            joints.append(TransBlock(sphere, translate=(x,y,z), scale=(self.girth, self.girth, self.girth)))


        rotation = math.degrees(math.atan2(self.end[2] - self.start[2], self.end[0] - self.start[0]))

        full_layer = TransBlock(
            Tree(sides + joints),
            (0,-1,0,rotation),
            center,
            (1,1,1)
        )

        return full_layer.scene_rep()


class TiltedSphereChain(SceneComponent):
    def __init__(self, material, start, end, count):
        self.material = material
        self.start = start
        self.end = end
        self.count = count

    def scene_rep(self):
        sphere = Sphere(self.material)
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
    def __init__(self, material, top, length, radius):
        self.material = material
        self.top = top
        self.length = length
        self.radius = radius

    def scene_rep(self):
        return TransBlock(
            Cube(self.material),
            translate=(self.top[0], self.top[1] - self.length / 2, self.top[2]),
            rotate=(1, 0, 0, 00),
            scale=(self.radius, self.length, self.radius),
        ).scene_rep()


def SphereChain(material, top, count, radius):
    sphere = Sphere(material)
    return TaperedChain(sphere, top, count, radius, radius)


def TaperedSphereChain(material, top, count, start_radius, end_radius):
    sphere = Sphere(material)
    return TaperedChain(sphere, top, count, start_radius, end_radius)


def DiamondChain(material, top, count, radius):
    diamond = Tree(
        [
            TransBlock(
                Cube(material),
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
