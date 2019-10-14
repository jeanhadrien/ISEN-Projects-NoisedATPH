#include "SceneOpenGL.h"


int main(int argc, char **argv)
{
	// Création de la sène (fenêtre)
	SceneOpenGL scene("Courbe ATPH", 800, 600);

	// Initialisation de la scène (fenêtre)
	if (scene.initialiserFenetre() == false)
		return -1;
	if (scene.initGL() == false)
		return -1;


	// Boucle Principale qui gére l'affichage
	scene.affichage();


	// Fin du programme
	return 0;
}