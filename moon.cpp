

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
#include <typeinfo>
#include <memory>



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

    

    Camera camera(std::unique_ptr<FreeflyCamera>(new FreeflyCamera()));
    //camera = Camera(std::unique_ptr<TrackballCamera>(new TrackballCamera()));

    glEnable(GL_DEPTH_TEST);
    glm::mat4 ProjMatrix,MVMatrix,NormalMatrix;
    
    float fov = 70;
    ProjMatrix = glm::perspective(glm::radians(fov),4.f/3.f,(float)0.00000930951,(float)20);

    srand (time(NULL));
    
    std::vector<Body>  bodies;

    bodies.push_back( Body(1392684e3/2.,1.9891e30,vec3(0,0,0),vec3(0,0,0)));
    float theta = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.14));
    bodies.push_back( Body(4879.e3*GRAPHIC_SCALE/2.,0.330e24,(float)57.9e9*vec3(cos(theta),sin(theta),0),(float)49.e3*vec3(cos(theta+M_PI/2),sin(theta+M_PI/2),0)));
    theta =theta = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.14));
    bodies.push_back( Body(12104.e3*GRAPHIC_SCALE/2.,4.87e24,(float)108.2e9*vec3(cos(theta),sin(theta),0),(float)35.e3*vec3(cos(theta+M_PI/2),sin(theta+M_PI/2),0)));
    theta = theta = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.14));
    bodies.push_back( Body(12756.e3*GRAPHIC_SCALE/2.,5.97e24,(float)149.6e9*vec3(cos(theta),sin(theta),0),(float)29.8e3*vec3(cos(theta+M_PI/2),sin(theta+M_PI/2),0)));
    theta = theta = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.14));
    bodies.push_back( Body(6792.e3*GRAPHIC_SCALE/2.,0.642e24,(float)227.9e9*vec3(cos(theta),sin(theta),0),(float)24.1e3*vec3(cos(theta+M_PI/2),sin(theta+M_PI/2),0)));
    theta = theta = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.14));
    bodies.push_back( Body(142984e3*GRAPHIC_SCALE/2.,18982e24,(float)778.6e9*vec3(cos(theta),sin(theta),0),(float)13.1e3*vec3(cos(theta+M_PI/2),sin(theta+M_PI/2),0)));
    camera.moveFront(-1392684e3*100/UNITEASTRONOMIQUE);




    // Application loop:
    glm::ivec2 prevMousePos = windowManager.getMousePosition();
    double PreviousT =0;
    std::vector<glm::mat4> matrix ;
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
                    camera = Camera(std::unique_ptr<FreeflyCamera>(new FreeflyCamera()));
                    camera.moveFront(-1392684e3*100/UNITEASTRONOMIQUE);
                }

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
                MVMatrix =bodies[i].viewMatrixBody(  &windowManager,camera.getViewMatrix(bodies[5].position,-1392684e3*100/UNITEASTRONOMIQUE,fov));
                //MVMatrix =bodies[i].viewMatrixBody(  &windowManager,camera.getViewMatrix());
              //  std::cout<<"hello"<<std::endl;
            }
            else{MVMatrix = bodies[i].viewMatrixBody(  &windowManager,camera.getViewMatrix() );
            //std::cout<<"hello2"<<std::endl;
            }
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
            engine.renderPlanet(MVMatrix,ProjMatrix,NormalMatrix);
            
            if(camera.getType()=="TrackballCamera")
            {
                matrix = bodies[i].viewMatrixTrail(  &windowManager,camera.getViewMatrix(bodies[5].position,-1392684e3*100/UNITEASTRONOMIQUE,fov));
                //matrix = bodies[i].viewMatrixTrail(  &windowManager,camera.getViewMatrix());
            }
            else {matrix = bodies[i].viewMatrixTrail(  &windowManager,camera.getViewMatrix() );}
            for(int j = 0 ; j<matrix.size();j+=TRAIL_RENDER_FACTOR)
            {
                NormalMatrix = glm::transpose(glm::inverse(matrix[j]));
                engine.renderTrail(matrix[j],ProjMatrix,NormalMatrix,t - bodies[i].previousPos[j].time);
            }
        }

 



        for (int i = 1;i< bodies.size();i++)
        {
            bodies[i].update(t,dt,bodies,glm::vec3(0,0,0));
        }



        //cout<<bodies[0].position<<endl;


        PreviousT= t;







        windowManager.swapBuffers() ;
    }


    return EXIT_SUCCESS;
}
