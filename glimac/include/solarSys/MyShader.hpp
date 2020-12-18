#pragma once
#include <glimac/FilePath.hpp>

using namespace glimac;

class IMyShader
{
    public :
        void getload(const FilePath& applicationPath,std::string fragShader, std::string vertexShader)
            {
                return load(applicationPath,fragShader,vertexShader);
                }
        void Uniforms(){return getUniforms();}
        void getrenderMVP(glm::mat4 MVMatrix,glm::mat4 ProjMatrix, glm::mat4 NormalMatrix){return renderMVP(MVMatrix,ProjMatrix, NormalMatrix);}
        GLuint* vbo(){return getVbo();};
        GLuint* vao(){return getVao();};
        void getUse(){use();};

        virtual ~IMyShader() = default;
    private :
        virtual void load(const FilePath& applicationPath,std::string fragShader, std::string vertexShader) = 0;
        virtual void getUniforms()=0;
        virtual void renderMVP(glm::mat4 MVMatrix,glm::mat4 ProjMatrix, glm::mat4 NormalMatrix) = 0;
        virtual GLuint* getVbo()=0;
        virtual GLuint* getVao()=0;   
        virtual void use()=0;    
};











class GenericShader: public IMyShader
{
    public:
        Program* program = new Program;
        GLuint *vbo = new GLuint();
        GLuint *vao = new GLuint();
        std::unordered_map<std::string,GLuint> uniforms;
        /*GLint uMVPMatrix;
        GLint uMVMatrix;
        GLint uNormalMatrix;
        GLuint uKd;
        GLuint uKs;
        GLuint uShininess;
        GLuint uLightDir_vs;
        GLuint uLightIntensity;
        GLuint uTime;*/

        GenericShader(){};
        
        void use(){this->program->use();};

        GLuint* getVbo(){return this->vbo;};
        GLuint* getVao(){return this->vao;};  

        void load(const FilePath& applicationPath,std::string fragShader, std::string vertexShader) {
            vertexShader = "shaders/" + vertexShader +".vs.glsl";
            fragShader="shaders/" + fragShader+".fs.glsl";
            *(this->program) = loadProgram(applicationPath.dirPath() + vertexShader , applicationPath.dirPath() + fragShader) ;
            std::cout<<program<<std::endl;
        };

        void getUniforms()
        {

            this->program->use();
            //this->uniforms["uTime"] = (glGetUniformLocation(this->program->getGLId(), "uTime"));
            this->uniforms["uMVPMatrix"] = glGetUniformLocation(this->program->getGLId(), "uMVPMatrix");
            this->uniforms["uMVMatrix"] = glGetUniformLocation(this->program->getGLId(), "uMVMatrix");
            this->uniforms["uNormalMatrix"] = glGetUniformLocation(this->program->getGLId(), "uNormalMatrix");
            /*this->uniforms["uKd"] = glGetUniformLocation(this->program->getGLId(), "uKd");
            this->uniforms["uKs"] = glGetUniformLocation(this->program->getGLId(),"uKs");
            this->uniforms["uShininess"] = glGetUniformLocation(this->program->getGLId(),"uShininess");
            this->uniforms["uLightDir_vs"] = glGetUniformLocation(this->program->getGLId(),"uLightDir_vs");
            this->uniforms["uLightIntensity"] = glGetUniformLocation(this->program->getGLId(),"uLightIntensity");*/ 
        };




        void renderMVP(glm::mat4 MVMatrix,glm::mat4 ProjMatrix, glm::mat4 NormalMatrix )
        {

            glUniformMatrix4fv( 	this->uniforms["uMVMatrix"],
                                        1,
                                        GL_FALSE,
                                        glm::value_ptr(MVMatrix));
            glUniformMatrix4fv( 	this->uniforms["uMVPMatrix"],
                                        1,
                                        GL_FALSE,
                                        glm::value_ptr(ProjMatrix*MVMatrix));
            glUniformMatrix4fv( 	this->uniforms["uNormalMatrix"],
                                        1,
                                        GL_FALSE,
                                        glm::value_ptr(NormalMatrix));   
        };
};




