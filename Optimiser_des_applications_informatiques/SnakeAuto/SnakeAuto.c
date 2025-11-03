/**
 * @file version3.c
 * @author Korentin Charreteur, Yann Tanneau 1B
 * @brief Programme faisant regarder a l'utilisateur un snake autonome dans le teminale
 * @version 3.0
 * 
 * @copyright Copyright (c) 2024
 * 
 * Ce programme est un snake dans le quel le but est de manger toute les pommes en deplaçant le serpent avec les touches.
 * A chaque pomme manger votre taille augmente et votre vitesse aussi.
 * Certain passage sont crées dans les mur pour y passer.
 */

#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

/**Deifnition des constante */

/**
 * @def TAILLE_SNAKE
 * @brief Constante servant à faire un tableau à double entrées avec un certain nombre de colones par ligne, elle reprsente les coordonées de chaque partie du serpent.
 * 
 * Le tabeaux des colonnes sera sous cette forme : {x1, x2, x3,...}.
 * 
 * Cette constante est modifiable pour changer le jeu
 */
#define TAILLE_SNAKE 10

/**
 * @def DIMENSION
 * @brief Constante servant à faire un tableau à double entrées avec un nombre de de ligne
 * 
 * Avec les lignes ajouter le tableau resemblera a ça : {{x1, x2, x3,...},{y1, y2, y3,...}}
 * Le serpent ne se deplaçant que dans deux plans dimension restera à 2 car il s'agit d'un tableau de coordonées.
 */
#define DIMENSION 2

/**
 * @def X
 * @brief Facilite la lacture du code en montrant de maniere plus intuitife sur quelle axe nous agissons.
 * 
 * laPosition[0][] sera pour l'axe x -> laPosition[X][]
 */
#define X 0

/**
 * @def Y
 * @brief Facilite la lacture du code en montrant de maniere plus intuitife sur quelle axe nous agissons.
 * 
 * laPosition[1][] sera pour l'axe y -> laPosition[Y][]
 */
#define Y 1

/**
 * @def SORTIE
 * @brief Touche de sortie 
 * 
 * Sort du jeu a l'appuie de cette touche
 * 
 * Cette constante est modifiable pour changer le jeu
 */
#define SORTIE 'a'

/**
 * @def TAILLE_TERRAIN_X
 * @brief Taille du terrain de jeu sur l'axe X
 * 
 * Cette constante est modifiable pour changer le jeu
 */
#define TAILLE_TERRAIN_X 80

/**
 * @def TAILLE_TERRAIN_Y
 * @brief Taille du terrain de jeu sur l'axe Y
 * 
 * Cette constante est modifiable pour changer le jeu
 */
#define TAILLE_TERRAIN_Y 40

/**
 * @def BORDURE_GAUCHE
 * @brief position de la bordure gauche et positions gloabale du terrain de jeu sur X, en modifiant l'emplacement de la première coordonée a gauche.
 * 
 * Cette constante est modifiable pour changer le jeu
 */
#define BORDURE_GAUCHE 1

/**
 * @def BORDURE_HAUT
 * @brief position de la bordure haute et positions gloabale du terrain de jeu sur Y, en modifiant l'emplacement de la première coordonée en haut.
 * 
 * Cette constante est modifiable pour changer le jeu
 */
#define BORDURE_HAUT 1

/**
 * @def BORDURE_DROITE
 * @brief Position de la bordure droite.
 * 
 * Cette constante est calculer automatiquement en fonction de la taille du pleateau et de la position de la bordure.
 */
#define BORDURE_DROITE TAILLE_TERRAIN_X + BORDURE_GAUCHE

/**
 * @def BORDURE_BAS
 * @brief Position de la bordure basse.
 * 
 * Cette constante est calculer automatiquement en fonction de la taille du pleateau et de la position de la bordure.
 */
#define BORDURE_BAS TAILLE_TERRAIN_Y + BORDURE_HAUT

/**
 * @def POS_SNAKE_X
 * @brief Definition de la position X du snake au debut dans le plateau de jeu
 * 
 * Cette constante est modifiable pour changer le jeu (valeur numerique seulement)
 */
#define POS_SNAKE_X BORDURE_GAUCHE + 39

/**
 * @def POS_SNAKE_Y
 * @brief Definition de la position Y du snake au debut dans le plateau de jeu 
 * 
 * Cette constante est modifiable pour changer le jeu (valeur numerique seulement)
 */
#define POS_SNAKE_Y BORDURE_HAUT + 19

/**
 * @def EXPLICATIONS
 * @brief Explications des regle du jeu et des touches.
 * 
 * Cette constante est modifiable pour changer le jeu
 */
#define EXPLICATIONS "Votre but est de ne rien faire.\nPour arreter le serpent appuiyer sur 'a' à tout moment.\nDebut dans : "

/**
 * @def NB_SECONDE_EXPLICATIONS
 * @brief Nombre de seconde que reste les explication à l'ecran.
 * 
 * Cette constante est modifiable pour changer le jeu
 */
#define NB_SECONDE_EXPLICATIONS 3

/**
 * @def PAUSE_BOUCLE
 * @brief Pause entre chaque boucle qui fait avancer le serpent, en resumer c'est la vitesse du snake/programme.
 * 
 * Cette constante est modifiable pour changer le jeu
 */
#define PAUSE_BOUCLE 200000

/**
 * @def PAUSE_AVANT_FIN
 * @brief Temps que reste le serpent a la fin du jeu pour laisser le temps a l'utilisateur de se rendre de compte de ce qui se passe.
 * 
 * Cette constante est modifiable pour changer le jeu
 */
#define PAUSE_AVANT_FIN 1000000

