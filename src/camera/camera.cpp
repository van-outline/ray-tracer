#include <stdexcept>
#include "camera.h"
#include <cmath>

Camera::Camera(SceneCameraData camera_data): m_cameradata(camera_data), m_heightAngle(m_cameradata.heightAngle) {
    glm::vec3 pos = m_cameradata.pos;
    glm::vec3 up = m_cameradata.up;
    glm::vec3 look = m_cameradata.look;
    glm::vec3 w = glm::normalize(-look);
    glm::vec3 v = glm::normalize((up-(glm::dot(up,w))*w));
    glm::vec3 u = glm::cross(v,w);
    glm::mat4 m_t = glm::mat4(1,0,0,0,
                              0,1,0,0,
                              0,0,1,0,
                              -pos[0],-pos[1],-pos[2],1);
    glm::mat4 m_r = glm::mat4(u[0],v[0],w[0],0,
                              u[1],v[1],w[1],0,
                              u[2],v[2],w[2],0,
                              0,0,0,1);
    m_viewmatrix = m_r * m_t;
    m_inverseMatrix = glm::inverse(m_viewmatrix);
}

glm::mat4 Camera::getViewMatrix() {
    return m_viewmatrix;
}

Ray Camera::generateRay(int row, int col, int width, int height) const {
    float m_widthAngle = 2 * std::atan((static_cast<float>(width)/height) * std::tan(m_heightAngle/2));
    float x = 2 * std::tan(m_widthAngle/2) * (((col+0.5)/width)-0.5);
    float y = 2 * std::tan(m_heightAngle/2) * (((height-1-row+0.5)/height)-0.5);
    float z = -1.f;
    glm::vec4 d = glm::vec4(x, y, z, 0.f);
    glm::vec3 dir = glm::normalize(glm::vec3(m_inverseMatrix * d));
    glm::vec4 pos = m_cameradata.pos;
    pos.w = 1;
    return Ray{pos, dir};
}


float Camera::getAspectRatio() const {
    // Optional TODO: implement the getter or make your own design
    throw std::runtime_error("not implemented");
}

float Camera::getHeightAngle() const {
    // Optional TODO: implement the getter or make your own design
    throw std::runtime_error("not implemented");
}

float Camera::getFocalLength() const {
    // Optional TODO: implement the getter or make your own design
    throw std::runtime_error("not implemented");
}

float Camera::getAperture() const {
    // Optional TODO: implement the getter or make your own design
    throw std::runtime_error("not implemented");
}
