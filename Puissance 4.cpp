																		//  *** Puissance 4 ***  //
																		// En C++ Natif (OPENGL) //
																		//      Fin Mai 2024	 //




#include <string>														// Pour Affichage des Textes //
#include <cstdbool>														// Pour Bool�ens //
#include <ctime>														// Pour Fonction rand() //
#include <Windows.h>													// Pour Masquer la Fen�tre Console //
#include <cstdio>														// Bib. Standard E/S C //
#include <cstdlib>														// Idem //
#include <playsoundapi.h>												// Idem //
#include <fstream>														// Pour Ouvrir Fichier Externe //

#include "Joueurs.h"													// Inclusion Fichier En-T�te 'Joueurs.h' //
#include "GL/freeglut.h"
#include "GL/freeglut_ext.h"
#include "GL/freeglut_std.h"
#include "GL/glut.h"													// Inclusion Bib. GLUT (OPENGL) //




using namespace std;													// Utilisation Espace de Noms Standard //


bool DepartJeu = false;													// Variable Bool�enne de D�marrage du Jeu //
bool FinJeu = false;													// Variable Bool�enne de Fin du Jeu //
bool ValidInit = false;													// Variable Bool�enne de Validation ou pas Choix Joueur (Couleur) au D�part //
bool ValidPose = false;													// Variable Bool�enne de Validation Pose Joueurs //
bool ValidAffRouge = false;												// Variable Bool�enne de Validation Affichage Joueur Rouge //
bool ValidAffJaune = false;												// Variable Bool�enne de Validation Affichage Joueur Jaune //
bool ValidVictoire = false;												// Variable Bool�enne de Validation Victoire d'un Joueur //
int NbColonnes = 7;														// Variable Valeur Nombre Colonnes dans Tableau //
int NbLignes = 6;														// Variable Valeur Nombre Lignes dans Tableau //
int CasesLibres = 42;													// Variable Nombre de Cases Libre (42 au D�part) //
int ChoixJoueur = 0;													// Variable Choix Joueur //
int ChoixTexte = 0;														// Variable Choix Texte a Afficher //
const int TimerMillis = 250;											// Variable Valeur Temps entre 2 Evenements Timer //
char** Matrice;															// Tableau � 2 Dimensions allou� par des Pointeurs dans le TAS //
string s;																// Chaine de Caracteres pour les Textes //


int MouseX = 0;															// Coordonn�e en X Souris (Coordonn�e dans Tableau) //
int MouseY = 0;															// Coordonn�e en Y Souris (Coordonn�e dans Tableau) //
double Normalis�X = 0.0;												// Coordonn�e en X Souris (Valeur Normalis�e) //
double Normalis�Y = 0.0;												// Coordonn�e en Y Souris (Valeur Normalis�e) //
int PosXTexte;															// Coordonn�e en X Position Texte a Afficher //
int PosYTexte;															// Coordonn�e en Y Position Texte a Afficher //


int PosXRouge;															//
int PosYRouge;															// Variables Coordonn�es des Joueurs //
int PosXJaune;															//
int PosYJaune;															//


Joueurs LeJoueurRouge;													//
Joueurs LeJoueurJaune;													// Objets Globaux de la Classe Joueurs //




void SceneAff();														// D�claration Fonction Affichage Sc�ne (Grille du Jeu) //
void DessinerJoueurs();													// D�claration Fonction Affichage des Joueurs //
void DessinerScene();													// D�claration Fonction Affichage des Elements qui Composent la Grille //
void SceneRedim(int x, int y);											// D�claration Fonction Redimensionnement Fen�tre OPENGL //
void SceneClavierSpecial(int key, int x, int y);						// D�claration Fonction Gestion Clavier //
void FonctionSouris(int button, int state, int x, int y);				// D�claration Fonction Gestion Souris //
void TimerPoseJoueur(int value);										// D�claration Fonction Timer Gestion Pose Joueur //
void OuvrirNiveau(char nom_fichier[]);									// D�claration Fonction Ouverture Fichier Externe (En Lecture) pour R�cup�rer Donn�es //
void LibererMemoire();													// D�claration Fonction Lib�ration TAS Allou� //
void AffTexte();														// D�claration Fonction Affichage des diff�rents Textes //
void TestVictoire();													// D�claration Fonction Test Victoire (R�solution de la Grille) //



