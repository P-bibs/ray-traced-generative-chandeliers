from common import SceneComponent

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



context = {
    "lights": []
}

next_id = 0

def get_id():
    global next_id
    next_id += 1
    return next_id
    

def get_lights():
    return context['lights']

def append_light(light):
    context['lights'].append(light)

