#ifndef CAMERA_H
#define CAMERA_H
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_projection.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <QObject>


static float speedFactor = .01f;
static float speedZoomFactor = .001f;
static float zoomFactor = 1.f;


class Camera : public QObject{
Q_OBJECT
public:
    Camera(glm::vec3 pos = glm::vec3(0.,0.,0.), glm::vec3 at = glm::vec3(0.,0.,-1.));
    void setAt(glm::vec3 _at);
    void setPos(glm::vec3 _pos);
    void setAspect(float w, float h);


    glm::vec3 getAt() const;
    glm::vec3 getPos() const;

    glm::mat4 & getViewMatrix();
    glm::mat4 & getProjMatrix();

    void walkRight();
    void walkLeft();
    void walkFront();
    void walkBack();
    void walkUp();
    void walkDown();
    void zoomIn();
    void zoomOut();
    void increaseSpeed();
    void decreaseSpeed();

    void resetView();

    float getFov() const;

    void centerCamera(glm::vec3 center, float height);



private:
    void updateView();
    void updateProj();

private:       

    glm::mat4 viewMatrix;
    glm::mat4 projMatrix;

    glm::vec3 at;
    glm::vec3 pos;
    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 worldUp;
    glm::vec3 zoom;
    float fov;
    float width;
    float height;
    float near;
    float far;

    float speed = .001f;
    float speedZoom = 0.01f;
};

#endif // CAMERA_HPP
