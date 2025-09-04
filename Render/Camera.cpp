#include "Camera.hpp"
#include <iostream>

Camera::Camera(glm::vec3 _pos, glm::vec3 _at){
    pos = _pos;
    at = _at;
    worldUp = glm::vec3(0.,1.,0.);
    up = worldUp;
    right = glm::vec3(1.);
    zoom = glm::vec3(1.);
    speed = speedFactor;
    speedZoom = speedZoomFactor;

    fov = 90.f;
    far = 100.f;
    near = 2.f;
    width = 956;
    height = 710;

    updateProj();
    updateView();
}

void Camera::setAt(glm::vec3 _at){
    at = _at;
    updateView();
    
}

void Camera::setPos(glm::vec3 _pos){
    pos = _pos;
    updateView();
    updateProj();
}

void Camera::setAspect(float w, float h){
    width = w;
    height = h;
    updateProj();
}


glm::vec3 Camera::getPos() const{
    return pos;
}

glm::vec3 Camera::getAt() const{
    return at;
}

float Camera::getFov() const{
    return fov;
}

glm::mat4 & Camera::getViewMatrix(){
    return viewMatrix;
}

glm::mat4 & Camera::getProjMatrix(){
    return projMatrix;
}


void Camera::walkRight(){
    //pos.x += speed;
    pos += right*speed;
    updateView();
}

void Camera::walkLeft(){
    pos -= right*speed;
    updateView();
}

void Camera::walkFront(){
    pos += at*speedZoom;
    near-= speedZoom;
    updateView();
    updateProj();
}

void Camera::walkBack(){
    pos -= at*speedZoom;
    near+=speedZoom;
    updateView();
    updateProj();
}

void Camera::walkUp(){
    pos += up*speed;
    updateView();
}

void Camera::walkDown(){
    pos -= up*speed;
    updateView();
}

void Camera::zoomIn(){
    fov-=zoomFactor;
    if (fov < 1.0f){
        fov = 1.0f;
    }
    updateProj();
}

void Camera::zoomOut(){
    fov+=zoomFactor;
    if (fov > 90.0f){
        fov = 90.0f;
    } 

    updateProj();
}

void Camera::increaseSpeed(){
    speed += speedFactor;
    speedZoom += speedZoomFactor;
}

void Camera::decreaseSpeed(){
    speed = std::max(speed - speedFactor,speedFactor);
    speedZoom = std::max(speedZoom - speedZoomFactor, speedZoomFactor);
}

void Camera::resetView(){
    pos = glm::vec3(0.,0.,1.);
    at = glm::vec3(0.,0.,-1.);
    right = glm::vec3(1.);
    zoom = glm::vec3(1.f);
    zoomFactor = 1.f;
    fov = 90.f;
    far = 100.f;
    near = 2.f;
    updateView();
    updateProj();

}

void Camera::updateView(){

    // also re-calculate the Right and Up vector
    right = glm::normalize(glm::cross(at, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up = glm::normalize(glm::cross(right, at));

    viewMatrix = glm::lookAtRH(pos,pos+at,up);
    //viewMatrix = zoomFactor>0.0001 ? glm::scale(viewMatrix,zoomFactor*zoom) : viewMatrix;

}

void Camera::updateProj(){
    projMatrix = glm::perspective(glm::radians(fov),width/height,near,far);
    //projMatrix = glm::orthoRH(-1.,1.,-1.,1.,0.01,100.);
    
}


void Camera::centerCamera(glm::vec3 center, float height) {


  float fovRad = glm::radians(fov);
  float distance = (height / 2.0f) / tan(fovRad / 2.0f);

//?????????????????????????????????
  //near+= (-1.f + distance);
  setPos({center.x, center.y, -1.f + distance});
}
