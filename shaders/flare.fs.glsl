#version 300 es

precision mediump float;


in vec3 localPosition;
out vec4 fFragColor;

vec3 color(vec3 position)
{
	float valueR =pow(distance(vec3(0,0,0),position),6.0f);
	float valueG = pow(sin(distance(vec3(0,0,0),position)),2.0f)/2.0f;
	return vec3(valueR,valueG,0);
}

void main()
{
    //fFragColor=vec4(1.f,1.f,0,0.5f);
    
   //fFragColor = vec4(1,0,0, 0);
   fFragColor = vec4(color(localPosition),0);
   //gl_FragColor = vec4(1.f,1.f,1.f,0.5f); 
}
