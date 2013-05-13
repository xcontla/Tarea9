#version 150
in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;
in vec3 in_Color;

uniform mat4 viewMatrix, projMatrix;
uniform sampler2D tex1;

out vec3 vPosition;
out vec3 vNormal;
out vec2 vTexCoord;
out vec3 vColor;

void main(void)
{
  vec4 dv = texture2D(tex1,in_TexCoord);
  float df = 0.30*dv.x + 0.59*dv.y + 0.11*dv.z;
  vec3 myNormal = vec3( viewMatrix * vec4(in_Normal, 0.0));
  vNormal = normalize(myNormal * in_Normal);
  vec4 newVertexPos =  vec4(in_Position,1.0) + vec4(vNormal*df*10.0,0.0);
  
  vPosition = in_Position;
  vTexCoord = in_TexCoord;
  vColor = in_Color;


  gl_Position = projMatrix * viewMatrix * newVertexPos;//vec4(in_Position,1.0);
  
}
