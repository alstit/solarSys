#version 300 es

precision mediump float;


in vec4 viewPosition;
in vec4 viewNormale;
in vec2 color;

out vec4 fFragColor;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;
uniform vec4 uLightPos_vs;
uniform vec3 uLightIntensity; 

uniform sampler2D uTexture;

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


void main()
{
    
	vec4 aTexture = texture(uTexture ,color);
    //fFragColor = vec3(color,1);
	//fFragColor = blinnPhong();
	fFragColor = vec4(vec3(blinnPhong().x*aTexture.x,blinnPhong().y*aTexture.y,blinnPhong().z*aTexture.z), 1.0);

}
