#include "raytracer.h"
#include "raytracescene.h"
#include <vector>
#include "hit/hit.h"
#include "shapes/shape.h"
#include "light/light.h"

RayTracer::RayTracer(Config config) :
    m_config(config)
{}

void RayTracer::render(RGBA *imageData, const RayTraceScene &scene) {
    // Note that we're passing `data` as a pointer (to its first element)
    // Recall from Lab 1 that you can access its elements like this: `data[i]`

    // TODO: Implement the ray tracing algorithm. Good luck!
    for (const RenderShapeData &s : scene.getShape()) {
        Shape shape = Shape(&s);
        m_shapes.push_back(shape);
    }
    for (const SceneLightData &l : scene.getLight()) {
        Light light = Light(&l);
        m_lights.push_back(light);
    }
    for (int i = 0; i < scene.height(); i++){
        for(int j = 0; j < scene.width(); j++){
            Ray ray = scene.getCamera().generateRay(i, j, scene.width(), scene.height());
            Hit hit = traceRay(ray, scene);
            imageData[i * scene.width() + j] = (hit.t >= 0) ? determineColor(hit, scene, ray) : RGBA{0, 0, 0, 255};
        }
    }
}

Hit RayTracer::traceRay(const Ray &ray, const RayTraceScene &scene) {
    Hit closest_hit{-1, nullptr, glm::vec3(0)};

    for (Shape shape : m_shapes) {
        Hit h = shape.intersect(ray);

        if (h.t >= 0 && (closest_hit.t < 0 || h.t < closest_hit.t)) {
            closest_hit = h;
        }
    }
    return closest_hit;
}

RGBA RayTracer::determineColor(const Hit &hit, const RayTraceScene &scene, const Ray &ray){
    SceneMaterial material = hit.shape->primitive.material;

    glm::vec4 illumination(0, 0, 0, 1);

    float ka = scene.getGlobalData().ka;

    illumination[0] += ka * material.cAmbient[0];
    illumination[1] += ka * material.cAmbient[1];
    illumination[2] += ka * material.cAmbient[2];

    for (Light light : m_lights) {
        illumination += determineShadow(light, hit, scene, ray);
    }
    RGBA color;
    color.r = 255 * std::min(std::max(illumination[0], 0.0f), 1.0f);
    color.g = 255 * std::min(std::max(illumination[1], 0.0f), 1.0f);
    color.b = 255 * std::min(std::max(illumination[2], 0.0f), 1.0f);
    return color;
}

glm::vec4 RayTracer::determineShadow(Light &light, const Hit &hit, const RayTraceScene &scene, const Ray &ray){
    switch(light.getLight().type){
        case LightType::LIGHT_POINT:{
            glm::vec3 lpos = glm::vec3(light.getLight().pos);
            glm::vec3 sdir = glm::normalize(lpos - hit.hitPoint);
            glm::vec4 sorigin = glm::vec4(hit.hitPoint + 1e-4f * sdir, 1.f);
            Ray shadowRay = Ray(sorigin, sdir);
            Hit shadowHit = traceRay(shadowRay, scene);
            float tlight = glm::length(lpos - glm::vec3(sorigin));
            if(shadowHit.t < 0 || shadowHit.t > tlight){
                return light.phong(hit, ray, shadowRay, scene, 0);
            }
            break;
        }
        case LightType::LIGHT_DIRECTIONAL:{
            glm::vec3 sdir = glm::normalize(light.getLight().dir);
            glm::vec4 sorigin = glm::vec4(hit.hitPoint + 1e-4f * sdir, 1.f);
            Ray shadowRay = Ray(sorigin, sdir);
            Hit shadowHit = traceRay(shadowRay, scene);
            if(shadowHit.t < 0){
                return light.phong(hit, ray, shadowRay, scene, 0);
            }
            break;
        }
        case LightType::LIGHT_SPOT:{
            glm::vec3 lpos = glm::vec3(light.getLight().pos);
            glm::vec3 sdir = glm::normalize(lpos - hit.hitPoint);

            glm::vec3 spotDir = glm::normalize(glm::vec3(light.getLight().dir));
            float theta = glm::abs(std::acos(glm::dot(spotDir, -sdir) / (glm::length(spotDir) * glm::length(-sdir))));
            if(theta <= light.getLight().angle){
                glm::vec4 sorigin = glm::vec4(hit.hitPoint + 1e-4f * sdir, 1.f);
                Ray shadowRay = Ray(sorigin, sdir);
                Hit shadowHit = traceRay(shadowRay, scene);
                float tlight = glm::length(lpos - glm::vec3(sorigin));
                if(shadowHit.t < 0 || shadowHit.t > tlight){
                    return light.phong(hit, ray, shadowRay, scene, theta);
                }
            }
            break;
        }
    }
}

    RGBA RayTracer::normalToColor(const glm::vec3 &normal){
    return RGBA{static_cast<uint8_t>(255.f * ((normal[0] + 1.f)/2)),
                static_cast<uint8_t>(255.f * ((normal[1] + 1.f)/2)),
                static_cast<uint8_t>(255.f * ((normal[2] + 1.f)/2)),
                static_cast<uint8_t>(255.f)};
}