#version 300 es

precision mediump float;

uniform float uTime;

out vec4 fFragColor;


float disappear()
{
    float coef;
    coef = 1./(2.*uTime);
    return coef;
}
void main()
{
    fFragColor=disappear()*vec4(1,1,1,0);
}