int main(int argc, char* argv[])										// Fonction Principale du Programme //
{

	FreeConsole();														// On Masque la Console //

	glutInit(&argc, argv);												// Fonction Initialisation GLUT //

	glutInitWindowPosition(785,375);									// Fonction GLUT Position Fen�tre � L'Ecran //
	glutInitWindowSize(350,400);										// Fonction GLUT Taille Fen�tre Jeu //
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);						// Affichage Mode 3 Couleurs & Double Tampon Affichage //
	glutCreateWindow("Puissance 4");									// Fonction GLUT Cr�ation Fen�tre Jeu avec son Titre // 
	glutDisplayFunc(SceneAff);											// Fonction GLUT Affichage //
	glutReshapeFunc(SceneRedim);										// Fonction GLUT Redimensionnement Fen�tre //
	glutSpecialFunc(SceneClavierSpecial);								// Fonction GLUT Gestion Clavier //
	glutMouseFunc(FonctionSouris);										// Fonction GLUT Gestion Souris //
	glutTimerFunc(TimerMillis, TimerPoseJoueur, 0);						// Fonction GLUT Timer Gestion Pose Joueurs //


	char nom_fichier[] = { "Datas\\niveau.txt" };						// Ouverture Fichier externe en Lecture //
	OuvrirNiveau(nom_fichier);											//



	glutMainLoop();														// Fonction GLUT Boucle Principale dans la Fonction main() //


	return 0;															// Retour de la Fonction Principale en Renvoyant la Valeur '0' � WINDOWS //


}



void SceneAff()
{
	glClearColor(0.0, 0.0, 1.0, 1.0);									// On Efface L'Ecran avec la Couleur Bleue //
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);											// Matrice Active -> Vue de Mod�les //

	DessinerScene();													// On Dessine la Scene //

	DessinerJoueurs();													// On Dessine les Joueurs //

	AffTexte();															// On Affiche les Textes � Afficher //

	TestVictoire();														// on execute le Test Victoire (Algorithme R�solution Grille) //



	glutSwapBuffers();													// On Inverse les 2 Tampons d'affichage //

}



void DessinerScene()
{


	glColor3f(0.0, 0.0, 0.0);											// On D�finit la Couleur Noire //

	glBegin(GL_LINES);													// D�but Dessin d'une Ligne de Raccord //
	glVertex2d(0, 6);
	glVertex2d(7, 6);
	glEnd();															// Fin du Dessin //

	glFlush();															// On Ach�ve tout Affichage //


	for (int i = 0; i < NbColonnes; i++)
		for (int j = 0; j < NbLignes; j++)
		{
			switch (Matrice[i][j])										// On Commute sur le Tableau //
			{

			case '0':													// Cas Grille Vide //
			{
				glPushMatrix();											// On Sauvegarde la Matrice Active avant Translation //
				glTranslated(i + 0.5, j + 0.5, 0.0);					// On Positionne la Case Grise par la Position dans le Tableau //
				glColor3d(0.5, 0.5, 0.5);								// De Couleur Grise //
				glutSolidSphere(0.4, 12, 12);							// On Dessine la Case Grise //
				glPopMatrix();											// On Restaure la Matrice Active apr�s Translation //
				break;
			}

			case '1':													// Cas Case Rouge //
			{
				glPushMatrix();											// On Sauvegarde la Matrice Active avant Translation //
				glTranslated(i + 0.5, j + 0.5, 0.0);					// On Positionne la Case Rouge par la Position dans le Tableau //
				glColor3d(1.0, 0.0, 0.0);								// De Couleur Rouge //
				glutSolidSphere(0.35, 12, 12);							// On Dessine la Case Rouge //
				glPopMatrix();											// On Restaure la Matrice Active apr�s Translation //
				break;
			}

			case '2':													// Cas Case Jaune //
			{
				glPushMatrix();											// On Sauvegarde la Matrice Active avant Translation //
				glTranslated(i + 0.5, j + 0.5, 0.0);					// On Positionne la Case Jaune par la Position dans le Tableau //
				glColor3d(1.0, 1.0, 0.0);								// De Couleur Jaune //
				glutSolidSphere(0.35, 12, 12);							// On Dessine la Case Jaune //
				glPopMatrix();											// On Restaure la Matrice Active apr�s Translation //
				break;
			}

			}


		}


		

}



