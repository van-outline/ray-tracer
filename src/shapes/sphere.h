#ifndef SPHERE_H
#define SPHERE_H

#include "glm/ext/vector_float3.hpp"
#include "hit/hit.h"
#include "shapes/shape.h"
class Sphere : public Shape
{
public:
    Sphere(const RenderShapeData *shape);
    Hit shape_intersect(glm::vec3 p, glm::vec3 d) const;
private:
    const RenderShapeData *s_shape;
};

#endif // SPHERE_H
