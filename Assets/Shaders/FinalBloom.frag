#version 330 core
out vec4 FragColor;
  
in vec2 fragTexCoord;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform float exposure;
uniform bool uIsBloom;

void main()
{             

    if(uIsBloom){
        
        const float gamma = 1.1;
        vec3 hdrColor = texture(scene, fragTexCoord).rgb;      
        vec3 bloomColor = texture(bloomBlur, fragTexCoord).rgb;
        hdrColor += bloomColor; 
 
        vec3 result = vec3(1.0) - exp(-hdrColor * exposure);

        result = pow(result, vec3(1.0 / gamma));
        FragColor = vec4(result, 1.0);
    }else{
        
        FragColor = texture(scene, fragTexCoord);
    
    }
}  