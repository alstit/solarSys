#include <glimac/SDLWindowManager.hpp>
#include <glm/glm.hpp>
#include <queue> 

float UNITEASTRONOMIQUE = 149.6e9;


using namespace glm;


class Body
{    public:
        float scale;
        float masse;
        vec3 position;
        vec3 initSpeed;
        vec3 positionScreen ;
        std::queue<glm::vec3> previousPos;

        Body( float scale,float masse,vec3 position,vec3 initSpeed)
        {
            this->scale = scale;
            this->masse = masse;
            this->position = position;
            this->initSpeed = initSpeed;

        }

        glm::mat4 viewMatrix( SDLWindowManager* windowManager,glm::mat4 MVMatrix)
        {
            MVMatrix = translate(MVMatrix,this->position/UNITEASTRONOMIQUE);
            MVMatrix = rotate(MVMatrix, windowManager->getTime(), glm::vec3(0,1,0));
            MVMatrix = glm::scale(MVMatrix, this->scale/UNITEASTRONOMIQUE*glm::vec3(1, 1, 1));

            return MVMatrix;
        }

        void update(float dt, std::vector<Body> bodyVect,glm::vec3 aforce);



};

glm::vec3 gforce(Body p1,Body p2){
    // Calculate the gravitational force exerted on p1 by p2.
    double G = 6.67e-11 ; 
    // Calculate distance vector between p1 and p2.
    float r_mag = glm::length(p1.position-p2.position);
    glm::vec3 r_hat = glm::normalize(p1.position-p2.position);
    // Calculate force magnitude.
    float force_mag = G*p1.masse*p2.masse/(r_mag*r_mag) ;///1000 = masse
    //float force_mag = 10000000;
    //Calculate force vector.
    glm::vec3 force_vec = -force_mag*r_hat;
    
    return force_vec;
};


void Body::update(float dt, std::vector<Body> bodies,glm::vec3 aforce)
{
    glm::vec3 Force = aforce;

    for(int i = 0; i<bodies.size();i++)
    {
        Force += gforce(*this,bodies[0]);
    }

    this->initSpeed+=Force/(float)(bodies.size()+1)/this->masse*(float)dt;
    this->position+=this->initSpeed*(float)dt;

    this->previousPos.push(this->position);
    if(this->previousPos.size()>10){this->previousPos.pop();}


}