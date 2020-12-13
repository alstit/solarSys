#pragma once
#include <solarSys/Disk.hpp>


class Flare
{
    glm::vec3 position;
    

    public:
            glm::mat4 viewMatrix(glm::vec4 lightPos_toCam){ //light pos to camera is position of light source in camera repere i.e. lightPos_toCam = MVMatrix*lightPos
                //MVMatrix = translate(MVMatrix,glm::vec3(0,0,0));
                
                //MVMatrix = rotate(MVMatrix, 1.5f, glm::vec3(0,1,0));
                
                //glm::mat4 MVMatrix = translate(glm::mat4(),glm::vec3(-1,-1,0));
                glm::mat4 MVMatrix = translate(glm::mat4(),10.f*glm::vec3(-lightPos_toCam[0],-lightPos_toCam[1],0.f));
                MVMatrix = glm::scale(MVMatrix,.5f*glm::vec3(1, 1, 1));
                return MVMatrix;
            }

};
