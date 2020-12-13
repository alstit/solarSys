

class Bloom
{
    public:
    float radius = 1;

    Bloom(Engine* engine, MyShader* ashader, std::string vertexShader,std::string fragShader)
    {
        engine->loadFlareShader(ashader,vertexShader,fragShader);
        engine->openglBindBuffDisk();
    };


        glm::mat4 viewMatrix(glm::mat4 MVMatrix,Camera* acamera,Body sun)
        {
            MVMatrix = translate(MVMatrix,sun.position/UNITEASTRONOMIQUE);
            //MVMatrix = rotate(MVMatrix,acamera., glm::vec3(0,1,0));
            std::cout<<" cam angle"<<acamera->getAngleX() <<std::endl;
            MVMatrix =  glm::rotate(MVMatrix,-acamera->getAngleX(), glm::vec3(1,0,0));
            MVMatrix = glm::rotate(MVMatrix, -acamera->getAngleY(), glm::vec3(0,1,0));
            MVMatrix = glm::scale(MVMatrix, 3.0f*glm::vec3(1, 1, 1));
            return MVMatrix;
        };

        void render(Engine* engine, MyShader* abloomShader, glm::mat4 MVMatrix ,glm::mat4 ProjMatrix, glm::mat4 NormalMatrix)
        {
            engine->flareShaders = abloomShader;
            engine->renderDisk(MVMatrix,ProjMatrix,NormalMatrix);
        };
} ;