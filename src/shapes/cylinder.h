#ifndef CYLINDER_H
#define CYLINDER_H

#include "hit/hit.h"
#include "shapes/shape.h"
#include "utils/sceneparser.h"
class Cylinder : public Shape
{
public:
    Cylinder(const RenderShapeData *shape);
    Hit shape_intersect(glm::vec3 p, glm::vec3 d) const;
private:
    const RenderShapeData *s_shape;
};

#endif // CYLINDER_H
