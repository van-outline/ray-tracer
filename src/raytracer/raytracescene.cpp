#include <stdexcept>
#include "raytracescene.h"
#include "utils/sceneparser.h"


RayTraceScene::RayTraceScene(int width, int height, const RenderData &metaData)
    : m_width(width), m_height(height), m_metaData(metaData), m_camera(metaData.cameraData) {
}

const int& RayTraceScene::width() const {
    return m_width;
}

const int& RayTraceScene::height() const {
    return m_height;
}

const SceneGlobalData& RayTraceScene::getGlobalData() const {
    return m_metaData.globalData;
}

const Camera& RayTraceScene::getCamera() const {
    return m_camera;
}

const std::vector<RenderShapeData> RayTraceScene::getShape() const {
    return m_metaData.shapes;
}

const std::vector<SceneLightData> RayTraceScene::getLight() const {
    return m_metaData.lights;
}


