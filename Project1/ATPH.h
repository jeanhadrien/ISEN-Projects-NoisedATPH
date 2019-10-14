#pragma once

// Includes
#include <iostream>
#include <glm.hpp>
#include <math.h>
#include <vector>
#include <iomanip>
#include "CalculTools.h"
#include "Quaternions.h"
#include "Shader.h"
#include "input.h"

using namespace glm;
using namespace std;


class ATPH
{
public:
	ATPH(vec3 p0, vec3 pf, vec3 di, vec3 df, double phi0, double phi2, double alpha, std::string const vertexShader, std::string const fragmentShader);
	~ATPH();

	//m�thode pour afficher la courbe avec les points de conntr�le
	void afficher(glm::mat4 &projection, glm::mat4 &modelview);

	//Pour les vertex object
	void charger(); 
	void update();
	void updateVBO(void *donnees, int tailleBytes, int decalage);

	//getters, setter des angles et leurs valeurs limites
	void setAlpha(double a);
	double getAlpha();
	float alphaMax = float(2.0 * M_PI / 3.0);
	float alphaMin = 0.0;

	void setPhi0(double a);
	double getPhi0();
	float Phi0Max = float(2.0 * M_PI);
	float Phi0Min = 0.0;

	void setPhi2(double a);
	double getPhi2();
	float Phi2Max = float(2.0 * M_PI);
	float Phi2Min = 0.0;

	//getters et setter des points qui peuvent bouger avec la souris
	void setP0(float x, float y, float z);
	vec3 getP0();

	void setP5(float x, float y, float z);
	vec3 getP5();

	//m�thode pour que l'utilisateur puisse saisir les coordon�es du point initial, final et les d�riv�es
	static vec3 *initialisationATPH();

	//pour afficher sur la console les coordonn�s des points de contr�le et les param�tres (alpha, phi0 et phi2)
	void details();

private :
	//m�thodes pour calculer les points de cont�le et l'ensemble de la courbe
	void Quaternions_A0A1A2_ATPH();
	void controlPointsATPH();
	void drawBasesSegNew();

	//nombre de points pour la courbe
	int precision = 100;

	//coordonn�es des points de contr�le
	vec3 di;
	vec3 df;
	vec3 p0;
	vec3 p1;
	vec3 p2;
	vec3 p3;
	vec3 p4;
	vec3 p5;

	//param�tres de la courbe
	double phi0;
	double phi2;
	double alpha;

	//tableau de coefficients
	vector<vector<double>> biBasic;

	Shader m_shader;

	//vertices des points de cont�le et de leur couleur (et taille des vertices pour les allocations m�moire du GPU)
	float m_vertices[18];
	float m_couleurs[18] = { 1.0,1.0,1.0, 1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0 };
	int m_verticesBytes;
	int m_couleursBytes;
	//vertices des points de la courbe et de leur couleur (et taille des vertices pour les allocations m�moire du GPU)
	vector<float> m_ATPH;
	vector<float> m_ATPH_couleurs;
	int m_ATPHBytes;
	int m_ATPH_couleursBytes;

	GLuint m_vboID;
	GLuint m_vaoID;

	//variables locales
	double s1;
	double c1;
	double s2;
	double c2;
	double n0;
	double n2;

	vec4 A0_ATPH;
	vec4 A1_ATPH;
	vec4 A2_ATPH;
};

