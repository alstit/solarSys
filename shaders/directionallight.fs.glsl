#version 300 es

precision mediump float;


in vec4 viewPosition;
in vec4 viewNormale;
in vec2 color;

out vec3 fFragColor;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;
uniform vec3 uLightDir_vs;
uniform vec3 uLightIntensity; 

vec3 blinnPhong()
{

    vec3 halfVec =normalize((-vec3(viewPosition.x,viewPosition.y,viewPosition.z)+uLightDir_vs)*0.5);
    float scal1 = dot(  normalize(uLightDir_vs),  normalize(  vec3(viewNormale.x,viewNormale.y,viewNormale.z)   )   );

    float scal2 = dot(halfVec,normalize(vec3(viewNormale.x,viewNormale.y,viewNormale.z))); 
    scal2 = pow(scal2,uShininess);
    
    vec3 glossy = uKs*scal2;
    vec3 diffuse = uKd*scal1; 

    return uLightIntensity*(glossy+diffuse);
}


void main()
{
    


	//fFragColor = blinnPhong();
    fFragColor = vec3(color,0);
}