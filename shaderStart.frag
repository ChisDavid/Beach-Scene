#version 410 core

in vec3 fNormal;
in vec4 fPosEye;
in vec2 fTexCoords;
in vec4 fragPosLightSpace;


out vec4 fColor;


//lighting
uniform	vec3 lightDir;
uniform	vec3 lightColor;

uniform float brightness;
uniform float fogIntensity;

//texture
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D shadowMap;

vec3 ambient;
float ambientStrength = 0.2f;
vec3 diffuse;
vec3 specular;
float specularStrength = 0.5f;
float shininess = 32.0f;


vec3 a;
vec3 d;
vec3 s;

vec3[21] felinar_A;
vec3[21] felinar_D;
vec3[21] felinar_S;


in vec4 pozitiaLuminii;
in vec4 dirLuminii;
in vec4[21] felinare;
float computeShadow()
{

	// Check whether current frag pos is in shadow
	float bias = 0.005f;
	

	vec3 normalizedCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	normalizedCoords = normalizedCoords * 0.5 + 0.5;

	if (normalizedCoords.z > 1.0f)
		return 0.0f;


	float closestDepth = texture(shadowMap, normalizedCoords.xy).r;

	// Get depth of current fragment from light's perspective
	float currentDepth = normalizedCoords.z;

	currentDepth = currentDepth - bias;

	// Check whether current frag pos is in shadow
	float shadow = currentDepth > closestDepth ? 1.0f : 0.0f;

	return shadow;
}
//modulate with shadow

float spec;
vec3 eyeNormal;
void computeLightComponents()
{		
	vec3 cameraPosEye = vec3(0.0f);//in eye coordinates, the viewer is situated at the origin
	
	//transform normal
	vec3 normalEye = normalize(fNormal);	
	eyeNormal=normalEye;
	//compute light direction
	vec3 lightDirN = normalize(lightDir);
	
	//compute view direction 
	vec3 viewDirN = normalize(cameraPosEye - fPosEye.xyz);
	

	
	//compute ambient light
	ambient = ambientStrength * lightColor;
	
	//compute diffuse light
	diffuse = max(dot(normalEye, lightDirN), 0.0f) * lightColor;
	
	//compute specular light
	vec3 reflection = reflect(-lightDirN, normalEye);
	float specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
    spec=specCoeff;
	specular = specularStrength * specCoeff * lightColor;
}




float computeFog(vec3 diffuse)
{
 float fogDensity = 0.1f;
 float fragmentDistance = length(fPosEye);
 
 float fogFactor = exp(-pow(fragmentDistance * fogDensity, 2))*pow (5,fogIntensity);
 return clamp(fogFactor, 0.3f, 1.0f);
}


float constant = 1.0f;
float linear = 0.014f;
float quadratic =0.007f;

float constant_felinar = 1.0f;
float linear_felinar = 0.7f;
float quadratic_felinar =1.8f;


float compute_attenuare(){
	float dist = length(pozitiaLuminii- fPosEye);	
	return 1.0f/(constant+linear*dist+ quadratic*dist*dist);
}
float rez[21];
void compute_attenuare_felinar(){
		for (int i=0;i<21;i++)
		{
			float dist = length (felinare[i] -fPosEye);
			rez[i]= 1.0f/(constant_felinar+linear_felinar*dist+ quadratic_felinar*dist*dist);
		}
	}



void main() 
{
	float intensity = compute_attenuare();

		
	a=intensity*ambientStrength*lightColor;
	d=intensity*max(dot(eyeNormal,normalize(pozitiaLuminii.xyz-fPosEye.xyz)),0.0f)* lightColor;
	s=intensity*specularStrength* spec *lightColor;
	
	
	
	float shadow = computeShadow();
	computeLightComponents();
	vec3 baseColor = vec3(1.0f, 1.0f, 1.0f);//white

compute_attenuare_felinar();
	for(int i=0;i<21;i++)
	{
		felinar_A[i] =rez[i]*ambientStrength*lightColor;
		felinar_D[i] = rez[i]*max(dot(eyeNormal,normalize(felinare[i].xyz-fPosEye.xyz)),0.0f)* lightColor;
		felinar_S[i] = rez[i]*specularStrength* spec *lightColor;
	}
	
	ambient *= texture(diffuseTexture, fTexCoords).rgb;
	diffuse *= texture(diffuseTexture, fTexCoords).rgb;
	specular *= texture(specularTexture, fTexCoords).rgb;
	
	ambient *=brightness;
	diffuse*=brightness;
	specular*=brightness;
	
		
	if (brightness<0.1)
	{
	a*=baseColor;
	d*=baseColor;
	s*=baseColor;
	ambient+=a;
	specular+=s;
	diffuse+=d;
	for(int i=0 ; i< 21 ;i++)
		{
			ambient +=felinar_A[i]*vec3(1,0.840,0);
			diffuse+=felinar_D[i]*vec3(1,0.840,0);
			specular+=felinar_S[i]*vec3(1,0.840,0);
		}
		
	}
	
	vec3 color = min((ambient + (1.0f - shadow)*diffuse) + (1.0f - shadow)*specular, 1.0f);

// FOG
	if (fogIntensity!=0)
	{
	  float fogFactor = computeFog(diffuse);
      vec3 fogColor = vec3(0.5f, 0.5f, 0.5f)*brightness;
      color = mix(fogColor, color, fogFactor);
	}
   
 
    fColor = vec4(color, 1.0f);
}