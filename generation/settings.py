

class Settings():
    def __init__(self):
        self.diffuse = 0.5
        self.specular = 0.5
        self.ambient = 0.5
        self.camera_position = [0, 0, 0]
        self.camera_focus = [0, 0, 0]
        self.camera_angle = 45

        self.default_diffuse = (1.0, 1.0, 1.0)
        self.default_specular = (0.7, 0.7, 0.7)
        self.default_ambient = (0.2, 0.2, 0.2)
        self.default_reflective = (0.0, 0.0, 0.0)
        self.default_transparent = (0.0, 0.0, 0.0)
        self.default_shininess = 7


settings = Settings()