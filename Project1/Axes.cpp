#include "Axes.h"


//pour l'offset du VBO
#ifndef BUFFER_OFFSET
#define BUFFER_OFFSET(offset) ((char*)NULL + (offset))
#endif



Axes::Axes(std::string const vertexShader, std::string const fragmentShader, float taille) : m_shader(vertexShader, fragmentShader), m_vboID(0), m_vaoID(0),
m_axis_verticesBytes(15*sizeof(float)),
m_axis_couleursBytes(15*sizeof(float))
{
	m_shader.charger();
	//on initialise la taille des axes
	m_axis_vertices[0] = taille;
	m_axis_vertices[7] = taille;
	m_axis_vertices[14] = taille;
}


Axes::~Axes()
{
	glDeleteBuffers(1, &m_vboID);
	glDeleteVertexArrays(1, &m_vaoID);
}


/// <summary>
/// Charge les donn�es dans la m�moire du GPU.
/// </summary>
void Axes::charger() {

	//v�rification pour �viter une fuite de m�moire
	if (glIsBuffer(m_vboID) == GL_TRUE)
		glDeleteBuffers(1, &m_vboID);

	//G�n�ration de l'ID du VBO
	glGenBuffers(1, &m_vboID);

	// Verrouillage du VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);


		// Allocation de la m�moire du VBO
		glBufferData(GL_ARRAY_BUFFER, m_axis_couleursBytes+m_axis_verticesBytes, 0, GL_STATIC_DRAW);

		//Transfert des donn�es
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_axis_verticesBytes, m_axis_vertices);
		glBufferSubData(GL_ARRAY_BUFFER, m_axis_verticesBytes,m_axis_couleursBytes, m_axis_couleurs);


	// D�verrouillage de l'objet
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Destruction d'un �ventuel ancien VAO
	if (glIsVertexArray(m_vaoID) == GL_TRUE)
		glDeleteVertexArrays(1, &m_vaoID);

	// G�n�ration de l'ID du VAO
	glGenVertexArrays(1, &m_vaoID);

	// Verrouillage du VAO
	glBindVertexArray(m_vaoID);

		// Verrouillage du VBO
		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

			//acc�s aux points de controles
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

			// Acc�s aux couleurs des points de controles
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(m_axis_verticesBytes));

			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(0);

		// D�verrouillage du VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	// D�verrouillage du VAO
	glBindVertexArray(0);
}



/// <summary>
/// pour afficher les axes
/// </summary>
/// <param name="projection">The projection matrix.</param>
/// <param name="modelview">The modelview matrix.</param>
void Axes::afficher(glm::mat4 &projection, glm::mat4 &modelview) {

	// Activation du shader
	glUseProgram(m_shader.getProgramID());

	// Verrouillage du VAO
	glBindVertexArray(m_vaoID);


		// Envoi des matrices
		glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "projection"), 1, GL_FALSE, value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "modelview"), 1, GL_FALSE, value_ptr(modelview));

		// Rendu
		glDrawArrays(GL_LINE_STRIP, 0, 5);
		glLineWidth(3);

	// D�verrouillage du VAO
	glBindVertexArray(0);

	// D�sactivation du shader
	glUseProgram(0);
}

