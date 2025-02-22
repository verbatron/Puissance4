																		//  *** Puissance 4 ***  //
																		// En C++ Natif (OPENGL) //
																		//      Fin Mai 2024	 //




#include <string>														// Pour Affichage des Textes //
#include <cstdbool>														// Pour Booléens //
#include <ctime>														// Pour Fonction rand() //
#include <Windows.h>													// Pour Masquer la Fenêtre Console //
#include <cstdio>														// Bib. Standard E/S C //
#include <cstdlib>														// Idem //
#include <playsoundapi.h>												// Idem //
#include <fstream>														// Pour Ouvrir Fichier Externe //

#include "Joueurs.h"													// Inclusion Fichier En-Tête 'Joueurs.h' //
#include "GL/freeglut.h"
#include "GL/freeglut_ext.h"
#include "GL/freeglut_std.h"
#include "GL/glut.h"													// Inclusion Bib. GLUT (OPENGL) //




using namespace std;													// Utilisation Espace de Noms Standard //


bool DepartJeu = false;													// Variable Booléenne de Démarrage du Jeu //
bool FinJeu = false;													// Variable Booléenne de Fin du Jeu //
bool ValidInit = false;													// Variable Booléenne de Validation ou pas Choix Joueur (Couleur) au Départ //
bool ValidPose = false;													// Variable Booléenne de Validation Pose Joueurs //
bool ValidAffRouge = false;												// Variable Booléenne de Validation Affichage Joueur Rouge //
bool ValidAffJaune = false;												// Variable Booléenne de Validation Affichage Joueur Jaune //
bool ValidVictoire = false;												// Variable Booléenne de Validation Victoire d'un Joueur //
int NbColonnes = 7;														// Variable Valeur Nombre Colonnes dans Tableau //
int NbLignes = 6;														// Variable Valeur Nombre Lignes dans Tableau //
int CasesLibres = 42;													// Variable Nombre de Cases Libre (42 au Départ) //
int ChoixJoueur = 0;													// Variable Choix Joueur //
int ChoixTexte = 0;														// Variable Choix Texte a Afficher //
const int TimerMillis = 250;											// Variable Valeur Temps entre 2 Evenements Timer //
char** Matrice;															// Tableau à 2 Dimensions alloué par des Pointeurs dans le TAS //
string s;																// Chaine de Caracteres pour les Textes //


int MouseX = 0;															// Coordonnée en X Souris (Coordonnée dans Tableau) //
int MouseY = 0;															// Coordonnée en Y Souris (Coordonnée dans Tableau) //
double NormaliséX = 0.0;												// Coordonnée en X Souris (Valeur Normalisée) //
double NormaliséY = 0.0;												// Coordonnée en Y Souris (Valeur Normalisée) //
int PosXTexte;															// Coordonnée en X Position Texte a Afficher //
int PosYTexte;															// Coordonnée en Y Position Texte a Afficher //


int PosXRouge;															//
int PosYRouge;															// Variables Coordonnées des Joueurs //
int PosXJaune;															//
int PosYJaune;															//


Joueurs LeJoueurRouge;													//
Joueurs LeJoueurJaune;													// Objets Globaux de la Classe Joueurs //




void SceneAff();														// Déclaration Fonction Affichage Scène (Grille du Jeu) //
void DessinerJoueurs();													// Déclaration Fonction Affichage des Joueurs //
void DessinerScene();													// Déclaration Fonction Affichage des Elements qui Composent la Grille //
void SceneRedim(int x, int y);											// Déclaration Fonction Redimensionnement Fenêtre OPENGL //
void SceneClavierSpecial(int key, int x, int y);						// Déclaration Fonction Gestion Clavier //
void FonctionSouris(int button, int state, int x, int y);				// Déclaration Fonction Gestion Souris //
void TimerPoseJoueur(int value);										// Déclaration Fonction Timer Gestion Pose Joueur //
void OuvrirNiveau(char nom_fichier[]);									// Déclaration Fonction Ouverture Fichier Externe (En Lecture) pour Récupérer Données //
void LibererMemoire();													// Déclaration Fonction Libération TAS Alloué //
void AffTexte();														// Déclaration Fonction Affichage des différents Textes //
void TestVictoire();													// Déclaration Fonction Test Victoire (Résolution de la Grille) //



