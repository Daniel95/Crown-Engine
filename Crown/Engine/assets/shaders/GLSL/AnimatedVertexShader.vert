#version 330 core

const int MAX_BONES = 128;
const int MAX_BONES_PER_VERTEX = 4;

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in ivec4 aBoneIDs;
layout(location = 4) in vec4 aWeights;

out vec3 Normal;
out vec2 TexCoord;

uniform mat4 gMVP;
uniform mat4 gBones[MAX_BONES];

void main()
{
    mat4 BoneTransform = mat4(0.0);
    
    for(int i = 0; i < MAX_BONES_PER_VERTEX; ++i)
    {
        BoneTransform += gBones[aBoneIDs[i]] * aWeights[i];
    }

    vec4 localPos = BoneTransform * vec4(aPos, 1.0);
    gl_Position = gMVP * localPos;

    vec4 localNormal = BoneTransform * vec4(aNormal, 0.0);
    localNormal = gMVP * localNormal;
    Normal = localNormal.xyz;

    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}