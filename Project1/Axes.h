#pragma once
// Includes
#include "Shader.h"

using namespace glm;
using namespace std;


class Axes
{
public:
	Axes(std::string const vertexShader, std::string const fragmentShader, float taille);
	~Axes();

	void charger();
	void afficher(glm::mat4 &projection, glm::mat4 &modelview);

private:
	Shader m_shader;
	//vertices pour les axes et leurs couleurs
	float m_axis_vertices[15] = { 0.0, 0.0, 0.0,   0.0, 0.0, 0.0,   0.0, 0.0, 0.0,   0.0, 0.0, 0.0,   0.0, 0.0, 0.0 };
	float m_axis_couleurs[15] = { 1.0, 0.0, 0.0,   1.0, 1.0, 1.0,   0.0, 1.0, 0.0, 	1.0, 1.0, 1.0,   0.0, 0.0, 1.0 };

	//tailles de vertices pour reserver de la mémoire dans le GPU
	int m_axis_verticesBytes;
	int m_axis_couleursBytes;

	GLuint m_vboID;
	GLuint m_vaoID;
};

