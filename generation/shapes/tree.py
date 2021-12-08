from common import SceneComponent

class Tree(SceneComponent):
    def __init__(self, inner):
        self.inner = inner

    def scene_rep(self):
        inner_string = '\n'.join([layer.scene_rep() for layer in self.inner])

        return (
                '<object type="tree">\n'
                "    {}\n"
                "</object >\n"
            ).format(inner_string)