/**
 * @def PAUSE_1_SECONDE
 * @brief Definit le temps que met 1 seconde en micro seconde.
 */
#define PAUSE_1_SECONDE 1000000

/**
 * @def ACCELERATION
 * @brief temps en moins que prend la boucle a se faire (10000)
 */
#define ACCELERATION 0

/**
 * @def TETE_SERPENT
 * @brief Definit la tête du serpent.
 * 
 * Cette constante est modifiable pour changer le jeu
 */
#define TETE_SERPENT 'O'

/**
 * @def CORP_SERPENT
 * @brief Definit le corps du serpent.
 * 
 * Cette constante est modifiable pour changer le jeu
 */
#define CORP_SERPENT 'X'

/**
 * @def MUR
 * @brief Charactere qui signifie une mort au toucher pour le serpent.
 * 
 * Cette constante est modifiable pour changer le jeu
 */
#define MUR '#'

/**
 * @def NOMBRE_PAVE
 * @brief Nombre de pavée present dans le jeu.
 * 
 * Cette constante est modifiable pour changer le jeu
 */
#define NOMBRE_PAVE 6

/**
 * @def TAILLE_X_PAVE
 * @brief Taille de tout les pavés en X qui apparaitront.
 * 
 * Cette constante est modifiable pour changer le jeu
 */
#define TAILLE_X_PAVE 5

/**
 * @def TAILLE_Y_PAVE
 * @brief Taille de tout les pavés en Y qui apparaitront.
 * 
 * Cette constante est modifiable pour changer le jeu
 */
#define TAILLE_Y_PAVE 5

/**
 * @def HAUT
 * @brief Definit le caractere qui fait aller le serpent vers le HAUT.
 * 
 * Cette constante est modifiable pour changer le jeu
 */
#define HAUT 'z'

/**
 * @def BAS
 * @brief Definit le caractere qui fait aller le serpent vers le BAS.
 * 
 * Cette constante est modifiable pour changer le jeu
 */
#define BAS 's'

/**
 * @def DROITE
 * @brief Definit le caractere qui fait aller le serpent vers la DROITE.
 * 
 * Cette constante est modifiable pour changer le jeu
 */
#define DROITE 'd'

/**
 * @def GAUCHE
 * @brief Definit le caractere qui fait aller le serpent vers la GAUCHE.
 * 
 * Cette constante est modifiable pour changer le jeu
 */
#define GAUCHE 'q'

/**
 * @def VIDE
 * @brief Servira a effacer et mettre du vide
 */
#define VIDE ' '

/**
 * @def ESPACE_DEVANT_APPARITION
 * @brief Pour laisser un espace minimum plus ou moins grand devant a l'apparition du serpent
 * 
 * Cette constante est modifiable pour changer le jeu
 */
#define ESPACE_DEVANT_APPARITION 3

/**
 * @def ESPACE_BORDURE
 * @brief Pour laisser un espace minimum plus ou moins grand entre les bordure et les pavées.
 * 
 * Cette constante est modifiable pour changer le jeu
 */
#define ESPACE_BORDURE 1

/**
 * @def NB_PAVE_POSSIBLE
 * @brief Calcule du nombre de pavé possible car en cas de surplus de pavé crée une boucle infini se crée quand toute les valeurs possible ont été attribué.
 * 
 * La constante est calulé automatiquement.
 */
#define NB_PAVE_POSSIBLE (TAILLE_TERRAIN_X * TAILLE_TERRAIN_Y) - TAILLE_SNAKE - ESPACE_DEVANT_APPARITION - ((TAILLE_TERRAIN_X + 1) * ESPACE_BORDURE) * 2 - (((TAILLE_TERRAIN_Y + 1) - (ESPACE_BORDURE * 2)) * ESPACE_BORDURE) * 2

/**
 * @def NB_POMMES
 * @brief Nombre de pommes qui seront presente et qui seront necesaire a la victoire.
 * 
 * Cette constante est modifiable pour changer le jeu
 */
#define NB_POMMES 10

/**
 * @def POMME
 * @brief Deifnit le caratere qui represente la pomme.
 * 
 * Cette constante est modifiable pour changer le jeu
 */
#define POMME '6'

/**
 * @def NOMBRE_DE_TROU_CALCULE
 * @brief Definit le nombre de trou present dans le plateau pour le calcule.
 * 
 * Cette constante n'est pas modifiable sans modifier le code
 */
#define NOMBRE_DE_TROU_CALCULE 5

/**
 * @def CHEMAIN_NORMALE
 * @brief Sert a mieux se reperer dans le code pour savoir qu'elle direction doit etre calucler
 * 
 * Cette constante peu etre changer mais ne fera rien
 */
#define CHEMAIN_NORMALE 'n'

/**
 * @def CHEMAIN_DROITE
 * @brief Sert a mieux se reperer dans le code pour savoir qu'elle direction doit etre calucler
 * 
 * Cette constante peu etre changer mais ne fera rien
 */
#define CHEMAIN_DROITE 'd'

/**
 * @def CHEMAIN_GAUCHE
 * @brief Sert a mieux se reperer dans le code pour savoir qu'elle direction doit etre calucler
 * 
 * Cette constante peu etre changer mais ne fera rien
 */
#define CHEMAIN_GAUCHE 'g'

/**
 * @def CHEMAIN_HAUT
 * @brief Sert a mieux se reperer dans le code pour savoir qu'elle direction doit etre calucler
 * 
 * Cette constante peu etre changer mais ne fera rien
 */
#define CHEMAIN_HAUT 'h'

/**
 * @def CHEMAIN_BAS
 * @brief Sert a mieux se reperer dans le code pour savoir qu'elle direction doit etre calucler
 * 
 * Cette constante peu etre changer mais ne fera rien
 */
