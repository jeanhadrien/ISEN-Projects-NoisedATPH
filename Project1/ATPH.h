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

	//méthode pour afficher la courbe avec les points de conntrôle
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

	//méthode pour que l'utilisateur puisse saisir les coordonées du point initial, final et les dérivées
	static vec3 *initialisationATPH();

	//pour afficher sur la console les coordonnés des points de contrôle et les paramètres (alpha, phi0 et phi2)
	void details();

private :
	//méthodes pour calculer les points de contôle et l'ensemble de la courbe
	void Quaternions_A0A1A2_ATPH();
	void controlPointsATPH();
	void drawBasesSegNew();

	//nombre de points pour la courbe
	int precision = 100;

	//coordonnées des points de contrôle
	vec3 di;
	vec3 df;
	vec3 p0;
	vec3 p1;
	vec3 p2;
	vec3 p3;
	vec3 p4;
	vec3 p5;

	//paramètres de la courbe
	double phi0;
	double phi2;
	double alpha;

	//tableau de coefficients
	vector<vector<double>> biBasic;

	Shader m_shader;

	//vertices des points de contôle et de leur couleur (et taille des vertices pour les allocations mémoire du GPU)
	float m_vertices[18];
	float m_couleurs[18] = { 1.0,1.0,1.0, 1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0 };
	int m_verticesBytes;
	int m_couleursBytes;
	//vertices des points de la courbe et de leur couleur (et taille des vertices pour les allocations mémoire du GPU)
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

