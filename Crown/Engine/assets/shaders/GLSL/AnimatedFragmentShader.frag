#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture0;

void main()
{
	vec4 texColor = texture(texture0, TexCoord);
	FragColor = texColor;
	//FragColor = vec4(1.0f, 0.5f, 0.25f, 1.0f);
}