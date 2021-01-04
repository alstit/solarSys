#version 300 es

precision mediump float;

out vec4 fFragColor;
in vec2 color;

uniform sampler2D uTexture;

void main()
{
    vec4 aTexture = texture(uTexture ,color);
   fFragColor = aTexture;

}