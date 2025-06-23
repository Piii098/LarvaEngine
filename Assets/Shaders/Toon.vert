#version 330

uniform mat4 uWorldTransform;
uniform mat4 uNormalMatrix;
uniform mat4 uViewProj;
uniform mat4 uLightViewProj;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

out vec2 fragTexCoord;
out vec3 fragNormal;
out vec3 fragWorldPos;
out vec4 fragShadowCoord;

void main()
{

    vec4 pos = vec4(inPosition, 1.0) * uWorldTransform;

    fragWorldPos = pos.xyz;
    
    fragNormal = normalize((uNormalMatrix * vec4(inNormal, 0.0)).xyz);

    gl_Position = pos * uViewProj;

    fragShadowCoord = pos * uLightViewProj;

    fragTexCoord = inTexCoord;
}