#pragma once
#include <glimac/SDLWindowManager.hpp>
#include <glimac/ICamera.hpp>
#include <string>
#include <solarSys/const.hpp>  
#include <iostream>

class TrackballCamera : public ICamera
{


    public:
        float m_fDistance, m_fAngleX ,m_fAngleY;
        std::string getType(){return "TrackballCamera";}

        TrackballCamera();
        void moveFront(float delta) ;
        void moveLeft(float t){}; //////// this function is here to fit the interfce ICamera
        void rotateLeft(float degrees);
        void rotateUp(float degrees);
        glm::mat4 getViewMatrix() ;
        glm::mat4  getViewMatrix(glm::vec3 lookat,float distance,float fov) ;
        float getAngleX(){return glm::radians(m_fAngleX);};
        float getAngleY(){return m_fAngleY;};
};

TrackballCamera::TrackballCamera()
{
    this->m_fDistance = 0;
    this->m_fAngleX = 0;
    this->m_fAngleY = 0;
};

void TrackballCamera::moveFront(float delta)
{
    this->m_fDistance+=delta;
};

void  TrackballCamera::rotateLeft(float degrees)
{
    this->m_fAngleX+=degrees;
       
};


void TrackballCamera::rotateUp(float degrees)
{
    this->m_fAngleY+=degrees;

};

glm::mat4  TrackballCamera::getViewMatrix() 
{
    
    glm::mat4 matrix = glm::translate(glm::mat4(),glm::vec3(0,0,this->m_fDistance));
    matrix =  glm::rotate(matrix,glm::radians(this->m_fAngleX ), glm::vec3(1,0,0));
    matrix = glm::rotate(matrix, glm::radians(this->m_fAngleY), glm::vec3(0,1,0));
    //return glm::translate(glm::mat4(),glm::vec3(0,0,3));
    return matrix;
};




glm::mat4  TrackballCamera::getViewMatrix(glm::vec3 lookat,float distance,float fov) 
{
    //glm::mat4 matrix = glm::translate(glm::mat4(),glm::vec3(0,0,this->m_fDistance));
     //glm::mat4 matrix = glm::translate(glm::mat4(),-lookat/UNITEASTRONOMIQUE+glm::vec3(0,0,this->m_fDistance));
    //matrix = glm::translate(matrix,glm::vec3(0,0,this->m_fDistance));
    glm::mat4 matrix =  glm::lookAt(lookat/UNITEASTRONOMIQUE-glm::vec3(0,0,this->m_fDistance),lookat/UNITEASTRONOMIQUE,(glm::vec3(0,1,0)));
    
    matrix =  glm::rotate(matrix,this->m_fAngleX, glm::vec3(1,0,0));
    matrix = glm::rotate(matrix, this->m_fAngleY, glm::vec3(0,0,1));
    //return glm::translate(glm::mat4(),glm::vec3(0,0,3));
    return matrix;
};