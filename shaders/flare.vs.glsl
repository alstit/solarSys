#version 300 es

precision mediump float;

layout(location = 0) in vec3 position;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;

out vec3 localPosition;
out vec4 globalPosition;

void main()
{
    globalPosition = uMVMatrix*vec4(position,1.0);
	localPosition=position;
    gl_Position=uMVMatrix*vec4(position,1.0);
}
