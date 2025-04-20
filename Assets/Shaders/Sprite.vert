#version 330

uniform mat4 uWorldTransform;
uniform mat4 uViewScreen;
uniform mat4 uViewProj;

layout(location = 0) in vec3 inPosition; // 頂点の座標
layout(location = 1) in vec2 inTexCoord;

out vec2 fragTexCoord;
out vec2 fragPosition;

uniform vec2 uTexOffset;
uniform vec2 uTexScale; 

void main(){

	vec4 worldPos = vec4(inPosition, 1.0) * uWorldTransform;

	vec4 screenPos = worldPos * uViewScreen;

	gl_Position = uViewProj * screenPos; // 現在読み込み中の頂点のワールド行列を作る

	fragTexCoord = inTexCoord * uTexScale + uTexOffset;
	fragPosition = worldPos.xy;
}
