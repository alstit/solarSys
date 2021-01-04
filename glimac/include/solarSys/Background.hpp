

class Background
{
    public:
    float scale = 2;
    GLuint* texture_id= new GLuint();

    Background(Engine* engine, MyShader* ashader, std::string vertexShader,std::string fragShader,std::shared_ptr<Image> texture)
    {

                ///////bind texture

        glGenTextures(1, texture_id);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, *(this->texture_id));

        glTexImage2D( 	GL_TEXTURE_2D,
                    0,
                    GL_RGBA,
                    texture->getWidth(),
                    texture->getHeight(),
                    0,
                    GL_RGBA,
                    GL_FLOAT,
                    texture->getPixels());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,0);


        engine->loadPlanetShader(ashader,vertexShader,fragShader);
        engine->openglBindBuffShpere();
    };


        glm::mat4 viewMatrix(glm::mat4 MVMatrix,Camera* acamera,Body sun)
        {
            //MVMatrix = translate(MVMatrix,glm::vec3(-.5f*this->scale,-.5f*this->scale,-5));
            //MVMatrix = rotate(MVMatrix,acamera., glm::vec3(0,1,0));
            //std::cout<<" cam angle"<<acamera->getAngleX() <<std::endl;
            //MVMatrix =  glm::rotate(MVMatrix,-acamera->getAngleX(), glm::vec3(1,0,0));
            //MVMatrix = glm::rotate(MVMatrix, -acamera->getAngleY(), glm::vec3(0,1,0));
            MVMatrix = glm::scale(MVMatrix, this->scale*glm::vec3(1, 1, 1));
            return MVMatrix;
        };

        void render(Engine* engine, MyShader* aquadShader, glm::mat4 MVMatrix ,glm::mat4 ProjMatrix, glm::mat4 NormalMatrix,std::shared_ptr<BackgroundShader> currentPlanetShader)
        {
            engine->quadShaders = aquadShader;

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, *(this->texture_id)); // la texture earthTexture est bindée sur l'unité GL_TEXTURE0

            //send texture to uniform
            glUniform1i(currentPlanetShader->uniforms["uTexture"],0);

            engine->renderBack(MVMatrix,ProjMatrix,NormalMatrix);

            //unbind texture
            glActiveTexture(GL_TEXTURE0);              
            glBindTexture(GL_TEXTURE_2D, 0);
        };
} ;
