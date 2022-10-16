#ifndef HEAD_H_INCLUDED
#define HEAD_H_INCLUDED


    ///Couleur pour l'affichage :

/// col = variable pour les couleurs    (ex : colwhite)
/// bcol = variable couleur qu'on utilise uniquement dans le buffer (pour les collisions / hitbox)

//Couleurs des 4 Teams
#define colteam1 makecol(0,205,250)      //Diamant
#define colteam2 makecol(125,250,30)      //Emeraude
#define colteam3 makecol(255,205,50)       //Gold
#define colteam4 makecol(240,20,60)      //Redstone

//Couleurs pour chaque effets du jeu
#define colnone makecol(200,200,200)
#define colhealh makecol(130,255,25)
#define collenteur makecol(160,255,245)
#define colpoison makecol(190,240,55)
#define colwither makecol(120,120,120)
#define colforce makecol(200,100,50)
#define coldef makecol(210,210,210)

//Couleurs du texte pour chaque classe
#define colzombie makecol(175,225,45)
#define colskeleton makecol(210,210,210)
#define colspider makecol(175,200,190)
#define colblaze makecol(255,200,10)
#define colpillager makecol(230,150,100)

//Couleurs buffer
#define bcolbutton1 makecol(249,0,0)
#define bcolbutton2 makecol(248,0,0)
#define bcolbutton3 makecol(247,0,0)
#define bcolbutton4 makecol(246,0,0)
#define bcolbutton5 makecol(245,0,0)
#define bcolbutton6 makecol(244,0,0)
#define bcolbutton7 makecol(243,0,0)
#define bcolbutton8 makecol(242,0,0)
#define bcolbutton9 makecol(241,0,0)
#define bcolbutton10 makecol(240,0,0)
#define bcolbutton11 makecol(239,0,0)
#define bcolbutton12 makecol(236,0,0)
#define bcolbutton13 makecol(235,0,0)
#define bcolbutton14 makecol(234,0,0)
#define bcolbutton15 makecol(233,0,0)

#define colwhite makecol(255,255,255)
#define colblack makecol(0,0,0)


///allegro est en qwerty
#define showClav KEY_TAB
#define clavDeplacement KEY_D
#define clavAttaque KEY_A
#define clavRetour KEY_Z
#define clavFinTour KEY_F
#define clavCAC KEY_I
#define clavSort1 KEY_J
#define clavSort2 KEY_K
#define clavSort3 KEY_L
#define clavSort4 KEY_SEMICOLON

///Autres constantes du jeu
#define taillePlateauX 12   //Taille du plateau : 12x12
#define taillePlateauY 12
#define nbJoueurMax 4
#define nbSortMax 5         //CAC + 4 sorts
#define nbDifBuff 4
#define nbImage 4
#define nbBotsMax 3         //Nombre max de bots par joueurs
#define timeMaxTour 600 //en seconde
#define NbLeafMax 60

#define pi 3.14159265358979323846

typedef struct pics{    ///Structure pics (Image) : stocke toutes les Images du jeu
    BITMAP* psort[42];
    BITMAP* pblock[30]; //0-5 cases de couleur   6:stone  7:cobblestone  8:log  9-10:cobweb  11-12:fire   13:ombre
    BITMAP* pmob[56];
    BITMAP* pmenu[120];
    BITMAP* panime[30];
}pics;

typedef struct leaf{    ///Feuille : sert uniquement pour le décors (background) du jeu
    int delay;
    int tabx[NbLeafMax];
    int taby[NbLeafMax];
    float speed[NbLeafMax];
    int color[NbLeafMax];
}leaf;

typedef struct Sorts{   ///Sorts : les effets du sorts dépendent uniquement des variables que l'on lui donne dans cette structure

    int type; ///0: offensif   1: soutient   2: terrain   3: invocation
    int typesort;  //0 = CAC, 1 = attaque 1 du zombie, etc... (de 0 à 20)
    int obstacle; //0 si passe a travere; 1 si ça passe pas
    int PA;

    int degat;
    float probDegatNul; // en %
    int porteMin, porteMax; //portee du sort
    int formePorte; // 0: cercle(=losange)   1: ligne (sans les diagonale)
    int taille; //rayon de la zone de dégats (default 0)

    ///buff
    int poison; // en tour
    int wither; // en tour
    int lenteur; //en tour
    int force; // en tour
    int def; // en tour
    int soins; //en PV
    int PMup; //en PM (augmentation des PM pendant ce tour)
    int debuff; //0: rien ;  1: perte de tous les buff

    ///obstacle (Ob)
    int Obtype; //même type que pour la structures Cases
    int ObPV; // -1 si invincible
    int Obtour; // -1 si immortel
    int Obbloque; // 0: bloque pas les sorts ;  1: bloque les sorts
}Sorts;


