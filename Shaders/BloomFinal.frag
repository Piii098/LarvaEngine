#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform float exposure;
uniform bool uIsBloom;

void main()
{             
    if(uIsBloom){
        
        const float gamma = 1.0;
        vec2 correntedTexCoords = vec2(TexCoords.x, 1.0 - TexCoords.y);
        vec3 hdrColor = texture(scene, correntedTexCoords).rgb;      
        vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
        hdrColor += bloomColor; 
 
        vec3 result = vec3(1.0) - exp(-hdrColor * exposure);

        result = pow(result, vec3(1.0 / gamma));
        FragColor = vec4(result, 1.0);
    }else{
        
        vec4 texColor = texture(scene, TexCoords);
        FragColor = texColor;
    
    }
}  