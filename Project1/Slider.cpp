#include "Slider.h"

using namespace glm;

//pour l'offset des VBO
#ifndef BUFFER_OFFSET
#define BUFFER_OFFSET(offset) ((char*)NULL + (offset))
#endif


Slider::Slider(std::string const vertexShader, std::string const fragmentShader,float x,float y) : m_shader(vertexShader, fragmentShader),x(x),y(y), m_vboID(0), m_vaoID(0),
m_couleursBBytes(12*sizeof(float)),m_verticesBBytes(8*sizeof(float)),
m_couleursCBytes(12*sizeof(float)),m_verticesCBytes(8*sizeof(float))
{
	// Chargement du shader
	m_shader.charger();

	//création des vertices du slider en fonction de la position du coin en bas à gauche
	//pour le contour
	m_verticesB[0] = x;
	m_verticesB[1] = y;
	m_verticesB[2] = x;
	m_verticesB[3] = y + h;
	m_verticesB[4] = x + w1;
	m_verticesB[5] = y + h;
	m_verticesB[6] = x + w1;
	m_verticesB[7] = y;
	//pour le curseur
	m_verticesC[0] = x;
	m_verticesC[1] = y;
	m_verticesC[2] = x;
	m_verticesC[3] = y + h;
	m_verticesC[4] = x + w2;
	m_verticesC[5] = y + h;
	m_verticesC[6] = x + w2;
	m_verticesC[7] = y;
}


Slider::~Slider()
{
	glDeleteBuffers(1, &m_vboID);
}


/// <summary>
/// Charge les données dans la mémoire du GPU.
/// </summary>
void Slider::charger() {
	//vérification pour éviter une fuite de mémoire
	if (glIsBuffer(m_vboID) == GL_TRUE)
		glDeleteBuffers(1, &m_vboID);

	//Génération de l'ID du VBO
	glGenBuffers(1, &m_vboID);

	// Verrouillage du VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

		// Allocation de la mémoire du VBO
		glBufferData(GL_ARRAY_BUFFER, m_couleursBBytes + m_verticesBBytes + m_couleursCBytes + m_verticesCBytes, 0, GL_DYNAMIC_DRAW);

		//Transfert des données
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_verticesCBytes, m_verticesC);
		glBufferSubData(GL_ARRAY_BUFFER,m_verticesCBytes, m_verticesBBytes, m_verticesB);
		glBufferSubData(GL_ARRAY_BUFFER, m_verticesBBytes + m_verticesCBytes, m_couleursCBytes, m_couleursC);
		glBufferSubData(GL_ARRAY_BUFFER, m_verticesBBytes + m_couleursCBytes + m_verticesCBytes, m_couleursBBytes, m_couleursB);

	// Déverrouillage de l'objet
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// Destruction d'un éventuel ancien VAO
	if (glIsVertexArray(m_vaoID) == GL_TRUE)
		glDeleteVertexArrays(1, &m_vaoID);

	// Génération de l'ID du VAO
	glGenVertexArrays(1, &m_vaoID);

	// Verrouillage du VAO
	glBindVertexArray(m_vaoID);

		// Verrouillage du VBO
		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

			//accès aux points
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

			// Accès aux couleurs
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(m_verticesBBytes + m_verticesCBytes));

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);


		// Déverrouillage du VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Déverrouillage du VAO
	glBindVertexArray(0);
}


/// <summary>
/// Updates the vbo.
/// </summary>
/// <param name="donnees">The donnees.</param>
/// <param name="tailleBytes">The taille bytes.</param>
/// <param name="decalage">The decalage.</param>
void Slider::updateVBO(void *donnees, int tailleBytes, int decalage)
{
	// Verrouillage du VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

	// Récupération de l'adresse du VBO
	void *adresseVBO = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	// Si l'adresse retournée est nulle alors on arrête le transfert
	if (adresseVBO == NULL)
	{
		std::cout << "Erreur au niveau de la récupération du VBO" << std::endl;
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return;
	}
	// Mise à jour des données
	memcpy((char*)adresseVBO + decalage, donnees, tailleBytes);

	// Annulation du pointeur
	glUnmapBuffer(GL_ARRAY_BUFFER);
	adresseVBO = 0;

	// Déverrouillage du VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


/// <summary>
/// Permet d'afficher le slider.
/// </summary>
/// <param name="projection">The projection matrix.</param>
/// <param name="modelview">The modelview matrix.</param>
void Slider::afficher(glm::mat4 &projection, glm::mat4 &modelview)
{
	// Activation du shader
	glUseProgram(m_shader.getProgramID());

	// Verrouillage du VAO
	glBindVertexArray(m_vaoID);

		// Envoi des matrices
		glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "projection"), 1, GL_FALSE, value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "modelview"), 1, GL_FALSE, value_ptr(modelview));

		// On affiche le premier rectangle
		glDrawArrays(GL_POLYGON, 0, 4);

		// On affiche le second rectangle
		glDrawArrays(GL_POLYGON, 4, 4);

	// Déverrouillage du VAO
	glBindVertexArray(0);

	// Désactivation du shader
	glUseProgram(0);
}


/// <summary>
/// Gets the curseur.
/// </summary>
/// <returns></returns>
float* Slider::getCurseur() {
	return m_verticesC;
}


/// <summary>
/// Sets the curseur.
/// </summary>
/// <param name="x">The x.</param>
/// <param name="w">The w.</param>
void Slider::setCurseur(int x, int w) {
	//changement du vertice pour le curseur de slider
	w = w / 2;
	float X = float(x);
	m_verticesC[0] = float((X - w) / w -0.01);
	m_verticesC[2] = float((X - w) / w -0.01);
	m_verticesC[4] = float((X - w) / w +0.01);
	m_verticesC[6] = float((X - w) / w +0.01);

	//update du VBO por mettre à jour l'affichage
	updateVBO(m_verticesC, m_verticesCBytes, 0);
}


/// <summary>
/// Gets the x.
/// </summary>
/// <returns></returns>
float Slider::getX() {
	return x;
}


/// <summary>
/// Gets the y.
/// </summary>
/// <returns></returns>
float Slider::getY() {
	return y;
}


/// <summary>
/// Determines whether the specified x/y is in the slider.
/// </summary>
/// <param name="x">The x.</param>
/// <param name="y">The y.</param>
/// <param name="h">The height of the window.</param>
/// <param name="w">The width of the window.</param>
/// <returns>
///   <c>true</c> if the specified x/y is in; otherwise, <c>false</c>.
/// </returns>
bool Slider::isIn(int x, int y, int h, int w) {
	//dimension de la fenêtre pour faire le lien entre le repére de la fenêtre et sa taille
	w = w / 2;
	h = h / 2;
	//si la souris est dans le slider on retourne true
	if (x<(m_verticesB[4] * w + w - 3) && x>(m_verticesB[0] * w + w + 3) && y<(m_verticesB[1] * (-h) + h) && y>(m_verticesB[3] * (-h) + h)) {
		return true;
	}
	//sinon on retourne false
	else {
		return false;
	}
}


float Slider::actionSlider(float Max, float Min, int w,int position) {
	//calcul de la valeur de la variable associée au slider en fonction de sa position et des valeurs limites de la variable
	float resultat = float(((position - ((x + 1) * (w / 2) + 4)) * (Max - Min) / ((w1 - w2) * w/2 + 1)+0.01));
	return resultat;
}