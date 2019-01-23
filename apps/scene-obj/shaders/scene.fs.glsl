#version 330


//lightning
uniform vec3 uDirectionalLightDir_vs;
uniform vec3 uDirectionalLightIntensity;
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


out vec3 fColor;

void main()
{
	vec3 wi = normalize(uDirectionalLightDir_vs);
	vec3 wo = normalize(-vViewSpacePosition);
	vec3 halfVector = (wo + wi)/2;
	vec3 N = normalize(vViewSpaceNormal);
	vec3 Li = uDirectionalLightIntensity;

	vec3 ka = uKa*vec3(texture(uKaSampler, vTexCoords));
	vec3 kd = uKd*vec3(texture(uKdSampler, vTexCoords));
	vec3 ks = uKs*vec3(texture(uKsSampler, vTexCoords));

	fColor =  Li * (kd * max(0.f, dot(wi,N)) + ks * pow( max(0.f, dot(halfVector,N)), uShininess)) + ka;
}