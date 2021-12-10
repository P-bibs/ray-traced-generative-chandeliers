#include "Scene.h"
#include "CS123ISceneParser.h"
#include "Camera.h"

#include "glm/ext.hpp"
#include "glm/gtx/transform.hpp"

#include <iostream>
#include <string>
#include <tuple>

Scene::Scene() {}

Scene::Scene(Scene &scene) {
    // We need to set the global constants to one when we duplicate a scene,
    // otherwise the global constants will be double counted (squared)
    // CS123SceneGlobalData global = {1, 1, 1, 1};
    // setGlobal(global);

    // TODO [INTERSECT]
    // Make sure to copy over the lights and the scenegraph from the old scene,
    // as well as any other member variables your new scene will need.
    m_lightData = std::vector<CS123SceneLightData>(scene.m_lightData);
    m_globalData = scene.m_globalData;
    m_primitives = std::vector<std::tuple<CS123ScenePrimitive, glm::mat4x4>>(scene.m_primitives);
    m_environmentMap = scene.m_environmentMap;

    // Clone existing textures to new scene
    //    auto new_textures = std::vector<std::shared_ptr<QImage>>();
    //    for (int i = 0; i < scene.m_textures.size(); i++) {
    //       std::shared_ptr<QImage> texture = scene.m_textures[i];
    //       new_textures.push_back(texture);
    //    }
    //    m_textures = new_textures;
}

Scene::~Scene() {
    // Do not delete m_camera, it is owned by SupportCanvas3D
}

void Scene::parse(Scene *sceneToFill, CS123ISceneParser *parser) {
    // TODO: load scene into sceneToFill using setGlobal(), addLight(), addPrimitive(), and
    //   finishParsing()

    // save global data
    CS123SceneGlobalData globalData;
    parser->getGlobalData(globalData);
    sceneToFill->setGlobal(globalData);

    // save Environment Map Data

    CS123SceneEnvironmentMap envMapData;
    parser->getEnvironmentMapData(envMapData);
    sceneToFill->setEnvironmentMap(envMapData);

    // save light data
    auto numLights = parser->getNumLights();
    sceneToFill->m_lightData.reserve(numLights);
    for (int i = 0; i < numLights; i++) {
        CS123SceneLightData lightData;
        parser->getLightData(i, lightData);
        sceneToFill->addLight(lightData);
    }

    // traverse tree and add primitive data while accumulating transformations
    CS123SceneNode rootNode = *(parser->getRootNode());
    sceneToFill->traverseTreeAndAddPrimitives(rootNode, glm::mat4(1.0f));
    printf("Scene parsed\n");
    //    std::cout << "\n\nPRINTING\n\n" << std::endl;
    //    for (auto p : m_primitives) {
    //        std::printf("%d\n", (std::get<0>(p).type));
    //        std::cout << glm::to_string(std::get<1>(p)) << std::endl;
    //    }
}

void Scene::traverseTreeAndAddPrimitives(CS123SceneNode &node, glm::mat4x4 transformationSoFar) {

    auto transformations = node.transformations;

    glm::mat4x4 totalTransformationMatrix = transformationSoFar;

    // apply transformations in proper order
    for (auto transformation : transformations) {
        if (transformation->type == TRANSFORMATION_MATRIX) {
            totalTransformationMatrix = totalTransformationMatrix * transformation->matrix;
        } else if (transformation->type == TRANSFORMATION_TRANSLATE) {
            totalTransformationMatrix =
                totalTransformationMatrix * glm::translate(transformation->translate);
        } else if (transformation->type == TRANSFORMATION_ROTATE) {
            totalTransformationMatrix = totalTransformationMatrix *
                                        glm::rotate(transformation->angle, transformation->rotate);
        } else if (transformation->type == TRANSFORMATION_SCALE) {
            totalTransformationMatrix =
                totalTransformationMatrix * glm::scale(transformation->scale);
        }
    }

    // add primitives with accumulated transform
    for (auto primitive : node.primitives) {
        m_primitives.push_back(std::make_tuple(*primitive, totalTransformationMatrix));
    }

    // traverse children
    for (auto child : node.children) {
        traverseTreeAndAddPrimitives(*child, totalTransformationMatrix);
    }
}

void Scene::addPrimitive(const CS123ScenePrimitive &scenePrimitive, const glm::mat4x4 &matrix) {
    m_primitives.push_back(
        std::make_tuple(CS123ScenePrimitive(scenePrimitive), glm::mat4x4(matrix)));
}

void Scene::addLight(const CS123SceneLightData &sceneLight) {
    m_lightData.push_back(CS123SceneLightData(sceneLight));
}

void Scene::setGlobal(const CS123SceneGlobalData &global) {
    m_globalData = CS123SceneGlobalData(global);
}

void Scene::setEnvironmentMap(const CS123SceneEnvironmentMap &envMap) {
    m_environmentMap = envMap;
}

void Scene::loadEvironmentMap() {
    CS123SceneEnvironmentMap envMap = this->m_environmentMap;
    if (envMap.isUsed) {
        this->m_environmentMap.pos_x = QImage(QString::fromStdString(envMap.filePath + "/posx.jpg"));
        this->m_environmentMap.pos_y = QImage(QString::fromStdString(envMap.filePath + "/posy.jpg"));
        this->m_environmentMap.pos_z = QImage(QString::fromStdString(envMap.filePath + "/posz.jpg"));

        this->m_environmentMap.neg_x = QImage(QString::fromStdString(envMap.filePath + "/negx.jpg"));
        this->m_environmentMap.neg_y = QImage(QString::fromStdString(envMap.filePath + "/negy.jpg"));
        this->m_environmentMap.neg_z = QImage(QString::fromStdString(envMap.filePath + "/negz.jpg"));
    }
}