class MyShader final
{
    private:
    std::shared_ptr<IMyShader> aShader;

public:
    MyShader(std::shared_ptr<IMyShader> new_shader)
        : aShader(std::move(new_shader))
    {};

    void use()
    {
        aShader->getUse();
    };

    GLuint* vbo()
    {
        return aShader->vbo();
    };
    GLuint* vao()
    {
        return aShader->vao();
    };  

    void load(const FilePath& applicationPath,std::string fragShader, std::string vertexShader) 
    {
        aShader->getload(applicationPath, fragShader, vertexShader);
    };

    void getUniforms()
    {
        aShader->Uniforms();
    };

    void renderMVP(glm::mat4 MVMatrix,glm::mat4 ProjMatrix, glm::mat4 NormalMatrix )
    {
        aShader->getrenderMVP(MVMatrix, ProjMatrix,  NormalMatrix);
    };


};


class TrailShader:public GenericShader
{
    public:
    TrailShader():GenericShader(){};

    void getUniforms()
    {

        this->program->use();
        this->uniforms["uTime"] = (glGetUniformLocation(this->program->getGLId(), "uTime"));
        this->uniforms["uMVPMatrix"] = glGetUniformLocation(this->program->getGLId(), "uMVPMatrix");
        this->uniforms["uMVMatrix"] = glGetUniformLocation(this->program->getGLId(), "uMVMatrix");
        this->uniforms["uNormalMatrix"] = glGetUniformLocation(this->program->getGLId(), "uNormalMatrix");
        /*this->uniforms["uKd"] = glGetUniformLocation(this->program->getGLId(), "uKd");
        this->uniforms["uKs"] = glGetUniformLocation(this->program->getGLId(),"uKs");
        this->uniforms["uShininess"] = glGetUniformLocation(this->program->getGLId(),"uShininess");
        this->uniforms["uLightDir_vs"] = glGetUniformLocation(this->program->getGLId(),"uLightDir_vs");
        this->uniforms["uLightIntensity"] = glGetUniformLocation(this->program->getGLId(),"uLightIntensity");*/ 
    };


    void setTime(float time)
    {
        this->program->use();

        glUniform1f(this->uniforms["uTime"],time/TRAIL_FADE_FACTOR);  
    };

};

class SunShader:public GenericShader
{
    public:
    SunShader():GenericShader(){};

    void getUniforms()
    {

        this->program->use();
        //this->uniforms["uTime"] = (glGetUniformLocation(this->program->getGLId(), "uTime"));
        this->uniforms["uMVPMatrix"] = glGetUniformLocation(this->program->getGLId(), "uMVPMatrix");
        this->uniforms["uMVMatrix"] = glGetUniformLocation(this->program->getGLId(), "uMVMatrix");
        this->uniforms["uNormalMatrix"] = glGetUniformLocation(this->program->getGLId(), "uNormalMatrix");
        this->uniforms["uKd"] = glGetUniformLocation(this->program->getGLId(), "uKd");
        this->uniforms["uKs"] = glGetUniformLocation(this->program->getGLId(),"uKs");
        this->uniforms["uShininess"] = glGetUniformLocation(this->program->getGLId(),"uShininess");
        this->uniforms["uLightDir_vs"] = glGetUniformLocation(this->program->getGLId(),"uLightDir_vs");
        this->uniforms["uLightIntensity"] = glGetUniformLocation(this->program->getGLId(),"uLightIntensity");
    };

    void setUniforms(glm::vec3 uKd,glm::vec3 uKs, glm::vec3 uLightDir_vs,glm::vec3 uLightIntensity, float uShininess)
    {
        this->program->use();

        glUniform1f(this->uniforms["uShininess"],uShininess);  
        glUniform3f(this->uniforms["uKd"],uKd[0],uKd[1],uKd[2]);
        glUniform3f(this->uniforms["uKs"],uKs[0],uKs[1],uKs[2]);        
        glUniform3f(this->uniforms["uLightDir_vs"],uLightDir_vs[0],uLightDir_vs[1],uLightDir_vs[2]);
        glUniform3f(this->uniforms["uLightIntensity"],uLightIntensity[0],uLightIntensity[1],uLightIntensity[2]);
    };

};

