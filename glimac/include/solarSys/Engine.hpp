#include <glimac/FilePath.hpp>
#include <glimac/Program.hpp>
#include <glimac/common.hpp>

using namespace glimac;
class Engine
{
    public:
    Program m_Program;

        GLint uMVPMatrix;
        GLint uMVMatrix;
        GLint uNormalMatrix;
        GLuint uKd;
        GLuint uKs;
        GLuint uShininess;
        GLuint uLightDir_vs;
        GLuint uLightIntensity;

        Engine(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                                applicationPath.dirPath() + "shaders/directionallight.fs.glsl")) 
                                    {
                                        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
                                        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
                                        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
                                        uKd = glGetUniformLocation(m_Program.getGLId(), "uKd");
                                        uKs = glGetUniformLocation(m_Program.getGLId(),"uKs");
                                        uShininess = glGetUniformLocation(m_Program.getGLId(),"uShininess");
                                        uLightDir_vs = glGetUniformLocation(m_Program.getGLId(),"uLightDir_vs");
                                        uLightIntensity = glGetUniformLocation(m_Program.getGLId(),"uLightIntensity"); 
                                    }
};