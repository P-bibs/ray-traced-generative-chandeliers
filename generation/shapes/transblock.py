from common import SceneComponent

class TransBlock(SceneComponent):
    def __init__(self, inner, rotate=None, translate=None, scale=None):
        if type(inner) is list:
            raise Exception("TransBlock can only have one inner component")
        self.inner = inner
        self.rotate = rotate or (0, 0, 0, 0)
        self.translate = translate or (0, 0, 0)
        self.scale = scale or (1, 1, 1)

    def scene_rep(self):
        rotate_string = (
            '<rotate x="{}" y="{}" z="{}" angle="{}"/>'.format(
                self.rotate[0], self.rotate[1], self.rotate[2], self.rotate[3]
            )
            if self.rotate != (0, 0, 0, 0)
            else ""
        )
        translate_string = (
            '<translate x="{}" y="{}" z="{}"/>'.format(
                self.translate[0], self.translate[1], self.translate[2]
            )
            if self.translate != (0, 0, 0)
            else ""
        )
        scale_string = (
            '<scale x="{}" y="{}" z="{}"/>'.format(
                self.scale[0], self.scale[1], self.scale[2]
            )
            if self.scale != (1, 1, 1)
            else ""
        )

        inner_string = self.inner.scene_rep()

        # if rotate_string == "" and translate_string == "" and scale_string == "":
        #     return inner_string
        # else:
        return (
            "<transblock>\n"
            "    {}\n"
            "    {}\n"
            "    {}\n"
            "    {}\n"
            "</transblock>\n"
        ).format(rotate_string, translate_string, scale_string, inner_string)
