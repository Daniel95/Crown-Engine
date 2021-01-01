#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;
//uniform sampler2D texture2;
uniform float textureBlend;
uniform vec4 tint;

void main()
{
	// FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), textureBlend);
	// vec4 tex = texture(texture1, TexCoord) * tint;
	// FragColor = texture(texture1, TexCoord);
	FragColor = texture(texture1, TexCoord) * tint;
}