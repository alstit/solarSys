#pragma once
#include <glimac/TrackballCamera.hpp>
#include <glimac/FreeflyCamera.hpp>
#include <memory>
#include <typeinfo>
#include <string>




class Camera final
{
private:
    std::unique_ptr<ICamera> aCamera;

public:
    Camera(std::unique_ptr<ICamera> new_camera)
        : aCamera(std::move(new_camera))
    {}

    std::string getType()
    {
        return aCamera->aGetType();
    }

    glm::mat4  getViewMatrix(glm::vec3 lookat,float distance,float fov)
    {
        return aCamera->getMatrix(lookat,distance,fov);
    }
    
    
    glm::mat4 getViewMatrix()
    {
        return aCamera->getMatrix();
    }

    void moveLeft(float delta)
    {
        aCamera->getMoveLeft(delta);
    }

    void rotateLeft(float delta)
    {
        aCamera->getRotateLeft(delta);
    }

    void rotateUp(float delta)
    {
        aCamera->getRotateUp(delta);
    }

    void moveFront(float delta)
    {
        aCamera->getMoveFront(delta);
    }

    void setAlgorithme(std::unique_ptr<ICamera> new_camera)
    {
        aCamera = std::move(new_camera);
    }
};