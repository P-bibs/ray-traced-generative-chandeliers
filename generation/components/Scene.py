from common import SceneComponent
from components.Chandelier import Chandelier
from settings import settings
import context

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
    def __init__(self):
        self.diffuse = settings.diffuse 
        self.specular = settings.specular 
        self.ambient = settings.ambient 
        self.transparent = settings.transparent
        self.camera = Camera(settings.camera_position, settings.camera_focus, (0,1,0), settings.camera_angle)

        self.env_map = settings.environment_map

        # context.append_light(
        #     PointLight(context.get_id(), (0.5, 0.5, 0.5), (10, 0, 0))
        # )
        # context.append_light(
        #     PointLight(context.get_id(), (0.5, 0.5, 0.5), (0, 0, 10))
        # )
        # context.append_light(
        #     PointLight(context.get_id(), (0.5, 0.5, 0.5), (-10, 0, 0))
        # )
        # context.append_light(
        #     PointLight(context.get_id(), (0.5, 0.5, 0.5), (0, 0, -10))
        # )
        context.append_light(
            DirectionalLight(context.get_id(), (0.5, 0.5, 0.5), (-1, -1, -1))
        )
        self.chandelier = Chandelier()

    def scene_rep(self):
        light_data = [light.scene_rep() for light in context.get_lights()]
        light_data = '\n'.join(light_data)
        return ('<scenefile>\n'
                '<globaldata>\n'
                '<diffusecoeff v="{}"/>\n'
                '<specularcoeff v="{}"/>\n'
                '<ambientcoeff v="{}"/>\n'
                '<transparentcoeff v="{}"/>\n'
                '</globaldata>\n'
                '<environmentdata>\n'
                '<folderpath folderpath="{}"/>\n'
                '</environmentdata>\n'
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
                    self.transparent,
                    self.env_map,
                    self.camera.scene_rep(),
                    light_data,
                    self.chandelier.scene_rep())


