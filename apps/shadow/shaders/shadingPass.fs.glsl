#version 330


//lightning
uniform vec3 uDirectionalLightDir_vs;
uniform vec3 uDirectionalLightIntensity;

uniform sampler2D uGPosition;
uniform sampler2D uGNormal;
uniform sampler2D uGAmbient;
uniform sampler2D uGDiffuse;
uniform sampler2D uGlossyShininess;

out vec3 fColor;

void main()
{
	vec3 fPosition = vec3(texelFetch(uGPosition, ivec2(gl_FragCoord.xy), 0)); // Correspond a vViewSpacePosition dans le forward renderer

	vec3 wi = normalize(uDirectionalLightDir_vs);
	vec3 wo = normalize(-fPosition);
	vec3 halfVector = (wo + wi)/2;
	vec3 N = vec3(texelFetch(uGNormal, ivec2(gl_FragCoord.xy), 0));
	vec3 Li = uDirectionalLightIntensity;

	vec3 ka = vec3(texelFetch(uGAmbient, ivec2(gl_FragCoord.xy), 0));
	vec3 kd = vec3(texelFetch(uGDiffuse, ivec2(gl_FragCoord.xy), 0));
	vec3 ks = vec3(texelFetch(uGlossyShininess, ivec2(gl_FragCoord.xy), 0));

	float shininess = texelFetch(uGlossyShininess, ivec2(gl_FragCoord.xy), 0).w;

	fColor =  Li * (kd * max(0.f, dot(wi,N)) + ks * pow( max(0.f, dot(halfVector,N)), shininess)) + ka;
}