#define CHEMAIN_BAS 'b'


/**Definition des differents types de tableaux utilisé */

/**
 * @typedef t_positionSnake
 * @brief Centralise une definition de tableau et allége la lecture dans les prototypes et procedures.
 */
typedef int t_positionSnake[DIMENSION][TAILLE_SNAKE + NB_POMMES];

/**
 * @typedef t_plateau
 * @brief Centralise une definition de tableau et allége la lecture dans les prototypes et procedures.
 */
typedef char t_plateau[BORDURE_DROITE][BORDURE_BAS];

/**Protorypes */

void afficher(int, int, char);
void effacer(int, int);
void dessinerSerpent(t_positionSnake);
void progresser(t_positionSnake, char, bool *, bool *);
void initPlateau();
void dessinerPlateau();
void calculeChemin(t_positionSnake, char);
void gotoXY(int, int);
int kbhit();
void disableEcho();
void enableEcho();
void ajouterPomme(t_positionSnake);
void calculeDeDistance(int, int, int distance[NOMBRE_DE_TROU_CALCULE], char chemain[NOMBRE_DE_TROU_CALCULE]);
char choixDirection(int, int);

/**
 * @var plateau
 * @brief Definition du plateau pour qu'il soit visible et utilisable a toutes les procedures et fonctions.
 */
t_plateau plateau;

t_plateau tabChemin;

/**
 * @var tailleSnakeJeu
 * @brief Initialisation d'une variable qui changera lors du jeu mais avec une valeur de base quand meme.
 */
int tailleSnakeJeu = TAILLE_SNAKE;

/**
 * @var score
 * @brief Definiti le score pour savoir ou nous en somme au niveau du nombre de pomme.
 */
int score = 0;

/**
 * @var lesPommes
 * @brief tableau de position des pommes.
 */
int lesPommes[DIMENSION][NB_POMMES] = {{75, 75, 78, 2, 8, 78, 74, 2, 72, 5}, { 8, 39, 2, 2, 5, 39, 33, 38, 35, 2}};

/**
 * @var lesPaves
 * @brief tableau de position des premiere coordonnées des pavées
 */
int lesPaves[DIMENSION][NOMBRE_PAVE] = {{ 3, 74, 3, 74, 38, 38},  { 3, 3, 34, 34, 21, 15}};

/**
 * @fn int main()
 * @brief Partie principale du programme.
 * 
 * @return int 
 */
int main(){
	t_positionSnake laPosition;
	char direction, toucheTaper;
	int acceleration = 0;
	bool collision = false;
	bool fin = true;
	bool pommeManger = false;
	int deplacement = 0;

	// Pour un aleatoir plus aleatoir sinon rand enverais a chaque fois la meme valeur
	srand(time(NULL));

	// Afficher les explications et faire attendre le nombre de seconde qu'il faut
	printf("%s", EXPLICATIONS);

	for (int i = 0; i < NB_SECONDE_EXPLICATIONS; i++)
	{
		printf("%d ", NB_SECONDE_EXPLICATIONS - i);
		fflush(stdout);
		usleep(PAUSE_1_SECONDE);
	}

	// Debut de la prise en compte du temps cpu
	clock_t begin = clock();

	// Initialisation des premieres valeurs du tableau a double entrées
	laPosition[X][0] = POS_SNAKE_X;
	laPosition[Y][0] = POS_SNAKE_Y;
	/*
	Calcule des premieres coordonnée qui servirons a afficher le serpent en entier dé le debut.
	Si l'on veut que le reste du corps du serpent sorte de la partie precedente (comme si il etait recrocviller sur une case) il faut enlever cette partie 
	*/
	for (int i = 1; i < tailleSnakeJeu; i++)
	{
		laPosition[X][i] = laPosition[X][0] - i;
		laPosition[Y][i] = laPosition[Y][0];
	}

	// Annule toutes les touches appuyer par l'utilisateur durant les explication
	while (kbhit())
	{
		getchar();
	}
	direction = DROITE;

	/*
	Efface le terminal
	Desactive l'écrture des touches tapées au clavier sans en empécher la lecture
	*/
	system("clear");
	disableEcho();

	// Afficher le plateau et initialiser les pavé ainsi que le premiere pomme
	initPlateau();

	// Ajouter la premiere pomme sur le plateau de jeu
	ajouterPomme(laPosition);

	// Calculer le premier chemin vers la pomme
	calculeChemin(laPosition, direction);

	// Dessine le serpent pour la premiere fois*/
	dessinerSerpent(laPosition);
	
	// Condition infini jusqu'a ce qu'il y est une condition qui fasse changer fin en false
	while (fin)
	{
		usleep((PAUSE_BOUCLE - acceleration));
		// Verifie si une touche est appuyer, si oui alors la toucheTaper change
		if (kbhit())
		{
			// Recupere le 1er caractere taper, ex : 'z''q''s''d' la touche taper sera 'z' il restera 'q''s''d' pour la prochaine boucle si l'utilisateur ne tape rien, si il saisie quelque chose les caracteres vont s'ajouter et crée une latance entre sa volonter et le programme
			toucheTaper = getchar();
		}

		direction = tabChemin[laPosition[X][0]][laPosition[Y][0]];

		// Progression, reception de si il y a collision ou non, puis vidage du buffer
		progresser(laPosition, direction, &collision, &pommeManger);
		fflush(stdout);
		deplacement++;

		// Une pomme a etait goulument manger
		if (pommeManger)
		{
			// Incrementation des variable de jeu
			score++;

			// Augmentation de la variable diminuant la pause de la boucle faisant ainsi "accelerer" le snake
			acceleration = score * ACCELERATION;

			// Verification du score pour ne pas afficher une pomme en trop a la fin
			if (score < NB_POMMES)
			{
				ajouterPomme(laPosition);
				calculeChemin(laPosition, direction);
			}
			
			pommeManger = false;
		}

		/*
		Teste si la touche tapé est le caractere de sortie si il s'agit de la condition de sortie, si il y a eu une collision ou si le core maximum est atteint :
		- Effacer le cmd
		- Redonner la capaciter d'afficher ce qui est taper dans le cmd
		- Changement de la condition de sortie de la boucle infinie en false
		*/
		if ((toucheTaper == SORTIE) || collision || (score == NB_POMMES))
		{
			usleep(PAUSE_AVANT_FIN);
			system("clear");
			enableEcho();
			printf("\n");
			fin = false;
		}
	}

	// Arret de la prise en compte du temps CPU
	clock_t end = clock();
	printf("Nombre de déplacement = %d\n", deplacement);

	// Calcule du temps CPU
	double tmpsCPU = ((end - begin)*1.0) / CLOCKS_PER_SEC;
	printf( "Temps CPU = %.3f secondes\n",tmpsCPU);

   return EXIT_SUCCESS;
}

