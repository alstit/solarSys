#include <glimac/SDLWindowManager.hpp>

class TrackballCamera
{
    float m_fDistance, m_fAngleX ,m_fAngleY;

    public:
        TrackballCamera();
        void moveFront(float delta) ;
        void rotateLeft(float degrees);
        void rotateUp(float degrees);
        glm::mat4 getViewMatrix() const;
        glm::mat4  getViewMatrix(glm::vec3 lookat,float distance,float fov) const;
};

TrackballCamera::TrackballCamera()
{
    this->m_fDistance = 0;
    this->m_fAngleX = 0;
    this->m_fAngleY = 0;
};

void TrackballCamera::moveFront(float delta)
{
    this->m_fDistance=delta;
};

void  TrackballCamera::rotateLeft(float degrees)
{
    this->m_fAngleX+=degrees/100;
};


void TrackballCamera::rotateUp(float degrees)
{
    this->m_fAngleY+=degrees/100;
};

glm::mat4  TrackballCamera::getViewMatrix() const
{
    glm::mat4 matrix = glm::translate(glm::mat4(),glm::vec3(0,0,-this->m_fDistance));
    matrix =  glm::rotate(matrix,glm::radians(this->m_fAngleX ), glm::vec3(1,0,0));
    matrix = glm::rotate(matrix, glm::radians(this->m_fAngleY), glm::vec3(0,1,0));
    //return glm::translate(glm::mat4(),glm::vec3(0,0,3));
    return matrix;
};

glm::mat4  TrackballCamera::getViewMatrix(glm::vec3 lookat,float distance,float fov) const
{

    glm::mat4 matrix = glm::translate(glm::mat4(),-lookat);
    matrix = glm::translate(matrix,glm::vec3(0,0,-this->m_fDistance));
    matrix =  glm::rotate(matrix,glm::radians(this->m_fAngleX ), glm::vec3(1,0,0));
    matrix = glm::rotate(matrix, glm::radians(this->m_fAngleY), glm::vec3(0,1,0));
    //return glm::translate(glm::mat4(),glm::vec3(0,0,3));
    return matrix;
};