#ifndef LIGHT_H
#define LIGHT_H

#include "hit/hit.h"
#include "ray/ray.h"
#include "raytracer/raytracescene.h"
#include "utils/rgba.h"
#include "utils/scenedata.h"
class Light
{
public:
    Light(const SceneLightData *light);
    glm::vec4 phong(const Hit &hit, const Ray &hitRay, const Ray &lightRay, const RayTraceScene &scene, const float theta
        // glm::vec3  position,
        //        glm::vec3  normal,
        //        glm::vec3  directionToCamera
               //,
               //Material  &material,
               //std::vector<LightInfo> &lights,
               //Sampler   &reflectionSampler
               );
    SceneLightData getLight();

private:
    RGBA toRGBA(const glm::vec4 &illumination);
    const SceneLightData *m_light;
};

#endif // LIGHT_H
