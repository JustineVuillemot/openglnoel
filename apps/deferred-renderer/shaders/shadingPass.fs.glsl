#version 330

uniform vec3 uLightDir_vs;
uniform vec3 uLightIntensity;

//GBuffer reading
uniform sampler2D uGPosition;
uniform sampler2D uGNormal;
uniform sampler2D uGAmbient;
uniform sampler2D uGDiffuse;
uniform sampler2D uGlossyShininess;

out vec3 fFragColor;

void main() {
	
	//position and normal reload
	vec3 position = vec3(texelFetch(uGPosition, ivec2(gl_FragCoord.xy), 0));
	vec3 normal = vec3(texelFetch(uGNormal, ivec2(gl_FragCoord.xy), 0));

	vec3 wi = normalize(uLightDir_vs);
	vec3 Li = uLightIntensity;
	vec3 halfVector = (normalize(-position) + wi) / 2;
	vec3 N = normal;

	//textures and materials reload
	vec3 ambientText = vec3(texelFetch(uGAmbient, ivec2(gl_FragCoord.xy), 0));
	vec3 diffuseText = vec3(texelFetch(uGDiffuse, ivec2(gl_FragCoord.xy), 0));
	vec3 glossyShininess = vec3(texelFetch(uGlossyShininess, ivec2(gl_FragCoord.xy), 0));

	float shininess = texelFetch(uGlossyShininess, ivec2(gl_FragCoord.xy), 0).w;

	fFragColor = Li * (diffuseText * max(0.f, dot(wi, N)) + glossyShininess * pow( max(0.f, dot(halfVector, N)), shininess) ) + ambientText;
}