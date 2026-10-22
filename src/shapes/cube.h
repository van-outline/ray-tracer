#ifndef CUBE_H
#define CUBE_H

#include "hit/hit.h"
#include "shapes/shape.h"
#include "utils/sceneparser.h"

class Cube : public Shape
{
public:
    Cube(const RenderShapeData *shape);
    Hit shape_intersect(glm::vec3 p, glm::vec3 d) const;
private:
    const RenderShapeData *s_shape;
};

#endif // CUBE_H
