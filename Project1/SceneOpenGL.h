#ifndef DEF_SCENEOPENGL
#define DEF_SCENEOPENGL


// Includes 
#include <GL/glew.h>
#include <SDL.h>
#include <iostream>
#include <string>
#include "Shader.h"
#include "Camera.h"
#include "Slider.h"
#include "ATPH.h"
#include "Axes.h"
#include "Input.h"


class SceneOpenGL
{
	public:
	//Constructeur et destructeur
	SceneOpenGL(std::string titreFenetre, int largeurFenetre, int hauteurFenetre);
	~SceneOpenGL();

	//prototypes
	bool initialiserFenetre();
	bool initGL();

	//methode principale qui va gérer l'affichage
	void affichage();



	private:
	//paramètres de la fenetre
	std::string m_titreFenetre;
	int m_largeurFenetre;
	int m_hauteurFenetre;

	SDL_Window* m_fenetre;
	SDL_GLContext m_contexteOpenGL;
	SDL_Event m_evenements;
	Input m_input;
};

#endif