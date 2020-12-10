#pragma once
#include <glimac/ICamera.hpp>



using namespace glm;

class FreeflyCamera: public ICamera
{
    public:
    glm::vec3 m_Position;
    float m_fPhi;
    float m_fTheta;
    glm::vec3 m_FrontVector ;


    std::string getType(){return "FreeflyCamera";};///to know type of camera, usefull for switching camera types
    glm::mat4  getViewMatrix(glm::vec3 lookat,float distance,float fov) {return glm::mat4();};//////// this function is here to fit the interfce ICamera
    glm::vec3 m_LeftVector,m_UpVector; 
    void computeDirectionVectors();
    FreeflyCamera(); 
    void moveLeft(float t);
    void moveFront(float t) ;
    void rotateLeft(float degrees) ;
    void rotateUp(float degrees);
    glm::mat4 getViewMatrix() override;
};



glm::mat4 FreeflyCamera::getViewMatrix() 
{
    return glm::lookAt(this->m_Position,this->m_Position+this->m_FrontVector,this->m_UpVector);
};

void FreeflyCamera::computeDirectionVectors()
{
    this->m_FrontVector = vec3(cos(this->m_fTheta)*sin(this->m_fPhi),sin(this->m_fTheta),cos(this->m_fTheta)*cos(this->m_fPhi));
    this->m_LeftVector = vec3(sin(m_fPhi+glm::pi<float>()/2),0,cos(this->m_fPhi+glm::pi<float>()/2));
    this->m_UpVector = glm::cross(this->m_FrontVector,this->m_LeftVector);
};

FreeflyCamera::FreeflyCamera()
{
    this->m_Position = glm::vec3(0,0,0);
    this->m_fPhi = glm::pi<float>();
    this->m_fTheta = 0;
    this->computeDirectionVectors();
};

void FreeflyCamera::moveLeft(float t)
{
    this->m_Position+=t*this->m_LeftVector;
};

void FreeflyCamera::moveFront(float t)
{
   
    this->m_Position+=t*this->m_FrontVector;
};

void FreeflyCamera::rotateLeft(float degrees)
{
    this->m_fPhi+=degrees;
}

void FreeflyCamera::rotateUp(float degrees)
{
    this->m_fTheta+=degrees;
}