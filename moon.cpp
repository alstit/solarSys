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

using namespace glimac;
using namespace std;

double scale=100;




// vec.pos = 3d mass

glm::vec3 gforce(glm::vec3 p1,glm::vec3 p2){
    // Calculate the gravitational force exerted on p1 by p2.
    double G = 1 ; // Change to 6.67e-11 to use real-world values.
    // Calculate distance vector between p1 and p2.
    float r_mag = glm::length(p1-p2);
    glm::vec3 r_hat = glm::normalize(p1-p2);
    // Calculate force magnitude.
    //float force_mag = G*10000*10000/(r_mag*r_mag) ;///1000 = masse
    float force_mag = 10000000;
    //Calculate force vector.
    glm::vec3 force_vec = -force_mag*r_hat;
    
    return force_vec;
}

void openglRender(Engine* engine,glm::mat4 MVMatrix,glm::mat4 ProjMatrix, glm::mat4 NormalMatrix,Sphere asphere,GLuint* vao){

        glUniformMatrix4fv( 	engine->uMVMatrix,
                                    1,
                                    GL_FALSE,
                                    glm::value_ptr(MVMatrix));
        glUniformMatrix4fv( 	engine->uMVPMatrix,
                                    1,
                                    GL_FALSE,
                                    glm::value_ptr(ProjMatrix*MVMatrix));
        glUniformMatrix4fv( 	engine->uNormalMatrix,
                                    1,
                                    GL_FALSE,
                                    glm::value_ptr(NormalMatrix));
        /*glUniform3f( engine.uKd,
                    Kd[0],Kd[1],Kd[2]);
        glUniform3f( engine.uKs,
                    Kd[0],Kd[1],Kd[2]);
        glUniform3f( engine.uLightDir_vs,
                    LightDir_vs[0],LightDir_vs[1],LightDir_vs[2]);
        glUniform3f( engine.uLightIntensity,
                    LightIntensity[0],LightIntensity[1],LightIntensity[2]);
        glUniform1f(engine.uShininess,Shininess);   */                                                     

        glBindVertexArray(*vao);
        glDrawArrays(GL_TRIANGLES,0,asphere.getVertexCount());
        glBindVertexArray(0);
};


void openglBindBuff(Sphere asphere,GLuint* vbo,GLuint* vao)
{

    glGenBuffers(1,vbo);
    glBindBuffer(GL_ARRAY_BUFFER,*vbo);
    glBufferData(GL_ARRAY_BUFFER,asphere.getVertexCount()*sizeof(Sphere),asphere.getDataPointer(),GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    glGenVertexArrays(1,vao);
    glBindVertexArray(*vao);
    glBindBuffer(GL_ARRAY_BUFFER,*vbo);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(ShapeVertex),(void*)offsetof(ShapeVertex,position));
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(ShapeVertex),(void*)offsetof(ShapeVertex,normal));
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(ShapeVertex),(void*)offsetof(ShapeVertex,texCoords));
    glBindVertexArray(0);
}


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

    engine.m_Program.use();

    FreeflyCamera freeCamera = FreeflyCamera();
    TrackballCamera ballCamera = TrackballCamera();
    //freeCamera.moveFront(-300000);

    

    Sphere asphere = Sphere(10, 64, 64);
    
    GLuint* vbo = new GLuint();
    GLuint* vao = new GLuint();
    openglBindBuff(asphere,vbo,vao);




    glEnable(GL_DEPTH_TEST);
    glm::mat4 ProjMatrix,MVMatrix,NormalMatrix;
    float fov = 70;
    ProjMatrix = glm::perspective(glm::radians(fov),4.f/3.f,0.1f,(float)1275e4);

    double angle = 0;
    int sign =0;
    double initDistance = 140000;
    double g = -1000;
    double v0 = 0;
glm::vec3 moonPosition = glm::vec3(initDistance,0,0);
        double PreviousT =0;
                glm::vec3 earthQmvt = glm::vec3(0,0,0);
        glm::vec3 moonQmvt = glm::vec3(0,10000000,10010);



    Body earth = Body(10,5.97e24,vec3(0,10000,0),vec3(0,0,0));
    cout<<earth.scale/tan(glm::radians(fov/4.))<<endl;
    //ballCamera.moveFront(earth.scale+earth.scale/tan(glm::radians(fov/4.)));
    ballCamera.moveFront(20);

    // Application loop:
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
            if(event.type == SDL_MOUSEWHEEL)
            {
                if(event.wheel.y > 0) // scroll up
                {
                    freeCamera.moveFront(1000e3);
                }
                else if(event.wheel.y < 0) // scroll down
                {
                    freeCamera.moveFront(-1000e3);

                }
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
                
                //glm::ivec2 mousePos = windowManager.getMousePosition();
                //camera.rotateLeft(mousePos[1]*.0001);
                //camera.rotateUp(mousePos[0]*.001);
            }
        }

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/



                ///////////////////////////////////////////////lighting model 

/*
        LightDir_vs =  vec4(10,10,10,0) ;
        LightIntensity =glm::vec4(5,5,5,0);
        Kd =  glm::vec4(1,1,1,0);
        Ks =  glm::vec4(1,1,1,0);
        Shininess = 2;*/

         ///////////////////////////////////////////draw planet////////////////
        engine.m_Program.use();


        //MVMatrix = glm::translate(glm::mat4(),glm::vec3(0,-10000,-10));
        MVMatrix = freeCamera.getViewMatrix() ;
        MVMatrix = translate(MVMatrix,earth.position);
        MVMatrix = rotate(MVMatrix, windowManager.getTime(), glm::vec3(0,1,0));
        MVMatrix = glm::scale(MVMatrix, (float)earth.scale*glm::vec3(1, 1, 1));
        NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        openglRender(&engine,MVMatrix,ProjMatrix,NormalMatrix,asphere,vao);










        ////// draw moon//////

       // Kd =  glm::vec4(1,1,1,0);
        //Ks =  glm::vec4(1,1,1,0);
        //Shininess = 0.5;
        //for(int i = 0 ; i< rotAxis.size();i++)
        //{




        double t = windowManager.getTime();

    double dt = t- PreviousT;
/*
        glm::vec3 earthForce = gforce(glm::vec3(0,0,0),moonPosition);
        glm::vec3 moonForce = gforce(moonPosition,glm::vec3(0,0,0));

        earthQmvt += earthForce*(float)dt;
         moonQmvt += moonForce*(float)dt;

        moonPosition += moonQmvt/(float)(1000)*(float)dt;

        PreviousT = t;

            MVMatrix = camera.getViewMatrix();
            //MVMatrix = glm::rotate(MVMatrix, acc*windowManager.getTime()*windowManager.getTime(), rotAxis[0]); //T*Rlarge
            MVMatrix = glm::translate(MVMatrix,moonPosition); // T*Rlarge*Tpos
            MVMatrix = glm::rotate(MVMatrix, windowManager.getTime(), -rotAxis[0]); // T*Rlarge*Tpos*RotationPropre
            MVMatrix = glm::scale(MVMatrix, (float)(3475./12756.)*glm::vec3(1, 1, 1));
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));



            cout<<moonPosition<<endl;

    cout<<"computed angle "<<angle<<endl;*/



        openglRender(&engine,MVMatrix,ProjMatrix,NormalMatrix,asphere,vao);

        //}

        windowManager.swapBuffers();
    }

    glDeleteBuffers(1,vbo);
    glDeleteVertexArrays(1,vao);
    return EXIT_SUCCESS;
}