void DessinerJoueurs()
{
	if (DepartJeu && !FinJeu)											// Si on est en Cours de Jeu //
	{
		switch (ChoixJoueur)											// On Commute sur le Choix en Cours Couleur Joueur //
		{
		case 1:															// Cas Rouge //	
		{
			if (ValidAffRouge)											// Si l'affichage est Valid� //
			{
				LeJoueurRouge.DessinerJoueurRouge();					// On Affiche le Joueur Rouge //
			}
			break;
		}

		case 2:															// Cas Jaune //	
		{
			if (ValidAffJaune)											// Si l'affichage est Valid� //
			{
				LeJoueurJaune.DessinerJoueurJaune();					// On Affiche le Joueur Jaune //
			}
			break;
		}

		}

	}
}



void SceneRedim(int x, int y)
{

	glViewport(0, 0, x, y);												// Fonction ViewPort (perspective) //
	glMatrixMode(GL_PROJECTION);										// Matrice de Projection //
	glLoadIdentity();													// On charge Identit� Matrice //
	gluOrtho2D(0.0, (double)NbColonnes, (double)NbLignes, 0.0);			// Fonction de redimensionnement GLUT //


}



void SceneClavierSpecial(int key, int x, int y)
{

	switch (key)														// On Commute sur la Touche Appuy�e //
	{
	case GLUT_KEY_F1:													// Appui sur 'F1' //
	{
		DepartJeu = true;												// D�marrage du Jeu //
		ValidInit = true;												// On Valide le Choix Couleur Joueur au D�marrage //
		FinJeu = false;	
		break;
	}

	case GLUT_KEY_F10:													// Appui sur 'F10' //
	{
		LibererMemoire();												// On Lib�re la M�moire Allou�e //
		exit(0);														// On Quitte le Jeu (Renvoie '0' � WINDOWS ) //
		break;
	}

	case GLUT_KEY_F2:													// Appui sur 'F2' //
	{
		if (ValidInit)													// Si on est amen� a choisir la Couleur du Joueur au D�marrage //
		{
			ChoixJoueur = 1;											// On Choisit Rouge au D�marrage //
			ValidInit = false;											// On a Fini de Choisir //
		}
		break;
	}

	case GLUT_KEY_F3:													// Appui sur 'F3' //
	{
		if (ValidInit)													// Si on est amen� a choisir la Couleur du Joueur au D�marrage //
		{
			ChoixJoueur = 2;											// On Choisit Jaune au D�marrage //
			ValidInit = false;											// On a Fini de Choisir //
		}
		break;
	}

	}


}



