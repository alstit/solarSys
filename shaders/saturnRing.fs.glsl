#version 300 es

precision mediump float;

out vec4 fFragColor;
in vec3 outPos;
in vec4 viewNormale;
in vec4 viewPosition;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;
uniform vec4 uLightPos_vs;
uniform vec3 uLightIntensity; 

vec3 blinnPhong()
{

    /////////////////////////////pour les commettes reprndre ce code et fixer d=1
    float d = distance(uLightPos_vs,viewPosition);
    d = 1.0/(d*d);
    //d=1.0;
    vec3 LightIntensity = uLightIntensity*d;
    vec4 halfVec =normalize((-viewPosition+uLightPos_vs+uLightPos_vs)*0.5);
    float scal1 = dot(  normalize(uLightPos_vs),  normalize(  viewNormale   )   );

    float scal2 = dot(halfVec,normalize(viewNormale)); 
    scal2 = pow(scal2,uShininess);
    
    vec3 glossy = uKs*scal2;
    vec3 diffuse = uKd*scal1; 

    return LightIntensity*(glossy+diffuse);

}

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
       fFragColor = vec4(particule(outPos)* blinnPhong()*vec3(0.8,0.5,0.3)+.1,1);
    }
//fFragColor = vec4(1,0,0,1);
}
