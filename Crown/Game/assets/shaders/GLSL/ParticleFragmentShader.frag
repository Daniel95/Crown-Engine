#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 Color;

// texture sampler
uniform sampler2D texture0;

void main()
{
	vec4 texColor = texture(texture0, TexCoord);
	FragColor = texColor * Color;
}