void FonctionSouris(int button, int state, int x, int y)
{

	if (DepartJeu && !FinJeu)											// Si on est en Cours de Jeu //
	{
		glutPostRedisplay();											// Rafraichissement de l'affichage //

		Normalis�X = x / 350.0;											// On Calcule les Valeurs Normalis�es de la Souris dans la Fen�tre OPENGL //
		Normalis�Y = y / 400.0;

		MouseX = (int)(Normalis�X *7);									// On Convertit les Valeurs Normalis�es en Valeurs dans le Tableau //
		MouseY = (int)(Normalis�Y *8);									// 7 Colonnes x 6 Lignes dans la Grille //

		if (Matrice[MouseX][MouseY] == '0')								// Si a la Position Cliqu�e il y a une Case Vide //
		{
			switch (ChoixJoueur)										// On Commute sur le Choix Joueur en Cours //
			{
			case 1 :													// Cas Joueur Rouge //
			{
				PosXRouge = MouseX;
				PosYRouge = MouseY;										// On Affecte les Coordonn�es du Joueur Rouge //
				ValidAffRouge = true;									// On Valide l'affichage du Joueur Rouge //
				ValidAffJaune = false;									// On Annule l'autre Joueur //
				ValidPose = true;										// On Valide la Pose Joueur (Pour le Timer) //
				ChoixTexte = 1;											// On Affiche le Texte Ad�quat //
				glutPostRedisplay();									// Rafraichissement Affichage //
				break;
			}

			case 2:														// Cas Joueur Jaune //
			{
				PosXJaune = MouseX;
				PosYJaune = MouseY;										// On Affecte les Coordonn�es du Joueur Jaune //
				ValidAffJaune = true;									// On Valide l'affichage du Joueur Jaune //
				ValidAffRouge = false;									// On Annule l'autre Joueur //
				ValidPose = true;										// On Valide la Pose Joueur (Pour le Timer) //
				ChoixTexte = 2;											// On Affiche le Texte Ad�quat //
				glutPostRedisplay();									// Rafraichissement Affichage //
				break;
			}

			}

		}

	}


}



void TimerPoseJoueur(int value)
{
	if (DepartJeu && !FinJeu && ValidPose)								// Si les Conditions sont R�unies pour executer la Fonction du Timer //
	{
		switch (ChoixJoueur)											// On Commute sur le Choix en Cours du Joueur //
		{
		case 1:															// Cas Joueur Rouge //
		{
			if ((Matrice[PosXRouge][PosYRouge + 1] == '0') && (PosYRouge < 5)) // Si la Case d'en dessous est Vide et que l'on a pas atteint le Bas de la Grille //
			{
				PosYRouge++;											// On Descend d'une Position le Cercle Rouge Joueur (Glissement) //
				glutPostRedisplay();									// Rafraichissement de l'affichage //
			}
			else
			{															// Sinon //
				ValidPose = false;										// Pour Stopper le Glissement et le Timer Pose //
				Matrice[PosXRouge][PosYRouge] = '1';					// On Affecte un Cercle Rouge a l'endroit du Cercle Joueur Rouge //
				ChoixJoueur = 2;										// On Change de Couleur Joueur //
				ChoixTexte = 2;											// On Affiche le Texte Ad�quat
				glutPostRedisplay();									// Rafraichissement de l'affichage //
				CasesLibres--;											// On a Occup� une Case en plus //
				
			}
			break;
		}

		case 2:															// Cas Joueur Jaune //
		{
			if ((Matrice[PosXJaune][PosYJaune + 1] == '0') && (PosYJaune < 5)) // Si la Case d'en dessous est Vide et que l'on a pas atteint le Bas de la Grille //
			{
				PosYJaune++;											// On Descend d'une Position le Cercle Jaune Joueur (Glissement) //
				glutPostRedisplay();									// Rafraichissement de l'affichage //
			}
			else
			{															// Sinon //
				ValidPose = false;										// Pour Stopper le Glissement et le Timer Pose //
				Matrice[PosXJaune][PosYJaune] = '2';					// On Affecte un Cercle Jaune a l'endroit du Cercle Joueur Jaune //
				ChoixJoueur = 1;										// On Change de Couleur Joueur //
				ChoixTexte = 1;											// On Affiche le Texte Ad�quat
				glutPostRedisplay();									// Rafraichissement de l'affichage //
				CasesLibres--;											// On a Occup� une Case en plus //

			}
			break;
		}

		}

	}
	
	glutTimerFunc(TimerMillis, TimerPoseJoueur, 0);						// On R�arme ce Timer //
}



