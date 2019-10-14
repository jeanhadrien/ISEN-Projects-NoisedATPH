#include "ATPH.h"


// Macro utile au VBO
#ifndef BUFFER_OFFSET
#define BUFFER_OFFSET(offset) ((char*)NULL + (offset))
#endif


ATPH::ATPH(vec3 p0, vec3 p5, vec3 di, vec3 df, double phi0, double phi2, double alpha, std::string const vertexShader, std::string const fragmentShader) : m_shader(vertexShader, fragmentShader), p0(p0), p5(p5), di(di),
df(df), phi0(phi0), phi2(phi2), alpha(alpha), m_vaoID(0), m_vboID(0),
m_verticesBytes(18 * sizeof(float)),m_couleursBytes(18 * sizeof(float)),m_ATPHBytes(3*precision * sizeof(float)),m_ATPH_couleursBytes(3* precision * sizeof(float))
{
	m_shader.charger();
	// on définit une taille pour les vertices (qui pourait changer si on veut plus de points)
	m_ATPH.resize(3* precision);
	biBasic.resize(precision, vector<double>(6));
	m_ATPH_couleurs.resize(3* precision);
	//on initialise la couleur de la courbe
	for (int i = 0; i < 3* precision; i = i + 3) {
		m_ATPH_couleurs[i] = 1.0;
		m_ATPH_couleurs[i + 1] = 0.0;
		m_ATPH_couleurs[i + 2] = 1.0;
	}
	//on fait les calculs pour la courbe ATPH 
	update();
}


ATPH::~ATPH()
{
	glDeleteBuffers(1, &m_vboID);
	glDeleteVertexArrays(1, &m_vaoID);
}


/// <summary>
/// Charge les données dans la mémoire du GPU.
/// </summary>
void ATPH::charger() {
	//pour le VBO
	//vérification pour éviter une fuite de mémoire
	if (glIsBuffer(m_vboID) == GL_TRUE)
		glDeleteBuffers(1, &m_vboID);

	//Génération de l'ID du VBO
	glGenBuffers(1, &m_vboID);

	// Verrouillage du VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

		// Allocation de la mémoire du VBO
		glBufferData(GL_ARRAY_BUFFER, m_verticesBytes + m_couleursBytes + m_ATPHBytes + m_ATPH_couleursBytes, 0, GL_DYNAMIC_DRAW);

		//Transfert des données
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_verticesBytes, m_vertices);
		glBufferSubData(GL_ARRAY_BUFFER, m_verticesBytes, m_ATPHBytes, m_ATPH.data());
		glBufferSubData(GL_ARRAY_BUFFER, m_verticesBytes + m_ATPHBytes, m_couleursBytes, m_couleurs);
		glBufferSubData(GL_ARRAY_BUFFER, m_verticesBytes + m_couleursBytes + m_ATPHBytes, m_ATPH_couleursBytes, m_ATPH_couleurs.data());

	// Déverrouillage de l'objet
	glBindBuffer(GL_ARRAY_BUFFER, 0);



	//pour le VAO
	// Destruction d'un éventuel ancien VAO
	if (glIsVertexArray(m_vaoID) == GL_TRUE)
		glDeleteVertexArrays(1, &m_vaoID);

	// Génération de l'ID du VAO
	glGenVertexArrays(1, &m_vaoID);

	// Verrouillage du VAO
	glBindVertexArray(m_vaoID);

		// Verrouillage du VBO
		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

			//accès aux points de contrôle
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

			// Accès aux couleurs des points de contrôle
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(m_verticesBytes + m_ATPHBytes));

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);


		// Déverrouillage du VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Déverrouillage du VAO
	glBindVertexArray(0);
}