/**
 * @fn void afficher(int x, int y, char c)
 * @brief Affiche le caractere qu'il ressoit en parametre a la position x,y en y deplaçant le curseur
 * 
 * @param x : Coordonées x a la quelle doit etre afficher le caractere
 * @param y : Coordonées y a la quelle doit etre afficher le caractere
 * @param c : Caractere a afficher
 */
void afficher(int x, int y, char c){
	gotoXY(x, y);
	printf("%c", c);
	fflush(stdout);
}

/**
 * @fn void effacer(int x, int y)
 * @brief Efface le caractere qui se trouve au coordonées x,y donnés en parametre
 * 
 * @param x : Coordonées x a la quelle doit etre afficher le caractere VIDE
 * @param y : Coordonées y a la quelle doit etre afficher le caractere VIDE
 */
void effacer(int x, int y){
	afficher(x, y, VIDE);
}

/**
 * @fn void dessinerSerpent(t_positionSnake laPosition)
 * @brief Desiner le snake en utilisant les coordonées present dans le tableau a double entrés
 * 
 * @param laPosition : Tableau de coordonnées du serpent qui sera afficher.
 */
void dessinerSerpent(t_positionSnake laPosition){
	int x, y;

	// Affichage des coordonnée du serpent grace au tableau
	for (int i = 1; i < tailleSnakeJeu; i++)
	{
		// Teste pour Afficher les parties de serpent que si elles sont compris dans les bordures
		if ((laPosition[X][i] > BORDURE_GAUCHE - 1) 
		 && (laPosition[Y][i] > BORDURE_HAUT - 1)
		 && (laPosition[X][i] < BORDURE_DROITE)
		 && (laPosition[Y][i] < BORDURE_BAS))
		{
			x = laPosition[X][i];
			y = laPosition[Y][i];
			afficher(x, y, CORP_SERPENT);
		}
    }

	// Affichage de la tete aux premieres coordonées du tableau si elle dans les bordures
    if ((laPosition[X][0] > BORDURE_GAUCHE - 1) 
	 && (laPosition[Y][0] > BORDURE_HAUT - 1)
	 && (laPosition[X][0] < BORDURE_DROITE)
	 && (laPosition[Y][0] < BORDURE_BAS))
	{
		x = laPosition[X][0];
		y = laPosition[Y][0];
		afficher(x, y, TETE_SERPENT);
	}
}

/**
 * @fn void progresser(t_positionSnake laPosition, char direction, bool *statut, bool *pommeManger)
 * @brief Faire avancer le snake dans la direction reçu en parametre, tout en verifiant si il y a eu une collision ou si il a manger une pomme.
 * 
 * @param laPosition : Tableau de coordonnées du serpent.
 * @param direction : Direction dans la quelle doit avancer le serpent.
 * @param statut : Renvoie si le serpent a taper quelque chose ou non.
 * @param pommeManger : Renvoie si le serpent a manger un pomme ou non.
 */
