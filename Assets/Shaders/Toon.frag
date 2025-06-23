#version 330

in vec3 fragNormal;
in vec3 fragWorldPos;
in vec2 fragTexCoord;
in vec4 fragShadowCoord;

layout (location = 0) out vec4 FragColor;

uniform mat4 uInvMatrix;
uniform vec3 uLightDir;

uniform sampler2D uTexture;    
uniform sampler2D uRampTexture;
uniform sampler2D uShadowMap;

void main()
{
	
	vec3 shadowCoord = fragShadowCoord.xyz / fragShadowCoord.w;
	shadowCoord = shadowCoord * 0.5 + 0.5;

	float closestDepth = texture(uShadowMap, shadowCoord.xy).r;
	float currentDepth = shadowCoord.z;

	// シャドウマップのセットアップ時に線形フィルタリングを有効にし、以下のようなコードを使用
	float shadow = 0.0;
	float bias = 0.005;
	vec2 texelSize = 1.0 / textureSize(uShadowMap, 0);
	for(int x = -2; x <= 2; ++x)
	{
		for(int y = -2; y <= 2; ++y)
		{
			float pcfDepth = texture(uShadowMap, shadowCoord.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /= 25.0; // 5x5カーネルの平均

	vec4 texColor = texture(uTexture, fragTexCoord);
	vec3 lightDir  = normalize(uLightDir);
	float diffuse = clamp(dot(fragNormal, lightDir), 0.0, 1.0);

	vec4 smpColor = texture2D(uRampTexture, vec2(diffuse, 0.0));

	if(shadow > 0.5){
		 // 方法2: 青色に少しブレンドする
		vec3 shadowTint = vec3(0.0, 0.0, 0.3); // 青色の色調
		float blendFactor = 0.3; // ブレンド強度
		smpColor.rgb = mix(smpColor.rgb * 0.5, shadowTint, blendFactor);
	}

	FragColor = vec4(texColor.rgb * smpColor.rgb, texColor.a);

   
}