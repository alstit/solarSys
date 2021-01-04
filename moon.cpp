

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
#include <solarSys/Background.hpp>


using namespace glimac;
using namespace std;




template<typename Base, typename T>
inline bool instanceof(const T*) {
   return is_base_of<Base, T>::value;
};

    


int main(int argv,char** argc) {
    // Initialize SDL and open a window
    SDLWindowManager windowManager(1024, 1024, "GLImac");

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
    std::shared_ptr<SaturnRingShader> aSaturnRingShader = std::shared_ptr<SaturnRingShader>(new SaturnRingShader);
    std::shared_ptr<BackgroundShader> aBackgroundShader = std::shared_ptr<BackgroundShader>(new BackgroundShader);

    MyShader flareShader(aflareShader);
    MyShader sunShader(asunShader);
    MyShader planetShader(aplanetShader);
    MyShader trailShader(atrailShader);
    MyShader bloomShader(abloomShader);
    MyShader saturnRingShader(aSaturnRingShader);
    MyShader backgroundShader(aBackgroundShader);

    engine.loadTrailShader(&trailShader,"trail","trail");
    //engine.loadQuadShader(&backgroundShader,"3D","generic");

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
    //ProjMatrix = glm::perspective(glm::radians(fov),4.f/3.f,(float)0.00000930951,(float)100);
    ProjMatrix = glm::perspective(glm::radians(fov),1.f,(float)0.00000930951,(float)1000);
    srand (time(NULL));

    ///background 
    std::shared_ptr<Image> textureBackground = loadImage("/home/als/TP_openGL/GLImac-Template/assets/texture/back.png");
    Background abackground(&engine,&backgroundShader,"3D","back",textureBackground);





    std::shared_ptr<Image> textureMercury = loadImage("/home/als/TP_openGL/GLImac-Template/assets/texture/mercurymap.jpg");
    std::shared_ptr<Image> textureVenus = loadImage("/home/als/TP_openGL/GLImac-Template/assets/texture/venusmap.jpg");
    std::shared_ptr<Image> textureEarth = loadImage("/home/als/TP_openGL/GLImac-Template/assets/texture/EarthMap.jpg");
    std::shared_ptr<Image> textureMars = loadImage("/home/als/TP_openGL/GLImac-Template/assets/texture/marsmap.jpg");
    std::shared_ptr<Image> textureJupiter = loadImage("/home/als/TP_openGL/GLImac-Template/assets/texture/jupitermap.jpg");
    std::shared_ptr<Image> textureSaturn = loadImage("/home/als/TP_openGL/GLImac-Template/assets/texture/saturnmap.jpg");    
    std::shared_ptr<Image> textureUranus = loadImage("/home/als/TP_openGL/GLImac-Template/assets/texture/uranusmap.jpg");    
    std::shared_ptr<Image> textureNeptune = loadImage("/home/als/TP_openGL/GLImac-Template/assets/texture/neptunemap.jpg");    
    std::shared_ptr<Image> texturePluto = loadImage("/home/als/TP_openGL/GLImac-Template/assets/texture/plutomap.jpg");   
    
    std::shared_ptr<Image> textureMoon = loadImage("/home/als/TP_openGL/GLImac-Template/assets/texture/MoonMap.jpg");   

   //////////make planets
    std::vector<Body>  bodies;

    bodies.push_back( Body(&engine,&sunShader,"3D","sun",1392684e3/2.,1.9891e30,vec3(0,0,0),vec3(0,0,0),textureEarth));
    float theta = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.14));
    bodies.push_back(  Body(&engine,&planetShader,"3D","planetNormal",4879.e3*GRAPHIC_SCALE/2.,0.330e24,(float)57.9e9*vec3(cos(theta),sin(theta),0),(float)35.e3*vec3(cos(theta+M_PI/2),sin(theta+M_PI/2),0),textureMercury));
    theta =theta = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.14));
    bodies.push_back( Body(&engine,&planetShader,"3D","planetNormal",12104.e3*GRAPHIC_SCALE/2.,4.87e24,(float)108.2e9*vec3(cos(theta),sin(theta),0),(float)35.e3*vec3(cos(theta+M_PI/2),sin(theta+M_PI/2),0),textureVenus));
    theta = theta = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.14));
    bodies.push_back( Body(&engine,&planetShader,"3D","planetNormal",12756.e3*GRAPHIC_SCALE/2.,5.97e24,(float)149.6e9*vec3(cos(theta),sin(theta),0),(float)29.8e3*vec3(cos(theta+M_PI/2),sin(theta+M_PI/2),0),textureEarth));
     //bodies.push_back( Body(&engine,&planetShader,"3D","planetNormal",12756.e3*GRAPHIC_SCALE/2/3.,0.073e24,(float)149.6003e9*vec3(cos(theta),sin(theta),0),(float)29.8e3*vec3(cos(theta+M_PI/2),sin(theta+M_PI/2),0),textureEarth));   
    theta = theta = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.14));
    bodies.push_back( Body(&engine,&planetShader,"3D","planetNormal",6792.e3*GRAPHIC_SCALE/2.,0.642e24,(float)227.9e9*vec3(cos(theta),sin(theta),0),(float)24.1e3*vec3(cos(theta+M_PI/2),sin(theta+M_PI/2),0),textureMars));
    theta = theta = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.14));
    bodies.push_back( Body(&engine,&planetShader,"3D","planetNormal",142984e3*GRAPHIC_SCALE/2.,18982e24,(float)778.6e9*vec3(cos(theta),sin(theta),0),(float)13.1e3*vec3(cos(theta+M_PI/2),sin(theta+M_PI/2),0),textureJupiter));
    theta = theta = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.14));
    Saturn aSaturn = ( Saturn(&engine,&planetShader,&saturnRingShader,"3D","planetNormal","saturnRing","saturnRing",120536e3*GRAPHIC_SCALE/2.,568e24,(float)1433.5e9*vec3(cos(theta),sin(theta),0),(float)9.7e3*vec3(cos(theta+M_PI/2),sin(theta+M_PI/2),0),textureSaturn));
    theta = theta = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.14));
    bodies.push_back( Body(&engine,&planetShader,"3D","planetNormal",142984e3*GRAPHIC_SCALE/2.,18982e24,vec3((float)778.6e9*cos(theta),(float)778.6e9*sin(theta),UNITEASTRONOMIQUE),(float)13.1e3*vec3(cos(theta+M_PI/2),sin(theta+M_PI/2),0),textureJupiter));

     
     
    ///satelites 
    std::vector<Moon> moonVectors;
    
    //earth moon
    moonVectors.push_back( Moon(1,&bodies[3],&engine,&planetShader,"3D","planetNormal",3000e3*GRAPHIC_SCALE/2.,568e24,(float)300.9e7*vec3(1,0,0),glm::vec3(0,0,1),textureMoon));
    //saturns' moon
     theta = theta = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.14));
    moonVectors.push_back( Moon(1,&aSaturn,&engine,&planetShader,"3D","planetNormal",40000e3*GRAPHIC_SCALE/2.,568e24,(float)3000e7*vec3(cos(theta),sin(theta),0),glm::vec3(0,0,1),textureMoon));  
     theta = theta = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.14));
    moonVectors.push_back( Moon(0.5,&aSaturn,&engine,&planetShader,"3D","planetNormal",50000e3*GRAPHIC_SCALE/2.,568e24,(float)5000e7*vec3(cos(theta),sin(theta),0),glm::vec3(0,0,1),textureMoon));      
     theta = theta = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.14));
    moonVectors.push_back( Moon(0.2,&aSaturn,&engine,&planetShader,"3D","planetNormal",90000e3*GRAPHIC_SCALE/2.,568e24,(float)9000e7*vec3(cos(theta),sin(theta),0),glm::vec3(0,0,1),textureMoon)); 

    /////random asteroids
    float minDistance = 280;
    float maxDistance = 300;
    float massmin = 0.330;
    float massmax = 3.30;

    for(int i=0 ;i<10;i++){
    //glm::vec3 rotAxis= glm::sphericalRand((float)1.0f);
    //glm::vec3 position = glm::sphericalRand(1.0f);
    float r = (minDistance + static_cast <float> (rand()) / (static_cast <float> ((float)RAND_MAX/(maxDistance-minDistance))))*1e9;//metres
    glm::vec3 rotAxis = glm::vec3(0,0,1);
    theta = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.14));
    glm::vec3 position = glm::vec3(cos(theta),sin(theta),0);
    moonVectors.push_back( Moon(0.05f,&bodies[0],&engine,&planetShader,"3D","planetNormal",30000e3*GRAPHIC_SCALE/2.,568e24,r*position,rotAxis,textureMoon));
    }


    for(int i = 0;i<2;i++)
    {
        float r = (minDistance + static_cast <float> (rand()) / (static_cast <float> ((float)RAND_MAX/(maxDistance-minDistance))))*1e9;//metres
        float theta = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.14));
        float mass = (massmin + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(massmax-massmin))))*1e23;//KG
        float speed = glm::sqrt(bodies[0].masse/r)/1e5;//

        //cout<<"speed position"<<speed<<" "<<r<<"randum "<<(maxDistance-minDistance)<<endl;
        float ecc = 0.5+static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/0.5));;
        //float ecc = ((double) rand() / (RAND_MAX)) + 1;
        bodies.push_back( Body( &engine,&planetShader,"3D","planetNormal",4879.e3*GRAPHIC_SCALE/2.,mass,r*vec3(cos(theta),sin(theta),0),ecc*speed*vec3(cos(theta+M_PI/2),sin(theta+M_PI/2),0),textureMercury));
    }

    float r = (minDistance + static_cast <float> (rand()) / (static_cast <float> ((float)RAND_MAX/(maxDistance-minDistance))))*1e9;//metres
     theta = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.14));
    float mass = (massmin + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(massmax-massmin))))*1e23;//KG
    float speed = glm::sqrt(bodies[0].masse/r)/1e5;//

    //cout<<"speed position"<<speed<<" "<<r<<"randum "<<(maxDistance-minDistance)<<endl;
    float ecc = 0.5;
    //float ecc = ((double) rand() / (RAND_MAX)) + 1;
    bodies.push_back( Body( &engine,&planetShader,"3D","planetNormal",4879.e3*GRAPHIC_SCALE/2.,mass,r*vec3(cos(theta),sin(theta),0),ecc*speed*vec3(cos(theta+M_PI/2),sin(theta+M_PI/2),0),textureMercury));



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
                
                camera.moveFront(1);
            }
            if(windowManager.isKeyPressed(SDLK_s))
            {   
                camera.moveFront(-1);

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
                camera.moveLeft(1.0f);
            }
            if(windowManager.isKeyPressed(SDLK_d))
            {   
                camera.moveLeft(-1.0f);
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

        ///////background
        if(camera.getType()=="TrackballCamera"){MVMatrix = abackground.viewMatrix(camera.getViewMatrix(bodies[bodyCam].position,-1392684e3*100/UNITEASTRONOMIQUE,fov),&camera,bodies[0]);}
        else{MVMatrix = abackground.viewMatrix(camera.getViewMatrix(),&camera,bodies[0]);}
        abackground.render(&engine, &backgroundShader,MVMatrix ,ProjMatrix, glm::mat4(),aBackgroundShader);

        for (int i = 0 ; i< bodies.size();i++)
        {
            aplanetShader->setUniforms(test,test,glm::vec4(-bodies[i].position[0]/UNITEASTRONOMIQUE,-bodies[i].position[1]/UNITEASTRONOMIQUE,-bodies[i].position[2]/UNITEASTRONOMIQUE,1),(float)i*i*glm::vec3(1,1,1),1.f);
            bodies[i].render(  &camera,&windowManager,ProjMatrix,bodies,bodyCam,atrailShader,aplanetShader);
           
        }
        aplanetShader->setUniforms(test,test,glm::vec4(-aSaturn.position[0]/UNITEASTRONOMIQUE,-aSaturn.position[1]/UNITEASTRONOMIQUE,-aSaturn.position[2]/UNITEASTRONOMIQUE,1),6.0f*6.0f*glm::vec3(1,1,1),1.f);
        aSaturnRingShader->setUniforms(test,test,glm::vec4(-aSaturn.position[0]/UNITEASTRONOMIQUE,-aSaturn.position[1]/UNITEASTRONOMIQUE,-aSaturn.position[2]/UNITEASTRONOMIQUE,1),6.0f*6.0f*glm::vec3(1,1,1),1.f);
        aSaturn.render( &camera,&windowManager,ProjMatrix,bodies,bodyCam,atrailShader,aplanetShader);
        
        for(int moons= 0 ; moons<moonVectors.size();moons++){
        aplanetShader->setUniforms(test,test,glm::vec4(-aSaturn.position[0]/UNITEASTRONOMIQUE,-aSaturn.position[1]/UNITEASTRONOMIQUE,-aSaturn.position[2]/UNITEASTRONOMIQUE,1),6.0f*6.0f*glm::vec3(1,1,1),1.f);
        moonVectors[moons].render( &camera,&windowManager,ProjMatrix,bodies,bodyCam,atrailShader,aplanetShader);
        }

        

        
        /////bloom effect
        if(camera.getType()=="TrackballCamera"){MVMatrix = abloom.viewMatrix(camera.getViewMatrix(bodies[bodyCam].position,-1392684e3*100/UNITEASTRONOMIQUE,fov),&camera,bodies[0]);}
        else{MVMatrix = abloom.viewMatrix(camera.getViewMatrix(),&camera,bodies[0]);}
        abloom.render(&engine, &bloomShader,MVMatrix ,ProjMatrix, glm::mat4());


        ////flare effect
        glm::vec4 sunPositionToCamera;
        glm::vec4 tempsSunCal = glm::vec4(bodies[0].position[0],bodies[0].position[1],bodies[0].position[2],1)/UNITEASTRONOMIQUE;
        if(camera.getType()=="TrackballCamera"){sunPositionToCamera = ProjMatrix*bodies[0].viewMatrixBody(&windowManager,camera.getViewMatrix(bodies[bodyCam].position,-1392684e3*100/UNITEASTRONOMIQUE,fov)) * tempsSunCal;}
        else{sunPositionToCamera = ProjMatrix*bodies[0].viewMatrixBody(  &windowManager,camera.getViewMatrix() ) * tempsSunCal;}
        std::vector<glm::mat4> lensMatrix=aflare.viewMatrix(sunPositionToCamera*UNITEASTRONOMIQUE);

        for(int i = 0;i<lensMatrix.size();i++)
        {
            aflare.render(&engine,&flareShader,lensMatrix[i],ProjMatrix);
        } 








        for (int i = 1;i< bodies.size();i++)//////////adapt lens flare position before moving the sun 
        {
            bodies[i].updateForce(t,dt,bodies,glm::vec3(0,0,0));
        }
        aSaturn.updateForce(t,dt,bodies,glm::vec3(0,0,0));

        for(int i =0;i<bodies.size();i++)
        {
            bodies[i].updatePosition(t,dt);
        }
        aSaturn.updatePosition(t,dt);
        //aMoon.updatePosition(t,dt);
        PreviousT= t;







        windowManager.swapBuffers() ;
    }


    return EXIT_SUCCESS;
}