typedef struct Classes{   ///Classes
    int type;                   //type=1 : zombie  / 2:skelette / 3:spider / 4:blaze / 5:pillager
    Sorts tabSort[nbSortMax]; //liste des sorts possible (si jamais un joueur gagne des sorts)
    BITMAP* image[nbImage]; //graphisme de la classe
}Classes;

typedef struct Joueurs{   ///Joueurs
    char nom[20];
    int posX, posY, sens; //position sur le plateau
    float ecranX, ecranY; //pour l'animation du déplacement
    int PV, PA, PM, PVmax ,PAmax, PMmax;    //PV : son nombre de PV actuel, PVmax : dépend de la classe que choisi le joueur
    Classes classe; //classe du joueur
    Sorts sort[nbSortMax]; // liste de sort du joueur
    int IDteam; //1 equipe par joueur si  1 vs 1 vs 1 ... (4teams possible)

    ///buff
    int poison; // en tour (si poison = 3, alors le poison dure 3 tours / 0 si pas d'effet)
    int wither; // en tour
    int lenteur; //en tour
    int force; // en tour
    int def; // en tour

    ///bots
    int nbbots; //Nb de bots que le joueur a invoqué
    struct Joueurs* ptbots[nbBotsMax];

    int IDcadavre;  //Determine l'ordre dans lequel les joueurs sont morts : pour afficher le classement
}Joueurs;

typedef struct Cases{   ///Cases (pour toutes les cases du plateau)
    int type; // sort et mouvement :   type>0: bloque ; type<=0 bloque pas
                //type -1: Toile ;  type -2: feu         type 3: buche
    int posX, posY;     //Position du milieu de la cases sur l'écran
    Joueurs* ptjoueur; //null si pas de joueur dessu
    BITMAP* images[nbImage]; //graphisme de la case
    int blocktype;  //Sert uniquement pour l'affichage (bloc de feuille, de pierre, ...)
    int PVobstacle; //-1 si invincible
    int PVMAXobstacle;
    int tourobstacle; //-1 si immortel
    int IDcreateur; //la place dans le tableau du joueur qui a creer l'obstacle
    int sortbloque;
}Cases;

