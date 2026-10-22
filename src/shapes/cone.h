#ifndef CONE_H
#define CONE_H

#include "shapes/shape.h"
class Cone : public Shape
{
public:
    Cone(const RenderShapeData *shape);
    Hit shape_intersect(glm::vec3 p, glm::vec3 d) const;
private:
    const RenderShapeData *s_shape;
};

#endif // CONE_H
