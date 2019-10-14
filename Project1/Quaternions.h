#pragma once
// Includes 
#include <glm.hpp>

class Quaternions
{
public:
	Quaternions();
	~Quaternions();

	//méthodes pour effectuer des calculs sur des quaternions
	static glm::vec4 quatmultiply(glm::vec4 vecteur1, glm::vec4 vecteur2);
	static glm::vec4 quatConj(glm::vec4 vecteur);
};

