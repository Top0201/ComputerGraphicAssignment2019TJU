#version 410 core

in vec2 TexCoord;

out vec4 color;
uniform sampler2D ourTexture;

void main()
{
    //color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.2);
	color= texture(ourTexture, TexCoord);
}
