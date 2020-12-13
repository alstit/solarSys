#version 300 es

precision mediump float;

layout(location = 0) in vec3 position;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;



void main()
{
    gl_Position=uMVPMatrix*vec4(position,1);
}