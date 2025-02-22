
#include "Joueurs.h"													// Inclusion du Fichier En-Tête 'Joueurs.h' //
#include "GL/glut.h"													// Inclusion Bib. GLUT (OPENGL) //


extern int PosXRouge;
extern int PosYRouge;
extern int PosXJaune;													// Variables Coordonnées Joueurs Rouge & Jaune issues du Source Principal //
extern int PosYJaune;



void Joueurs::DessinerJoueurRouge()
{

	glPushMatrix();														// On Sauvegarde la Matrice Active avant Translation //
	glTranslated(PosXRouge + 0.5, PosYRouge + 0.5, 0.0);				// On Positionne le Joueur Rouge par ses Coordonnées //
	glColor3d(1.0, 0.0, 0.0);											// De Couleur Rouge //
	glutSolidSphere(0.35, 12, 12);										// On Dessine le Joueur Rouge //
	glPopMatrix();														// On Restaure la Matrice Active après Translation //

}



void Joueurs::DessinerJoueurJaune()
{

	glPushMatrix();														// On Sauvegarde la Matrice Active avant Translation //
	glTranslated(PosXJaune + 0.5, PosYJaune + 0.5, 0.0);				// On Positionne le Joueur Jaune par ses Coordonnées //
	glColor3d(1.0, 1.0, 0.0);											// De Couleur Jaune //
	glutSolidSphere(0.35, 12, 12);										// On Dessine le Joueur Jaune //
	glPopMatrix();														// On Restaure la Matrice Active après Translation //

}