void progresser(t_positionSnake laPosition, char direction, bool *statut, bool *pommeManger){
	int suppX, suppY;

	// Verifie si la patie a supprimer est encore visible pour ne pas faire bouger le curseur inutilement et effacer des choses non voulu 
	if ((laPosition[X][tailleSnakeJeu - 1] > BORDURE_GAUCHE - 1)
	 && (laPosition[Y][tailleSnakeJeu - 1] > BORDURE_HAUT - 1)
	 && (laPosition[X][tailleSnakeJeu - 1] < BORDURE_DROITE)
	 && (laPosition[Y][tailleSnakeJeu - 1] < BORDURE_BAS))
	{
		// Creation des coordonnée a effacer et appelle de la precedure pour les effacer
		suppX = laPosition[X][tailleSnakeJeu - 1];
		suppY = laPosition[Y][tailleSnakeJeu - 1];
		effacer(suppX, suppY);
	}
	
	// Calcule des coordonées x et y pour tout le reste du corp en partant de la fin
	for (int i = 1; i < tailleSnakeJeu; i++)
	{
		laPosition[X][(tailleSnakeJeu - i)] = laPosition[X][(tailleSnakeJeu - i - 1)];
		laPosition[Y][(tailleSnakeJeu - i)] = laPosition[Y][(tailleSnakeJeu - i - 1)];
	}

	// Progression da la coordonnée reference du tableau suivant la direction donnée puis transmision du tableau a "dessinerSerpent" 
	if (direction == DROITE)
	{
		laPosition[X][0] = laPosition[X][0] + 1;
	}
	else if (direction == GAUCHE)
	{
		laPosition[X][0] = laPosition[X][0] - 1;
	}
	else if (direction == HAUT)
	{
		laPosition[Y][0] = laPosition[Y][0] - 1;
	}
	else if (direction == BAS)
	{
		laPosition[Y][0] = laPosition[Y][0] + 1;
	}

	// Verification de passage dans un troue quelconque dans la bordure de n'importe quelle coté 
	if (laPosition[X][0] == BORDURE_DROITE)
	{
		laPosition[X][0] = BORDURE_GAUCHE - 1;
	}
	else if (laPosition[X][0] == BORDURE_GAUCHE - 1)
	{
		laPosition[X][0] = BORDURE_DROITE - 1;
	}
	else if (laPosition[Y][0] == BORDURE_BAS)
	{
		laPosition[Y][0] = BORDURE_HAUT - 1;
	}
	else if (laPosition[Y][0] == BORDURE_HAUT - 1)
	{
		laPosition[Y][0] = BORDURE_BAS - 1;
	}

	// Verification de si la touche de direction est differente de celles de SORTIE pour ne pas afficher le serpent une fois de trop 
	if (!(*statut))
	{
		dessinerSerpent(laPosition);
	}
	
	// Verification d'une collision a un mur a l'aide du tableau plateau 
	if (plateau[laPosition[X][0]][laPosition[Y][0]] == MUR)
	{
		*statut=true;
	}

	// Verification de si la pomme est manger par la tete 
	if ((laPosition[X][0] == lesPommes[X][score])
	 && (laPosition[Y][0] == lesPommes[Y][score]))
	{
		*pommeManger = true;
	}
}

/**
 * @fn void initPlateau()
 * @brief Initialisation du tableau du plateau pour ensuite afficher chaque element tableau.
 */
void initPlateau(){
	int paveX, paveY, paveACree; 
	// int nbPaveCree; 

	// Verification de si le nombre de pavé ne depasse pas la capaciter maximum du plateau
	if (NOMBRE_PAVE > NB_PAVE_POSSIBLE)
	{
		paveACree = NB_PAVE_POSSIBLE;
	}
	else{
		paveACree = NOMBRE_PAVE;
	}

	// int posiPave[DIMENSION][paveACree];

	// Initialisation des bordures du plateau 
	for (int initX = 0; initX < BORDURE_DROITE; initX++)
	{
		for (int initY = 0; initY < BORDURE_BAS; initY++)
		{
			// remplissage du tableau par du vide 
			plateau[initX][initY] = VIDE;

			// Remplacement du vide a l'emplacement des bordures par des mur en laissant un troue au milieux de chaque coté 
			if ((((initY == BORDURE_HAUT) // Constrution de la bordure haute
			 ||   (initY == BORDURE_BAS - 1)) // Constrution de la bordure basse
			 &&  ((BORDURE_GAUCHE <= initX) && (initX <= BORDURE_DROITE) && (initX != ((TAILLE_TERRAIN_X + BORDURE_GAUCHE) / 2)))) // Verification de position voulu
			 || (((initX == BORDURE_GAUCHE) // Constrution de la bordure gauche
			 ||   (initX == BORDURE_DROITE - 1)) // Constrution de la bordure droite
			 &&  ((BORDURE_HAUT <= initY) && (initY <= BORDURE_BAS) && (initY != ((TAILLE_TERRAIN_Y + BORDURE_HAUT) / 2))))) // Verification de position voulu
			{
				plateau[initX][initY] = MUR;
			}	
		}
	}

	// Intialisation de toutes les valeurs d'un pavé en mur 
	for (int contPave = 0; contPave < paveACree; contPave++)
	{
		// Initialisation des valeurs a utiliser dans des variable pour ne pas perdre celles du tableau 
		paveX = lesPaves[X][contPave];
		paveY = lesPaves[Y][contPave];

		// Creation des pave au coordonées aleatoirs dans le tableau du plateau 
		for (int x = 0; x < TAILLE_X_PAVE; x++)
		{
			for (int y = 0; y < TAILLE_Y_PAVE; y++)
			{
				plateau[paveX][paveY] = MUR;
				paveY++;
			}

			// paveY ayant etait modifié on lui redonne la valeur qu'il avait du tableau 
			paveY = lesPaves[Y][contPave];
			paveX++;
		}
	}
	
	dessinerPlateau();
}

/**
 * @fn void dessinerPlateau()
 * @brief Afficher tout les elements du tableau plateau, le vide, les murs ... 
 */
void dessinerPlateau(){
	for (int x = 0; x < BORDURE_DROITE; x++)
	{
		for (int y = 0; y < BORDURE_BAS; y++)
		{
			afficher(x, y, plateau[x][y]);
		}
	}
}

/**
 * @fn void ajouterPomme(t_positionSnake laPosition)
 * @brief Ajoute un pomme sur le plateau, et partage les nouvelle coordonées aux varables globales.
 * 
 * @param laPosition : Tableau de coordonnées du serpent, aux quelles la pomme ne doit pas apparaitre.
 */
void ajouterPomme(t_positionSnake laPosition){

	afficher(lesPommes[X][score], lesPommes[Y][score], POMME);
}

/**
 * @fn void calculeChemin()
 * @brief Calcule l'itinerraire du serpent pour aller a la pomme de maniere direct.
 * 
 * Le calcule se fait un mettant un curseur sur les coordonées de la tete du serpent.
 * En faisant bouger ce curseur dans la bonne direction on ajoute les directions correcte que doit prendre le serpent a chaque coordonée dans un tableau de type plateau.
 * Donc a chaque nouvelle coordonée le serpent sait ou aller en lisant la direction qu'il y a aux coordonées de sa tete dans un tableau.
 * 
 * Le calcule se fait juste apres l'affichage d'une pomme.
 * C'est l'equivalant d'un calque ou d'un gps qui vous dit ou aller a l'instant T.
 */
