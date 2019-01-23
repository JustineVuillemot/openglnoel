#version 330

in vec3 vViewSpacePosition;
in vec3 vViewSpaceNormal;
in vec2 vTexCoords;

uniform vec3 uLightDir_vs;
uniform vec3 uLightIntensity;

uniform float shininess;

uniform vec3 uKa;
uniform vec3 uKd;
uniform vec3 uKs;

uniform sampler2D uKaSampler;
uniform sampler2D uKdSampler;
uniform sampler2D uKsSampler;

out vec3 fFragColor;

vec3 blinnPhong() {
	vec3 wi = normalize(uLightDir_vs);
	vec3 Li = uLightIntensity;
	vec3 halfVector = (normalize(-vViewSpacePosition) + wi) / 2;
	vec3 N = vViewSpaceNormal;

	vec3 uKaTexture = vec3(texture(uKaSampler, vTexCoords));
	vec3 uKdTexture = vec3(texture(uKdSampler, vTexCoords));
	vec3 uKsTexture = vec3(texture(uKsSampler, vTexCoords));

	return Li * (uKd * uKdTexture * max(0.f, dot(wi, N)) + uKs * uKsTexture * pow( max(0.f, dot(halfVector, N)), shininess) ) + uKa * uKaTexture;
}

void main() {

	fFragColor = blinnPhong();
}