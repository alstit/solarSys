#version 300 es

precision mediump float;

uniform float uTime;

out vec3 fFragColor;


float disappear()
{
    float coef;
    coef = 1./(2.*uTime);
    return coef;
}
void main()
{
    fFragColor=disappear()*vec3(1,1,1);
}