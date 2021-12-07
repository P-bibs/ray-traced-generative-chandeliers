from common import SceneComponent


def format_inner_properties(shininess, specular, reflective, diffuse, ambient):
    return (
        '    <shininess v="{}"/>\n'
        '    <specular r="{}" g="{}" b="{}"/>\n'
        '    <reflective r="{}" g="{}" b="{}"/>\n'
        '    <diffuse r="{}" g="{}" b="{}"/>\n'
        '    <ambient r="{}" g="{}" b="{}"/>\n'
    ).format(
        shininess,
        specular[0], specular[1], specular[2],
        reflective[0], reflective[1], reflective[2],
        diffuse[0], diffuse[1], diffuse[2],
        ambient[0], ambient[1], ambient[2]
    )

valid_primitives = ["cube", "sphere", "cylinder", "cone"]
class Primitive(SceneComponent):
    def __init__(self, type, shininess=None, specular=None, reflective=None, diffuse=None, ambient=None):
        if type not in valid_primitives:
            raise ValueError("Invalid primitive type: {}".format(type))
        self.type = type
        self.shininess = shininess or 7
        self.specular = specular or (1.0, 1.0, 1.0)
        self.reflective = reflective or (0.0, 0.0, 0.0)
        self.diffuse = diffuse or (1.0, 1.0, 1.0)
        self.ambient = ambient or (0.2, 0.2, 0.2)

    def scene_rep(self):
        return (
            '<object type="primitive" name="{}">\n'
            '{}'
            '</object>\n'
        ).format(self.type, format_inner_properties(self.shininess, self.specular, self.reflective, self.diffuse, self.ambient))

def Cube(shininess=None, specular=None, reflective=None, diffuse=None, ambient=None):
    return Primitive("cube", shininess, specular, reflective, diffuse, ambient)

def Sphere(shininess=None, specular=None, reflective=None, diffuse=None, ambient=None):
    return Primitive("sphere", shininess, specular, reflective, diffuse, ambient)

def Cylinder(shininess=None, specular=None, reflective=None, diffuse=None, ambient=None):
    return Primitive("cylinder", shininess, specular, reflective, diffuse, ambient)

def Cone(shininess=None, specular=None, reflective=None, diffuse=None, ambient=None):
    return Primitive("cone", shininess, specular, reflective, diffuse, ambient)