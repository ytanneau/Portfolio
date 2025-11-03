/**
* \page Général 
* 
* \author TANNEAU YANN
* \version 4.0
* \date 11/2024
*
* Ce programme est un Snake
*/

#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

/**
* \def SNAKE_TAILLE_INISIAL
* \brief constante pour la taille du serpent au depart.
*/
#define SNAKE_TAILLE_INISIAL 10

/**
* \def TETE_SERPENT
* \brief constante qui représente la tete du serpent.
*/
#define TETE_SERPENT 'O'

/**
* \def CORPS_SERPENT
* \brief constante qui représente le corp du serpent.
*/
#define CORPS_SERPENT 'X'

/**
* \def POSITION_INITIAL_X
* \brief constante d'emplacement de la tête en X.
*/
#define POSITION_INITIAL_X 40

/**
* \def POSITION_INITIAL_Y
* \brief constante d'emplacement de la tête en Y.
*/
#define POSITION_INITIAL_Y 20

/**
* \def SORTIE
* \brief constante touche d'arrête du jeu.
*/
#define SORTIE 'a'

/**
* \def DEPLACEMENT_HAUT
* \brief constante touche direction haut.
*/
#define DEPLACEMENT_HAUT 'z'

/**
* \def DEPLACEMENT_BAS
* \brief constante touche direction bas.
*/
#define DEPLACEMENT_BAS 's'

/**
* \def DEPLACEMENT_DROIT
* \brief constante touche direction droit.
*/
#define DEPLACEMENT_DROIT 'd'

/**
* \def DEPLACEMENT_GAUCHE
* \brief constante touche direction gauche.
*/
#define DEPLACEMENT_GAUCHE 'q'

/**
* \def ORIGINE_X
* \brief constante origine du jeu en X.
*/
#define ORIGINE_X 1

/**
* \def ORIGINE_Y
* \brief constante origine du jeu en Y.
*/
#define ORIGINE_Y 1

/**
* \def AIRE_DE_JEU_X
* \brief constante definition du jeu en X.
*/
#define AIRE_DE_JEU_X 80

/**
* \def AIRE_DE_JEU_Y
* \brief constante definition du jeu en Y.
*/
#define AIRE_DE_JEU_Y 40

/**
* \def VIDE
* \brief constante caratère vide.
*/
#define VIDE ' '

/**
* \def MUR_SIMBOLE
* \brief constante caratère simbolisent les obstacle.
*/
#define MUR_SIMBOLE '#'

/**
* \def NOMBRE_PAVES
* \brief constante pour le nombre de paves.
*/
#define NOMBRE_PAVES 4

/**
* \def PAVES_LONGEUR
* \brief constante pour la longeur des paves.
*/
#define PAVES_LONGEUR 5

/**
* \def PAVES_LARGEUR
* \brief constante pour la largeur des paves.
*/
#define PAVES_LARGEUR 5

/**
* \def DECALAGE_PAVES
* \brief constante de decalage de bordure paraport a la bordure.
*/
#define DECALAGE_PAVES 2

/**
* \def TEMPS_INISIAL
* \brief constante du temps de départ.
*/
#define TEMPS_INISIAL 250000

/**
* \def TEMPS_MAX
* \brief constante du temps final.
*/
#define TEMPS_MAX 150000

/**
* \def TEMPS_VARIATION
* \brief constante de variation du temps .
*/
#define TEMPS_VARIATION (TEMPS_INISIAL-TEMPS_MAX)/(POMME_A_MANGER-1)

/**
* \def POMME_SIMBOLE
* \brief constante pour le symbole de la pomme.
*/
#define POMME_SIMBOLE '6'

/**
* \def POMME_A_MANGER
* \brief constante de pomme a manger pour ganier.
*/
#define POMME_A_MANGER 10

/**
* \typedef snake_coordonnee
* \brief type tableau d'entier a 2 dimentions de 2 lignes et de SNAKE_TAILLE_INISIAL+POMME_A_MANGER colones
* Le type sanke_coordonnee sert de stockage des coordonnee des élément du serpent
*/
typedef int snake_coordonnee[2][SNAKE_TAILLE_INISIAL+POMME_A_MANGER];