void initialisation_allegro();
void calculCoordPossible(Joueurs actjoueur, Cases plateau[taillePlateauX][taillePlateauY], int cible[taillePlateauX][taillePlateauY], int x, int y, int coutPM);
void calculTrajet(int etapes[][2], int cible[taillePlateauX][taillePlateauY], int destX, int destY);
void deplacement(BITMAP* backscreen, BITMAP* buffer, Joueurs* actjoueur,Joueurs tabjoueur[],Cases plateau[taillePlateauX][taillePlateauY], double* debut,pics Image);
void deplacement_auto(BITMAP* backscreen, BITMAP* buffer, Joueurs* actjoueur,Joueurs tabjoueur[],Cases plateau[taillePlateauX][taillePlateauY],pics Image);
void effetTour(BITMAP* backscreen, BITMAP* buffer, Cases plateau[taillePlateauX][taillePlateauY], Joueurs tabjoueur[], int tour,pics Image);
void effetPlateau(Cases plateau[taillePlateauX][taillePlateauY], Joueurs tabjoueur[], Joueurs* actjoueur, int effetbot);
void lancerSorts(BITMAP* backscreen, BITMAP* buffer, Cases plateau[taillePlateauX][taillePlateauY], Joueurs tabjoueur[], int tour, Sorts spell, double* debut, pics Image);
void rotationBITMAP(BITMAP* source, BITMAP* dest, double angle);
void sortAnimation_projectile(BITMAP* backscreen, BITMAP* buffer, Cases plateau[taillePlateauX][taillePlateauY], Joueurs tabjoueur[], int x1, int y1, int x2, int y2, Sorts spell, BITMAP* image,pics Image);
void sortAnimation_hit(BITMAP* backscreen, BITMAP* buffer, Cases plateau[taillePlateauX][taillePlateauY], Joueurs tabjoueur[], Sorts spell,int x2, float y2, float degat[30][3],pics Image);
void sortAnimation_explosion(BITMAP* backscreen, BITMAP* buffer, Cases plateau[taillePlateauX][taillePlateauY], Joueurs tabjoueur[],int x2, float y2,Sorts spell,float degat[30][3],int nbparticule,pics Image);
void sortAnimation_effect(BITMAP* backscreen, BITMAP* buffer, Cases plateau[taillePlateauX][taillePlateauY], Joueurs tabjoueur[],int x2, float y2, Sorts spell, int missed,int effect,pics Image);
void sortAnimation(BITMAP* backscreen, BITMAP* buffer, Cases plateau[taillePlateauX][taillePlateauY], Joueurs tabjoueur[], int x1, int y1, int x2, int y2, Sorts spell,int degat[30][3],int missed,pics Image);
void affichePlateau(BITMAP *backscreen,BITMAP *buffer,Cases plateau[taillePlateauX][taillePlateauY],int* xclic, int* yclic,pics Image);
void afficheTerrain(BITMAP *backscreen,BITMAP *buffer,Joueurs tabjoueur[],Cases plateau[taillePlateauX][taillePlateauY],int mouv_animation,pics Image, int cible[taillePlateauX][taillePlateauY]);
int Menusort(BITMAP *backscreen,BITMAP *buffer,Joueurs tabjoueur[],Joueurs actJoueur,int selectsort,pics Image);
void MenuJeu(BITMAP *backscreen,BITMAP *buffer,Joueurs tabjoueur[],Cases plateau[taillePlateauX][taillePlateauY],int *tour, int *val,int xclic,int yclic,int *delay,int nbJoueur,int *menusort, double* debut, pics Image);
int AfficheGagnant (BITMAP *backscreen,BITMAP *buffer,Joueurs tabjoueur[],Cases plateau[taillePlateauX][taillePlateauY],int nbJoueur,leaf l[], int tour, int team[], pics Image);
int MenuFin (BITMAP *backscreen,BITMAP *buffer,Joueurs tabjoueur[],Cases plateau[taillePlateauX][taillePlateauY],int nbJoueur, leaf l[], int tour, int team[], pics Image);
int Jeu (BITMAP *backscreen,BITMAP *buffer,Joueurs tabjoueur[],Cases plateau[taillePlateauX][taillePlateauY],int nbJoueur, leaf l[], int tour, pics Image, SAMPLE* music, int* volume);
void supp_bot(Cases plateau[taillePlateauX][taillePlateauY], Joueurs tabjoueur[]);
void MenuTransition (BITMAP *backscreen);
void DecorsFeuilles(BITMAP *backscreen, leaf l[],pics Image, int confetti);
int rules(BITMAP *backscreen,BITMAP *buffer,pics Image,leaf l[]);
int credits(BITMAP* backscreen, BITMAP* buffer, leaf l[], pics Image);
int MenuChoixClasse (BITMAP *backscreen,BITMAP *buffer,Joueurs tabjoueur[],int i,int nbJoueur,pics Image,leaf l[]);
int MenuChoixTerrain (BITMAP *backscreen,BITMAP *buffer,Joueurs tabjoueur[], pics Image,leaf l[]);
int MenuNBJoueur (BITMAP *backscreen,BITMAP *buffer,Joueurs tabjoueur[],pics Image,leaf l[]);
void InitialisationClasses(Joueurs tabjoueur[],int i);
pics InitialisationImages(pics Image);      ///INITIALISATION Bitmaps
int InitialisationDebutJeu(BITMAP *backscreen,BITMAP *buffer,Joueurs tabjoueur[],Cases plateau[taillePlateauX][taillePlateauY],int nbJoueur,pics Image);
int MenuPrincipale (BITMAP *backscreen,BITMAP *buffer,Joueurs tabjoueur[],Cases plateau[taillePlateauX][taillePlateauY],pics Image,leaf l[],int *restart,int*pnbJoueur,int*pchoixterrain, SAMPLE* music, int* volume);
void InitialisationPlateau(Cases plateau[taillePlateauX][taillePlateauY], Joueurs tabjoueur[], int val);
void sauvegarde(Cases plateau[taillePlateauX][taillePlateauY], Joueurs tabjoueur[nbJoueurMax], int nbjoueur, int tour);
void chargerparti(Cases plateau[taillePlateauX][taillePlateauY], Joueurs tabjoueur[nbJoueurMax], int* nbjoueur, int* tour);


#endif // HEAD_H_INCLUDED
