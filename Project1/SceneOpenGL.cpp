#include "SceneOpenGL.h"

using namespace glm;


SceneOpenGL::SceneOpenGL(std::string titreFenetre, int largeurFenetre, int hauteurFenetre) :
	m_titreFenetre(titreFenetre), m_largeurFenetre(largeurFenetre),
	m_hauteurFenetre(hauteurFenetre), m_fenetre(0), m_contexteOpenGL(0),
	m_input()
{
}


SceneOpenGL::~SceneOpenGL()
{
	SDL_GL_DeleteContext(m_contexteOpenGL);
	SDL_DestroyWindow(m_fenetre);
	SDL_Quit();
}


/// <summary>
/// Initialise la fenetre.
/// </summary>
/// <returns></returns>
bool SceneOpenGL::initialiserFenetre()
{
	// Initialisation de la SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Erreur lors de l'initialisation de la SDL : " << SDL_GetError() << std::endl;
		SDL_Quit();

		return false;
	}

	// Version d'OpenGL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);


	// Double Buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


	// Création de la fenêtre
	m_fenetre = SDL_CreateWindow(m_titreFenetre.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_largeurFenetre, m_hauteurFenetre, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if (m_fenetre == 0)
	{
		std::cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << std::endl;
		SDL_Quit();

		return false;
	}


	// Création du contexte OpenGL
	m_contexteOpenGL = SDL_GL_CreateContext(m_fenetre);

	if (m_contexteOpenGL == 0)
	{
		std::cout << SDL_GetError() << std::endl;
		SDL_DestroyWindow(m_fenetre);
		SDL_Quit();

		return false;
	}

	return true;
}


/// <summary>
/// Initialise les librairies OpenGL et GLEW.
/// </summary>
/// <returns></returns>
bool SceneOpenGL::initGL()
{
	// On initialise GLEW
	GLenum initialisationGLEW(glewInit());

	glEnable(GL_DEPTH_TEST);

	// Si l'initialisation a échoué :
	if (initialisationGLEW != GLEW_OK)
	{
		// On affiche l'erreur grâce à la fonction : glewGetErrorString(GLenum code)
		std::cout << "Erreur d'initialisation de GLEW : " << glewGetErrorString(initialisationGLEW) << std::endl;


		// On quitte la SDL
		SDL_GL_DeleteContext(m_contexteOpenGL);
		SDL_DestroyWindow(m_fenetre);
		SDL_Quit();

		return false;
	}

	// Tout s'est bien passé, on retourne true
	return true;
}


