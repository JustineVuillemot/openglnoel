#version 330


//lightning
//uniform vec3 uDirectionalLightDir;
//uniform vec3 uDirectionalLightIntensity;
//uniform vec3 uPointLightPosition;
//uniform vec3 uPointLightIntensity;
uniform vec3 uKa;
uniform vec3 uKd;
uniform vec3 uKs;
uniform sampler2D uKaSampler;
uniform sampler2D uKdSampler;
uniform sampler2D uKsSampler;

in vec3 vViewSpacePosition; 
in vec3 vViewSpaceNormal;
in vec2 vTexCoords;


out vec3 fColor;

void main()
{
   	//float distToPointLight = length(uPointLightPosition - vViewSpacePosition);
	//vec3 dirToPointLight = (uPointLightPosition - vViewSpacePosition) / distToPointLight;
	//vec3 uKdFinal = uKd; 
	//* vec3(texture(uKdSampler, vTexCoords));

	//fColor = uKdFinal * (uDirectionalLightIntensity * max(0.0, dot(vViewSpaceNormal, uDirectionalLightDir)) 
	//+ uPointLightIntensity * max(0.0, dot(vViewSpaceNormal, dirToPointLight)) / (distToPointLight * distToPointLight));

	fColor = uKd*vec3(texture(uKdSampler, vTexCoords)) + uKa*vec3(texture(uKaSampler, vTexCoords)) + uKs*vec3(texture(uKsSampler, vTexCoords));
}