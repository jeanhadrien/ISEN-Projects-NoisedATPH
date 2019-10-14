#pragma once
// Includes
#include <glm.hpp>

class CalculTools
{
public:
	CalculTools();
	~CalculTools();

	//méthodes pour faire des calculs sur des vecteur 3
	static float norm(glm::vec3 vecteur);
	static double scalairProduct(glm::vec3 vecteur1, glm::vec3 vecteur2);
	static glm::vec3 crossProduct(glm::vec3 vecteur1, glm::vec3 vecteur2);
};

