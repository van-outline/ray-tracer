#include "light.h"
#include "light/Directionallight.h"
#include "utils/rgba.h"

Light::Light(const SceneLightData *light) : m_light(light){}

RGBA Light::toRGBA(const glm::vec4 &illumination) {
    // Task 1
    RGBA color;
    color.r = 255 * std::min(std::max(illumination[0], 0.0f), 1.0f);
    color.g = 255 * std::min(std::max(illumination[1], 0.0f), 1.0f);
    color.b = 255 * std::min(std::max(illumination[2], 0.0f), 1.0f);
    return color;
}

// Calculates the RGBA of a pixel from intersection infomation and globally-defined coefficients
glm::vec4 Light::phong(const Hit &hit, const Ray &hitRay, const Ray &lightRay, const RayTraceScene &scene, const float theta
                  //,
           // glm::vec3  normal,
           // glm::vec3  directionToCamera
           //,
           //Material  &material,
           //std::vector<LightInfo> &lights,
           //Sampler   &reflectionSampler
           ) {
    glm::vec3 normal = glm::normalize(hit.normal);
    glm::vec3 directionToCamera = -glm::normalize(hitRay.direction);
    SceneMaterial material = hit.shape->primitive.material;

    glm::vec4 illumination(0, 0, 0, 0);

    float kd = scene.getGlobalData().kd;
    float ks = scene.getGlobalData().ks;

    float red = m_light->color[0];
    float green = m_light->color[1];
    float blue = m_light->color[2];

    float att = 1;

    glm::vec3 w;

    if(m_light->type != LightType::LIGHT_DIRECTIONAL){
        float c1 = m_light->function[0];
        float c2 = m_light->function[1];
        float c3 = m_light->function[2];

        float d = glm::distance(hit.hitPoint, glm::vec3(m_light->pos));
        att = glm::min(1.0f, 1.0f/(c1 + c2 * d + c3 * d * d));

    }

    switch (m_light->type) {
    case LightType::LIGHT_DIRECTIONAL: {

        w = glm::normalize(glm::vec3(m_light->dir));
        break;

    }
    case LightType::LIGHT_POINT: {

        w = glm::normalize(lightRay.direction);
        break;

    }
    case LightType::LIGHT_SPOT: {

        w = glm::normalize(lightRay.direction);

        float outer = m_light->angle / 2;
        float inner = outer - m_light->penumbra;

        if(inner < theta & theta <= outer){
            float falloff = -2.f * std::pow(((theta - inner)/(outer - inner)), 3) +
                            3.f * std::pow(((theta - inner)/(outer - inner)), 2);
            red *= (1 - falloff);
            green *= (1 - falloff);
            blue *= (1 - falloff);
        }

        break;
    }
    default:
        break;
    }

    float nl = -glm::dot(normal, w);
    if (nl < 0){
        nl = 0;
    }
    illumination[0] += att * red * (kd * material.cDiffuse[0] * (nl));
    illumination[1] += att * green * (kd * material.cDiffuse[1] * (nl));
    illumination[2] += att * blue * (kd * material.cDiffuse[2] * (nl));

    glm::vec3 r = 2 * (glm::dot(w, normal)) * normal - w;
    float rv = glm::dot(r, directionToCamera);
    if (rv < 0){
        rv = 0;
    }

    illumination[0] += att * red * (ks * material.cSpecular[0] * glm::pow(rv, material.shininess));
    illumination[1] += att * green * (ks * material.cSpecular[1] * glm::pow(rv, material.shininess));
    illumination[2] += att * blue * (ks * material.cSpecular[2] * glm::pow(rv, material.shininess));

    // // Task 7: uncomment the following lines and correct the reflection term.
    // //      The following code uses Sampler::getReflection(glm::vec3 startPosition, glm::vec3 lightDirection)
    // //      to get the reflection intensity when "recursively raytracing" in some direction from some position

    // glm::vec3 reflectedRay = -directionToCamera - 2.f*glm::dot(-directionToCamera, normal)*normal; // <-- fix this calculation
    // illumination += kr * reflectionSampler.getReflection(position, reflectedRay);   // <-- no need to edit this after uncommenting
    return illumination;
}

SceneLightData Light::getLight(){ return *m_light;}