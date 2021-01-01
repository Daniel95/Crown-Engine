#version 330 core
layout(location = 0) in vec3 aVertexPos;
layout(location = 1) in vec2 aVertexTexCoord;

layout(location = 2) in vec3 aInstancePos;
layout(location = 3) in vec4 aInstanceColor;
layout(location = 4) in vec2 aInstanceScale;
layout(location = 5) in float aInstanceRotation;
layout(location = 6) in vec2 aInstanceTexCoordOffset;

out vec2 TexCoord;
out vec4 Color;

uniform vec3 cameraRight;
uniform vec3 cameraUp;
uniform mat4 mvp;
uniform vec2 texAtlasCellSize;

void main()
{   
    //vec2 rotatedVertex;
    //rotatedVertex.x = cos(aInstanceRotation) * (aVertexPos.x) - sin(aInstanceRotation) * (aVertexPos.y);
    //rotatedVertex.y = sin(aInstanceRotation) * (aVertexPos.x) + cos(aInstanceRotation) * (aVertexPos.y);
    //aVertexPos = rotatedVertex;

    vec3 vertexWorldPos = aInstancePos
    + cameraRight * aVertexPos.x * aInstanceScale.x
    + cameraUp * aVertexPos.y * aInstanceScale.y
    ;

    gl_Position = mvp * vec4(vertexWorldPos, 1.0f);

    TexCoord = aInstanceTexCoordOffset + aVertexTexCoord * texAtlasCellSize;

    Color = aInstanceColor;
}