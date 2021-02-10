#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;
layout(location=2) in vec2 vTexCoords;

out vec3 fNormal;
out vec4 fPosEye;
out vec2 fTexCoords;
out vec4 fragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform	mat3 normalMatrix;
uniform mat4 lightSpaceTrMatrix;

out vec4 pozitiaLuminii;
out vec4 dirLuminii;
out vec4[21] felinare;
void main() 
{
	fragPosLightSpace = lightSpaceTrMatrix * model * vec4(vPosition, 1.0f);

	//compute eye space coordinates
	fPosEye = view * model * vec4(vPosition, 1.0f);
	fNormal = normalize(normalMatrix * vNormal);
	fTexCoords = vTexCoords;
	gl_Position = projection * view * model * vec4(vPosition, 1.0f);

	pozitiaLuminii=vec4(10.4763f,11.8182f,3.422,1.0f);
	pozitiaLuminii=view*model*pozitiaLuminii;
	
	felinare[0] = vec4(236.67,4.09,-67.2871,1);
	felinare[1] = vec4(256.1,4.09,-67.25,1);
	felinare[2] = vec4(280.24,4.09,-78.14,1);
	felinare[3] = vec4(272.63,4.09,-102.79,1);
	felinare[4] = vec4(256.68,4.09,-138.29,1);
	felinare[5] = vec4(222.95,4.09,-129.29,1);
	felinare[6] = vec4(266.1,4.09,-67.24,1);
	felinare[7] = vec4(205.17,4.09,-155.12,1);
	felinare[8] = vec4(291.5,4.09,-208.62,1);
	felinare[9] = vec4(302.14,4.09,-187.23,1);
	felinare[10] = vec4(348.62,4.09,-191.65,1);
	felinare[11] = vec4(348.62,4.09,-160.52,1);
	felinare[12] = vec4(390.99,4.09,-191.65,1);
	felinare[13] = vec4(439.81,4.09,-210.04,1);
	felinare[14] = vec4(464.99,4.09,-187.79,1);
	felinare[15] = vec4(439.81,4.09,-176.93,1);
	felinare[16] = vec4(449.25,4.09,-138.33,1);
	felinare[17] = vec4(449.00,4.09,-138.29,1);
	felinare[18] = vec4(464.99,4.09,-124.22,1);
	felinare[19] = vec4(411.99,4.09,-122.61,1);
	felinare[20] = vec4(358.21,4.09,-122.29,1);

	
	
	for(int i=0;i<21;i++)
		{
			felinare[i] = view*model*felinare[i];
		}
       
	dirLuminii = pozitiaLuminii;
}