/**
* \typedef terrain_jeu
* \brief type tableau de carcatère a 2 dimentions de AIRE_DE_JEU_Y lignes et de AIRE_DE_JEU_X colones
* Le type terrain_jeu sert de stockage pour les élément du terain de jeu
*/
typedef char terrain_jeu[AIRE_DE_JEU_Y][AIRE_DE_JEU_X];

/**
* \typedef paves_coordonnee
* \brief type tableau d'entier a 2 dimentions de 2 lignes et de NOMBRE_PAVES colones
* Le type paves_coordonnee sert de stockage des coordonnee des paves
*/
typedef int paves_coordonnee[2][NOMBRE_PAVES];


void disableEcho();
void enableEcho();
void initPlateau();
void calculerPaves(paves_coordonnee coordonneePaves);
void ajouterPomme(snake_coordonnee laPosition);
void dessinerPlateau();
void dessinerSerpent(snake_coordonnee laPosition);
void afficher(int, int, char);
void effacer(int, int);
void progresser(snake_coordonnee laPosition, char, bool*, bool*);
void gotoXY(int, int);
int kbhit(void);

terrain_jeu tableauDeJeu;
int tailleSerpent = SNAKE_TAILLE_INISIAL;


/**
* \fn int main()
* \brief Programme principal.
* \return Code de sortie du programme (0 : sortie normale).
* 
* Le programme principal dessine un serpent en mouvement tant que l'utilisateur 
* n'a pas taper sur la touche de fin.
*/
int main(){
	snake_coordonnee laPosition;
	bool arretDuJeu = false;
	bool jeuPerdu = false;
	char direction, directionInterdit;
	char toucheTaper = DEPLACEMENT_DROIT;
    bool pommeManger = false;
	int nombrePommeManger = 0;
    bool jeuGanier = false;
    int pause = TEMPS_INISIAL;

	srand(time(NULL));
	disableEcho();
	system("clear");
	initPlateau();
    
	//initialisation du serpent
	//initialisation de la tete
	laPosition[0][0] = POSITION_INITIAL_X;
	laPosition[1][0] = POSITION_INITIAL_Y;
	//initialisation du corp
	for (int i = 1; i < SNAKE_TAILLE_INISIAL; i++)
	{
		laPosition[0][i] = laPosition[0][0]-i;
		laPosition[1][i] = laPosition[1][0];
	}
	//initialisation de la tete
    for (int i = SNAKE_TAILLE_INISIAL; i < (SNAKE_TAILLE_INISIAL+POMME_A_MANGER); i++)
	{
    	laPosition[0][i] = 0;
		laPosition[1][i] = 0;
    }
    
    ajouterPomme(laPosition);
	// Affichage du serpent aux coordonnées de départ
	dessinerSerpent(laPosition);

	while ((arretDuJeu == false) && (jeuPerdu == false) && (jeuGanier == false))
	{
		// Pause entre chaque mouvement
		usleep(pause);
	
		if (toucheTaper != directionInterdit)
		{
				switch (toucheTaper)
				{

					case DEPLACEMENT_HAUT:
						direction = DEPLACEMENT_HAUT;
						directionInterdit = DEPLACEMENT_BAS;
						break;

					case DEPLACEMENT_BAS:
						direction = DEPLACEMENT_BAS;
						directionInterdit = DEPLACEMENT_HAUT;
						break;

					case DEPLACEMENT_DROIT:
						direction = DEPLACEMENT_DROIT;
						directionInterdit = DEPLACEMENT_GAUCHE;
						break;

					case DEPLACEMENT_GAUCHE:
						direction = DEPLACEMENT_GAUCHE;
						directionInterdit = DEPLACEMENT_DROIT;
						break;

					default:
						break;
				}
		}
		// Calcul les prochaines coordonnées et y affichage le serpent
		progresser(laPosition, direction, &jeuPerdu, &pommeManger);
        if (pommeManger)
        {
            pommeManger = false;
            nombrePommeManger++;

            if (nombrePommeManger == POMME_A_MANGER)
			{
                jeuGanier = true;
            }
            else
			{
                ajouterPomme(laPosition);
                tailleSerpent++;
                pause = pause-TEMPS_VARIATION;
            }
        }
        

		//Vérification q'une touche est appuyée
		if (kbhit())
		{
			//Récupère la valeur de la touche appuyée
			toucheTaper = getchar();
		}
			
			//Teste de la touche pour verifier si il s'agit de la condition de sortie
		if (toucheTaper == SORTIE)
		{
			arretDuJeu = true;
		}
		
	}
	enableEcho();
	gotoXY(1, AIRE_DE_JEU_Y+1);
    return EXIT_SUCCESS;
}


