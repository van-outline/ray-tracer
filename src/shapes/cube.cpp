#include "cube.h"

Cube::Cube(const RenderShapeData *shape)
    : Shape(shape), s_shape(shape) {}

Hit Cube::shape_intersect(glm::vec3 p, glm::vec3 d) const {
    float tmin = -std::numeric_limits<float>::infinity();
    float tmax =  std::numeric_limits<float>::infinity();
    for (int k = 0; k < 3; k++) {
        float t1 = (-0.5f - p[k]) / d[k];
        float t2 = ( 0.5f - p[k]) / d[k];
        tmin = std::max(tmin, std::min(t1, t2));
        tmax = std::min(tmax, std::max(t1, t2));
    }
    float t = -1.f;
    float ins = 1.f;
    if (tmin <= tmax && tmax > 0) {
        if (tmin > 0) {
            t = tmin;
        } else {
            t = tmax;
            ins = -1.f;
        }
    }
    int ax = 0;
    glm::vec3 obj_point = p + t * d;
    glm::vec3 n_o;
    for (int k = 1; k < 3; k++) {
        if (std::abs(obj_point[k]) > std::abs(obj_point[ax])) ax = k;
    }
    n_o = glm::vec3(0.f);
    if (obj_point[ax] > 0.f) {
        n_o[ax] = ins;
    } else {
        n_o[ax] = -ins;
    }
    return Hit{t, s_shape, n_o};
}