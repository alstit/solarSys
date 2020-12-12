#pragma once
#include <glimac/SDLWindowManager.hpp>
#include <glm/glm.hpp>
#include <queue> 
#include <vector>
#include <glm/gtx/vector_angle.hpp>
#include <solarSys/const.hpp>
#include <math.h>



using namespace glm;

struct PreviousPos{
    glm::vec3 Pos;
    glm::vec3 speed;
    float time;
};






class Body
{    public:
        float scale;
        float masse;
        vec3 position;
        vec3 initSpeed;
        std::vector<PreviousPos> previousPos;
        Engine* myEngine;
        MyShader* myShader;

        Body(Engine* engine ,MyShader *engineShader,std::string vertexShader,std::string fragShader, float scale,float masse,vec3 position,vec3 initSpeed)
        {
            this->myEngine = engine;
            this->myShader = engineShader;
            //engine->planetShaders=aShader;
            //engine->trailShaders=aShader;
            engine->loadPlanetShader(engineShader,vertexShader,fragShader);
            engine->openglBindBuffShpere();
            //engine->openglBindBuffTrail();
            this->scale = scale;
            this->masse = masse;
            this->position = position;
            this->initSpeed = initSpeed;

        };

        std::vector<glm::mat4> viewMatrixTrail( SDLWindowManager* windowManager,glm::mat4 MVMatrix)
        {
            std::vector<glm::mat4> matrix;
            glm::mat4 amatrix;
            float theta;
            glm::vec3 ascale = glm::vec3(1,(float)TRAIL_RENDER_FACTOR,1)*(float)std::pow(this->scale/UNITEASTRONOMIQUE,1);
          
            for (int i=0;i<this->previousPos.size();i++)
            {
                 theta =  glm::orientedAngle(glm::vec3(0,1,0),glm::normalize(this->previousPos[i].speed),glm::vec3(0,0,1));//////// update direction vector according to randomness

                //amatrix= translate(MVMatrix,10.f*this->scale/UNITEASTRONOMIQUE*glm::vec3(0,(float)TRAIL_RENDER_FACTOR*sin(theta),0));
                amatrix= translate(MVMatrix,this->previousPos[i].Pos/UNITEASTRONOMIQUE);
                amatrix = rotate(amatrix, theta, glm::vec3(0,0,1));
                //amatrix = glm::scale(amatrix,(float)TRAIL_RENDER_FACTOR/100*glm::vec3(0,1,0));
                amatrix = glm::scale(amatrix,ascale);
                matrix.push_back(amatrix);
            }
            return matrix;
        };

        glm::mat4 viewMatrixBody(SDLWindowManager* windowManager,glm::mat4 MVMatrix)
        {
            this->myEngine->planetShaders = myShader;
            MVMatrix = translate(MVMatrix,this->position/UNITEASTRONOMIQUE);
            MVMatrix = rotate(MVMatrix, windowManager->getTime(), glm::vec3(0,1,0));
            MVMatrix = glm::scale(MVMatrix, this->scale/UNITEASTRONOMIQUE*glm::vec3(1, 1, 1));
            return MVMatrix;
        }
        


        void update(float t,float dt, std::vector<Body> bodies,glm::vec3 aforce);



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


void Body::update(float t,float dt, std::vector<Body> bodies,glm::vec3 aforce)
{
    glm::vec3 Force = aforce;

    for(int i = 0; i<bodies.size();i++)
    {
        Force += gforce(*this,bodies[0]);
    }

    this->initSpeed+=Force/(float)(bodies.size()+1)/this->masse*(float)dt;
    this->position+=this->initSpeed*(float)dt;

    PreviousPos apreviousPos = {this->position,this->initSpeed,t};
    this->previousPos.push_back(apreviousPos);
    if(this->previousPos.size()>TRAILSIZE*10*length(this->position)/UNITEASTRONOMIQUE){this->previousPos.erase(previousPos.begin());}


}