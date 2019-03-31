#version 330 core

// Attributs de sommet
layout(location = 0) in vec3 aPosition; // Position du sommet
layout(location = 1) in vec3 aNormal; // Normale du sommet
layout(location = 2) in vec2 aTexCoords; // Coordonnées de texture du sommet
layout(location = 3) in vec2 aTexCoords1; // Coordonnées de texture du sommet

// Matrices de transformations reçues en uniform
uniform mat4 uModelViewProjMatrix;
uniform mat4 uModelViewMatrix;
uniform mat4 uNormalMatrix;

// Sorties du shader
out vec3 vViewSpacePosition; // Position du sommet transformé dans l'espace View
out vec3 vViewSpaceNormal; // Normale du sommet transformé dans l'espace View
out vec2 vTexCoords; // Coordonnées de texture du sommet 0
out vec2 vTexCoords1; // Coordonnées de texture du sommet 1

void main() {
    // Passage en coordonnées homogènes
    vec4 vertexPosition = vec4(aPosition, 1);
    vec4 vertexNormal = vec4(aNormal, 0);

    // Calcul des valeurs de sortie
    vViewSpacePosition = vec3(uModelViewMatrix * vertexPosition);
    vViewSpaceNormal = vec3(uNormalMatrix * vertexNormal);
    vTexCoords = aTexCoords;
	vTexCoords1 = aTexCoords1;

    // Calcul de la position projetée
    gl_Position = uModelViewProjMatrix * vertexPosition;

}