#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Camera {
public:
  glm::vec3 position;
  glm::vec3 target;
  glm::vec3 up;

  float rotationSpeed;

  Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float rotationSpeed): position(position), target(target), up(up), rotationSpeed(rotationSpeed) {}

  void rotate(float deltaX, float deltaY) {
    glm::quat quatRotY = glm::angleAxis(glm::radians(deltaX * rotationSpeed), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat quatRotX = glm::angleAxis(glm::radians(deltaY * rotationSpeed), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::vec3 rotatedPos = quatRotY * quatRotX * (position - target);

    position = target + rotatedPos;
  }

  void move(float deltaZ) {
    glm::vec3 dir = glm::normalize(target - position);
    position += dir * deltaZ;
  }

  glm::mat4 getViewMatrix() const {
        return glm::lookAt(position, target, up);
    }
};

