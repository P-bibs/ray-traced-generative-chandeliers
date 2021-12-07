from common import SceneComponent
from components.Chandelier import Chandelier

class PointLight(SceneComponent):
    def __init__(self, id, color, position):
        self.id = id
        self.color = color
        self.position = position

    def scene_rep(self):
        return ('<lightdata>\n'
                '<id v="{}" />\n'
                '<position x="{}" y="{}" z="{}"/>\n'
                '<color r="{}" g="{}" b="{}"/>\n'
                '</lightdata>\n'
                ).format(
                    self.id,
                    self.position[0],
                    self.position[1],
                    self.position[2],
                    self.color[0],
                    self.color[1],
                    self.color[2])
                    

class DirectionalLight(SceneComponent):
    def __init__(self, id, color, direction):
        self.id = id
        self.color = color
        self.direction = direction

    def scene_rep(self):
        return ('<lightdata>\n'
                '<id v="{}" />\n'
                '<type v="directional"/>\n'
                '<color r="{}" g="{}" b="{}"/>\n'
                '<direction x="{}" y="{}" z="{}"/>\n'
                '</lightdata>\n'
                ).format(
            self.id,
            self.color[0],
            self.color[1],
            self.color[2],
            self.direction[0],
            self.direction[1],
            self.direction[2])

class Camera(SceneComponent):
    def __init__(self, position, focus, up, height_angle):
        self.position = position
        self.focus = focus
        self.up = up
        self.height_angle = height_angle

    def scene_rep(self):
        return ('<cameradata>\n'
                '<pos x="{}" y="{}" z="{}"/>\n'
                '<look x="{}" y="{}" z="{}"/>\n'
                '<up x="{}" y="{}" z="{}"/>\n'
                '<heightangle v="{}"/>\n'
                '</cameradata>\n'
                ).format(
                    self.position[0],
                    self.position[1],
                    self.position[2],
                    self.focus[0],
                    self.focus[1],
                    self.focus[2],
                    self.up[0],
                    self.up[1],
                    self.up[2],
                    self.height_angle)

class Scene(SceneComponent):
    def __init__(self, diffuse=None, specular=None, ambient=None, camera=None):
        self.diffuse = diffuse or 0.5
        self.specular = specular or 0.5
        self.ambient = ambient or 0.5
        self.camera = camera or Camera((0,0,3), (0,0,-1), (0,1,0), 45)
        self.lights = [PointLight(1, (1, 1, 1), (0, 0, 1))]
        self.chandelier = Chandelier()

    def scene_rep(self):
        light_data = [light.scene_rep() for light in self.lights]
        light_data = '\n'.join(light_data)
        return ('<scenefile>\n'
                '<globaldata>\n'
                '<diffusecoeff v="{}"/>\n'
                '<specularcoeff v="{}"/>\n'
                '<ambientcoeff v="{}"/>\n'
                '</globaldata>\n'
                '{}\n'
                '{}\n'
                '<object type="tree" name="root">\n'
                '{}\n'
                '</object>\n'
                '</scenefile>\n'
                ).format(
                    self.diffuse,
                    self.specular,
                    self.ambient,
                    self.camera.scene_rep(),
                    light_data,
                    self.chandelier.scene_rep())


