#ifndef HIT_H
#define HIT_H

#include "utils/sceneparser.h"
#include <glm/glm.hpp>

struct Hit{
    float t;
    const RenderShapeData *shape;
    glm::vec3 normal;
    glm::vec3 hitPoint;
};

#endif // HIT_H
