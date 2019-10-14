// Includes 
#include <GL/glew.h>
#include <glm.hpp>
#include "Shader.h"

#pragma once
class Slider
{
public:

	//constructeur et destructeur
	Slider(std::string const vertexShader, std::string const fragmentShader, float x, float y);
	~Slider();
	
	void afficher(glm::mat4 &projection, glm::mat4 &modelview);
	float* getCurseur();
	void setCurseur(int x,int w);
	float actionSlider(float Max, float Min, int w, int x);

	//methode pour récupérer les coordonnées du point en bas à gauche du curseur
	float getX();
	float getY();

	//méthode pour déterminer si la souris est dans le slider
	bool isIn(int x, int y, int h, int w);

	//methodes pour l'affichage
	void charger();
	void updateVBO(void *donnees, int tailleBytes, int decalage);


private:

	Shader m_shader;

	//vertices pour afficher le contour de slider
	float m_verticesB[8];
	float m_couleursB[12] = { 0.85f ,0.84f,0.83f,  0.85f,0.84f,0.83f,  0.85f,0.84f,0.83f,  0.85f, 0.84F,0.83f};
	int m_verticesBBytes;
	int m_couleursBBytes;

	//vertices pour afficher le curseur du slider
	float m_verticesC[8];
	float m_couleursC[12] = { 0.80f,0.f,0.f,  0.80f,0.f,0.f,  0.80f,0.f,0.f,  0.80f,0.f,0.f};
	int m_verticesCBytes;
	int m_couleursCBytes;
	
	//position du coin en bas à gauche du slider
	float x;
	float y;

	//taille du slider
	float h = float(0.1);
	float w1 = float(0.3);
	float w2 = float(0.02);

	GLuint m_vboID;
	GLuint m_vaoID;
};

