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
            //glm::vec3 ascale = glm::vec3(1,(float)TRAIL_RENDER_FACTOR,1)*(float)std::pow(this->scale/UNITEASTRONOMIQUE,1);
          glm::vec3 ascale ;
            for (int i=0;i<this->previousPos.size();i++)
            {
                 theta =  glm::orientedAngle(glm::vec3(0,1,0),glm::normalize(this->previousPos[i].speed),glm::vec3(0,0,1));//////// update direction vector according to randomness

                //amatrix= translate(MVMatrix,10.f*this->scale/UNITEASTRONOMIQUE*glm::vec3(0,(float)TRAIL_RENDER_FACTOR*sin(theta),0));
                amatrix= translate(MVMatrix,this->previousPos[i].Pos/UNITEASTRONOMIQUE);
                amatrix = rotate(amatrix, theta, glm::vec3(0,0,1));
                //amatrix = glm::scale(amatrix,(float)TRAIL_RENDER_FACTOR/100*glm::vec3(0,1,0));
                if(i<this->previousPos.size()-1){
                    ascale =glm::vec3(1,(float)TRAIL_RENDER_FACTOR*(float)abs(length(this->previousPos[i+1].Pos-this->previousPos[i].Pos))/UNITEASTRONOMIQUE,1);
                }
                else{ascale =glm::vec3(1,(float)TRAIL_RENDER_FACTOR*(float)abs(length(this->position-this->previousPos[i].Pos))/UNITEASTRONOMIQUE,1);}
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
        

        glm::mat4 render(Camera* acamera,SDLWindowManager* windowManager,glm::mat4 ProjMatrix,std::vector<Body> bodies,int bodyCam,std::shared_ptr<TrailShader> atrailShader)
        {
             glm::mat4 MVMatrix;
            
            if(acamera->getType()=="TrackballCamera")
            {
                MVMatrix = this->viewMatrixBody(  windowManager,acamera->getViewMatrix(bodies[bodyCam].position,-1392684e3*100/UNITEASTRONOMIQUE,1));
            }
            else {MVMatrix = this->viewMatrixBody(  windowManager,acamera->getViewMatrix() );}



            //MVMatrix = this->viewMatrixBody(  &windowManager,acamera.getViewMatrix() )

            glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
            this->myEngine->renderPlanet(MVMatrix,ProjMatrix,NormalMatrix);
            this->renderTrail(acamera,windowManager,ProjMatrix,bodies,bodyCam,atrailShader);



            return MVMatrix;////// for class who inherites from this one
        }   

        void renderTrail(Camera* acamera,SDLWindowManager* windowManager,glm::mat4 ProjMatrix,std::vector<Body> bodies,int bodyCam,std::shared_ptr<TrailShader> atrailShader)
        {
            double t = windowManager->getTime();
            std::vector<glm::mat4> matrix;
            if(acamera->getType()=="TrackballCamera")
            {
                matrix = this->viewMatrixTrail(  windowManager,acamera->getViewMatrix(bodies[bodyCam].position,-1392684e3*100/UNITEASTRONOMIQUE,1));
                
            }
            else {matrix = this->viewMatrixTrail( windowManager,acamera->getViewMatrix() );}

            for(int j = 0 ; j<matrix.size();j+=TRAIL_RENDER_FACTOR)
            {
                glm::mat4 NormalMatrix = glm::transpose(glm::inverse(matrix[j]));
                atrailShader->setTime(t - this->previousPos[j].time);
                myEngine->renderTrail(matrix[j],ProjMatrix,NormalMatrix,t - this->previousPos[j].time);
            }
        }



        void updateForce(float t,float dt, std::vector<Body> bodies,glm::vec3 aforce);
        void updatePosition(float t,float dt);



};


glm::vec3 gforce(Body p1,Body p2){
    // Calculate the gravitational force exerted on p1 by p2.
    double G = 6.67e-11 ; 
    // Calculate distance vector between p1 and p2.
    //std::cout<<"vectros  "<<p1.position<<" "<<p2.position<<std::endl;
    double r_mag = glm::length(p1.position-p2.position);
    //std::cout << "r_hat" << r_mag<<std::endl;
    glm::vec3 r_hat = glm::normalize(p1.position-p2.position);
    if(r_mag == 0){r_hat = vec3(0,0,0);}
    //std::cout << "r_hat" << r_hat<<std::endl;
    // Calculate force magnitude.
    double force_mag = G*p1.masse*p2.masse/(r_mag*r_mag) ;///1000 = masse
    //std::cout << "force mag" << force_mag<<std::endl;
    //float force_mag = 10000000;
    //Calculate force vector.
    glm::vec3 force_vec = -(float)force_mag*r_hat;
    
    return force_vec;
};


void Body::updateForce(float t,float dt, std::vector<Body> bodies,glm::vec3 aforce)
{
    

    for(int i = 0; i<bodies.size();i++)
    {
        if(bodies[i].position==this->position){continue;}
        aforce += gforce(*this,bodies[i]);
    }

    this->initSpeed+=aforce/this->masse*(float)dt;

}


void Body::updatePosition(float t,float dt)
{
    this->position+=this->initSpeed*(float)dt;

    PreviousPos apreviousPos = {this->position,this->initSpeed,t};
    this->previousPos.push_back(apreviousPos);
    if(this->previousPos.size()>TRAILSIZE*10*length(this->position)/UNITEASTRONOMIQUE){this->previousPos.erase(previousPos.begin());}
}






class Saturn: public Body
{
    MyShader* ringShaders;
    public : 
        Saturn(Engine* engine ,MyShader *engineShader,MyShader* aringShader,std::string PlanetVertexShader,std::string PlanetFragShader,std::string RingVertexShader,std::string RingFragShader, float scale,float masse,vec3 position,vec3 initSpeed):Body(engine ,engineShader,PlanetVertexShader,PlanetFragShader,scale,masse,position,initSpeed)
        {
            this->myEngine->flareShaders = aringShader;
            this->ringShaders = aringShader;
            this->myEngine->loadFlareShader(ringShaders,RingVertexShader,RingFragShader);
            this->myEngine->openglBindBuffDisk();
            //this->myEngine->openglBindBuffDisk();
        };
        
        
        void renderRing(glm::mat4 MVMatrix,glm::mat4 ProjMatrix, glm::mat4 NormalMatrix,SDLWindowManager* windowManager)
        {
            //MVMatrix = translate(MVMatrix, this->position/UNITEASTRONOMIQUE);
            MVMatrix = rotate(MVMatrix,-windowManager->getTime(), glm::vec3(0,1,0));/// reverse Planete self rotation
            MVMatrix = glm::scale(MVMatrix,30.0f*glm::vec3(1,1,1));
            

            this->myEngine->renderDisk(MVMatrix,ProjMatrix,NormalMatrix);

        };
        void render(Camera* acamera,SDLWindowManager* windowManager,glm::mat4 ProjMatrix,std::vector<Body> bodies,int bodyCam,std::shared_ptr<TrailShader> atrailShader)
        {
            this->myEngine->flareShaders  = this->ringShaders;
            glm::mat4 MVMatrix = this->Body::render(acamera, windowManager, ProjMatrix,bodies, bodyCam,atrailShader);
            MVMatrix = this->viewMatrixBody(  windowManager,acamera->getViewMatrix() );
            this->renderRing(MVMatrix,ProjMatrix,glm::mat4(),windowManager);
        }


};