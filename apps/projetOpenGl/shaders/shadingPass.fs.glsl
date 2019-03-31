#version 330


//lightning
uniform vec3 uDirectionalLightDir_vs;
uniform vec3 uDirectionalLightIntensity;

uniform sampler2D uGPosition;
uniform sampler2D uGNormal;
uniform sampler2D uGAmbient;
uniform sampler2D uGDiffuse;

out vec3 fColor;

void main()
{
	vec3 fNormal = vec3(texelFetch(uGNormal, ivec2(gl_FragCoord.xy), 0)); // Correspond a vViewSpaceNormal
	vec3 emissionColor = vec3(texelFetch(uGAmbient, ivec2(gl_FragCoord.xy), 0));
	vec3 baseColor = vec3(texelFetch(uGDiffuse, ivec2(gl_FragCoord.xy), 0));

	float dotAngle = dot(fNormal, uDirectionalLightDir_vs);
	float lenNormal = length(fNormal);
	float lenDir = length(uDirectionalLightDir_vs);
	float angle = acos(dotAngle / sqrt(lenNormal * lenDir));

	fColor =  emissionColor + baseColor * cos(angle) * uDirectionalLightIntensity ;
}
