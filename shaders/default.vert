varying vec3 norm;	
varying vec4 vertex;
void main(void)
{
  vertex = gl_ModelViewProjectionMatrix *  gl_Vertex;
  norm = gl_NormalMatrix * gl_Normal;
  gl_Position = vertex;



}
