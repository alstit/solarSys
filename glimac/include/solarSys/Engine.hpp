#pragma once
#include <glimac/FilePath.hpp>
#include <glimac/Program.hpp>
#include <glimac/common.hpp>
#include <string>
#include <glimac/SDLWindowManager.hpp>
#include <solarSys/const.hpp>    
 #include <solarSys/MyShader.hpp>
 #include <solarSys/Disk.hpp>
using namespace glimac;










class Engine
{
    public:
    Sphere asphere = Sphere(10, 64, 64);// defini par le rayon 
    Disk adisk = Disk(1,30);
    MyShader *planetShaders;
    MyShader* trailShaders;
    MyShader* flareShaders; ///// flare shader is actually the diskShader this is an error, flareShader should be called diskShader
    FilePath applicationPath;



    

    ~Engine()
    {
        //glDeleteBuffers(1,this->planetShaders->vbo());
        //glDeleteVertexArrays(1,this->trailShaders->vao());
    }



    Engine(const FilePath& aapplicationPath){
        //this->planetShaders->load(applicationPath,"directionallight","3D");

        //this->trailShaders->load(applicationPath,"trail","trail");

        //this->openglBindBuffShpere();
        //this->openglBindBuffTrail();
        this->applicationPath = aapplicationPath;

        glEnable(GL_LINE_WIDTH);
        glLineWidth(3);

    };
        
    void loadPlanetShader(MyShader *aPlanetShader,std::string vertexPath,std::string fragmentPath)
    {
        this->planetShaders = aPlanetShader;
        this->planetShaders->load(applicationPath,fragmentPath,vertexPath);
    }    

    void loadFlareShader(MyShader *aFlareShader,std::string vertexPath,std::string fragmentPath)
    {
        this->flareShaders = aFlareShader;
        this->flareShaders->load(applicationPath,fragmentPath,vertexPath);
    }   


    void openglBindBuffDisk()
    {
        this->flareShaders->use();
        this->flareShaders->getUniforms();

        glGenBuffers(1,this->flareShaders->vbo());
        glBindBuffer(GL_ARRAY_BUFFER,*(this->flareShaders->vbo()));
        glBufferData(GL_ARRAY_BUFFER,this->adisk.getVertexCount()*sizeof(Disk),this->adisk.getDataPointer(),GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER,0);


        glGenVertexArrays(1,this->flareShaders->vao());
        glBindVertexArray(*(this->flareShaders->vao()));
        glBindBuffer(GL_ARRAY_BUFFER,*(this->flareShaders->vbo()));

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex3DColor),(void*)offsetof(Vertex3DColor,position));
        glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,sizeof(Vertex3DColor),(void*)offsetof(Vertex3DColor,color));
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);

    }

    void renderDisk(glm::mat4 MVMatrix, glm::mat4 ProjMatrix, glm::mat4 NormalMatrix)
    {
        this->flareShaders->use();
        flareShaders->renderMVP(MVMatrix,ProjMatrix,NormalMatrix);   
        glBindVertexArray(*(this->flareShaders->vao()));
	    //glDrawArray(primitives, firstVertex,nbrOfVertex)
	    glDrawArrays(GL_TRIANGLES,0,this->adisk.getVertexCount());
	    glBindVertexArray(0);

    }



    void openglBindBuffShpere()
    {
        this->planetShaders->use();

        planetShaders->getUniforms();


        glGenBuffers(1,this->planetShaders->vbo());
        glBindBuffer(GL_ARRAY_BUFFER,*(this->planetShaders->vbo()));
        glBufferData(GL_ARRAY_BUFFER,this->asphere.getVertexCount()*sizeof(Sphere),this->asphere.getDataPointer(),GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER,0);


        glGenVertexArrays(1,this->planetShaders->vao());
        glBindVertexArray(*(this->planetShaders->vao()));
        glBindBuffer(GL_ARRAY_BUFFER,*(this->planetShaders->vbo()));

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(ShapeVertex),(void*)offsetof(ShapeVertex,position));
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(ShapeVertex),(void*)offsetof(ShapeVertex,normal));
        glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(ShapeVertex),(void*)offsetof(ShapeVertex,texCoords));
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
    };

    void renderPlanet(glm::mat4 MVMatrix,glm::mat4 ProjMatrix, glm::mat4 NormalMatrix)
    {
        this->planetShaders->use();

        planetShaders->renderMVP(MVMatrix,ProjMatrix,NormalMatrix);                       

        glBindVertexArray(*(this->planetShaders->vao()));
        glDrawArrays(GL_TRIANGLES,0,this->asphere.getVertexCount());
        glBindVertexArray(0);
    };


    void loadTrailShader(MyShader* atrailshader,std::string vertexPath,std::string fragmentPath)
    {
        this->trailShaders=atrailshader;
        this->trailShaders->load(applicationPath,fragmentPath,vertexPath);
        this->trailShaders->use();
        this->trailShaders->getUniforms();
        this->openglBindBuffTrail();
    }

    void openglBindBuffTrail()
    {
        this->trailShaders->use();

        vec3 vertices[] = {vec3(0,0,0),vec3(0,-1.f,0)}; 

        glGenBuffers(1,this->trailShaders->vbo());
        glBindBuffer(GL_ARRAY_BUFFER,*(this->trailShaders->vbo()));
        glBufferData(GL_ARRAY_BUFFER,2*sizeof(vec3),vertices,GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER,0);

        glGenVertexArrays(1,this->trailShaders->vao());
        glBindVertexArray(*(this->trailShaders->vao()));
        glBindBuffer(GL_ARRAY_BUFFER,*(this->trailShaders->vbo()));

        glEnableVertexAttribArray(0);


        glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(vec3),0);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glEnable(GL_LINE_WIDTH);
    };
   
    void renderTrail(glm::mat4 MVMatrix,glm::mat4 ProjMatrix, glm::mat4 NormalMatrix, float time)
    {
                  
        this->trailShaders->use(); 

        trailShaders->renderMVP(MVMatrix,ProjMatrix,NormalMatrix);    
        //glUniform1f( this->trailShaders->uTime,time/TRAIL_FADE_FACTOR);                     

        glBindVertexArray(*(this->trailShaders->vao()));
        glDrawArrays(GL_LINE_STRIP,0,2);
        glBindVertexArray(0);
    };

 



};