void calculeChemin(t_positionSnake laPosition, char directionActuelle){
	int posX, posY, posXVoulu, posYVoulu;
	int distance[NOMBRE_DE_TROU_CALCULE];
	int nbDeplacement;
	int chemainAPrendre = 0;

	char direction, modeChoisiParCalc;
	char ordreDesChemains[NOMBRE_DE_TROU_CALCULE];

	bool finDeCalcule;
	bool vraiFinDeCalcule;

	vraiFinDeCalcule = false;
	finDeCalcule = false;

	nbDeplacement = 0;

	posX = laPosition[X][0];
	posY = laPosition[Y][0];

	// Calcule et trei des distances avec tout les passages
	calculeDeDistance(posX, posY, distance, ordreDesChemains);

	modeChoisiParCalc = ordreDesChemains[chemainAPrendre];

	// Switch qui en fonction du chemin choisi met la cible a un certain endroit
	switch (modeChoisiParCalc)
	{
	case CHEMAIN_NORMALE:
		posXVoulu = lesPommes[X][score];
		posYVoulu = lesPommes[Y][score];
		break;

	case CHEMAIN_DROITE:
		posXVoulu = BORDURE_DROITE - 2;
		posYVoulu = ((TAILLE_TERRAIN_Y + BORDURE_HAUT) / 2);
		break;

	case CHEMAIN_GAUCHE:
		posXVoulu = BORDURE_GAUCHE + 1;
		posYVoulu = ((TAILLE_TERRAIN_Y + BORDURE_HAUT) / 2);
		break;

	case CHEMAIN_HAUT:
		posXVoulu = ((TAILLE_TERRAIN_X + BORDURE_HAUT) / 2);
		posYVoulu = BORDURE_HAUT + 1;
		break;

	case CHEMAIN_BAS:
		posXVoulu = ((TAILLE_TERRAIN_X + BORDURE_HAUT) / 2);
		posYVoulu = BORDURE_BAS - 2;
		break;

	default:
		break;
	}

	// Calcule du chemain totale jusqu'a la pomme
	while (!vraiFinDeCalcule)
	{
		// Calcule du chemain jusqu'a la cible
		while (!finDeCalcule)
		{
			// Cible a droite
			if (posX < posXVoulu)
			{
				direction = DROITE;

				// Je ne l'explique qu'une fois pour ne pas me repeter :
				// Si le curseur essaye de retourner sur une coordonnée du serpent moins le nombre de deplacement car le serpent bougera
				// Alors il teste si il peut aller vers le bas sans rien toucher si il touche il essaye vers le haut, si il ne peut rien faire il ira vers la droite
				for (int i = 0; i < tailleSnakeJeu; i++)
				{
					if ((((posX + 1) == laPosition[X][i]) && (posY == laPosition[Y][i]))
					 || (plateau[posX + 1][posY] == MUR)
					 || (directionActuelle == GAUCHE))
					{
						if ((plateau[posX][posY + 1] != MUR) && (laPosition[Y][i] != posY + 1) && (posY < posYVoulu))
						{
							direction = BAS;
							i = tailleSnakeJeu;
						}
						else if ((plateau[posX][posY - 1] != MUR) && (laPosition[Y][i] != posY - 1) && (posY > posYVoulu))
						{
							direction = HAUT;
							i = tailleSnakeJeu;
						}
						else if ((laPosition[X][i] != posX - 1) && (directionActuelle != DROITE))
						{
							direction = GAUCHE;
							i = tailleSnakeJeu;
						}
						else
						{
							direction = DROITE;
						}
					}
				}
			}
			// Cible a gauche
			else if (posX > posXVoulu)
			{
				direction = GAUCHE;

				// Voir "Cible a droite" pour plus d'explication
				for (int i = 0; i < tailleSnakeJeu; i++)
				{
					if ((((posX - 1) == laPosition[X][i]) && (posY == laPosition[Y][i]))
					 || (plateau[posX - 1][posY] == MUR)
					 || (directionActuelle == DROITE))
					{
						if ((plateau[posX][posY + 1] != MUR) && !((laPosition[Y][i] == posY + 1) && (laPosition[X][i] == posX)) && (posY < posYVoulu))
						{
							direction = BAS;
							i = tailleSnakeJeu;
						}
						else if ((plateau[posX][posY - 1] != MUR) && !((laPosition[Y][i] == posY - 1) && (laPosition[X][i] == posX)) && (posY > posYVoulu))
						{
							direction = HAUT;
							i = tailleSnakeJeu;
						}
						else if ((laPosition[X][i] != posX + 1) && (directionActuelle != GAUCHE))
						{
							direction = DROITE;
							i = tailleSnakeJeu;
						}
						else
						{
							direction = GAUCHE;
						}
					}
				}
			}
			// Cible au dessus ou en dessous du serpent
			else
			{
				// Cible en dessous du serpent
				if (posY < posYVoulu)
				{
					direction = BAS;

					// Voir "Cible a droite" pour plus d'explication
					for (int i = 0; i < tailleSnakeJeu; i++)
					{
						if ((((posY + 1) == laPosition[Y][i]) && (posX == laPosition[X][i]))
						 || (plateau[posX][posY + 1] == MUR)
						 || (directionActuelle == HAUT))
						{
							if ((plateau[posX + 1][posY] != MUR) && !((laPosition[X][i] == posY + 1) && (laPosition[Y][i] == posX)) && (directionActuelle != GAUCHE))
							{
								direction = DROITE;
								i = tailleSnakeJeu;
							}
							else if ((plateau[posX - 1][posY] != MUR) && !((laPosition[X][i] == posY - 1) && (laPosition[Y][i] == posX)) && (directionActuelle != DROITE))
							{
								direction = GAUCHE;
								i = tailleSnakeJeu;
							}
							else if ((laPosition[Y][i] != posY - 1) && (directionActuelle != BAS))
							{
								direction = HAUT;
								i = tailleSnakeJeu;
							}
							else
							{
								direction = BAS;
							}
						}
					}
				}
				// Cible au dessus du serpent
				else if (posY > posYVoulu)
				{
					direction = HAUT;

					// Voir "Cible a droite" pour plus d'explication
					for (int i = 0; i < tailleSnakeJeu; i++)
					{
						if ((((posY - 1) == laPosition[Y][i]) && (posX == laPosition[X][i])) 
						 || (plateau[posX][posY - 1] == MUR)
						 || (directionActuelle == BAS))
						{
							if ((plateau[posX + 1][posY] != MUR) && !((laPosition[X][i] == posY + 1) && (laPosition[Y][i] == posX)) && (directionActuelle != GAUCHE))
							{
								direction = DROITE;
								i = tailleSnakeJeu;
							}
							else if ((plateau[posX - 1][posY] != MUR) 
								 && !((laPosition[X][i] == posY - 1) && (laPosition[Y][i] == posX))
								 &&  (directionActuelle != DROITE))
							{
								direction = GAUCHE;
								i = tailleSnakeJeu;
							}
							else if ((laPosition[Y][i] != posY + 1) && (directionActuelle != HAUT))
							{
								direction = BAS;
								i = tailleSnakeJeu;
							}
							else
							{
								direction = HAUT;
							}
						}
					}
				}
				// Serpent sur la Cible
				else if (posY == posYVoulu)
				{
					
					// Afin d'eviter que le serpent ne ce deplace trop loins dans ça direction actuelle 
					// On fait en sorte que ça derniere direction soit celle dont il aurait besoin pour aller dans un passage
					switch (modeChoisiParCalc)
					{
					case CHEMAIN_NORMALE:
						direction = directionActuelle;
						break;

					case CHEMAIN_DROITE:
						direction = DROITE;
						break;

					case CHEMAIN_GAUCHE:
						direction = GAUCHE;
						break;

					case CHEMAIN_HAUT:
						direction = HAUT;
						break;

					case CHEMAIN_BAS:
						direction = BAS;
						break;
					
					default:
						break;
					}
					
					// Fini la boucle de calcule vers la cible
					finDeCalcule = true;

					// Fini la boucle de calcule vers la Pomme
					if ((posX == lesPommes[X][score]) && (posY == lesPommes[Y][score]))
					{
						vraiFinDeCalcule = true;
					}
				}
			}

			// Dans un tableau de la meme taille que le tableau creation de l'itineraire complet du serpent
			tabChemin[posX][posY] = direction;
			directionActuelle = direction;

			// Implementation de la valeur du curseur
			if (!finDeCalcule)
			{
				if (directionActuelle == DROITE)
				{
					posX = posX + 1;
				}
				else if (directionActuelle == GAUCHE)
				{
					posX = posX - 1;
				}
				else if (directionActuelle == HAUT)
				{
					posY = posY - 1;
				}
				else if (directionActuelle == BAS)
				{
					posY = posY + 1;
				}
				nbDeplacement++;
			}
		}

		// Nouvelle cible a atteindre apres etre aller devant un portail
		posXVoulu = lesPommes[X][score];
		posYVoulu = lesPommes[Y][score];

		// Passage dans un portail
		while (!vraiFinDeCalcule && finDeCalcule && ((posX == ((TAILLE_TERRAIN_X + BORDURE_GAUCHE) / 2)) || (posY == ((TAILLE_TERRAIN_Y + BORDURE_HAUT) / 2))))
		{
			// Ajout de ce qui faut pour passer par un trou
			// C'est une sorte de passage "manuel" par les trous
			// La pos qui vas passer un portail et modifier jusqu'a ce qu'elle ai passé le portail puis fini la boucle une fois traversé
			if (modeChoisiParCalc == CHEMAIN_DROITE)
			{
				posX = posX + 1;
				tabChemin[posX][posY] = DROITE;
				if (posX == BORDURE_GAUCHE + 1)
				{
					finDeCalcule = false;
					directionActuelle = DROITE;
				}
			}
			else if (modeChoisiParCalc == CHEMAIN_GAUCHE)
			{
				posX = posX - 1;
				tabChemin[posX][posY] = GAUCHE;
				if (posX == BORDURE_DROITE - 2)
				{
					finDeCalcule = false;
					directionActuelle = GAUCHE;
				}
			}
			else if (modeChoisiParCalc == CHEMAIN_HAUT)
			{
				posY = posY - 1;
				tabChemin[posX][posY] = HAUT;
				if (posY == BORDURE_BAS - 2)
				{
					finDeCalcule = false;
					directionActuelle = HAUT;
				}
			}
			else if (modeChoisiParCalc == CHEMAIN_BAS)
			{
				posY = posY + 1;
				tabChemin[posX][posY] = BAS;
				if (posY == BORDURE_HAUT + 1)
				{
					finDeCalcule = false;
					directionActuelle = BAS;
				}
			}
			
			// Verification de passage dans un troue quelconque dans la bordure de n'importe quelle coté 
			if (modeChoisiParCalc == CHEMAIN_DROITE && (posX == BORDURE_DROITE - 1) && (posY == ((TAILLE_TERRAIN_Y + BORDURE_HAUT) / 2)))
			{
				posX = BORDURE_GAUCHE - 2;
			}
			else if (modeChoisiParCalc == CHEMAIN_GAUCHE && (posX == BORDURE_GAUCHE) && (posY == ((TAILLE_TERRAIN_Y + BORDURE_HAUT) / 2)))
			{
				posX = BORDURE_DROITE;
			}
			else if (modeChoisiParCalc == CHEMAIN_BAS && (posY == BORDURE_BAS - 1) && (posX == ((TAILLE_TERRAIN_X + BORDURE_HAUT) / 2)))
			{
				posY = BORDURE_HAUT - 2;
			}
			else if (modeChoisiParCalc == CHEMAIN_HAUT && (posY == BORDURE_HAUT) && (posX == ((TAILLE_TERRAIN_X + BORDURE_HAUT) / 2)))
			{
				posY = BORDURE_BAS;
			}
		}
	}
}

