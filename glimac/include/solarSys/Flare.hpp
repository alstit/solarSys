#pragma once
#include <solarSys/Disk.hpp>
#include <vector>


class Lens
{
    public:
        float scale;
        float moveFactor;
        Lens(float ascale, float amoveFactor)
        {
            scale=ascale;
            moveFactor=amoveFactor;
        };
};

class Flare
{
    std::vector<Lens> lens;

    
    public:


        Flare(int nbrofLens,Engine* engine, MyShader* ashader, std::string vertexShader,std::string fragShader)
        {
            for(int i =0;i<nbrofLens;i++)
            {
                lens.push_back(Lens((i+1)/20.0f,1+2*nbrofLens/(float)(nbrofLens+1-i)*nbrofLens/(float)(nbrofLens+1-i)*nbrofLens/(float)(nbrofLens+1-i)));
            }
            engine->loadFlareShader(ashader,vertexShader,fragShader);
            engine->openglBindBuffDisk();
        }

        std::vector<glm::mat4> viewMatrix(glm::vec4 lightPos_toCam) //light pos to camera is position of light source in camera repere i.e. lightPos_toCam = MVMatrix*lightPos
        {    //MVMatrix = translate(MVMatrix,glm::vec3(0,0,0));
            
            //MVMatrix = rotate(MVMatrix, 1.5f, glm::vec3(0,1,0));
            
            //glm::mat4 MVMatrix = translate(glm::mat4(),glm::vec3(-1,-1,0));
            std::vector<glm::mat4> matrix;
            for(int i = 0;i<lens.size();i++)
            {
                glm::mat4 MVMatrix = translate(glm::mat4(),lens[i].moveFactor*glm::vec3(-lightPos_toCam[0],-lightPos_toCam[1],0.f));
                MVMatrix = glm::scale(MVMatrix,lens[i].scale*glm::vec3(1, 1, 1));
                matrix.push_back(MVMatrix);
            }
            return matrix;
        }

        void render(Engine* engine,MyShader* aflareShader,glm::mat4 MVMatrix,glm::mat4 ProjMatrix)
        {   engine->flareShaders = aflareShader;
            engine->renderDisk(MVMatrix,ProjMatrix,glm::mat4());
        };


};
