#version 300 es

precision mediump float;


in vec3 localPosition;
in vec4 globalPosition;
out vec4 fFragColor;

vec3 color(vec3 position)
{
	float valueR =pow(distance(vec3(0,0,0),position),6.0f);
	float valueG = pow(sin(distance(vec3(0,0,0),position)),2.0f)/2.0f;
	return vec3(valueR,valueG,0);
}

float intensity(vec4 aposition)
{
    float intensity = min(1.0f,pow(2.0f*distance(2.0*vec2(aposition.x,aposition.y),vec2(0,0)),10.0f));

    //if(intensity<.5f){return 0.0f;}
    
    return intensity;
}


void main()
{
    //fFragColor=vec4(1.f,1.f,0,0.5f);
    
   //fFragColor = vec4(1,0,0, 0);
   fFragColor = intensity(globalPosition)*vec4(color(localPosition),0);
   //gl_FragColor = vec4(1.f,1.f,1.f,0.5f); 
}
