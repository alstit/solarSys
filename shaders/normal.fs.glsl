#version 300 es

precision mediump float;

in vec2 texture;
in vec4 viewPosition;
in vec4 viewNormale;

out vec2 fFragColor;

void main()
{
    fFragColor = normalize(texture); 
}

