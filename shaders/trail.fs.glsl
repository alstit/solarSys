#version 300 es

precision mediump float;

uniform float uTime;

out vec4 fFragColor;


float disappear()
{
    float coef;
    coef = 1./(5.*uTime);

    return coef;
}
void main()
{
    fFragColor=disappear()*vec4(0.3,0.5,0.5,0);
}
