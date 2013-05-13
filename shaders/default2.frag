//reset values Fragment Shader
#version 150
in vec3 vColor;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vPosition;
out vec4 outputF;
uniform sampler2D tex1;
vec4 texel;
void main(void)
{

   texel  = texture2D(tex1,vTexCoord);
   outputF = texel;
   // outputF = vec4(vColor.b,vColor.r,vColor.g,1.0);
   //outputF = clamp(texel + vec4(vColor.b,vColor.r,vColor.g,1.0),0.0,1.0) ;
   
}
