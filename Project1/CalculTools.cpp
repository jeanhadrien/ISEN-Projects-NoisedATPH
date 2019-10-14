#include "CalculTools.h"


CalculTools::CalculTools()
{
}


CalculTools::~CalculTools()
{
}



/// <summary>
/// Norms the specified vector.
/// </summary>
/// <param name="vecteur">The vector.</param>
/// <returns></returns>
float CalculTools::norm(glm::vec3 vecteur) {
	float sum = vecteur.x * vecteur.x + vecteur.y * vecteur.y + vecteur.z * vecteur.z;
	return sqrt(sum);

}



/// <summary>
/// calcul du produit scalaire de 2 vecteurs 3
/// </summary>
/// <param name="vecteur1">The vecteur1.</param>
/// <param name="vecteur2">The vecteur2.</param>
/// <returns></returns>
double CalculTools::scalairProduct(glm::vec3 vecteur1, glm::vec3 vecteur2) {
	return (vecteur1.x * vecteur2.x + vecteur1.y * vecteur2.y + vecteur1.z * vecteur2.z);
}



/// <summary>
/// calcul du poduit vectoriel de 2 vecteurs 3
/// </summary>
/// <param name="vecteur1">The vecteur1.</param>
/// <param name="vecteur2">The vecteur2.</param>
/// <returns></returns>
glm::vec3 CalculTools::crossProduct(glm::vec3 vecteur1, glm::vec3 vecteur2) {
	glm::vec3 resultat;
	resultat.x = vecteur1.y*vecteur2.z - vecteur1.z*vecteur2.y;
	resultat.y = vecteur1.z*vecteur2.x - vecteur1.x*vecteur2.z;
	resultat.z = vecteur1.x*vecteur2.y - vecteur1.y*vecteur2.x;
	return resultat;
}
