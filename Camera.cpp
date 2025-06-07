#include "Camera.h"
#include "glm/glm/detail/qualifier.hpp"
#include "glm/glm/ext/matrix_clip_space.hpp"
#include "glm/glm/ext/matrix_float4x4.hpp"
#include "glm/glm/ext/vector_float3.hpp"
#include "glm/glm/geometric.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "glm/glm/trigonometric.hpp"
#include <cmath>
#include <cstring>

Camera::Camera(){
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    look = glm::vec3(0,0, 1);
    perspective =glm::mat4(glm::perspective(glm::radians(85.0f), 800.0f/600.0f, .1f, 100.0f));
    setView();
}
Camera::Camera(GLfloat fov,GLfloat aspect,GLfloat  near,GLfloat  far){
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    look = glm::vec3(0,0, 1);
    perspective =glm::mat4(glm::perspective(fov, aspect, near, far));
    setView();
}

void Camera::translate(glm::vec3 t){
    position += t;
}
void Camera::moveLook(glm::vec3 t){
    if(t != glm::vec3(0,0,0))
        look = t;
}
void Camera::setView(){
    this->view = glm::lookAt(
        position, // eye
        position + look, // look at
        glm::vec3(0, 1, 0)  // up
    );

}
glm::mat4 Camera::getView(){
    return this->view;
}
