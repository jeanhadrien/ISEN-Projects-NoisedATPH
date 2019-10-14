#include "SceneOpenGL.h"


int main(int argc, char **argv)
{
	// Cr�ation de la s�ne (fen�tre)
	SceneOpenGL scene("Courbe ATPH", 800, 600);

	// Initialisation de la sc�ne (fen�tre)
	if (scene.initialiserFenetre() == false)
		return -1;
	if (scene.initGL() == false)
		return -1;


	// Boucle Principale qui g�re l'affichage
	scene.affichage();


	// Fin du programme
	return 0;
}