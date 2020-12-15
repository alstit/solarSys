#version 300 es

precision mediump float;

out vec4 fFragColor;
in vec3 outPos;

float particule(vec3 aposition)
{
   float d = distance(aposition,vec3(0,0,0));
   d = pow(sin(50.0f*d),2.0f);
   if(d<0.05){d = 0.f;}
   if(d>0.8){d=1.0f;}
    return d;
}


void main()
{
    if(distance(outPos,vec3(0,0,0))<0.5)
    {
        fFragColor = vec4(0,0,0,0);     
    }
   else
   {
       fFragColor = vec4(particule(outPos)*vec3(0.8,0.5,0.3),1);
    }
//fFragColor = vec4(1,0,0,1);
}
