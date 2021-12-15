from common import SceneComponent
from settings import settings
from shapes.transblock import TransBlock
from shapes.tree import Tree
import materials

def format_inner_properties(diffuse=None, specular=None, ambient=None, reflective=None, transparent=None, shininess=None, refraction=None):
    concatenated = []
    if diffuse:
        concatenated.append('<diffuse r="{}" g="{}" b="{}"/>'.format(diffuse[0], diffuse[1], diffuse[2]))

    if specular:
        concatenated.append('<specular r="{}" g="{}" b="{}"/>'.format(specular[0], specular[1], specular[2]))

    if ambient:
        concatenated.append('<ambient r="{}" g="{}" b="{}"/>'.format(ambient[0], ambient[1], ambient[2]))

    if reflective:
        concatenated.append('<reflective r="{}" g="{}" b="{}"/>'.format(reflective[0], reflective[1], reflective[2]))

    if transparent:
        concatenated.append('<transparent r="{}" g="{}" b="{}"/>'.format(transparent[0], transparent[1], transparent[2]))

    if shininess:
        concatenated.append('<shininess v="{}"/>'.format(shininess))

    if refraction:
        concatenated.append('<ior v="{}"/>'.format(refraction))
    
    return '\n'.join(["    " + s for s in concatenated])

valid_primitives = ["cube", "sphere", "cylinder", "cone", "torus"]
class Primitive(SceneComponent):
    def __init__(self, type, material):
        if type not in valid_primitives:
            raise ValueError("Invalid primitive type: {}".format(type))
        self.type = type
        self.material = material

    def scene_rep(self):
        return (
            '<object type="primitive" name="{}">\n'
            '{}'
            '</object>\n'
        ).format(self.type, self.material.scene_rep())

def Cube(material):
    return Primitive("cube", material)

def Sphere(material):
    return Tree([TransBlock(Primitive("sphere", material), scale=(2,2,2))])

def Cylinder(material):
    return Tree([TransBlock(Primitive("cylinder", material), scale=(2,1,2))])

def Cone(material):
    return Tree([TransBlock(Primitive("cone", material), scale=(2,1,2))])