int main(int argc, char* argv[])										// Fonction Principale du Programme //
{

	FreeConsole();														// On Masque la Console //

	glutInit(&argc, argv);												// Fonction Initialisation GLUT //

	glutInitWindowPosition(785,375);									// Fonction GLUT Position Fenêtre à L'Ecran //
	glutInitWindowSize(350,400);										// Fonction GLUT Taille Fenêtre Jeu //
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);						// Affichage Mode 3 Couleurs & Double Tampon Affichage //
	glutCreateWindow("Puissance 4");									// Fonction GLUT Création Fenêtre Jeu avec son Titre // 
	glutDisplayFunc(SceneAff);											// Fonction GLUT Affichage //
	glutReshapeFunc(SceneRedim);										// Fonction GLUT Redimensionnement Fenêtre //
	glutSpecialFunc(SceneClavierSpecial);								// Fonction GLUT Gestion Clavier //
	glutMouseFunc(FonctionSouris);										// Fonction GLUT Gestion Souris //
	glutTimerFunc(TimerMillis, TimerPoseJoueur, 0);						// Fonction GLUT Timer Gestion Pose Joueurs //


	char nom_fichier[] = { "Datas\\niveau.txt" };						// Ouverture Fichier externe en Lecture //
	OuvrirNiveau(nom_fichier);											//



	glutMainLoop();														// Fonction GLUT Boucle Principale dans la Fonction main() //


	return 0;															// Retour de la Fonction Principale en Renvoyant la Valeur '0' à WINDOWS //


}



void SceneAff()
{
	glClearColor(0.0, 0.0, 1.0, 1.0);									// On Efface L'Ecran avec la Couleur Bleue //
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);											// Matrice Active -> Vue de Modèles //

	DessinerScene();													// On Dessine la Scene //

	DessinerJoueurs();													// On Dessine les Joueurs //

	AffTexte();															// On Affiche les Textes à Afficher //

	TestVictoire();														// on execute le Test Victoire (Algorithme Résolution Grille) //



	glutSwapBuffers();													// On Inverse les 2 Tampons d'affichage //

}



void DessinerScene()
{


	glColor3f(0.0, 0.0, 0.0);											// On Définit la Couleur Noire //

	glBegin(GL_LINES);													// Début Dessin d'une Ligne de Raccord //
	glVertex2d(0, 6);
	glVertex2d(7, 6);
	glEnd();															// Fin du Dessin //

	glFlush();															// On Achève tout Affichage //


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
				glPopMatrix();											// On Restaure la Matrice Active après Translation //
				break;
			}

			case '1':													// Cas Case Rouge //
			{
				glPushMatrix();											// On Sauvegarde la Matrice Active avant Translation //
				glTranslated(i + 0.5, j + 0.5, 0.0);					// On Positionne la Case Rouge par la Position dans le Tableau //
				glColor3d(1.0, 0.0, 0.0);								// De Couleur Rouge //
				glutSolidSphere(0.35, 12, 12);							// On Dessine la Case Rouge //
				glPopMatrix();											// On Restaure la Matrice Active après Translation //
				break;
			}

			case '2':													// Cas Case Jaune //
			{
				glPushMatrix();											// On Sauvegarde la Matrice Active avant Translation //
				glTranslated(i + 0.5, j + 0.5, 0.0);					// On Positionne la Case Jaune par la Position dans le Tableau //
				glColor3d(1.0, 1.0, 0.0);								// De Couleur Jaune //
				glutSolidSphere(0.35, 12, 12);							// On Dessine la Case Jaune //
				glPopMatrix();											// On Restaure la Matrice Active après Translation //
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
			if (ValidAffRouge)											// Si l'affichage est Validé //
			{
				LeJoueurRouge.DessinerJoueurRouge();					// On Affiche le Joueur Rouge //
			}
			break;
		}

		case 2:															// Cas Jaune //	
		{
			if (ValidAffJaune)											// Si l'affichage est Validé //
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
	glLoadIdentity();													// On charge Identité Matrice //
	gluOrtho2D(0.0, (double)NbColonnes, (double)NbLignes, 0.0);			// Fonction de redimensionnement GLUT //


}



