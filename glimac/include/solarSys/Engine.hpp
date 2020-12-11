#pragma once
#include <glimac/FilePath.hpp>
#include <glimac/Program.hpp>
#include <glimac/common.hpp>
#include <string>
#include <glimac/SDLWindowManager.hpp>
#include <solarSys/const.hpp>    
 
using namespace glimac;




class Object3D 
{
    public:
        Program program;
        GLuint *vbo = new GLuint();
        GLuint *vao = new GLuint();
        GLint uMVPMatrix;
        GLint uMVMatrix;
        GLint uNormalMatrix;
        GLuint uKd;
        GLuint uKs;
        GLuint uShininess;
        GLuint uLightDir_vs;
        GLuint uLightIntensity;
        GLuint uTime;

        Object3D(){};


        void load(const FilePath& applicationPath,std::string fragShader, std::string vertexShader) {
            vertexShader = "shaders/" + vertexShader +".vs.glsl";
            fragShader="shaders/" + fragShader+".fs.glsl";
            this->program = loadProgram(applicationPath.dirPath() + vertexShader , applicationPath.dirPath() + fragShader) ;
        };

        void getUniforms()
        {
            this->program.use();
            this->uTime = glGetUniformLocation(this->program.getGLId(), "uTime");
            this->uMVPMatrix = glGetUniformLocation(this->program.getGLId(), "uMVPMatrix");
            this->uMVMatrix = glGetUniformLocation(this->program.getGLId(), "uMVMatrix");
            this->uNormalMatrix = glGetUniformLocation(this->program.getGLId(), "uNormalMatrix");
            this->uKd = glGetUniformLocation(this->program.getGLId(), "uKd");
            this->uKs = glGetUniformLocation(this->program.getGLId(),"uKs");
            this->uShininess = glGetUniformLocation(this->program.getGLId(),"uShininess");
            this->uLightDir_vs = glGetUniformLocation(this->program.getGLId(),"uLightDir_vs");
            this->uLightIntensity = glGetUniformLocation(this->program.getGLId(),"uLightIntensity"); 
        };


};



void renderMVP(Object3D *aObject3D,glm::mat4 MVMatrix,glm::mat4 ProjMatrix, glm::mat4 NormalMatrix )
{
    glUniformMatrix4fv( 	aObject3D->uMVMatrix,
                                1,
                                GL_FALSE,
                                glm::value_ptr(MVMatrix));
    glUniformMatrix4fv( 	aObject3D->uMVPMatrix,
                                1,
                                GL_FALSE,
                                glm::value_ptr(ProjMatrix*MVMatrix));
    glUniformMatrix4fv( 	aObject3D->uNormalMatrix,
                                1,
                                GL_FALSE,
                                glm::value_ptr(NormalMatrix));   
};





class Engine
{
    public:
    Sphere asphere = Sphere(10, 64, 64);// defini par le rayon 
    Object3D planetShaders;
    Object3D trailShaders;





    

    ~Engine()
    {
        glDeleteBuffers(1,this->planetShaders.vbo);
        glDeleteVertexArrays(1,this->trailShaders.vao);
    }



    Engine(const FilePath& applicationPath){
        this->planetShaders.load(applicationPath,"directionallight","3D");

        this->trailShaders.load(applicationPath,"trail","trail");

        this->openglBindBuffShpere();
        this->openglBindBuffTrail();


    };
        

    void openglBindBuffShpere()
    {
        this->planetShaders.program.use();

        planetShaders.getUniforms();

        glGenBuffers(1,this->planetShaders.vbo);
        glBindBuffer(GL_ARRAY_BUFFER,*(this->planetShaders.vbo));
        glBufferData(GL_ARRAY_BUFFER,this->asphere.getVertexCount()*sizeof(Sphere),this->asphere.getDataPointer(),GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER,0);

        glGenVertexArrays(1,this->planetShaders.vao);
        glBindVertexArray(*(this->planetShaders.vao));
        glBindBuffer(GL_ARRAY_BUFFER,*(this->planetShaders.vbo));

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
        this->planetShaders.program.use();

        renderMVP(&planetShaders,MVMatrix,ProjMatrix,NormalMatrix);                       

        glBindVertexArray(*(this->planetShaders.vao));
        glDrawArrays(GL_TRIANGLES,0,this->asphere.getVertexCount());
        glBindVertexArray(0);
    };




    void openglBindBuffTrail()
    {
        this->trailShaders.program.use();
        this->trailShaders.getUniforms();

        vec3 vertices[] = {vec3(0,0,0),vec3(0,-1.f,0)};

        glGenBuffers(1,this->trailShaders.vbo);
        glBindBuffer(GL_ARRAY_BUFFER,*(this->trailShaders.vbo));
        glBufferData(GL_ARRAY_BUFFER,2*sizeof(vec3),vertices,GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER,0);

        glGenVertexArrays(1,this->trailShaders.vao);
        glBindVertexArray(*(this->trailShaders.vao));
        glBindBuffer(GL_ARRAY_BUFFER,*(this->trailShaders.vbo));

        glEnableVertexAttribArray(0);


        glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(vec3),0);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glEnable(GL_LINE_WIDTH);
    };
   
    void renderTrail(glm::mat4 MVMatrix,glm::mat4 ProjMatrix, glm::mat4 NormalMatrix, float time)
    {
        glEnable(GL_LINE_WIDTH);
        glLineWidth(3);

        this->trailShaders.program.use();

        renderMVP(&trailShaders,MVMatrix,ProjMatrix,NormalMatrix);    
        glUniform1f( this->trailShaders.uTime,time/TRAIL_FADE_FACTOR);                     

        glBindVertexArray(*(this->trailShaders.vao));
        glDrawArrays(GL_LINE_STRIP,0,2);
        glBindVertexArray(0);
    };

 



};