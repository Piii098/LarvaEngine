#version 330

uniform mat4 uWorldTransform;
uniform mat4 uViewScreen;
uniform mat4 uViewProj;

layout(location = 0) in vec3 inPosition; // ���_�̍��W

out vec2 fragPosition;

void main(){

	vec4 worldPos = vec4(inPosition, 1.0) * uWorldTransform;

	vec4 screenPos = worldPos * uViewScreen;

	gl_Position = uViewProj * screenPos; // ���ݓǂݍ��ݒ��̒��_�̃��[���h�s������

	fragPosition = worldPos.xy;
}
