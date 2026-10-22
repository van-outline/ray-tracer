#include "sceneparser.h"
#include "scenefilereader.h"
#include <glm/gtx/transform.hpp>

#include <chrono>
#include <iostream>

bool SceneParser::parse(std::string filepath, RenderData &renderData) {
    ScenefileReader fileReader = ScenefileReader(filepath);
    bool success = fileReader.readJSON();
    if (!success) {
        return false;
    }

    // Task 10: populate renderData with global data, lights, and camera data

    renderData.globalData = fileReader.getGlobalData();
    renderData.cameraData = fileReader.getCameraData();
    renderData.lights.clear();


    // Task 11: populate renderData's list of primitives and their transforms.
    //          This will involve traversing the scene graph, and we recommend
    //          you create a helper function to do so!

    renderData.shapes.clear();
    SceneNode* root = fileReader.getRootNode();
    glm::mat4 origin = glm::mat4(0.0f);
    SceneNode* root_node = fileReader.getRootNode();
    dfs(root_node, renderData, glm::mat4(1.0f));
    return true;
}

void SceneParser::dfs(SceneNode* node, RenderData& renderData, glm::mat4 old_ctm){
    if (!node){
        return;
    }
    glm::mat4 new_ctm = old_ctm;
    for(SceneTransformation* t : node->transformations){
        switch(t->type){
        case TransformationType::TRANSFORMATION_TRANSLATE:
            new_ctm = new_ctm * glm::translate(glm::mat4(1.0f), t->translate);
            break;
        case TransformationType::TRANSFORMATION_SCALE:
            new_ctm = new_ctm * glm::scale(glm::mat4(1.0f), t->scale);
            break;
        case TransformationType::TRANSFORMATION_ROTATE:
            new_ctm = new_ctm * glm::rotate(glm::mat4(1.0f), t->angle, t->rotate);
            break;
        case TransformationType::TRANSFORMATION_MATRIX:
            new_ctm = new_ctm * t->matrix;
            break;
        }
    }
    for(ScenePrimitive* p : node->primitives){
        RenderShapeData shape;
        shape.primitive = *p;
        shape.ctm = new_ctm;
        shape.inverse_ctm = glm::inverse(new_ctm);
        renderData.shapes.push_back(shape);
    }
    for (SceneLight* l : node->lights) {
        SceneLightData light;
        light.id = l->id;
        light.type = l->type;
        light.color = l->color;
        light.function = l->function;
        light.penumbra = l->penumbra;
        light.angle = l->angle;
        light.width = l->width;
        light.height = l->height;

        light.pos = new_ctm * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        light.dir = new_ctm * glm::vec4(l->dir);

        renderData.lights.push_back(light);
    }
    for (SceneNode* child : node->children) {
        dfs(child, renderData, new_ctm);
    }
}
