#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{ 
    //FragColor = texture(screenTexture, TexCoords);
    //float gamma = 2.2;
   // FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));

   const float gamma = 2.0;
   const float exposure = 1.4;
   vec3 hdrColor = texture(screenTexture, TexCoords).rgb;
  
    // exposure tone mapping
   vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    // gamma correction 
   mapped = pow(mapped, vec3(1.0 / gamma));
  
   FragColor = vec4(mapped, 1.0);
}