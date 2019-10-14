#include "Camera.h"



Camera::Camera() : m_phi(0.0), m_theta(0.0), m_orientation(), m_axeVertical(0, 0, 1), m_deplacementLateral(), m_position(), m_pointCible(), m_sensibilite(0.0), m_vitesse(0.0)
{
}


Camera::Camera(glm::vec3 position, glm::vec3 pointCible, glm::vec3 axeVertical, float sensibilite, float vitesse) : m_phi(-35.26), m_theta(-135), m_orientation(), m_axeVertical(axeVertical),
m_deplacementLateral(), m_position(position), m_pointCible(pointCible), m_sensibilite(sensibilite), m_vitesse(vitesse)
{
	// Actualisation du point ciblé
	setPointcible(pointCible);
}


Camera::~Camera()
{
}


/// <summary>
/// Permet de déplacer la caméra
/// </summary>
/// <param name="input">The input.</param>
void Camera::deplacer(Input const &input)
{
	// Avancée de la caméra
	if (input.getTouche(SDL_SCANCODE_UP))
	{
		m_position = m_position + m_orientation * m_vitesse;
		m_pointCible = m_position + m_orientation;
	}


	// Recul de la caméra
	if (input.getTouche(SDL_SCANCODE_DOWN))
	{
		m_position = m_position - m_orientation * m_vitesse;
		m_pointCible = m_position + m_orientation;
	}
}




/// <summary>
/// place la caméra
/// </summary>
/// <param name="modelview">The modelview matrix.</param>
void Camera::lookAt(glm::mat4 &modelview)
{
	// Actualisation de la vue dans la matrice
	modelview = glm::lookAt(m_position, m_pointCible, m_axeVertical);
}




/// <summary>
/// Set le vecteur point cible.
/// </summary>
/// <param name="pointCible">point cible.</param>
void Camera::setPointcible(glm::vec3 pointCible)
{
	// Calcul du vecteur orientation
	m_orientation = m_pointCible - m_position;
	m_orientation = normalize(m_orientation);


	// Si l'axe vertical est l'axe X
	if (m_axeVertical.x == 1.0)
	{
		// Calcul des angles
		m_phi = asin(m_orientation.x);
		m_theta = acos(m_orientation.y / cos(m_phi));

		if (m_orientation.y < 0)
			m_theta *= -1;
	}

	// Si c'est l'axe Y
	else if (m_axeVertical.y == 1.0)
	{
		// Calcul des angles

		m_phi = asin(m_orientation.y);
		m_theta = acos(m_orientation.z / cos(m_phi));

		if (m_orientation.z < 0)
			m_theta *= -1;
	}

	// Sinon c'est l'axe Z
	else
	{
		// Calcul des angles
		m_phi = asin(m_orientation.x);
		m_theta = acos(m_orientation.z / cos(m_phi));

		if (m_orientation.z < 0)
			m_theta *= -1;
	}

	// Conversion en degrés
	m_phi = m_phi * 180 / M_PI;
	m_theta = m_theta * 180 / M_PI;
}




/// <summary>
/// Set le vecteur position.
/// </summary>
/// <param name="position">The position.</param>
void Camera::setPosition(glm::vec3 position)
{
	// Mise à jour de la position
	m_position = position;

	// Actualisation du point ciblé
	m_pointCible = m_position + m_orientation;
}



/// <summary>
/// Gets the sensibilite.
/// </summary>
/// <returns></returns>
float Camera::getSensibilite() const
{
	return m_vitesse;
}


/// <summary>
/// Gets the vitesse.
/// </summary>
/// <returns></returns>
float Camera::getVitesse() const
{
	return m_vitesse;
}


/// <summary>
/// Sets the sensibilite.
/// </summary>
/// <param name="sensibilite">The sensibilite.</param>
void Camera::setSensibilite(float sensibilite)
{
	m_sensibilite = sensibilite;
}


/// <summary>
/// Sets the vitesse.
/// </summary>
/// <param name="vitesse">The vitesse.</param>
void Camera::setVitesse(float vitesse)
{
	m_vitesse = vitesse;
}

/// <summary>
/// Sets the angle.
/// </summary>
/// <param name="variation">The variation.</param>
/// <param name="angle">The angle.</param>
void Camera::setAngle(int variation, float *angle) {
	//modification de l'angle
	*angle = *angle + variation;
	//verification de l'intervalle de l'angle
	if (*angle >= 360)
		*angle -= 360;
	if (*angle <= 0)
		*angle += 360;
}