/// <summary>
/// Met à jour les données présente dans le GPU.
/// </summary>
/// <param name="donnees">donnees.</param>
/// <param name="tailleBytes">taille en bytes.</param>
/// <param name="decalage">offset.</param>
void ATPH::updateVBO(void *donnees, int tailleBytes, int decalage)
{
	// Verrouillage du VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

	// Récupération de l'adresse du VBO
	void *adresseVBO = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	// Si l'adresse retournée est nulle alors on arrête le transfert
	if (adresseVBO == NULL)
	{
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
/// Sets the alpha.
/// </summary>
/// <param name="a">a.</param>
void ATPH::setAlpha(double a) {
	alpha = a;
	update();
}


/// <summary>
/// Gets the alpha.
/// </summary>
/// <returns></returns>
double ATPH::getAlpha() {
	return alpha;
}


/// <summary>
/// Sets the phi0.
/// </summary>
/// <param name="a">a.</param>
void ATPH::setPhi0(double a) {
	phi0 = a;
	update();
}


/// <summary>
/// Gets the phi0.
/// </summary>
/// <returns></returns>
double ATPH::getPhi0() {
	return phi0;
}

/// <summary>
/// Sets the phi2.
/// </summary>
/// <param name="a">a.</param>
void ATPH::setPhi2(double a) {
	phi2 = a;
	update();
}


/// <summary>
/// Gets the phi2.
/// </summary>
/// <returns></returns>
double ATPH::getPhi2() {
	return phi2;
}



/// <summary>
/// Set les coordonnées du points p0 et réactualise la courbe
/// </summary>
/// <param name="x">x.</param>
/// <param name="y">y.</param>
/// <param name="z">z.</param>
void ATPH::setP0(float x, float y, float z) {
	p0.x = x;
	p0.y = y;
	p0.z = z;
	update();
}


/// <summary>
/// Gets the p0.
/// </summary>
/// <returns></returns>
vec3 ATPH::getP0() {
	return p0;
}


/// <summary>
/// Set les coordonnées du points p5 et réactualise la courbe
/// </summary>
/// <param name="x">The x.</param>
/// <param name="y">The y.</param>
/// <param name="z">The z.</param>
void ATPH::setP5(float x, float y, float z) {
	p5.x = x;
	p5.y = y;
	p5.z = z;
	update();
}


/// <summary>
/// Gets the p5.
/// </summary>
/// <returns></returns>
vec3 ATPH::getP5() {
	return p5;
}



/// <summary>
/// calcul de A0_ATPH, A2_ATPH et A1_ATPH
/// </summary>
void ATPH::Quaternions_A0A1A2_ATPH() {
	vec3 u = vec3(1.0, 0.0, 0.0);
	//On calcule la norme de di et df
	float n_di = CalculTools::norm(di);
	float n_df = CalculTools::norm(df);

	//On normalise 
	vec3 delta_i;
	delta_i.x = di.x / n_di;
	delta_i.y = di.y / n_di;
	delta_i.z = di.z / n_di;

	vec3 delta_f;
	delta_f.x = df.x / n_df;
	delta_f.y = df.y / n_df;
	delta_f.z = df.z / n_df;

	vec3 ni = (delta_i + u);
	ni.x = ni.x / CalculTools::norm(delta_i + u);
	ni.y = ni.y / CalculTools::norm(delta_i + u);
	ni.z = ni.z / CalculTools::norm(delta_i + u);

	vec3 nf = (delta_f + u);
	nf.x = nf.x / CalculTools::norm(delta_f + u);
	nf.y = nf.y / CalculTools::norm(delta_f + u);
	nf.z = nf.z / CalculTools::norm(delta_f + u);

	s1 = sin(alpha / 2);
	s2 = sin(alpha);
	c2 = cos(alpha);
	n0 = 6 * alpha + 2 * s2 *(c2 - 4);
	n2 = (2 + c2) * alpha - 3 * s2;

	double rho = n0 - 6 * n2;
	double omega = 4 * n2 * (1 + c2);

	vec3 c;
	c.x = float(16 * pow(s1, 4) * omega * (p5.x - p0.x) + (pow(rho, 2) - omega * n0) * (di.x + df.x));
	c.y = float(16 * pow(s1, 4) * omega * (p5.y - p0.y) + (pow(rho, 2) - omega * n0) * (di.y + df.y));
	c.z = float(16 * pow(s1, 4) * omega * (p5.z - p0.z) + (pow(rho, 2) - omega * n0) * (di.z + df.z));

	vec3 e;
	e.x = float(2 * sqrt(n_di*n_df) * (CalculTools::scalairProduct(u, nf)*ni.x + CalculTools::scalairProduct(u, ni)*nf.x - CalculTools::scalairProduct(ni, nf)*u.x));
	e.y = float(2 * sqrt(n_di*n_df) * (CalculTools::scalairProduct(u, nf)*ni.y + CalculTools::scalairProduct(u, ni)*nf.y - CalculTools::scalairProduct(ni, nf)*u.y));
	e.z = float(2 * sqrt(n_di*n_df) * (CalculTools::scalairProduct(u, nf)*ni.z + CalculTools::scalairProduct(u, ni)*nf.z - CalculTools::scalairProduct(ni, nf)*u.z));

	vec3 f;
	f.x = 2 * sqrt(n_di*n_df) * CalculTools::crossProduct(nf, ni).x;
	f.y = 2 * sqrt(n_di*n_df) * CalculTools::crossProduct(nf, ni).y;
	f.z = 2 * sqrt(n_di*n_df) * CalculTools::crossProduct(nf, ni).z;

	vec3 d;
	d.x = float(c.x + (pow(rho, 2) - 2 * omega*n2)*(cos(phi2)*e.x + sin(phi2)*f.x));
	d.y = float(c.y + (pow(rho, 2) - 2 * omega*n2)*(cos(phi2)*e.y + sin(phi2)*f.y));
	d.z = float(c.z + (pow(rho, 2) - 2 * omega*n2)*(cos(phi2)*e.z + sin(phi2)*f.z));

	vec4 nibis = vec4(0, ni.x, ni.y, ni.z);
	vec4 A0_ATPH_1 = vec4(cos(phi0 - 0.5*phi2), sin(phi0 - 0.5*phi2), 0, 0);
	A0_ATPH.x = sqrt(n_di)* Quaternions::quatmultiply(nibis, A0_ATPH_1).x;
	A0_ATPH.y = sqrt(n_di)* Quaternions::quatmultiply(nibis, A0_ATPH_1).y;
	A0_ATPH.z = sqrt(n_di)* Quaternions::quatmultiply(nibis, A0_ATPH_1).z;
	A0_ATPH.w = sqrt(n_di)* Quaternions::quatmultiply(nibis, A0_ATPH_1).w;

	vec4 nfbis = vec4(0, nf.x, nf.y, nf.z);
	vec4 A2_ATPH_1 = vec4(cos(phi0 + 0.5*phi2), sin(phi0 + 0.5*phi2), 0, 0);
	A2_ATPH.x = sqrt(n_df)* Quaternions::quatmultiply(nfbis, A2_ATPH_1).x;
	A2_ATPH.y = sqrt(n_df)* Quaternions::quatmultiply(nfbis, A2_ATPH_1).y;
	A2_ATPH.z = sqrt(n_df)* Quaternions::quatmultiply(nfbis, A2_ATPH_1).z;
	A2_ATPH.w = sqrt(n_df)* Quaternions::quatmultiply(nfbis, A2_ATPH_1).w;

	double n_d = CalculTools::norm(d);
	vec3 delta_d = vec3(d.x / n_d, d.y / n_d, d.z / n_d);
	vec3 n;
	n.x = (delta_d.x + u.x) / CalculTools::norm(delta_d + u);
	n.y = (delta_d.y + u.y) / CalculTools::norm(delta_d + u);
	n.z = (delta_d.z + u.z) / CalculTools::norm(delta_d + u);

	vec4 part1;
	part1.x = float((-rho / omega)*(A0_ATPH.x + A2_ATPH.x));
	part1.y = float((-rho / omega)*(A0_ATPH.y + A2_ATPH.y));
	part1.z = float((-rho / omega)*(A0_ATPH.z + A2_ATPH.z));
	part1.w = float((-rho / omega)*(A0_ATPH.w + A2_ATPH.w));

	vec4 nbis = vec4(0, n.x, n.y, n.z);
	vec4 part2;
	part2.x = float((1.0 / omega) * sqrt(n_d)*nbis.x);
	part2.y = float((1.0 / omega) * sqrt(n_d)*nbis.y);
	part2.z = float((1.0 / omega) * sqrt(n_d)*nbis.z);
	part2.w = float((1.0 / omega) * sqrt(n_d)*nbis.w);

	A1_ATPH = part1 + part2;
}



/// <summary>
/// calcul des points de contrôle
/// </summary>
void ATPH::controlPointsATPH() {
	s1 = sin(alpha / 2);
	s2 = sin(alpha);
	c2 = cos(alpha);
	n0 = 6 * alpha + 2 * s2 * (c2 - 4);
	n2 = (2 + c2) * alpha - 3 * s2;

	vec4 p0_quat = vec4(0, p0.x, p0.y, p0.z);
	vec4 i = vec4(0, 1, 0, 0);
	vec4 A0_s = Quaternions::quatConj(A0_ATPH);
	vec4 A1_s = Quaternions::quatConj(A1_ATPH);
	vec4 A2_s = Quaternions::quatConj(A2_ATPH);

	vec4 A0_i = Quaternions::quatmultiply(A0_ATPH, i);
	vec4 A1_i = Quaternions::quatmultiply(A1_ATPH, i);
	vec4 A2_i = Quaternions::quatmultiply(A2_ATPH, i);

	vec4 A0_i_A0_s = Quaternions::quatmultiply(A0_i, A0_s);
	vec4 A0_i_A1_s = Quaternions::quatmultiply(A0_i, A1_s);
	vec4 A0_i_A2_s = Quaternions::quatmultiply(A0_i, A2_s);

	vec4 A1_i_A0_s = Quaternions::quatmultiply(A1_i, A0_s);
	vec4 A1_i_A1_s = Quaternions::quatmultiply(A1_i, A1_s);
	vec4 A1_i_A2_s = Quaternions::quatmultiply(A1_i, A2_s);

	vec4 A2_i_A0_s = Quaternions::quatmultiply(A2_i, A0_s);
	vec4 A2_i_A1_s = Quaternions::quatmultiply(A2_i, A1_s);
	vec4 A2_i_A2_s = Quaternions::quatmultiply(A2_i, A2_s);

	float coef_1 = float(n0 / (16.0 * pow(s1, 4)));
	float coef_2 = float((n0 - 6.0 * n2) / (16.0 * pow(s1, 4)));
	float coef_3 = float((n2 / (8.0 * pow(s1, 4))));
	vec4 coef_4 = vec4(float(2.0 * (1.0 + c2)) * A1_i_A1_s);

	vec4 p1_1 = vec4(coef_1 * A0_i_A0_s);
	vec4 p1_quat = vec4(p0_quat + p1_1);
	vec4 p2_1 = vec4(coef_2 * (A0_i_A1_s + A1_i_A0_s));
	vec4 p2_quat = vec4(p1_quat + p2_1);
	vec4 p3_1 = vec4(A0_i_A2_s + coef_4);
	vec4 p3_2 = vec4(p3_1 + A2_i_A0_s);
	vec4 p3_3 = vec4(coef_3 * p3_2);
	vec4 p3_quat = vec4(p2_quat + p3_3);
	vec4 p4_1 = vec4(coef_2 * (A1_i_A2_s + A2_i_A1_s));
	vec4 p4_quat = vec4(p3_quat + p4_1);
	vec4 p5_1 = vec4(coef_1 * A2_i_A2_s);
	vec4 p5_quat = vec4(p4_quat + p5_1);

	p0.x = p0_quat.y;
	p0.y = p0_quat.z;
	p0.z = p0_quat.w;

	p1.x = p1_quat.y;
	p1.y = p1_quat.z;
	p1.z = p1_quat.w;

	p2.x = p2_quat.y;
	p2.y = p2_quat.z;
	p2.z = p2_quat.w;

	p3.x = p3_quat.y;
	p3.y = p3_quat.z;
	p3.z = p3_quat.w;

	p4.x = p4_quat.y;
	p4.y = p4_quat.z;
	p4.z = p4_quat.w;

	p5.x = p5_quat.y;
	p5.y = p5_quat.z;
	p5.z = p5_quat.w;
}


/// <summary>
/// Calcul des coefficients BiBasics nécessaires aux calculs des points de la courbe
/// </summary>
void ATPH::drawBasesSegNew() {
	s1 = sin(alpha / 2.0);
	c1 = cos(alpha / 2.0);
	s2 = sin(alpha);
	c2 = cos(alpha);

	double g = 6.0 * alpha + (2 * s2*(c2 - 4));
	double h = (c1 * (s2 - 3 * alpha)) + (4 * s1);
	double j = ((2 + c2)*alpha) - (3 * s2);

	vector<double> temp(6, 0);
	int z = 0;
	for (double i = 0; i < alpha; i = i + alpha / (precision - 1)) {
		temp[0] = ((2.0 / g)*(3.0*(alpha - i) + sin(alpha - i)*(cos(alpha - i) - 4.0)));
		temp[1] = (((4.0 * s1) / (g*h))*(g*(pow((sin((alpha - i) / 2.0)), 4)) - (2.0 * pow(s1, 4)*(3.0*(alpha - i) + sin(alpha - i)*(cos(alpha - i) - 4.0)))));
		temp[2] = ((2.0 * s1)*(8.0 * ((pow((sin((alpha - i) / 2.0)), 3))*sin(i / 2.0)) - (g / h)*(pow((sin((alpha - i) / 2.0)), 4)) + ((2.0 * pow(s1, 4)) / h)*((3.0*(alpha - i)) + sin(alpha - i)*(cos(alpha - i) - 4.0))) / (3.0 * j));
		temp[3] = ((2 * s1)*(8 * ((pow((sin(i / 2.0)), 3))*(sin((alpha - i) / 2.0))) - (g / h)*(pow((sin(i / 2.0)), 4)) + (2 * pow((s1), 4) / h)*((3 * i) + sin(i)*(cos(i) - 4.0))) / (3 * j));
		temp[4] = (((4 * s1) / (g*h))*(g*(pow((sin(i / 2.0)), 4)) - 2 * (pow(s1, 4))*((3 * i) + sin(i)*(cos(i) - 4))));
		temp[5] = ((2 / g)*((3 * i) + sin(i)*(cos(i) - 4)));
		biBasic[z] = temp;
		z++;
	}
}



/// <summary>
/// méthode qui crée la courbe
/// </summary>
void ATPH::update() {
	//appel des méthodes qui calculent tous les coefficients
	Quaternions_A0A1A2_ATPH();
	controlPointsATPH();
	drawBasesSegNew();

	//Création de tous les points de la courbe
	int z = 0;
	for (int i = 0; i < int(biBasic.size()); i++) {
		z = 3 * i;
		m_ATPH[z] = float(p0.x * biBasic[i][0] + p1.x * biBasic[i][1] + p2.x *biBasic[i][2] + p3.x *biBasic[i][3] + p4.x *biBasic[i][4] + p5.x *biBasic[i][5]);
		m_ATPH[z + 1] = float(p0.y * biBasic[i][0] + p1.y * biBasic[i][1] + p2.y *biBasic[i][2] + p3.y *biBasic[i][3] + p4.y *biBasic[i][4] + p5.y *biBasic[i][5]);
		m_ATPH[z + 2] = float(p0.z * biBasic[i][0] + p1.z * biBasic[i][1] + p2.z *biBasic[i][2] + p3.z *biBasic[i][3] + p4.z *biBasic[i][4] + p5.z *biBasic[i][5]);
	}


	//Creation d'un tableau pour afficher les points de contrôle
	m_vertices[0] = (p0.x);
	m_vertices[1] = (p0.y);
	m_vertices[2] = (p0.z);
	m_vertices[3] = (p1.x);
	m_vertices[4] = (p1.y);
	m_vertices[5] = (p1.z);
	m_vertices[6] = (p2.x);
	m_vertices[7] = (p2.y);
	m_vertices[8] = (p2.z);
	m_vertices[9] = (p3.x);
	m_vertices[10] = (p3.y);
	m_vertices[11] = (p3.z);
	m_vertices[12] = (p4.x);
	m_vertices[13] = (p4.y);
	m_vertices[14] = (p4.z);
	m_vertices[15] = (p5.x);
	m_vertices[16] = (p5.y);
	m_vertices[17] = (p5.z);

	//update du VBO
	this->updateVBO(m_vertices, m_verticesBytes, 0);
	this->updateVBO(m_ATPH.data(), m_ATPHBytes, m_verticesBytes);
}



/// <summary>
/// affichage des points de contrôle et de la courbe
/// </summary>
/// <param name="projection">The projection matrix.</param>
/// <param name="modelview">The modelview matrix.</param>
void ATPH::afficher(glm::mat4 &projection, glm::mat4 &modelview) {

	// Activation du shader
	glUseProgram(m_shader.getProgramID());

	// Verrouillage du VAO
	glBindVertexArray(m_vaoID);

		// Envoi des matrices
		glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "projection"), 1, GL_FALSE, value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "modelview"), 1, GL_FALSE, value_ptr(modelview));

		// Rendu
		glDrawArrays(GL_POINTS, 0, 6);
		glPointSize(8);

		// Rendu
		glDrawArrays(GL_LINE_STRIP, 6, precision);
		glLineWidth(3);


	// Déverrouillage du VAO
	glBindVertexArray(0);


	// Désactivation du shader
	glUseProgram(0);

}



