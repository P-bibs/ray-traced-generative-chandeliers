from common import SceneComponent

class TransBlock(SceneComponent):
    def __init__(self, inner, rotate, translate, scale):
        self.inner = inner
        self.rotate = rotate
        self.translate = translate
        self.scale = scale

    def scene_rep(self):
        return ('<transblock>\n'
                '    <rotate x="{}" y="{}" z="{}" angle="{}"/>\n'
                '    <translate x="{}" y="{}" z="{}"/>\n'
                '    <scale x="{}" y="{}" z="{}"/>\n'
                '    {}\n'
                '</transblock>\n'
        ).format(self.rotate[0], self.rotate[1], self.rotate[2], self.rotate[3],
                    self.translate[0], self.translate[1], self.translate[2],
                    self.scale[0], self.scale[1], self.scale[2], self.inner.scene_rep())