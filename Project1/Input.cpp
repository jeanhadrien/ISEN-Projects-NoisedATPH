#include "Input.h"



Input::Input() : m_x(0), m_y(0), m_xRel(0), m_yRel(0), m_terminer(false)
{
	// Initialisation du tableau m_touches[]
	for (int i(0); i < SDL_NUM_SCANCODES; i++)
		m_touches[i] = false;


	// Initialisation du tableau m_boutonsSouris[]
	for (int i(0); i < 8; i++)
		m_boutonsSouris[i] = false;
}


Input::~Input()
{
}


/// <summary>
/// G�re les entr�es clavier et souris
/// </summary>
void Input::updateEvenements()
{	
	// Pour �viter des mouvements fictifs de la souris, on r�initialise les coordonn�es relatives
	m_xRel = 0;
	m_yRel = 0;

	// Boucle d'�v�nements
	while (SDL_PollEvent(&m_evenements))
	{
		// Switch sur le type d'�v�nement
		switch (m_evenements.type)
		{

			// Cas d'une touche enfonc�e
		case SDL_KEYDOWN:
			m_touches[m_evenements.key.keysym.scancode] = true;
			break;


			// Cas d'une touche rel�ch�e
		case SDL_KEYUP:
			m_touches[m_evenements.key.keysym.scancode] = false;
			break;


			// Cas de pression sur un bouton de la souris
		case SDL_MOUSEBUTTONDOWN:

			m_boutonsSouris[m_evenements.button.button] = true;

			break;


			// Cas du rel�chement d'un bouton de la souris
		case SDL_MOUSEBUTTONUP:

			m_boutonsSouris[m_evenements.button.button] = false;

			break;


			// Cas d'un mouvement de souris
		case SDL_MOUSEMOTION:

			m_x = m_evenements.motion.x;
			m_y = m_evenements.motion.y;

			m_xRel = m_evenements.motion.xrel;
			m_yRel = m_evenements.motion.yrel;

			break;


			// Cas de la fermeture de la fen�tre
		case SDL_WINDOWEVENT:

			if (m_evenements.window.event == SDL_WINDOWEVENT_CLOSE)
				m_terminer = true;

			break;


		default:
			break;
		}
	}
}


/// <summary>
/// M�thode pour quitter le programme.
/// </summary>
/// <returns></returns>
bool Input::terminer() const
{
	return m_terminer;
}


/// <summary>
/// R�cup�re la touche pr�ss�e
/// </summary>
/// <param name="touche">The touche.</param>
/// <returns></returns>
bool Input::getTouche(const SDL_Scancode touche) const
{
	return m_touches[touche];
}


/// <summary>
/// R�cup�re le bouton de souris pr�ss�.
/// </summary>
/// <param name="bouton">The bouton.</param>
/// <returns></returns>
bool Input::getBoutonSouris(const Uint8 bouton) const
{
	return m_boutonsSouris[bouton];
}


/// <summary>
/// D�tecte le mouvement de la souris.
/// </summary>
/// <returns></returns>
bool Input::mouvementSouris() const
{
	if (m_xRel == 0 && m_yRel == 0)
		return false;

	else
		return true;
}


/// <summary>
/// Gets the x.
/// </summary>
/// <returns></returns>
int Input::getX() const
{
	return m_x;
}

/// <summary>
/// Gets the y.
/// </summary>
/// <returns></returns>
int Input::getY() const
{
	return m_y;
}

/// <summary>
/// Gets the x relative.
/// </summary>
/// <returns></returns>
int Input::getXRel() const
{
	return m_xRel;
}

/// <summary>
/// Gets the y relative.
/// </summary>
/// <returns></returns>
int Input::getYRel() const
{
	return m_yRel;
}



/// <summary>
/// Permet d'afficher le pointeur.
/// </summary>
/// <param name="reponse">if set to <c>true</c> [reponse].</param>
void Input::afficherPointeur(bool reponse) const
{
	if (reponse)
		SDL_ShowCursor(SDL_ENABLE);

	else
		SDL_ShowCursor(SDL_DISABLE);
}


/// <summary>
/// ca^pture le pointeur dans la fenetre
/// </summary>
/// <param name="reponse">if set to <c>true</c> [reponse].</param>
void Input::capturerPointeur(bool reponse) const
{
	if (reponse)
		SDL_SetRelativeMouseMode(SDL_TRUE);

	else
		SDL_SetRelativeMouseMode(SDL_FALSE);
}


/// <summary>
/// demande � l'utilisateur de rentrer un nombre.
/// </summary>
/// <param name="defaultValue">The default value.</param>
/// <returns></returns>
float Input::saisieUtilisateur(float defaultValue) {
	//on initialise "saisie" avec "a" par defaut
	std::string saisie = "a";
	float resultat;
	//tant que l'on n'a pas rentrer un nombre ou rien
	while (saisie.compare("a") == 0) {

		//on r�cup�re la saisie
		std::getline(std::cin, saisie);

		//si l'utilisateur n'a rien rentr�, on renvoie la valeur par d�faut
		if (saisie.compare("") == 0) {
			return defaultValue;
		}
		//sinon, si c'est un nombre, on renvoie sa valeur
		try {
			resultat = std::stof(saisie);
		}
		//sinon, on recommence
		catch (const std::invalid_argument) {
			std::cout << "Mauvaise saisie. Recommencez : " << std::endl;
			saisie = "a";
		}
	}
	return resultat;
}