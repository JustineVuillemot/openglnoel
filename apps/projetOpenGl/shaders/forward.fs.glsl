#version 330


//lightning
//uniform vec3 uDirectionalLightDir;
//uniform vec3 uDirectionalLightIntensity;
//uniform vec3 uPointLightPosition;
//uniform vec3 uPointLightIntensity;
//uniform vec3 uKd;
//uniform sampler2D uKdSampler;

in vec3 vViewSpacePosition; 
in vec3 vViewSpaceNormal;
in vec2 vTexCoords;


out vec3 fColor;

void main()
{
   	//float distToPointLight = length(uPointLightPosition - vViewSpacePosition);
	//vec3 dirToPointLight = (uPointLightPosition - vViewSpacePosition) / distToPointLight;
	//vec3 uKdFinal = uKd * vec3(texture(uKdSampler, vTexCoords));

	//fColor = uKdFinal * (uDirectionalLightIntensity * max(0.0, dot(vViewSpaceNormal, uDirectionalLightDir)) + uPointLightIntensity * max(0.0, dot(vViewSpaceNormal, dirToPointLight)) / (distToPointLight * distToPointLight));
	fColor = vViewSpaceNormal;
}