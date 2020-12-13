

#include <glimac/SDLWindowManager.hpp>
#include <glimac/Image.hpp> 
#include <GL/glew.h>
#include <iostream>
#include <glimac/FilePath.hpp>
#include <glimac/Program.hpp>
#include <glimac/common.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/Camera.hpp>
#include <math.h>
#include <solarSys/Engine.hpp>
#include <solarSys/Body.hpp>
#include <glm/glm.hpp>
#include <limits>
#include <SDL/SDL.h>
#include <stdlib.h> 
#include <solarSys/const.hpp>
#include <solarSys/MyShader.hpp>
#include <typeinfo>
#include <memory>
#include <string>
#include <solarSys/Flare.hpp>
#include <solarSys/Disk.hpp>
#include <solarSys/Bloom.hpp>


using namespace glimac;
using namespace std;




template<typename Base, typename T>
inline bool instanceof(const T*) {
   return is_base_of<Base, T>::value;
};

    


int main(int argv,char** argc) {
    // Initialize SDL and open a window
    SDLWindowManager windowManager(800, 600, "GLImac");

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }


    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;


    FilePath applicationPath(argc[0]);

    Engine engine(applicationPath);

    /////////////////////
    //Load Shaders Here//
    /////////////////////

    std::shared_ptr<PlanetShader> aplanetShader = std::shared_ptr<PlanetShader>(new PlanetShader);
    std::shared_ptr<SunShader> asunShader = std::shared_ptr<SunShader>(new SunShader);
    std::shared_ptr<TrailShader> atrailShader = std::shared_ptr<TrailShader>(new TrailShader);
    std::shared_ptr<GenericShader> aflareShader = std::shared_ptr<GenericShader>(new GenericShader);
    std::shared_ptr<GenericShader> abloomShader = std::shared_ptr<GenericShader>(new GenericShader);

    MyShader flareShader(aflareShader);
    MyShader sunShader(asunShader);
    MyShader planetShader(aplanetShader);
    MyShader trailShader(atrailShader);
    MyShader bloomShader(abloomShader);

    engine.loadTrailShader(&trailShader,"trail","trail");

    /////for lens flare effect
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND); 
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    ///light effects (flare and bloom)
    Flare aflare(3,&engine,&flareShader,"flare","flare");
    engine.openglBindBuffDisk();
    Bloom abloom(&engine,&bloomShader,"generic","generic");

    Camera camera(std::unique_ptr<FreeflyCamera>(new FreeflyCamera()));
    //camera = Camera(std::unique_ptr<TrackballCamera>(new TrackballCamera()));


    glEnable(GL_DEPTH_TEST);
    glm::mat4 ProjMatrix,MVMatrix,NormalMatrix;
    
    float fov = 70;
    ProjMatrix = glm::perspective(glm::radians(fov),4.f/3.f,(float)0.00000930951,(float)100);

    srand (time(NULL));

    std::vector<Body>  bodies;


    bodies.push_back( Body(&engine,&sunShader,"3D","directionallight",1392684e3/2.,1.9891e30,vec3(0,0,0),vec3(0,0,0)));
    float theta = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.14));
    bodies.push_back(  Body(&engine,&planetShader,"3D","planetNormal",4879.e3*GRAPHIC_SCALE/2.,0.330e24,(float)57.9e9*vec3(cos(theta),sin(theta),0),(float)49.e3*vec3(cos(theta+M_PI/2),sin(theta+M_PI/2),0)));
    theta =theta = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.14));
    bodies.push_back( Body(&engine,&planetShader,"3D","planetNormal",12104.e3*GRAPHIC_SCALE/2.,4.87e24,(float)108.2e9*vec3(cos(theta),sin(theta),0),(float)35.e3*vec3(cos(theta+M_PI/2),sin(theta+M_PI/2),0)));
    theta = theta = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.14));
    bodies.push_back( Body(&engine,&planetShader,"3D","planetNormal",12756.e3*GRAPHIC_SCALE/2.,5.97e24,(float)149.6e9*vec3(cos(theta),sin(theta),0),(float)29.8e3*vec3(cos(theta+M_PI/2),sin(theta+M_PI/2),0)));
    theta = theta = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.14));
    bodies.push_back( Body(&engine,&planetShader,"3D","planetNormal",6792.e3*GRAPHIC_SCALE/2.,0.642e24,(float)227.9e9*vec3(cos(theta),sin(theta),0),(float)24.1e3*vec3(cos(theta+M_PI/2),sin(theta+M_PI/2),0)));
    theta = theta = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.14));
    bodies.push_back( Body(&engine,&planetShader,"3D","planetNormal",142984e3*GRAPHIC_SCALE/2.,18982e24,(float)778.6e9*vec3(cos(theta),sin(theta),0),(float)13.1e3*vec3(cos(theta+M_PI/2),sin(theta+M_PI/2),0)));
    
    camera.moveFront(-1392684e3*100/UNITEASTRONOMIQUE);


    glm::vec3 test = glm::vec3(1,1,1);
    asunShader->setUniforms(test,test,test,test,1.f);
    aplanetShader->setUniforms(test,test,glm::vec4(1,0,0,1),glm::vec3(1,1,1),1.f);


    // Application loop:
    glm::ivec2 prevMousePos = windowManager.getMousePosition();
    double PreviousT =0;
    std::vector<glm::mat4> matrix ;
    bool done = false;
    int bodyCam = 0;





    while(!done) 
    {
        // Event loop:
        SDL_Event e;
        while(windowManager.pollEvent(e)) 
        {

            if(e.type == SDL_QUIT) 
            {
                done = true; // Leave the loop after this iteration
            }
            

            if(windowManager.isKeyPressed(SDLK_z))
            {   
                
                //camera.moveFront(10000);
            }
            if(windowManager.isKeyPressed(SDLK_z))
            {   
                
                //camera.moveFront(10000);
            }
            if(windowManager.isKeyPressed(SDLK_s))
            {   
               // camera.moveFront(-1000);

            }
            if(windowManager.isKeyPressed(SDLK_c))
            {   

                if(camera.getType()=="FreeflyCamera")
                {
                    camera = Camera(std::unique_ptr<TrackballCamera>(new TrackballCamera()));
                    camera.moveFront(-1392684e3*100/UNITEASTRONOMIQUE);

                }
                else if(camera.getType()=="TrackballCamera")
                {   
                    bodyCam+=1%bodies.size();
                    camera = Camera(std::unique_ptr<FreeflyCamera>(new FreeflyCamera()));
                    camera.moveFront(-1392684e3*100/UNITEASTRONOMIQUE);
                }

            }
            if(windowManager.isKeyPressed(SDLK_q))
            {   
                camera.moveLeft(0.001f);
            }
            if(windowManager.isKeyPressed(SDLK_d))
            {   
                //camera.moveLeft(-.1);
            }
            if(windowManager.isMouseButtonPressed(SDL_BUTTON_RIGHT))
            {
                
                glm::ivec2 mousePos = windowManager.getMousePosition();
                camera.rotateLeft(-(-mousePos[0]+prevMousePos[0])*.0001);
                camera.rotateUp(-(-mousePos[1]+prevMousePos[1])*.0001);
            }
            if(windowManager.isKeyPressed(SDLK_LCTRL))
            {
                glm::ivec2 mousePos = windowManager.getMousePosition();
                camera.moveFront((-mousePos[1]+prevMousePos[1])*0.000930951);
                
            }
            

        }
        prevMousePos = windowManager.getMousePosition();

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        double t = windowManager.getTime();

        double dt = (t- PreviousT)*SIMSPEED;


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        for (int i = 0 ; i< bodies.size();i++)
        {
            if(camera.getType()=="TrackballCamera")
            {
                MVMatrix =bodies[i].viewMatrixBody(  &windowManager,camera.getViewMatrix(bodies[bodyCam].position,-1392684e3*100/UNITEASTRONOMIQUE,fov));
            }
            else{MVMatrix = bodies[i].viewMatrixBody(  &windowManager,camera.getViewMatrix() );
            }
            aplanetShader->setUniforms(test,test,glm::vec4(-bodies[i].position[0]/UNITEASTRONOMIQUE,-bodies[i].position[1]/UNITEASTRONOMIQUE,-bodies[i].position[2]/UNITEASTRONOMIQUE,1),glm::vec3(1,1,1),1.f);
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
            bodies[i].render(MVMatrix,ProjMatrix,NormalMatrix);
    
            if(camera.getType()=="TrackballCamera")
            {
                matrix = bodies[i].viewMatrixTrail(  &windowManager,camera.getViewMatrix(bodies[bodyCam].position,-1392684e3*100/UNITEASTRONOMIQUE,fov));
            }
            else {matrix = bodies[i].viewMatrixTrail(  &windowManager,camera.getViewMatrix() );}

            for(int j = 0 ; j<matrix.size();j+=TRAIL_RENDER_FACTOR)
            {
                NormalMatrix = glm::transpose(glm::inverse(matrix[j]));
                atrailShader->setTime(t - bodies[i].previousPos[j].time);
                engine.renderTrail(matrix[j],ProjMatrix,NormalMatrix,t - bodies[i].previousPos[j].time);
            }
        }


        
        /////bloom effect
        if(camera.getType()=="TrackballCamera"){MVMatrix = abloom.viewMatrix(camera.getViewMatrix(bodies[bodyCam].position,-1392684e3*100/UNITEASTRONOMIQUE,fov),&camera,bodies[0]);}
        else{MVMatrix = abloom.viewMatrix(camera.getViewMatrix(),&camera,bodies[0]);}
        abloom.render(&engine, &bloomShader,MVMatrix ,ProjMatrix, glm::mat4());
        
        ////flare effect
        glm::vec4 tempsSunCal = glm::vec4(bodies[0].position[0],bodies[0].position[1],bodies[0].position[2],1)/UNITEASTRONOMIQUE;
        glm::vec4 sunPositionToCamera = ProjMatrix*bodies[0].viewMatrixBody(  &windowManager,camera.getViewMatrix() ) * tempsSunCal;
        std::vector<glm::mat4> lensMatrix=aflare.viewMatrix(sunPositionToCamera*UNITEASTRONOMIQUE);

        for(int i = 0;i<lensMatrix.size();i++)
        {
            aflare.render(&engine,&flareShader,lensMatrix[i],ProjMatrix);
        } 








        for (int i = 1;i< bodies.size();i++)
        {
            bodies[i].update(t,dt,bodies,glm::vec3(0,0,0));
        }
        PreviousT= t;







        windowManager.swapBuffers() ;
    }


    return EXIT_SUCCESS;
}