/// <summary>
/// Méthode principale d'affichage du programme.
/// </summary>
void SceneOpenGL::affichage()
{
	//définition des angles pour la rotation du repère
	float angle1(0.0);
	float angle2(0.0);

	//nombre de boucles par seconde (30)
	unsigned int frameRate(1000 / 30);
	Uint32 debutBoucle(0), finBoucle(0), tempsEcoule(0);

	//pour le slider
	int x = 0;
	int getX = 0;

	//pour afficher les informations de la courbe dans la console
	bool details;
	details = true;

	// Matrices nécessaires à l'affichage
	mat4 projection;
	mat4 modelview;
	projection = perspective(70.0, (double)m_largeurFenetre / m_hauteurFenetre, 1.0, 100.0);
	modelview = mat4(1.0);

	//création d'une caméra mobile
	Camera camera = Camera(vec3(1, 2, 9), vec3(0, 0, 0), vec3(0, 1, 0), float(0.25), float(0.1));

	// Capture du pointeur
	m_input.afficherPointeur(true);
	m_input.capturerPointeur(false);

	//création des 3 sliders
	Slider slider = Slider("Shaders/couleur2D.vert", "Shaders/couleur2D.frag", float(-0.9), float(0.8));
	slider.charger();
	Slider slider2 = Slider("Shaders/couleur2D.vert", "Shaders/couleur2D.frag", float(0.6), float(0.8));
	slider2.charger();
	Slider slider3 = Slider("Shaders/couleur2D.vert", "Shaders/couleur2D.frag", float(0.6), float(0.6));
	slider3.charger();

	//création des axes
	Axes axe =Axes("Shaders/couleur3D.vert", "Shaders/couleur3D.frag",5.0);
	axe.charger();


	///////////////////////////////////////////////////////////////////////////////////
	//variables pour ATPH
	vec3 *temp = ATPH::initialisationATPH();

	double phi0 = 0;
	double phi2 = 0;
	double alpha = 0.11;
	///////////////////////////////////////////////////////////////////////////////////

	//création d'une instance ATPH
	ATPH test = ATPH(temp[0], temp[1], temp[2], temp[3], phi0, phi2, alpha, "Shaders/couleur3D.vert", "Shaders/couleur3D.frag");
	test.charger();
	
	//tableaux pour bouger les points avec la souris
	double matModelView[16], matProjection[16];
	int viewport[4];

	glGetDoublev(GL_MODELVIEW_MATRIX, matModelView);
	glGetDoublev(GL_PROJECTION_MATRIX, matProjection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	//initialisation du tableau "matProjection"
	for (int i = 0; i < 4; i++) {
		matProjection[i * 4] = projection[i].x;
		matProjection[i * 4 + 1] = projection[i].y;
		matProjection[i * 4 + 2] = projection[i].z;
		matProjection[i * 4 + 3] = projection[i].w;
	}

	//variables pour les coordonnées des points à bouger
	GLdouble xP0;
	GLdouble yP0;
	GLdouble zP0;
	GLdouble xPf;
	GLdouble yPf;
	GLdouble zPf;
	GLdouble xtest;
	GLdouble ytest;
	GLdouble ztest;



	//****************************************************
	// Boucle principale
	while (!m_input.terminer())
	{
		// Nettoyage de l'écran
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//on place la caméra
		camera.lookAt(modelview);
		debutBoucle = SDL_GetTicks();

		//déplacement de la caméra avec la souris
		if (m_input.getBoutonSouris(SDL_BUTTON_RIGHT)) {
			Camera::setAngle(m_input.getXRel(), &angle1);
			Camera::setAngle(m_input.getYRel(), &angle2);
		}
		// Gestion des évènements
		m_input.updateEvenements();

		//si on veut quitter (echap)
		if (m_input.getTouche(SDL_SCANCODE_ESCAPE))
			break;
		
		//ajustement des coefficients en fonction de la position des sliders
		getX = m_input.getX();
		//si on fait un clic gauche
		if (m_input.getBoutonSouris(SDL_BUTTON_LEFT) &&  x !=getX) {
			//on regarde le premier slider
			if (slider.isIn(getX, m_input.getY(), m_hauteurFenetre, m_largeurFenetre) == true) {
				slider.setCurseur(getX, m_largeurFenetre);
				x = m_input.getX();
				test.setAlpha(slider.actionSlider(test.alphaMax, test.alphaMin, m_largeurFenetre, x));
				details = true;
			}
			//sinon on regarde le deuxième slider
			else if (slider2.isIn(getX, m_input.getY(), m_hauteurFenetre, m_largeurFenetre) == true) {
				slider2.setCurseur(getX, m_largeurFenetre);
				x = m_input.getX();
				test.setPhi0(slider2.actionSlider(test.Phi0Max, test.Phi0Min, m_largeurFenetre, x));
				details = true;
			}
			//sinon on regarde le troisième slider
			else if (slider3.isIn(getX, m_input.getY(), m_hauteurFenetre, m_largeurFenetre) == true) {
				slider3.setCurseur(getX, m_largeurFenetre);
				x = m_input.getX();
				test.setPhi2(slider3.actionSlider(test.Phi2Max, test.Phi2Min, m_largeurFenetre, x));
				details = true;
			}
		}

		//si la courbe a changé, on affiche les nouvelles informations
		if (m_input.getBoutonSouris(SDL_BUTTON_LEFT) == false && details == true) {
			test.details();
			details = false;
		}

		//on déplace la caméra si besoin
		camera.deplacer(m_input);

		//on bouge le repère
		modelview = rotate(modelview, angle2, vec3(1, 0, 0));
		modelview = rotate(modelview, angle1, vec3(0, 1, 0));

		//si on fait un clic droit
		if (m_input.getBoutonSouris(SDL_BUTTON_LEFT)) {

			//on met à jour le tableau "matModelView"
			for (int i = 0; i < 4; i++) {
				matModelView[i * 4] = modelview[i].x;
				matModelView[i * 4 + 1] = modelview[i].y;
				matModelView[i * 4 + 2] = modelview[i].z;
				matModelView[i * 4 + 3] = modelview[i].w;
			}

			//on récupere la position de la souris
			double winX = (double)m_input.getX();
			double winY = viewport[3] - (double)m_input.getY();

			//on projette les points à bouger sur l'écran
			gluProject(test.getP0().x, test.getP0().y, test.getP0().z, matModelView, matProjection,
				viewport, &xP0, &yP0, &zP0);
			gluProject(test.getP5().x, test.getP5().y, test.getP5().z, matModelView, matProjection,
				viewport, &xPf, &yPf, &zPf);
			
			//si les coordonnées correspondent à plus ou moins 8 pixels
			if (abs(winX - xP0) < 8 && abs(winY - yP0) < 8) {
				//on modifie les coordonnées du point 0
				gluUnProject(winX, winY,zP0, matModelView, matProjection,viewport, &xtest, &ytest, &ztest);
				test.setP0(float(xtest), float(ytest), float(ztest));
				details = true;
			}
			if (abs(winX - xPf) < 8 && abs(winY - yPf) < 8) {
				//on modifie les coordonnées du point 5
				gluUnProject(winX, winY, zPf, matModelView, matProjection, viewport, &xtest, &ytest, &ztest);
				test.setP5(float (xtest), float (ytest), float (ztest));
				details = true;
			}
		}


		//affichage des objets
		axe.afficher(projection, modelview);
		test.afficher(projection, modelview);
		slider.afficher(projection, modelview);
		slider2.afficher(projection, modelview);
		slider3.afficher(projection, modelview);

		
		// Actualisation de la fenêtre
		SDL_GL_SwapWindow(m_fenetre);

		//calcul du temps mis pour la boucle
		finBoucle = SDL_GetTicks();
		tempsEcoule = finBoucle - debutBoucle;

		// Si nécessaire, on met en pause le programme
		if (tempsEcoule < frameRate)
			SDL_Delay(frameRate - tempsEcoule);
	}	
}


