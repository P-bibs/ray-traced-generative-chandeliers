import abc

class SceneComponent(metaclass=abc.ABCMeta):
    @abc.abstractmethod
    def scene_rep(self):
        raise Exception("Scene rep not implemented")

