#include "Quaternions.h"



Quaternions::Quaternions()
{
}


Quaternions::~Quaternions()
{
}

/// <summary>
/// effectue la multiplication de deux vecteurs.
/// </summary>
/// <param name="vecteur1">The vecteur1.</param>
/// <param name="vecteur2">The vecteur2.</param>
/// <returns></returns>
glm::vec4 Quaternions::quatmultiply(glm::vec4 vecteur1, glm::vec4 vecteur2) {
	//on crée un "quernions" à renvoyer
	glm::vec4 result;
	//on fait les calcules sur chaques composantes
	result.x = vecteur2.x*vecteur1.x - vecteur2.y*vecteur1.y - vecteur2.z*vecteur1.z - vecteur2.w*vecteur1.w;
	result.y = vecteur2.x*vecteur1.y + vecteur2.y*vecteur1.x - vecteur2.z*vecteur1.w + vecteur2.w*vecteur1.z;
	result.z = vecteur2.x*vecteur1.z + vecteur2.y*vecteur1.w + vecteur2.z*vecteur1.x - vecteur2.w*vecteur1.y;
	result.w = vecteur2.x*vecteur1.w - vecteur2.y*vecteur1.z + vecteur2.z*vecteur1.y + vecteur2.w*vecteur1.x;
	return result;
}

/// <summary>
/// Calcule le conjugué d'un quaternion.
/// </summary>
/// <param name="vecteur">The vecteur.</param>
/// <returns></returns>
glm::vec4 Quaternions::quatConj(glm::vec4 vecteur) {
	return glm::vec4(vecteur.x, -vecteur.y, -vecteur.z, -vecteur.w);
}