/**
 * @fn void calculeDeDistance(int departX, int departY, int distance[NOMBRE_DE_TROU_CALCULE])
 * @brief Calcule des different chemains pour aller a la pomme.
 * 
 * Calucle tous les chemain par lesquels le serpent pourrais passer.
 */
void calculeDeDistance(int departX, int departY, int distance[NOMBRE_DE_TROU_CALCULE], char chemain[NOMBRE_DE_TROU_CALCULE]){
    int x , y, min, indiceTemp;
	char dirTemp;
	bool finTrie = false;

    // Calcule de la distance normale
    x = abs(departX - lesPommes[X][score]);
    y = abs(departY - lesPommes[Y][score]);
    distance[0] = x + y;
	chemain[0] = CHEMAIN_NORMALE;

    // Calcule distance avec le trou a droite
    x = abs(departX - BORDURE_DROITE) + abs(BORDURE_GAUCHE - lesPommes[X][score]);
    y = abs(departY - ((TAILLE_TERRAIN_Y) / 2 + BORDURE_HAUT)) + abs(((TAILLE_TERRAIN_Y) / 2 + BORDURE_HAUT) - lesPommes[Y][score]);
    distance[1] = x + y;
	chemain[1] = CHEMAIN_DROITE;

    // Calcule distance avec le trou a gauche
    x = abs(departX - BORDURE_GAUCHE) + abs(BORDURE_DROITE - lesPommes[X][score]);
    y = abs(departY - ((TAILLE_TERRAIN_Y) / 2 + BORDURE_HAUT)) + abs(((TAILLE_TERRAIN_Y) / 2 + BORDURE_HAUT) - lesPommes[Y][score]);
    distance[2] = x + y ;
	chemain[2] = CHEMAIN_GAUCHE;

    // Calcule distance avec le trou en Haut
    x = abs(departX - ((TAILLE_TERRAIN_X) / 2) + BORDURE_GAUCHE) + abs(((TAILLE_TERRAIN_X) / 2) + BORDURE_GAUCHE - lesPommes[X][score]);
    y = abs(departY - BORDURE_HAUT) + abs(BORDURE_BAS - lesPommes[Y][score]);
    distance[3] = x + y;
	chemain[3] = CHEMAIN_HAUT;

    // Calcule distance avec le trou en Bas
    x = abs(departX - ((TAILLE_TERRAIN_X) / 2) + BORDURE_GAUCHE) + abs(((TAILLE_TERRAIN_X) / 2) + BORDURE_GAUCHE - lesPommes[X][score]);
    y = abs(departY - BORDURE_BAS) + abs(BORDURE_HAUT - lesPommes[Y][score]);
    distance[4] = x + y;
	chemain[4] = CHEMAIN_BAS;
	
	indiceTemp = 0;

	while (!finTrie)
	{
		min = distance[indiceTemp];

		for (int i = indiceTemp; i < NOMBRE_DE_TROU_CALCULE; i++)
		{
			if (distance[i] < min)
			{
				min = distance[i];
				distance[i] = distance[indiceTemp];
				distance[indiceTemp] = min;

				dirTemp = chemain[i];
				chemain[i] = chemain[indiceTemp];
				chemain[indiceTemp] = dirTemp;
			}
		}

		indiceTemp++;

		if (indiceTemp == NOMBRE_DE_TROU_CALCULE - 1)
		{
			finTrie = true;
		}
	}
}