/**********************************************************/
/* Partie 0 parametre et inisalisation */
/**********************************************************/


/**
* \fn disableEcho()
* \brief procedure qui permet de desactiver l'affichage du systeme
*/
void disableEcho() {
    struct termios tty;

    // Obtenir les attributs du terminal
    if (tcgetattr(STDIN_FILENO, &tty) == -1)
	{
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Desactiver le flag ECHO
    tty.c_lflag &= ~ECHO;

    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1)
	{
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}


/**
* \fn enableEcho()
* \brief procedure qui permet d'activer l'affichage du systeme
*/
void enableEcho() {
    struct termios tty;

    // Obtenir les attributs du terminal
    if (tcgetattr(STDIN_FILENO, &tty) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Reactiver le flag ECHO
    tty.c_lflag |= ECHO;

    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}

/**
* \fn initPlateau()
* \brief procedure qui permet l'inisialisation du tableau
*/
void initPlateau(){
	int x, y;
	paves_coordonnee coordonneePaves;
	for (int videY = 0; videY < AIRE_DE_JEU_Y; videY++)
	{
		for (int videX = 0; videX < AIRE_DE_JEU_X; videX++)
		{
			tableauDeJeu[videY][videX] = VIDE;
		}
	}

	y = 0;
	for (int i = 0; i < AIRE_DE_JEU_X; i++){
		tableauDeJeu[y][i] = MUR_SIMBOLE;

	}

	y = AIRE_DE_JEU_Y-1;
	for (int i = 0; i < AIRE_DE_JEU_X; i++)
	{
		tableauDeJeu[y][i] =  MUR_SIMBOLE;

	}

	x = 0;
	for (int i = 0; i < AIRE_DE_JEU_Y; i++)
	{
		tableauDeJeu[i][x] = MUR_SIMBOLE;

	}

	x = AIRE_DE_JEU_X-1;
	for (int i = 0; i < AIRE_DE_JEU_Y; i++)
	{
		tableauDeJeu[i][x] = MUR_SIMBOLE;

	}
	
	tableauDeJeu[ORIGINE_Y-1][AIRE_DE_JEU_X/2-1] = VIDE;
	tableauDeJeu[AIRE_DE_JEU_Y/2-1][ORIGINE_X-1] = VIDE;
	tableauDeJeu[AIRE_DE_JEU_Y/2-1][AIRE_DE_JEU_X-1] = VIDE;
	tableauDeJeu[AIRE_DE_JEU_Y-1][AIRE_DE_JEU_X/2-1] = VIDE;

	calculerPaves(coordonneePaves);
	for (int i = 0; i < NOMBRE_PAVES; i++)
	{
		for (int pavesY = 0; pavesY < PAVES_LARGEUR; pavesY++)
		{
			for (int pavesX = 0; pavesX < PAVES_LONGEUR; pavesX++)
			{
				tableauDeJeu[coordonneePaves[1][i]+pavesY-ORIGINE_Y][coordonneePaves[0][i]+pavesX-ORIGINE_X] = MUR_SIMBOLE;
			}
		}
	}
	dessinerPlateau(tableauDeJeu);
}

/**
* \fn calculerPaves(paves_coordonnee coordonneePaves)
* \param coordonneePaves tableau remplire par la procédure
* \brief procedure qui permet de donnée les coordonnées des paves
*/
void calculerPaves(paves_coordonnee coordonneePaves){
	int x,y;
	bool verifie;
	for (int i = 0; i < NOMBRE_PAVES; i++)
	{
		coordonneePaves[0][i] = 0;
		coordonneePaves[1][i] = 0;
	}

	for (int i = 0; i < NOMBRE_PAVES; i++)
	{
		verifie=true;
		do {
			do {

				x=rand()%(AIRE_DE_JEU_X-PAVES_LONGEUR-DECALAGE_PAVES-1)+(DECALAGE_PAVES+1);
				y=rand()%(AIRE_DE_JEU_Y-PAVES_LARGEUR-DECALAGE_PAVES-1)+(DECALAGE_PAVES+1);

			} while((POSITION_INITIAL_X-SNAKE_TAILLE_INISIAL-PAVES_LONGEUR<=x) &&
			(x<POSITION_INITIAL_X+ORIGINE_X) &&
			(POSITION_INITIAL_Y-PAVES_LARGEUR<=y) &&
			(y<POSITION_INITIAL_Y));

			for (int test = 0; test < i; test++)
			{
				if ((x == coordonneePaves[0][test]) && (y == coordonneePaves[1][test]))
				{
					verifie = false;
				}
			}
		} while(!verifie);	
		coordonneePaves[0][i] = x;
		coordonneePaves[1][i] = y;
	}
}

/**
* \fn dessinerPlateau()
* \brief procedure qui permet d'afficher le tableau
*/
void dessinerPlateau(){
	for (int y = 0; y < AIRE_DE_JEU_Y; y++)
	{
		for (int x = 0; x < AIRE_DE_JEU_X; x++)
		{
			if(tableauDeJeu[y][x] != VIDE)
			{
				afficher(x+ORIGINE_X, y+ORIGINE_Y, tableauDeJeu[y][x]);
			}
		}
		
	}
	
}

/**
* \fn ajouterPomme(snake_coordonnee laPosition))
* \param laPosition pour eviter de faire apparaitre la pomme sur le serpent
* \brief procedure qui permet de placer une pomme sur le terrain de jeu
*/
void ajouterPomme(snake_coordonnee laPosition){
    int x, y;
	bool verifie = false;
    bool verifieSerpent = true;

	do {
		x = rand()%(AIRE_DE_JEU_X-1)+1;
		y = rand()%(AIRE_DE_JEU_Y-1)+1;
        for (int i = 0; i < tailleSerpent; i++){
            if ((laPosition[0][i] == x) && (laPosition[1][i] == y))
			{
                verifieSerpent = false;
            }
        }
		if ((tableauDeJeu[y-1][x-1] == ' ') && (verifieSerpent))
		{
            verifie = true;
        }
	} while(!verifie);
	tableauDeJeu[y-1][x-1] = POMME_SIMBOLE;
	afficher(x, y, POMME_SIMBOLE);
}

/**********************************************************/
/* Partie 1 afficher le serpent dans la console */
/**********************************************************/

/**
* \fn dessinerSerpent(snake_coordonnee laPosition)
* \brief procedure qui permet d'afficher le serpent
* \param laPosition : tableau des cordonée du serpent.
*/
void dessinerSerpent(snake_coordonnee laPosition){
	int x, y;

	for (int i = 1; i < tailleSerpent; i++){
		/**affichage des coordonnée du serpent grace au tableau */
        x = laPosition[0][i];
		y = laPosition[1][i];
        afficher(x, y, CORPS_SERPENT);//affiche le corps
    }

	/** calcule les coordonée x et y pour la tete */
    x = laPosition[0][0];
	y = laPosition[1][0];
	afficher(x, y, TETE_SERPENT);//affiche la tete
	// vide le buffer
	fflush(stdout);
}

/**
* \fn afficher(int x, int y, char c)
* \brief procédure qui affiche un caratère
* \param x : absice du terminal
* \param y : ordonnée du terminal
* \param c : caractère qui sera afficher
*/
void afficher(int x, int y, char c){	
	gotoXY(x, y);
	printf("%c", c);
}

/**
* \fn effacer(int x, int y)
* \brief procédure qui efface un caractère
* \param x : absice du terminal
* \param y : ordonnée du terminal
*/
void effacer(int x, int y){
	afficher(x, y, VIDE);
}

/**
* \fn void gotoXY(int x, int y)
* \brief place le curseur dans le terminal
* \param x : absice du terminal
* \param y : ordonnée du terminal
*/
void gotoXY(int x, int y){ 
    printf("\033[%d;%df", y, x);
}

/**********************************************************/
/* Partie 2 mettre a jour les données du serpent          */
/**********************************************************/

/**
* \fn void progresser(int laPosition[SNAKE_DIMENSION][SNAKE_TAILLE])
* \brief met a jour les coordonnées du serpent, et regarde si il a renconter un élément
* \param laPosition : tableau des corrdonnées du serpent
* \param direction : quelle deplacement le serpent va suivre
* \param jeuPerdu : permet de savoir si il a renconter un obstacle
* \param pommeManger : permet de savoir si une pomme a été manger
*/
void progresser(snake_coordonnee laPosition, char direction, bool *jeuPerdu, bool *pommeManger){
	int suppX, suppY, deplaX, deplaY, coorX, coorY;
	/**Creation des coordonnées à éffacer et appelle de la procédure pour les effacer*/
	suppX = laPosition[0][tailleSerpent-1];
	suppY = laPosition[1][tailleSerpent-1];
	effacer(suppX, suppY);

	switch (direction){

		case DEPLACEMENT_HAUT:
			deplaX = 0;
			deplaY = -1;
			break;

		case DEPLACEMENT_BAS:
			deplaX = 0;
			deplaY = 1;
			break;

		case DEPLACEMENT_DROIT:
			deplaX = 1;
			deplaY = 0;
			break;

		case DEPLACEMENT_GAUCHE:
			deplaX = -1;
			deplaY = 0;
			break;

		default:
			break;
	}
	

	/**Progression puis dessinage */
	for (int i = tailleSerpent - 1; 0 < i ; i--)
	{
		laPosition[0][i] = laPosition[0][i-1];
		laPosition[1][i] = laPosition[1][i-1];
	}
	laPosition[0][0] = (laPosition[0][0] + deplaX);
	laPosition[1][0] = (laPosition[1][0] + deplaY);

	for (int i = 1; i < tailleSerpent; i++)
	{
		if ((laPosition[0][0] == laPosition[0][i]) && (laPosition[1][0] == laPosition[1][i]))
		{
			*jeuPerdu = true;
		}
	}
	
	if ((laPosition[0][0] == AIRE_DE_JEU_X/2) && (laPosition[1][0] == ORIGINE_Y-1))
	{
		laPosition[1][0] = AIRE_DE_JEU_Y;
	}
	else if ((laPosition[0][0] == AIRE_DE_JEU_X/2) && (laPosition[1][0] == AIRE_DE_JEU_Y+1))
	{
		laPosition[1][0] = ORIGINE_Y;
	}
	else if ((laPosition[1][0] == AIRE_DE_JEU_Y/2) && (laPosition[0][0] == ORIGINE_X-1))
	{
		laPosition[0][0] = AIRE_DE_JEU_X;
	}
	else if ((laPosition[1][0] == AIRE_DE_JEU_Y/2) && (laPosition[0][0] == AIRE_DE_JEU_X+1))
	{
		laPosition[0][0] = ORIGINE_X;
	}	
	
	coorX=laPosition[0][0];
	coorY=laPosition[1][0];
	
    if (tableauDeJeu[coorY-1][coorX-1] == POMME_SIMBOLE)
    {
        *pommeManger = true;
        tableauDeJeu[coorY-1][coorX-1]=  VIDE;
    }
    
	if (tableauDeJeu[coorY-1][coorX-1] == MUR_SIMBOLE)
	{
		*jeuPerdu = true;
	}
	
	if (*jeuPerdu == false)
	{
		dessinerSerpent(laPosition);
	}
	
	
}

/**********************************************************/
/* Partie 3 verifier si le jeu est fini                   */
/**********************************************************/

/**Obtenir les touches que l'utilisateur tape au clavier en boucle sans arreter le code*/
/**
* \fn int kbhit(void)
* \brief Indique si une touche a été tapée
* \return 1 si un caractère a été taper
*/
int kbhit(void){
	
	int unCaractere = 0;
	struct termios oldt, newt;
	int ch;
	int oldf;

	// mettre le terminal en mode non bloquant
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
	ch = getchar();

	// restaurer le mode du terminal
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
 
	if(ch != EOF)
	{
		ungetc(ch, stdin);
		unCaractere = 1;
	} 
	return unCaractere;
}
