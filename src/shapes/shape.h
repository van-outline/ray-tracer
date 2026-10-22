#ifndef SHAPE_H
#define SHAPE_H

#include "hit/hit.h"
#include "ray/ray.h"
class Shape
{
public:
    Shape(const RenderShapeData *shape);
    Hit intersect(const Ray &ray) const;
private:
    const RenderShapeData *m_shape;
};


#endif // SHAPE_H
