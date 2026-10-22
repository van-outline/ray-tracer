#include "cone.h"
#include "utils/sceneparser.h"

Cone::Cone(const RenderShapeData *shape)
    : Shape(shape), s_shape(shape) {}

Hit Cone::shape_intersect(glm::vec3 p, glm::vec3 d) const {
    float a = d[0]*d[0] + d[2]*d[2] - 0.25f * (d[1] * d[1]);
    float b = 2 * (p[0] * d[0]) + 2 * (p[2] * d[2]) - 0.5f * (p[1] * d[1]) + 0.25f * d[1];
    float c = p[0] * p[0] + p[2] * p[2] - 0.25f * (p[1] * p[1]) + 0.25f * p[1] - 0.0625f;
    float disc = b * b - 4.f * a * c;
    float t1 = (-b + glm::sqrt(disc))/(2 * a);
    float t2 = (-b - glm::sqrt(disc))/(2 * a);
    glm::vec3 sect1 = p + t1*d;
    glm::vec3 sect2 = p + t2*d;
    if(sect1[1] > 0.5 || sect1[1] < -0.5){
        t1 = -1.f;
    }
    if(sect2[1] > 0.5 || sect2[1] < -0.5){
        t2 = -1.f;
    }
    float t3 = (-0.5f - p[1]) / d[1];
    glm::vec3 sect3 = p + t3*d;
    if(sect3[0] * sect3[0] + sect3[2] * sect3[2] > 0.25){
        t3 = -1.f;

    }
    float t = std::numeric_limits<float>::infinity();
    for (float ti : {t1, t2, t3}) {
        if (ti >= 0.f && ti < t){
            t = ti;
        }
    }
    if (t == std::numeric_limits<float>::infinity()) t = -1.f;
    glm::vec3 obj_point = p + t * d;
    glm::vec3 n_o;
    if(t == t3){
        n_o = glm::vec3(0,-1,0);
    }
    else{
        n_o = glm::normalize((glm::vec3(8 * obj_point[0], -2 * obj_point[1] + 1, 8 * obj_point[2])));
    }
    return Hit{t, s_shape, n_o};
}