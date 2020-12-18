#version 300 es

precision mediump float;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normale;
layout(location = 2) in vec2 texCoord;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;

out vec4 viewPosition;
out vec4 viewNormale;
out vec2 color;
out vec3 outPos;


void main()
{
    outPos  =position;
    color = texCoord;
    viewNormale = uNormalMatrix*vec4(normale,1.0);
    viewPosition = uMVMatrix*vec4(position,1.0);
    gl_Position=uMVPMatrix*vec4(position,1.0);
}