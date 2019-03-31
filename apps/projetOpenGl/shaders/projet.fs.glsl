#version 330


//lightning
//uniform vec3 uDirectionalLightDir;
//uniform vec3 uDirectionalLightIntensity;

//uniform vec3 uKd;
uniform sampler2D uBaseColor;
uniform sampler2D uEmissionColor;
uniform vec4 uBaseFactor;
uniform vec3 uEmissionFactor;
uniform int uBaseCoord;
uniform int uEmissionCoord;

in vec3 vViewSpacePosition; 
in vec3 vViewSpaceNormal;
in vec2 vTexCoords;
in vec2 vTexCoords1;

//out vec3 fColor;
layout(location = 0) out vec3 fPosition;
layout(location = 1) out vec3 fNormal;
layout(location = 2) out vec3 fAmbient;
layout(location = 3) out vec3 fDiffuse;

void main()
{
	//float dotAngle = dot(vViewSpaceNormal, uDirectionalLightDir);
	//float lenNormal = length(vViewSpaceNormal);
	//float lenDir = length(uDirectionalLightDir);
	//float angle = acos(dotAngle / sqrt(lenNormal * lenDir));

	vec3 baseColor;
	if(uBaseCoord == 0){
		baseColor = vec3(texture(uBaseColor, vTexCoords));
	}
	else if(uBaseCoord == 1){
		baseColor = vec3(texture(uBaseColor, vTexCoords1));
	}
	else{
		baseColor = vec3(uBaseFactor);
	}

	vec3 emissionColor;
	if(uEmissionCoord == 0){
		emissionColor = vec3(texture(uEmissionColor, vTexCoords));
	}
	else if(uEmissionCoord == 1){
		emissionColor = vec3(texture(uEmissionColor, vTexCoords1));
	}
	else{
		emissionColor = uEmissionFactor;
	}
	

	//fColor = vViewSpaceNormal;
	//fColor = emissionColor + baseColor * cos(angle) * uDirectionalLightIntensity;

	vec3 N = normalize(vViewSpaceNormal);

	fPosition = vViewSpacePosition;
	fNormal = N;
	fAmbient = emissionColor;
	fDiffuse = baseColor;
}