/**
 * @fn void gotoXY(int x, int y)
 * @brief Placer le curseur au coordonnées x,y donner en parametre
 * 
 * @param x : Position x du curseur.
 * @param y : Position y du curseur.
 */
void gotoXY(int x, int y){ 
    printf("\033[%d;%df", y, x);
}

/**
 * @fn int kbhit(void)
 * @brief Obtenir les touche que l'utilisateur tape au clavier en boucle sans arreter le code
 * 
 * @return int : Retourne si un caractere a été appuyer ou non.
 */
int kbhit(void){
	// la fonction retourne :
	// 1 si un caractere est present
	// 0 si pas de caractere present
	
	int unCaractere=0;
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
 
	if(ch != EOF){
		ungetc(ch, stdin);
		unCaractere=1;
	} 
	return unCaractere;
}

/**
 * @fn void disableEcho()
 * @brief Desactive la posibilité d'ecrire sur le terminale.
 */
void disableEcho(){
    struct termios tty;

    // Obtenir les attributs du terminal
    if (tcgetattr(STDIN_FILENO, &tty) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Desactiver le flag ECHO
    tty.c_lflag &= ~ECHO;

    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}

/**
 * @fn void enableEcho()
 * @brief Réactive la posibilité d'ecrire sur le terminale.
 */
void enableEcho(){
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