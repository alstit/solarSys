
#include <glm/glm.hpp>

using namespace glm;
class Body
{    public:
        float scale;
        float masse;
        vec3 position;
        vec3 initSpeed;

        Body( float scale,float masse,vec3 position,vec3 initSpeed)
        {
            this->scale = scale;
            this->masse = masse;
            this->position = position;
            this->initSpeed = initSpeed;
        }

        viewMatrix(glm::mat4 MVMatrix)
        {
            MVMatrix = translate(MVMatrix,earth.position);
            MVMatrix = rotate(MVMatrix, windowManager.getTime(), glm::vec3(0,1,0));
            MVMatrix = glm::scale(MVMatrix, (float)earth.scale*glm::vec3(1, 1, 1));
        }

};
