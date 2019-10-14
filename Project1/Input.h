#ifndef DEF_INPUT
#define DEF_INPUT

// Includes
#include <SDL.h>
#include <iostream>
#include <string>

// Classe

class Input
{
public:

	Input();
	~Input();

	void updateEvenements();
	bool terminer() const;
	bool getTouche(const SDL_Scancode touche) const;
	bool getBoutonSouris(const Uint8 bouton) const;
	bool mouvementSouris() const;

	// Getters
	int getX() const;
	int getY() const;
	int getXRel() const;
	int getYRel() const;

	void afficherPointeur(bool reponse) const;
	void capturerPointeur(bool reponse) const;

	//pour que l'utilisateur pusisse rentrer un nombre (pour choisir le point initial, final et les dérivées)
	static float saisieUtilisateur(float defaultValue);


private:

	SDL_Event m_evenements;
	bool m_touches[SDL_NUM_SCANCODES];
	bool m_boutonsSouris[8];

	//position de la souris
	int m_x;
	int m_y;
	//déplacement de la souris
	int m_xRel;
	int m_yRel;

	bool m_terminer;	
};

#endif