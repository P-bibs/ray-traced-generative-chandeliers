
from common import SceneComponent


class Material(SceneComponent):
    def __init__(self, diffuse=None, specular=None, ambient=None, reflective=None, transparent=None, shininess=None, refraction=None):
        self.diffuse = diffuse
        self.specular = specular
        self.ambient = ambient
        self.reflective = reflective
        self.transparent = transparent
        self.shininess = shininess
        self.refraction = refraction

    def scene_rep(self):
        concatenated = []
        if self.diffuse:
            concatenated.append('<diffuse r="{}" g="{}" b="{}"/>'.format(self.diffuse[0], self.diffuse[1], self.diffuse[2]))
        else:
            concatenated.append('<diffuse r="{}" g="{}" b="{}"/>'.format(0, 0, 0))

        if self.specular:
            concatenated.append('<specular r="{}" g="{}" b="{}"/>'.format(self.specular[0], self.specular[1], self.specular[2]))

        if self.ambient:
            concatenated.append('<ambient r="{}" g="{}" b="{}"/>'.format(self.ambient[0], self.ambient[1], self.ambient[2]))

        if self.reflective:
            concatenated.append('<reflective r="{}" g="{}" b="{}"/>'.format(self.reflective[0], self.reflective[1], self.reflective[2]))

        if self.transparent:
            concatenated.append('<transparent r="{}" g="{}" b="{}"/>'.format(self.transparent[0], self.transparent[1], self.transparent[2]))

        if self.shininess:
            concatenated.append('<shininess v="{}"/>'.format(self.shininess))

        if self.refraction:
            concatenated.append('<ior v="{}"/>'.format(self.refraction))
        
        return '\n'.join(["    " + s for s in concatenated])


gold = Material(
    ambient=(0.24, 0.19, 0.07),
    diffuse=(0.75, 0.6, 0.22),
    specular=(0.62, 0.55, 0.366),
    reflective=(0.2,0.2,0.2),
    shininess=27.8
)

silver = Material(
    ambient=( 0.231, 0.231, 0.231),
    diffuse=(0.27, 0.27, 0.27),
    specular=(0.7739, 0.7739, 0.7739),
    reflective=(0.2,0.2,0.2),
    shininess=89
)

bronze = Material(
    ambient=(0.25, 0.15, 0.064),
    diffuse=(0.4, 0.236, 0.103),
    specular=(0.77, 0.458, 0.2006),
    reflective=(0.2,0.2,0.2),
    shininess=76.8
)

glass = Material(
    specular=(0.2, 0.2, 0.2),
    shininess=15,
    transparent=(0.7, 0.7, 0.7),
    reflective=(0.3,0.3,0.3),
    refraction=1.03
)