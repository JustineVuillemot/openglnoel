#version 330

in vec3 vViewSpacePosition;
in vec3 vViewSpaceNormal;
in vec2 vTexCoords;

uniform float shininess;

uniform vec3 uKa;
uniform vec3 uKd;
uniform vec3 uKs;

uniform sampler2D uKaSampler;
uniform sampler2D uKdSampler;
uniform sampler2D uKsSampler;

layout(location = 0) out vec3 fPosition;
layout(location = 1) out vec3 fNormal;
layout(location = 2) out vec3 fAmbient;
layout(location = 3) out vec3 fDiffuse;
layout(location = 4) out vec4 fGlossyShininess;

void main() {

	fNormal = normalize(vViewSpaceNormal);
	fPosition = vViewSpacePosition;

	fAmbient = uKa * vec3(texture(uKaSampler, vTexCoords));
	fDiffuse = uKd * vec3(texture(uKdSampler, vTexCoords));
	fGlossyShininess = vec4(uKs * vec3(texture(uKsSampler, vTexCoords)), shininess);
}