void SceneClavierSpecial(int key, int x, int y)
{

	switch (key)														// On Commute sur la Touche Appuyée //
	{
	case GLUT_KEY_F1:													// Appui sur 'F1' //
	{
		DepartJeu = true;												// Démarrage du Jeu //
		ValidInit = true;												// On Valide le Choix Couleur Joueur au Démarrage //
		FinJeu = false;	
		break;
	}

	case GLUT_KEY_F10:													// Appui sur 'F10' //
	{
		LibererMemoire();												// On Libère la Mémoire Allouée //
		exit(0);														// On Quitte le Jeu (Renvoie '0' à WINDOWS ) //
		break;
	}

	case GLUT_KEY_F2:													// Appui sur 'F2' //
	{
		if (ValidInit)													// Si on est amené a choisir la Couleur du Joueur au Démarrage //
		{
			ChoixJoueur = 1;											// On Choisit Rouge au Démarrage //
			ValidInit = false;											// On a Fini de Choisir //
		}
		break;
	}

	case GLUT_KEY_F3:													// Appui sur 'F3' //
	{
		if (ValidInit)													// Si on est amené a choisir la Couleur du Joueur au Démarrage //
		{
			ChoixJoueur = 2;											// On Choisit Jaune au Démarrage //
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

		NormaliséX = x / 350.0;											// On Calcule les Valeurs Normalisées de la Souris dans la Fenêtre OPENGL //
		NormaliséY = y / 400.0;

		MouseX = (int)(NormaliséX *7);									// On Convertit les Valeurs Normalisées en Valeurs dans le Tableau //
		MouseY = (int)(NormaliséY *8);									// 7 Colonnes x 6 Lignes dans la Grille //

		if (Matrice[MouseX][MouseY] == '0')								// Si a la Position Cliquée il y a une Case Vide //
		{
			switch (ChoixJoueur)										// On Commute sur le Choix Joueur en Cours //
			{
			case 1 :													// Cas Joueur Rouge //
			{
				PosXRouge = MouseX;
				PosYRouge = MouseY;										// On Affecte les Coordonnées du Joueur Rouge //
				ValidAffRouge = true;									// On Valide l'affichage du Joueur Rouge //
				ValidAffJaune = false;									// On Annule l'autre Joueur //
				ValidPose = true;										// On Valide la Pose Joueur (Pour le Timer) //
				ChoixTexte = 1;											// On Affiche le Texte Adéquat //
				glutPostRedisplay();									// Rafraichissement Affichage //
				break;
			}

			case 2:														// Cas Joueur Jaune //
			{
				PosXJaune = MouseX;
				PosYJaune = MouseY;										// On Affecte les Coordonnées du Joueur Jaune //
				ValidAffJaune = true;									// On Valide l'affichage du Joueur Jaune //
				ValidAffRouge = false;									// On Annule l'autre Joueur //
				ValidPose = true;										// On Valide la Pose Joueur (Pour le Timer) //
				ChoixTexte = 2;											// On Affiche le Texte Adéquat //
				glutPostRedisplay();									// Rafraichissement Affichage //
				break;
			}

			}

		}

	}


}



void TimerPoseJoueur(int value)
{
	if (DepartJeu && !FinJeu && ValidPose)								// Si les Conditions sont Réunies pour executer la Fonction du Timer //
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
				ChoixTexte = 2;											// On Affiche le Texte Adéquat
				glutPostRedisplay();									// Rafraichissement de l'affichage //
				CasesLibres--;											// On a Occupé une Case en plus //
				
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
				ChoixTexte = 1;											// On Affiche le Texte Adéquat
				glutPostRedisplay();									// Rafraichissement de l'affichage //
				CasesLibres--;											// On a Occupé une Case en plus //

			}
			break;
		}

		}

	}
	
	glutTimerFunc(TimerMillis, TimerPoseJoueur, 0);						// On Réarme ce Timer //
}



void OuvrirNiveau(char nom_fichier[])
{

	ifstream fichier;													// On lance la Gestion Fichier //
	fichier.open(nom_fichier);											// On ouvre le Fichier //

	fichier >> NbColonnes;												// On récupère le Nombre de Colonnes et de Lignes //
	fichier >> NbLignes;

	Matrice = new char* [NbColonnes];
	for (int i = 0; i < NbColonnes; i++)
																		// Allocation en Espace Mémoire adressable (TAS) de la Matrice avec des Pointeurs //
	{
		Matrice[i] = new char[NbLignes];
	}

	for (int i = 0; i < NbColonnes; i++)
		for (int j = 0; j < NbLignes; j++)
		{
			Matrice[i][j] = '0';										// On initialise le Contenu à 0 //
		}


	for (int j = 0; j < NbLignes; j++)
		for (int i = 0; i < NbColonnes; i++)

		{
			fichier >> Matrice[i][j];									// On récupère le Contenu de chaque Colonne et Lignes à partir du Fichier pour la Matrice //
		}


	fichier.close();													// Fermeture Fichier après Lecture Données //

}




