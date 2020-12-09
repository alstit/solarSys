#include <glimac/SDLWindowManager.hpp>
#include <glimac/Image.hpp> 
#include <GL/glew.h>
#include <iostream>
#include <glimac/FilePath.hpp>
#include <glimac/Program.hpp>
#include <glimac/common.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/TrackballCamera.hpp>
#include <glimac/FreeflyCamera.hpp>
#include <math.h>
#include <solarSys/Engine.hpp>
#include <solarSys/Body.hpp>
#include <glm/glm.hpp>
#include <limits>
#include <SDL/SDL.h>
#include <stdlib.h> 

using namespace glimac;
using namespace std;




int main(int argv,char** argc) {
    // Initialize SDL and open a window
    SDLWindowManager windowManager(1280, 1024, "GLImac");

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

    

    FreeflyCamera freeCamera = FreeflyCamera();
    TrackballCamera ballCamera = TrackballCamera();


    


    






    glEnable(GL_DEPTH_TEST);
    glm::mat4 ProjMatrix,MVMatrix,NormalMatrix;
    float fov = 70;
    ProjMatrix = glm::perspective(glm::radians(fov),4.f/3.f,(float)0.00000930951,(float)20);





    float graphicScale = 30;
    srand (time(NULL));
    
    std::vector<Body>  bodies;

    bodies.push_back( Body(1392684e3/2.,1.9891e30,vec3(0,0,0),vec3(0,0,0)));
    float theta = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.14));
    bodies.push_back( Body(4879.e3*graphicScale/2.,0.330e24,(float)57.9e9*vec3(cos(theta),sin(theta),0),(float)49.e3*vec3(cos(theta+M_PI/2),sin(theta+M_PI/2),0)));
    theta =theta = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.14));
    bodies.push_back( Body(12104.e3*graphicScale/2.,4.87e24,(float)108.2e9*vec3(cos(theta),sin(theta),0),(float)35.e3*vec3(cos(theta+M_PI/2),sin(theta+M_PI/2),0)));
    theta = theta = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.14));
    bodies.push_back( Body(12756.e3*graphicScale/2.,5.97e24,(float)149.6e9*vec3(cos(theta),sin(theta),0),(float)29.8e3*vec3(cos(theta+M_PI/2),sin(theta+M_PI/2),0)));
    freeCamera.moveFront(-1392684e3*100/UNITEASTRONOMIQUE);




    // Application loop:
    glm::ivec2 prevMousePos = windowManager.getMousePosition();
    double PreviousT =0;

    bool done = false;
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
            if(windowManager.isKeyPressed(SDLK_s))
            {   
               // camera.moveFront(-1000);

            }
            if(windowManager.isKeyPressed(SDLK_q))
            {   
                //camera.moveLeft(10);
            }
            if(windowManager.isKeyPressed(SDLK_d))
            {   
                //camera.moveLeft(-.1);
            }
            if(windowManager.isMouseButtonPressed(SDL_BUTTON_RIGHT))
            {
                
                glm::ivec2 mousePos = windowManager.getMousePosition();
                //camera.rotateLeft(mousePos[1]*.0001);
                //camera.rotateUp(mousePos[0]*.001);
            }
            if(windowManager.isKeyPressed(SDLK_LCTRL))
            {
                glm::ivec2 mousePos = windowManager.getMousePosition();
                freeCamera.moveFront((-mousePos[1]+prevMousePos[1])*0.0000930951);
                
            }

        }
        prevMousePos = windowManager.getMousePosition();

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/




        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (int i = 0 ; i< bodies.size();i++)
        {
            MVMatrix = bodies[i].viewMatrix(  &windowManager,freeCamera.getViewMatrix() );
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
            engine.renderPlanet(MVMatrix,ProjMatrix,NormalMatrix);
        }


        double t = windowManager.getTime();

        double dt = (t- PreviousT)*100000;


        for (int i = 1;i< bodies.size();i++)
        {
            bodies[i].update(dt,bodies,glm::vec3(0,0,0));
        }






        PreviousT= t;







        windowManager.swapBuffers() ;
    }


    return EXIT_SUCCESS;
}
