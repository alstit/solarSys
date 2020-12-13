#version 300 es

precision mediump float;

out vec4 fFragColor;
in vec3 outPos;

float particule(vec3 aposition)
{
   float d = distance(aposition,vec3(0,0,0));
   return exp(-5.0*d);
}


void main()
{
   fFragColor = vec4(particule(outPos)*vec3(1,1,0.1),1);

}
