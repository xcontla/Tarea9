
varying vec3 norm;
vec3 lightPos;
vec3 lightDir;
vec3 eyeVec;
varying vec4 vertex;
void main(void)
{
  vec4 ambient = vec4(0.2);
  vec4 especular = vec4(1.0);
  vec4 diffuse = vec4(0.6,0.7,0.3,1.0);

  lightPos = vec3(40.0,40.0,-40.0);
  lightDir = lightPos - vertex.xyz;
  eyeVec = vertex.xyz;

  vec3 N = normalize(norm);
  vec3 L = normalize(lightDir);
  float lambertTerm = dot(N,L);
  vec4 final_color = (ambient + vec4(0.5,0.3,0.3,1.0))*0.3;
  if(lambertTerm > 0.0)
    {
      final_color += diffuse * vec4(1.0,0.0,0.0,1.0) * lambertTerm;	
		
      vec3 E = normalize(eyeVec);
      vec3 R = reflect(-L, N);
      //vec3 H = normalize(E + L);
      float ke =  2.0 * dot(N,L);
      vec3 H =normalize((ke*N)-L);
      float specular = pow( max(dot(E,H), 0.0), 20.0);

      final_color += especular * specular;
      gl_FragColor = final_color;
    }
  else
    gl_FragColor = final_color;
}
