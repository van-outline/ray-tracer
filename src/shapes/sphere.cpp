#include "sphere.h"
#include "hit/hit.h"

Sphere::Sphere(const RenderShapeData *shape)
    : Shape(shape), s_shape(shape) {}

Hit Sphere::shape_intersect(glm::vec3 p, glm::vec3 d) const {
    float a = glm::dot(d,d);
    float b = 2.f * glm::dot(p, d);
    float c = glm::dot(p, p) - 0.25f;
    float disc = b * b - 4.f * a * c;
    float t;
    float ins = 1.f;

    if (disc > 0){
        float t1 = (-b + glm::sqrt(disc))/(2 * a);
        float t2 = (-b - glm::sqrt(disc))/(2 * a);

        if(t1 > 0 && t2 > 0){
            if(t1 < t2) t = t1;
            else t = t2;
        }
        else if(t1 > 0 && t2 <= 0){
            t = t1;
            ins = -1.f;
        }
        else if(t1 <= 0 && t2 > 0){
            t = t2;
            ins = -1.f;
        }
        else{
            t = -1;
        }
    }
    else{
        t = -1;
    }

    glm::vec3 obj_point = p + t * d;
    glm::vec3 n_o = glm::normalize(
                        glm::vec3(2 * obj_point[0], 2 * obj_point[1], 2 * obj_point[2])
                        );
    return Hit{t, s_shape, n_o};
}