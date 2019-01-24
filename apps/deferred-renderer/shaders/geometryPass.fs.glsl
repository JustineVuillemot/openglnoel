#version 330


//lightning
//uniform vec3 uDirectionalLightDir_vs;
//uniform vec3 uDirectionalLightIntensity;
uniform float uShininess;

uniform vec3 uKa;
uniform vec3 uKd;
uniform vec3 uKs;

uniform sampler2D uKaSampler;
uniform sampler2D uKdSampler;
uniform sampler2D uKsSampler;

in vec3 vViewSpacePosition; 
in vec3 vViewSpaceNormal;
in vec2 vTexCoords;


layout(location = 0) out vec3 fPosition;
layout(location = 1) out vec3 fNormal;
layout(location = 2) out vec3 fAmbient;
layout(location = 3) out vec3 fDiffuse;
layout(location = 4) out vec4 fGlossyShininess;


void main()
{
	vec3 N = normalize(vViewSpaceNormal);

	vec3 ka = uKa*vec3(texture(uKaSampler, vTexCoords));
	vec3 kd = uKd*vec3(texture(uKdSampler, vTexCoords));
	vec3 ks = uKs*vec3(texture(uKsSampler, vTexCoords));

	fPosition = vViewSpacePosition;
	fNormal = N;
	fAmbient = ka;
	fDiffuse = kd;
	fGlossyShininess = vec4(ks.x, ks.y, ks.z, uShininess);

}