void LibererMemoire()
{

	for (int i = 0; i < NbColonnes; i++)								// On Va Supprimer la Matrice Alloué en Mémoire // 

		delete[] Matrice[i];
			delete[] Matrice;											// De Manière Manuelle avec Instruction delete //

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
	glLoadIdentity();													// On charge la Matrice à son Identité //
	gluOrtho2D(0.0, 350.0,400.0, 0.0);									// On définit le Minimum / Maximum de la Fenetre //

	glMatrixMode(GL_MODELVIEW);											// Matrice de Vue de Modèles //
	glPushMatrix();														// On Sauvegarde la Matrice //
	glLoadIdentity();													// On charge la Matrice à son Identité //

	glColor3d(0.0, 0.0, 0.0);											// Texte en Rouge //

	glRasterPos2i(PosXTexte,PosYTexte);									// Position du Texte //


	void* font = GLUT_BITMAP_TIMES_ROMAN_24;							// On définit le Type de Police et sa Taille //

	for (string::iterator i = s.begin(); i != s.end(); ++i)
	{
		char c = *i;													// Fonction GLUT affichage Texte //
		glutBitmapCharacter(font, c);
	}

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();														// On restaure la Matrice de Vue de Modèles // 

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
				{														// Si le Joueur est Rouge et 4 Pions Rouge sont alignés en Lignes //
					ValidVictoire = true;								// On Valide la Victoire //
					ChoixTexte = 3;										// Texte Adéquat a Afficher //
					glutPostRedisplay();								// Rafraichissement Affichage //
					PlaySound(TEXT("Datas\\Victoire.wav"), NULL, SND_FILENAME | SND_ASYNC); // On Joue le Son de Victoire //
					FinJeu = true;										// Validation FIN DU JEU //
				}

				if ((Matrice[i][j] == '2') && (Matrice[i][j] == Matrice[i][j + 1]) && (Matrice[i][j + 1] == Matrice[i][j + 2]) && (Matrice[i][j + 2] == Matrice[i][j + 3]))
				{														// Si le Joueur est Jaune et 4 PionsJaune sont alignés en Lignes //
					ValidVictoire = true;								// On Valide la Victoire //
					ChoixTexte = 4;										// Texte Adéquat a Afficher //
					glutPostRedisplay();								// Rafraichissement Affichage //
					PlaySound(TEXT("Datas\\Victoire.wav"), NULL, SND_FILENAME | SND_ASYNC); // On Joue le Son de Victoire //
					FinJeu = true;										// Validation FIN DU JEU //
				}

			}


		// TEST COLONNES //

		for(int i = 0; i < 4; i++)										// Pour Parcourir les 4 1ères Colonnes //
			for (int j = 0; j < NbLignes; j++)							// Et l'ensemble des Lignes //
			{
				if ((Matrice[i][j] == '1') && (Matrice[i][j] == Matrice[i+1][j]) && (Matrice[i+1][j] == Matrice[i+2][j]) && (Matrice[i+2][j] == Matrice[i+3][j]))
				{														// Si le Joueur est Rouge et 4 Pions Rouge sont alignés en Colonnes //
					ValidVictoire = true;								// On Valide la Victoire //
					ChoixTexte = 3;										// Texte Adéquat a Afficher //
					glutPostRedisplay();								// Rafraichissement Affichage //
					PlaySound(TEXT("Datas\\Victoire.wav"), NULL, SND_FILENAME | SND_ASYNC); // On Joue le Son de Victoire //
					FinJeu = true;										// Validation FIN DU JEU //
				}

				if ((Matrice[i][j] == '2') && (Matrice[i][j] == Matrice[i + 1][j]) && (Matrice[i + 1][j] == Matrice[i + 2][j]) && (Matrice[i + 2][j] == Matrice[i + 3][j]))
				{														// Si le Joueur est Jaune et 4 Pions Jaune sont alignés en Colonnes //
					ValidVictoire = true;								// On Valide la Victoire //
					ChoixTexte = 4;										// Texte Adéquat a Afficher //
					glutPostRedisplay();								// Rafraichissement Affichage //
					PlaySound(TEXT("Datas\\Victoire.wav"), NULL, SND_FILENAME | SND_ASYNC); // On Joue le Son de Victoire //
					FinJeu = true;										// Validation FIN DU JEU //
				}

			}

		// TEST DIAGONALES SENS 1 //

		for (int i = 0; i < 4; i++)										// Pour Parcourir les 4 1ères Colonnes //
			for (int j = 0; j < 3; j++)									// Et les 3 1ères Lignes //
			{
				if ((Matrice[i][j] == '1') && (Matrice[i][j] == Matrice[i + 1][j+1]) && (Matrice[i + 1][j+1] == Matrice[i + 2][j+2]) && (Matrice[i + 2][j+2] == Matrice[i + 3][j+3]))
				{														// Si le Joueur est Rouge et 4 Pions Rouge sont alignés en Diagonale //
					ValidVictoire = true;								// On Valide la Victoire //
					ChoixTexte = 3;										// Texte Adéquat a Afficher //
					glutPostRedisplay();								// Rafraichissement Affichage //
					PlaySound(TEXT("Datas\\Victoire.wav"), NULL, SND_FILENAME | SND_ASYNC); // On Joue le Son de Victoire //
					FinJeu = true;										// Validation FIN DU JEU //
				}

				if ((Matrice[i][j] == '2') && (Matrice[i][j] == Matrice[i + 1][j + 1]) && (Matrice[i + 1][j + 1] == Matrice[i + 2][j + 2]) && (Matrice[i + 2][j + 2] == Matrice[i + 3][j + 3]))
				{														// Si le Joueur est Jaune et 4 Pions Jaune sont alignés en Diagonale //
					ValidVictoire = true;								// On Valide la Victoire //
					ChoixTexte = 4;										// Texte Adéquat a Afficher //
					glutPostRedisplay();								// Rafraichissement Affichage //
					PlaySound(TEXT("Datas\\Victoire.wav"), NULL, SND_FILENAME | SND_ASYNC); // On Joue le Son de Victoire //
					FinJeu = true;										// Validation FIN DU JEU //
				}
			}

		// TEST DIAGONALES SENS 2 //

		for (int i = 6; i > 2; i--)										// Pour Parcourir les 4 1ères Colonnes //
			for (int j = 0; j < 3; j++)									// Et les 3 1ères Lignes //
			{
				if ((Matrice[i][j] == '1') && (Matrice[i][j] == Matrice[i - 1][j + 1]) && (Matrice[i - 1][j + 1] == Matrice[i - 2][j + 2]) && (Matrice[i - 2][j + 2] == Matrice[i - 3][j + 3]))
				{														// Si le Joueur est Rouge et 4 Pions Rouge sont alignés en Diagonale //
					ValidVictoire = true;								// On Valide la Victoire //
					ChoixTexte = 3;										// Texte Adéquat a Afficher //
					glutPostRedisplay();								// Rafraichissement Affichage //
					PlaySound(TEXT("Datas\\Victoire.wav"), NULL, SND_FILENAME | SND_ASYNC); // On Joue le Son de Victoire //
					FinJeu = true;										// Validation FIN DU JEU //
				}

				if ((Matrice[i][j] == '2') && (Matrice[i][j] == Matrice[i - 1][j + 1]) && (Matrice[i - 1][j + 1] == Matrice[i - 2][j + 2]) && (Matrice[i - 2][j + 2] == Matrice[i - 3][j + 3]))
				{														// Si le Joueur est Jaune et 4 Pions Jaune sont alignés en Diagonale //
					ValidVictoire = true;								// On Valide la Victoire //
					ChoixTexte = 4;										// Texte Adéquat a Afficher //
					glutPostRedisplay();								// Rafraichissement Affichage //
					PlaySound(TEXT("Datas\\Victoire.wav"), NULL, SND_FILENAME | SND_ASYNC); // On Joue le Son de Victoire //
					FinJeu = true;										// Validation FIN DU JEU //
				}
			}


		// TEST MATCH NUL //

		if (!ValidVictoire && CasesLibres == 0)
		{
			ChoixTexte = 5;												// Texte Adéquat a Afficher //
			glutPostRedisplay();										// Rafraichissement Affichage //
			PlaySound(TEXT("Datas\\Notif.wav"), NULL, SND_FILENAME | SND_ASYNC); // On Joue le Son de Notification //
			FinJeu = true;												// Validation FIN DU JEU //
		}

	}

}