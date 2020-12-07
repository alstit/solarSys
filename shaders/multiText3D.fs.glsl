#version 300 es

precision mediump float;


in vec4 viewPosition;
in vec4 viewNormale;
in vec2 color;

out vec3 fFragColor;

uniform sampler2D uTexture1;
uniform sampler2D uTexture2;

void main()
{
    
	vec4 text1 = texture(uTexture1 ,color);
	vec4 text2 = texture(uTexture2,color);

	fFragColor = vec3(text1.x + text2.x,text1.y + text2.y,text1.z+text2.z);
	
}