void OuvrirNiveau(char nom_fichier[])
{

	ifstream fichier;													// On lance la Gestion Fichier //
	fichier.open(nom_fichier);											// On ouvre le Fichier //

	fichier >> NbColonnes;												// On r�cup�re le Nombre de Colonnes et de Lignes //
	fichier >> NbLignes;

	Matrice = new char* [NbColonnes];
	for (int i = 0; i < NbColonnes; i++)
																		// Allocation en Espace M�moire adressable (TAS) de la Matrice avec des Pointeurs //
	{
		Matrice[i] = new char[NbLignes];
	}

	for (int i = 0; i < NbColonnes; i++)
		for (int j = 0; j < NbLignes; j++)
		{
			Matrice[i][j] = '0';										// On initialise le Contenu � 0 //
		}


	for (int j = 0; j < NbLignes; j++)
		for (int i = 0; i < NbColonnes; i++)

		{
			fichier >> Matrice[i][j];									// On r�cup�re le Contenu de chaque Colonne et Lignes � partir du Fichier pour la Matrice //
		}


	fichier.close();													// Fermeture Fichier apr�s Lecture Donn�es //

}




void LibererMemoire()
{

	for (int i = 0; i < NbColonnes; i++)								// On Va Supprimer la Matrice Allou� en M�moire // 

		delete[] Matrice[i];
			delete[] Matrice;											// De Mani�re Manuelle avec Instruction delete //

}



void AffTexte()
{

	switch (ChoixTexte)													// On Commute sur le Choix du Texte a Afficher //
	{
	case 0:																// Texte Initial //
	{
		s = "ROUGE (F2) OU JAUNE (F3) ?";								// Texte a Afficher //
		PosXTexte = 11;													//
		PosYTexte = 375;												// Position du Texte //
		PlaySound(TEXT("Datas\\Notif.wav"), NULL, SND_FILENAME | SND_ASYNC); // On Joue le Son de Notification //
		break;
	}

	case 1:																// Texte Rouge Joue //
	{
		s = "ROUGE JOUE";												// Texte a Afficher //
		PosXTexte = 96;													//
		PosYTexte = 375;												// Position du Texte //
		PlaySound(TEXT("Datas\\Notif.wav"), NULL, SND_FILENAME | SND_ASYNC); // On Joue le Son de Notification //
		break;
	}

	case 2:																// Texte Jaune Joue //
	{
		s = "JAUNE JOUE";												// Texte a Afficher //
		PosXTexte = 96;													//
		PosYTexte = 375;												// Position du Texte //
		PlaySound(TEXT("Datas\\Notif.wav"), NULL, SND_FILENAME | SND_ASYNC); // On Joue le Son de Notification //
		break;
	}

	case 3:																// Texte Rouge Gagne //
	{
		s = "ROUGE GAGNE";												// Texte a Afficher //
		PosXTexte = 85;													//
		PosYTexte = 375;												// Position du Texte //
		break;
	}

	case 4:																// Texte Jaune Gagne //
	{
		s = "JAUNE GAGNE";												// Texte a Afficher //
		PosXTexte = 85;													//
		PosYTexte = 375;												// Position du Texte //
		break;
	}

	case 5:																// Texte Match Nul //
	{
		s = "MATCH NUL !";												// Texte a Afficher //
		PosXTexte = 85;													//
		PosYTexte = 375;												// Position du Texte //
		break;
	}

	}


	glMatrixMode(GL_PROJECTION);										// Matrice de Projection //
	glPushMatrix();														// On Sauvegarde la Matrice//
	glLoadIdentity();													// On charge la Matrice � son Identit� //
	gluOrtho2D(0.0, 350.0,400.0, 0.0);									// On d�finit le Minimum / Maximum de la Fenetre //

	glMatrixMode(GL_MODELVIEW);											// Matrice de Vue de Mod�les //
	glPushMatrix();														// On Sauvegarde la Matrice //
	glLoadIdentity();													// On charge la Matrice � son Identit� //

	glColor3d(0.0, 0.0, 0.0);											// Texte en Rouge //

	glRasterPos2i(PosXTexte,PosYTexte);									// Position du Texte //


	void* font = GLUT_BITMAP_TIMES_ROMAN_24;							// On d�finit le Type de Police et sa Taille //

	for (string::iterator i = s.begin(); i != s.end(); ++i)
	{
		char c = *i;													// Fonction GLUT affichage Texte //
		glutBitmapCharacter(font, c);
	}

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();														// On restaure la Matrice de Vue de Mod�les // 

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();														// On restaure la Matrice de Projection //



}



