#include "shape.h"
#include "ray/ray.h"
#include "shapes/cube.h"
#include "shapes/cylinder.h"
#include "shapes/sphere.h"
#include "shapes/cone.h"

Shape::Shape(const RenderShapeData *shape) : m_shape(shape){}

Hit Shape::intersect(const Ray &ray) const{
    glm::vec3 p = glm::vec3(m_shape->inverse_ctm * ray.origin);
    glm::vec3 d = glm::vec3(m_shape->inverse_ctm * glm::vec4(ray.direction, 0.f));

    Hit hit{-1, nullptr, glm::vec3(0)};

    switch (m_shape->primitive.type) {
    case PrimitiveType::PRIMITIVE_SPHERE: {
        Sphere sphere(m_shape);
        hit = sphere.shape_intersect(p, d);
        break;
    }
    case PrimitiveType::PRIMITIVE_CONE: {
        Cone cone(m_shape);
        hit = cone.shape_intersect(p, d);
        break;
    }
    case PrimitiveType::PRIMITIVE_CYLINDER: {
        Cylinder cylinder(m_shape);
        hit = cylinder.shape_intersect(p, d);
        break;
    }
    case PrimitiveType::PRIMITIVE_CUBE: {
        Cube cube(m_shape);
        hit = cube.shape_intersect(p, d);
        break;
    }
    default:
        break;
    }
    if(hit.t != -1){
        hit.hitPoint = p + hit.t * d;
        hit.normal = glm::normalize(glm::vec3(glm::transpose(hit.shape->inverse_ctm) * glm::vec4(hit.normal, 0.f)));
    }
    return hit;
}