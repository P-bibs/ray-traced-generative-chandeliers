

class Settings():
    def __init__(self):
        self.diffuse = 0.6
        self.specular = 1
        self.ambient = 0.5
        self.transparent = 1
        self.camera_position = [0, 1, -10]
        self.camera_focus = [0, 0.1, 1]
        self.camera_angle = 45

        self.environment_map = "../assets/fireplace"

        self.default_diffuse = (1.0, 1.0, 1.0)
        self.default_specular = (0.7, 0.7, 0.7)
        self.default_ambient = (0.2, 0.2, 0.2)
        self.default_reflective = (0.5, 0.5, 0.5)
        self.default_transparent = (0.5, 0.5, 0.5)
        self.default_shininess = 7
        self.default_refraction = 1.5


settings = Settings()