class PlanetShader:public GenericShader
{
    public:
    PlanetShader():GenericShader(){};

    void getUniforms()
    {

        this->program->use();
        //this->uniforms["uTime"] = (glGetUniformLocation(this->program->getGLId(), "uTime"));
        this->uniforms["uMVPMatrix"] = glGetUniformLocation(this->program->getGLId(), "uMVPMatrix");
        this->uniforms["uMVMatrix"] = glGetUniformLocation(this->program->getGLId(), "uMVMatrix");
        this->uniforms["uNormalMatrix"] = glGetUniformLocation(this->program->getGLId(), "uNormalMatrix");
        this->uniforms["uKd"] = glGetUniformLocation(this->program->getGLId(), "uKd");
        this->uniforms["uKs"] = glGetUniformLocation(this->program->getGLId(),"uKs");
        this->uniforms["uShininess"] = glGetUniformLocation(this->program->getGLId(),"uShininess");
        this->uniforms["uLightPos_vs"] = glGetUniformLocation(this->program->getGLId(),"uLightPos_vs");
        this->uniforms["uLightIntensity"] = glGetUniformLocation(this->program->getGLId(),"uLightIntensity");
        this->uniforms["uTexture"] = glGetUniformLocation(this->program->getGLId(), "uTexture");
    };

    void setUniforms(glm::vec3 uKd,glm::vec3 uKs, glm::vec4 uLightPos_vs,glm::vec3 uLightIntensity, float uShininess)
    {
        this->program->use();

        glUniform1f(this->uniforms["uShininess"],uShininess);  
        glUniform3f(this->uniforms["uKd"],uKd[0],uKd[1],uKd[2]);
        glUniform3f(this->uniforms["uKs"],uKs[0],uKs[1],uKs[2]);        
        glUniform4f(this->uniforms["uLightPos_vs"],uLightPos_vs[0],uLightPos_vs[1],uLightPos_vs[2],uLightPos_vs[3]);
        glUniform3f(this->uniforms["uLightIntensity"],uLightIntensity[0],uLightIntensity[1],uLightIntensity[2]);
        glUniform1i(this->uniforms["uTexture"],0);
        
    };

};


class SaturnRingShader:public GenericShader
{
    public:
    SaturnRingShader():GenericShader(){};

    void getUniforms()
    {

        this->program->use();
        //this->uniforms["uTime"] = (glGetUniformLocation(this->program->getGLId(), "uTime"));
        this->uniforms["uMVPMatrix"] = glGetUniformLocation(this->program->getGLId(), "uMVPMatrix");
        this->uniforms["uMVMatrix"] = glGetUniformLocation(this->program->getGLId(), "uMVMatrix");
        this->uniforms["uNormalMatrix"] = glGetUniformLocation(this->program->getGLId(), "uNormalMatrix");
        this->uniforms["uKd"] = glGetUniformLocation(this->program->getGLId(), "uKd");
        this->uniforms["uKs"] = glGetUniformLocation(this->program->getGLId(),"uKs");
        this->uniforms["uShininess"] = glGetUniformLocation(this->program->getGLId(),"uShininess");
        this->uniforms["uLightPos_vs"] = glGetUniformLocation(this->program->getGLId(),"uLightPos_vs");
        this->uniforms["uLightIntensity"] = glGetUniformLocation(this->program->getGLId(),"uLightIntensity");
    };

    void setUniforms(glm::vec3 uKd,glm::vec3 uKs, glm::vec4 uLightPos_vs,glm::vec3 uLightIntensity, float uShininess)
    {
        this->program->use();

        glUniform1f(this->uniforms["uShininess"],uShininess);  
        glUniform3f(this->uniforms["uKd"],uKd[0],uKd[1],uKd[2]);
        glUniform3f(this->uniforms["uKs"],uKs[0],uKs[1],uKs[2]);        
        glUniform4f(this->uniforms["uLightPos_vs"],uLightPos_vs[0],uLightPos_vs[1],uLightPos_vs[2],uLightPos_vs[3]);
        glUniform3f(this->uniforms["uLightIntensity"],uLightIntensity[0],uLightIntensity[1],uLightIntensity[2]);
        
    };

};