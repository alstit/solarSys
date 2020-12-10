#pragma once
#include <string>

using namespace glm;

class ICamera
{
    public :
        glm::mat4 getMatrix(){return getViewMatrix();}
        glm::mat4  getMatrix(glm::vec3 lookat,float distance,float fov) {return getViewMatrix(lookat,distance,fov);}
        void getMoveFront(float delta){moveFront(delta);}
        void getMoveLeft(float delta){moveLeft(delta);}
        void getRotateUp(float delta){rotateUp(delta);}
        void getRotateLeft(float delta){rotateLeft(delta);}
        std::string aGetType(){return getType();}

        virtual ~ICamera() = default;
    private :
        virtual glm::mat4  getViewMatrix(glm::vec3 lookat,float distance,float fov) =0;
        virtual glm::mat4 getViewMatrix() = 0;
        virtual void moveFront(float delta) = 0;
        virtual void moveLeft(float delta) = 0;
        virtual void rotateLeft(float delta) = 0;
        virtual void rotateUp(float delta) = 0;
        virtual std::string getType() = 0;
};
