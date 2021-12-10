#ifndef SCENE_H
#define SCENE_H

#include "CS123SceneData.h"
#include <QImage>
#include <memory>

class Camera;
class CS123ISceneParser;

/**
 * @class Scene
 *
 * @brief This is the base class for all scenes. Modify this class if you want to provide
 * common functionality to all your scenes.
 */
class Scene {
  public:
    Scene();
    Scene(Scene &scene);
    virtual ~Scene();

    virtual void settingsChanged() {}

    void loadEvironmentMap();

    static void parse(Scene *sceneToFill, CS123ISceneParser *parser);

    std::vector<std::tuple<CS123ScenePrimitive, glm::mat4x4>> &getPrimitives() {
        return m_primitives;
    };
    CS123ScenePrimitive *getPrimitive(int i) { return &std::get<0>(m_primitives[i]); };

    CS123SceneEnvironmentMap &getEnvironmentMap() { return m_environmentMap; };

    CS123SceneGlobalData &getGlobalData() { return m_globalData; };
    std::vector<CS123SceneLightData> &getLightData() { return m_lightData; };

  protected:
    // Adds a primitive to the scene.
    virtual void addPrimitive(const CS123ScenePrimitive &scenePrimitive, const glm::mat4x4 &matrix);

    // Adds a light to the scene.
    virtual void addLight(const CS123SceneLightData &sceneLight);

    // Sets the global data for the scene.
    virtual void setGlobal(const CS123SceneGlobalData &global);

    // Set environment map information
    virtual void setEnvironmentMap(const CS123SceneEnvironmentMap &envMap);

  private:
    void traverseTreeAndAddPrimitives(CS123SceneNode &node, glm::mat4x4 transformationSoFar);

    std::vector<CS123SceneLightData> m_lightData;
    CS123SceneGlobalData m_globalData;
    std::vector<std::tuple<CS123ScenePrimitive, glm::mat4x4>> m_primitives;
    CS123SceneEnvironmentMap m_environmentMap;
};

#endif // SCENE_H
