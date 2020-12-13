#pragma once

#include <vector>
#include <glimac/common.hpp>




namespace glimac {


    class Vertex3DColor
    {
        public:
        Vertex3DColor(){};
        Vertex3DColor(glm::vec3 position, glm::vec3 color):position(position),color(color){};
        
        glm::vec3 position;
        glm::vec3 color;

    };

    Vertex3DColor vertPosition(float rayon,float teta, glm::vec3 color)
    {
        
        return Vertex3DColor(glm::vec3(rayon*glm::cos(teta),rayon*glm::sin(teta),0),color);
    };


    // Représente une sphère discrétisée centrée en (0, 0, 0) (dans son repère local)
    // Son axe vertical est (0, 1, 0) et ses axes transversaux sont (1, 0, 0) et (0, 0, 1)
    class Disk 
    {
        // Alloue et construit les données (implantation dans le .cpp)
        void build(GLfloat radius, GLuint nbrVertex);

        public:
            // Constructeur: alloue le tableau de données et construit les attributs des vertex
            Disk(GLfloat radius, GLuint nbrVertex):
                m_nVertexCount(0) {
                build(radius, nbrVertex); // Construction (voir le .cpp)
            }

            // Renvoit le pointeur vers les données
            const Vertex3DColor* getDataPointer() const {
                return &m_Vertices[0];
            }
            

            // Renvoit le nombre de vertex
            GLsizei getVertexCount() const {
                return m_nVertexCount;
            }

        private:
            std::vector<Vertex3DColor> m_Vertices;
            GLsizei m_nVertexCount; // Nombre de sommets
    };
        


    void Disk::build(GLfloat radius,GLuint nbrVertex)
    {
        //std::cout<<"hello"<<std::endl;
        int Ntriangle = nbrVertex;
        float verticesIncr = 2.0*glm::pi<float>()/Ntriangle; 
        float teta = 0;
        float rayon = radius;

        glm::vec3 color = glm::vec3(0,0,0);

        Vertex3DColor vertices[3*Ntriangle];
        this->m_nVertexCount = 3*Ntriangle;

        for(int i =0 ; i < 3*Ntriangle ; i+=3)
        {

            vertices[i] = vertPosition(0,0,color);
            //std::cout << "x,y 0 :" << 0*glm::cos(0.) << 0*glm::sin(0.)<<std::endl;

            vertices[i+1] = vertPosition(rayon,teta,color);
            //std::cout << "x,y 1 :" << rayon*glm::cos(teta) << rayon*glm::sin(teta)<<std::endl;

            teta +=verticesIncr;
            vertices[i+2] = vertPosition(rayon,teta,color);
            //std::cout << "x,y 2 :" << rayon*glm::cos(teta) << rayon*glm::sin(teta)<<std::endl;
        }

        for(int i = 0 ; i< 3*Ntriangle;i++)
        {
            this->m_Vertices.push_back(vertices[i]);
        }

    };


}