/// <summary>
/// méthode pour que l'utilisateur puisse saisir les coordonnées du points initial, final et les dérivées
/// </summary>
/// <returns></returns>
vec3* ATPH::initialisationATPH() {
	vec3 *resultat = new vec3[4];
	std::string points[4] = { "p0","pf","di","df" };
	float defaultvalues[12] = { 5.0 ,0.0 ,1.0 ,-3.0 ,-4.0 ,1.0 ,25.0 ,-15.0 ,1.0 ,25.0 ,-15.0 ,1.0 };
	for (int i = 0; i < 4; i++) {
		cout << "Rentrer les composantes x, y et z de " << points[i] << " : (entree pour les valeurs par defaut)" << endl;
		cout << "x : ";
		resultat[i].x = Input::saisieUtilisateur(defaultvalues[3 * i]);
		cout << "y : ";
		resultat[i].y = Input::saisieUtilisateur(defaultvalues[3 * i + 1]);
		cout << "z : ";
		resultat[i].z = Input::saisieUtilisateur(defaultvalues[3 * i + 2]);
	}
	return resultat;

}



/// <summary>
/// pour afficher sur la console les coordonnés des points de contrôle et les paramètres (alpha, phi0 et phi2)
/// </summary>
void ATPH::details() {
	cout << "\n\n\n-----------------------------------------------------------------\n" << endl;
	cout << "\n      |      x      |      y      |      z      |" << endl;
	cout << fixed << setprecision(2) << "  p0        " << p0.x << "         " << p0.y << "         " << p0.z << "  " << endl;
	cout << fixed << setprecision(2) << "  p1        " << p1.x << "         " << p1.y << "         " << p1.z << "  " << endl;
	cout << fixed << setprecision(2) << "  p2        " << p2.x << "         " << p2.y << "         " << p2.z << "  " << endl;
	cout << fixed << setprecision(2) << "  p3        " << p3.x << "         " << p3.y << "         " << p3.z << "  " << endl;
	cout << fixed << setprecision(2) << "  p4        " << p4.x << "         " << p4.y << "         " << p4.z << "  " << endl;
	cout << fixed << setprecision(2) << "  p5        " << p5.x << "         " << p5.y << "         " << p5.z << "  " << endl;
	cout << fixed << setprecision(2) << "  di        " << di.x << "         " << di.y << "         " << di.z << "  " << endl;
	cout << fixed << setprecision(2) << "  df        " << df.x << "         " << df.y << "         " << df.z << "  " << endl;
	cout << "\n alpha : " << alpha << endl;
	cout << " phi0 : " << phi0 << endl;
	cout << " phi2 : " << phi2 << "\n\n\n" << endl;
}
