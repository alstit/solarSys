#version 300 es

precision mediump float;


in vec4 viewPosition;
in vec4 viewNormale;
in vec2 color;

out vec3 fFragColor;

uniform sampler2D uTexture;

void main()
{
	vec4 text = texture(uTexture,color);

	fFragColor = vec3(text.x,text.y,text.z);
	
}