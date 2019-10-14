#ifndef DEF_CAMERA
#define DEF_CAMERA

#include <glm.hpp>
#include <gtx/transform.hpp>
#include "Input.h"
#include <math.h>

using namespace glm;


class Camera
{
public:

	Camera();	
	Camera(glm::vec3 position, glm::vec3 pointCible, glm::vec3 axeVertical, float sensibilite, float vitesse);
	~Camera();

	
	void deplacer(Input const &input);
	void lookAt(glm::mat4 &modelview);
	void setPointcible(glm::vec3 pointCible);
	void setPosition(glm::vec3 position);

	// Getters et Setters
	float getSensibilite() const;
	float getVitesse() const;
	void setSensibilite(float sensibilite);
	void setVitesse(float vitesse);
	static void setAngle(int variation, float *angle);

private:
	//angles de la caméra
	double m_phi;
	double m_theta;

	glm::vec3 m_orientation;

	glm::vec3 m_axeVertical;
	glm::vec3 m_deplacementLateral;

	glm::vec3 m_position;
	glm::vec3 m_pointCible;

	float m_sensibilite;
	float m_vitesse;
};

#endif 