void TestVictoire()
{

	

	if (DepartJeu && !FinJeu)											// Si on est en Cours de Jeu //
	{
		// TEST LIGNES //

		for (int i = 0; i < NbColonnes; i++)							// Pour Parcourir l'ensemble des Colonnes //
			for (int j = 0; j < 3; j++)									// Et les 3 1eres Lignes //
			{
				if ((Matrice[i][j] == '1') && (Matrice[i][j] == Matrice[i][j + 1]) && (Matrice[i][j + 1] == Matrice[i][j + 2]) && (Matrice[i][j + 2] == Matrice[i][j + 3]))
				{														// Si le Joueur est Rouge et 4 Pions Rouge sont align�s en Lignes //
					ValidVictoire = true;								// On Valide la Victoire //
					ChoixTexte = 3;										// Texte Ad�quat a Afficher //
					glutPostRedisplay();								// Rafraichissement Affichage //
					PlaySound(TEXT("Datas\\Victoire.wav"), NULL, SND_FILENAME | SND_ASYNC); // On Joue le Son de Victoire //
					FinJeu = true;										// Validation FIN DU JEU //
				}

				if ((Matrice[i][j] == '2') && (Matrice[i][j] == Matrice[i][j + 1]) && (Matrice[i][j + 1] == Matrice[i][j + 2]) && (Matrice[i][j + 2] == Matrice[i][j + 3]))
				{														// Si le Joueur est Jaune et 4 PionsJaune sont align�s en Lignes //
					ValidVictoire = true;								// On Valide la Victoire //
					ChoixTexte = 4;										// Texte Ad�quat a Afficher //
					glutPostRedisplay();								// Rafraichissement Affichage //
					PlaySound(TEXT("Datas\\Victoire.wav"), NULL, SND_FILENAME | SND_ASYNC); // On Joue le Son de Victoire //
					FinJeu = true;										// Validation FIN DU JEU //
				}

			}


		// TEST COLONNES //

		for(int i = 0; i < 4; i++)										// Pour Parcourir les 4 1�res Colonnes //
			for (int j = 0; j < NbLignes; j++)							// Et l'ensemble des Lignes //
			{
				if ((Matrice[i][j] == '1') && (Matrice[i][j] == Matrice[i+1][j]) && (Matrice[i+1][j] == Matrice[i+2][j]) && (Matrice[i+2][j] == Matrice[i+3][j]))
				{														// Si le Joueur est Rouge et 4 Pions Rouge sont align�s en Colonnes //
					ValidVictoire = true;								// On Valide la Victoire //
					ChoixTexte = 3;										// Texte Ad�quat a Afficher //
					glutPostRedisplay();								// Rafraichissement Affichage //
					PlaySound(TEXT("Datas\\Victoire.wav"), NULL, SND_FILENAME | SND_ASYNC); // On Joue le Son de Victoire //
					FinJeu = true;										// Validation FIN DU JEU //
				}

				if ((Matrice[i][j] == '2') && (Matrice[i][j] == Matrice[i + 1][j]) && (Matrice[i + 1][j] == Matrice[i + 2][j]) && (Matrice[i + 2][j] == Matrice[i + 3][j]))
				{														// Si le Joueur est Jaune et 4 Pions Jaune sont align�s en Colonnes //
					ValidVictoire = true;								// On Valide la Victoire //
					ChoixTexte = 4;										// Texte Ad�quat a Afficher //
					glutPostRedisplay();								// Rafraichissement Affichage //
					PlaySound(TEXT("Datas\\Victoire.wav"), NULL, SND_FILENAME | SND_ASYNC); // On Joue le Son de Victoire //
					FinJeu = true;										// Validation FIN DU JEU //
				}

			}

		// TEST DIAGONALES SENS 1 //

		for (int i = 0; i < 4; i++)										// Pour Parcourir les 4 1�res Colonnes //
			for (int j = 0; j < 3; j++)									// Et les 3 1�res Lignes //
			{
				if ((Matrice[i][j] == '1') && (Matrice[i][j] == Matrice[i + 1][j+1]) && (Matrice[i + 1][j+1] == Matrice[i + 2][j+2]) && (Matrice[i + 2][j+2] == Matrice[i + 3][j+3]))
				{														// Si le Joueur est Rouge et 4 Pions Rouge sont align�s en Diagonale //
					ValidVictoire = true;								// On Valide la Victoire //
					ChoixTexte = 3;										// Texte Ad�quat a Afficher //
					glutPostRedisplay();								// Rafraichissement Affichage //
					PlaySound(TEXT("Datas\\Victoire.wav"), NULL, SND_FILENAME | SND_ASYNC); // On Joue le Son de Victoire //
					FinJeu = true;										// Validation FIN DU JEU //
				}

				if ((Matrice[i][j] == '2') && (Matrice[i][j] == Matrice[i + 1][j + 1]) && (Matrice[i + 1][j + 1] == Matrice[i + 2][j + 2]) && (Matrice[i + 2][j + 2] == Matrice[i + 3][j + 3]))
				{														// Si le Joueur est Jaune et 4 Pions Jaune sont align�s en Diagonale //
					ValidVictoire = true;								// On Valide la Victoire //
					ChoixTexte = 4;										// Texte Ad�quat a Afficher //
					glutPostRedisplay();								// Rafraichissement Affichage //
					PlaySound(TEXT("Datas\\Victoire.wav"), NULL, SND_FILENAME | SND_ASYNC); // On Joue le Son de Victoire //
					FinJeu = true;										// Validation FIN DU JEU //
				}
			}

		// TEST DIAGONALES SENS 2 //

		for (int i = 6; i > 2; i--)										// Pour Parcourir les 4 1�res Colonnes //
			for (int j = 0; j < 3; j++)									// Et les 3 1�res Lignes //
			{
				if ((Matrice[i][j] == '1') && (Matrice[i][j] == Matrice[i - 1][j + 1]) && (Matrice[i - 1][j + 1] == Matrice[i - 2][j + 2]) && (Matrice[i - 2][j + 2] == Matrice[i - 3][j + 3]))
				{														// Si le Joueur est Rouge et 4 Pions Rouge sont align�s en Diagonale //
					ValidVictoire = true;								// On Valide la Victoire //
					ChoixTexte = 3;										// Texte Ad�quat a Afficher //
					glutPostRedisplay();								// Rafraichissement Affichage //
					PlaySound(TEXT("Datas\\Victoire.wav"), NULL, SND_FILENAME | SND_ASYNC); // On Joue le Son de Victoire //
					FinJeu = true;										// Validation FIN DU JEU //
				}

				if ((Matrice[i][j] == '2') && (Matrice[i][j] == Matrice[i - 1][j + 1]) && (Matrice[i - 1][j + 1] == Matrice[i - 2][j + 2]) && (Matrice[i - 2][j + 2] == Matrice[i - 3][j + 3]))
				{														// Si le Joueur est Jaune et 4 Pions Jaune sont align�s en Diagonale //
					ValidVictoire = true;								// On Valide la Victoire //
					ChoixTexte = 4;										// Texte Ad�quat a Afficher //
					glutPostRedisplay();								// Rafraichissement Affichage //
					PlaySound(TEXT("Datas\\Victoire.wav"), NULL, SND_FILENAME | SND_ASYNC); // On Joue le Son de Victoire //
					FinJeu = true;										// Validation FIN DU JEU //
				}
			}


		// TEST MATCH NUL //

		if (!ValidVictoire && CasesLibres == 0)
		{
			ChoixTexte = 5;												// Texte Ad�quat a Afficher //
			glutPostRedisplay();										// Rafraichissement Affichage //
			PlaySound(TEXT("Datas\\Notif.wav"), NULL, SND_FILENAME | SND_ASYNC); // On Joue le Son de Notification //
			FinJeu = true;												// Validation FIN DU JEU //
		}

	}

}