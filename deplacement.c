#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include <time.h>
#include <math.h>
#include "head.h"


void calculCoordPossible(Joueurs actjoueur, Cases plateau[taillePlateauX][taillePlateauY], int cible[taillePlateauX][taillePlateauY], int x, int y, int coutPM)
{
    ///fonction récursive qui liste toutes les déplacement possible pour ce joueur dans le tableau "cible"
    ///et détermine le cout en PM du déplacement selon le déplacement
    ///IN  : joueur qui se déplace (ou bot) ; plateau ; cible; coordonnée x et y de départ ; coutPM (0 au début)
    ///out : remplie le tableau cible par adresse

    if((plateau[x][y].type<=0 || plateau[x][y].PVobstacle>0) && (plateau[x][y].ptjoueur==NULL || coutPM==0)) // vérifie qu'il n'y a pas d'obstacle
    {
        if(plateau[x][y].type>0 && plateau[x][y].PVobstacle>0) coutPM+=100; //on rajoute 100PM si il y a un obstacle (pour les bots)
        if((cible[x][y]==0 || cible[x][y]>coutPM)&& actjoueur.PM>=coutPM)
        {// enregistre le cout le plus faible et stop la fonction si contraire
            cible[x][y]=coutPM;

            if(coutPM<actjoueur.PM) // vérifie qu'il reste des PM pour aller plus loin
            {
                // les if bloque les déplacements en dehors de la bordure
                if(x-1>=0) calculCoordPossible(actjoueur, plateau, cible, x-1, y, coutPM+1);
                if(y-1>=0) calculCoordPossible(actjoueur, plateau, cible, x, y-1, coutPM+1);
                if(x+1<=taillePlateauX-1) calculCoordPossible(actjoueur, plateau, cible, x+1, y, coutPM+1);
                if(y+1<=taillePlateauY-1) calculCoordPossible(actjoueur, plateau, cible, x, y+1, coutPM+1);
            }
        }
    }
}

void calculTrajet(int etapes[][2], int cible[taillePlateauX][taillePlateauY], int destX, int destY)
{
    ///determine un des chemin le plus cour et sauvegarde toutes les étapes entre le joueur et la destination
    ///IN  : tableau pour stocker le parcour ; cible pour le cout en PM ; coordonné d'arriver x et y
    ///OUT : tableau etapes du parcour par adresse
    int x=destX;
    int y=destY;

    int nbBoucle=cible[destX][destY]%100;
    //sauvegarde la destination
    etapes[nbBoucle][0]=destX;
    etapes[nbBoucle][1]=destY;

    ///cherche le chemin
    for(int i=nbBoucle; i>0; i--)
    {
        ///bloque les bordures   et       cherche la case qui coute 1 de moin que la case actuel
        //cherche un chemin avec moin d'obstacle
        if     (x-1>=0                && ((cible[x-1][y]%100==i-1 && cible[x-1][y]!=0 && cible[x-1][y]/100<cible[x][y]/100) || cible[x-1][y]==-1)) x-=1;
        else if(y-1>=0                && ((cible[x][y-1]%100==i-1 && cible[x][y-1]!=0 && cible[x][y-1]/100<cible[x][y]/100) || cible[x][y-1]==-1)) y-=1;
        else if(x+1<=taillePlateauX-1 && ((cible[x+1][y]%100==i-1 && cible[x+1][y]!=0 && cible[x+1][y]/100<cible[x][y]/100) || cible[x+1][y]==-1)) x+=1;
        else if(y+1<=taillePlateauY-1 && ((cible[x][y+1]%100==i-1 && cible[x][y+1]!=0 && cible[x][y+1]/100<cible[x][y]/100) || cible[x][y+1]==-1)) y+=1;

        //cherche un chemin autant d'obstacle
        else if     (x-1>=0                && ((cible[x-1][y]%100==i-1 && cible[x-1][y]!=0 && cible[x-1][y]/100==cible[x][y]/100) || cible[x-1][y]==-1)) x-=1;
        else if(y-1>=0                && ((cible[x][y-1]%100==i-1 && cible[x][y-1]!=0 && cible[x][y-1]/100==cible[x][y]/100) || cible[x][y-1]==-1)) y-=1;
        else if(x+1<=taillePlateauX-1 && ((cible[x+1][y]%100==i-1 && cible[x+1][y]!=0 && cible[x+1][y]/100==cible[x][y]/100) || cible[x+1][y]==-1)) x+=1;
        else if(y+1<=taillePlateauY-1 && ((cible[x][y+1]%100==i-1 && cible[x][y+1]!=0 && cible[x][y+1]/100==cible[x][y]/100) || cible[x][y+1]==-1)) y+=1;
        //on refais sans considérer les obstacles si pas de chemin optimal
        else if(x-1>=0                && ((cible[x-1][y]%100==i-1 && cible[x-1][y]!=0) || cible[x-1][y]==-1)) x-=1;
        else if(y-1>=0                && ((cible[x][y-1]%100==i-1 && cible[x][y-1]!=0) || cible[x][y-1]==-1)) y-=1;
        else if(x+1<=taillePlateauX-1 && ((cible[x+1][y]%100==i-1 && cible[x+1][y]!=0) || cible[x+1][y]==-1)) x+=1;
        else if(y+1<=taillePlateauY-1 && ((cible[x][y+1]%100==i-1 && cible[x][y+1]!=0) || cible[x][y+1]==-1)) y+=1;

        //sauvegarde l'étape actuel
        etapes[i-1][0]=x;
        etapes[i-1][1]=y;
    }
}

void deplacement(BITMAP* backscreen, BITMAP* buffer, Joueurs* actjoueur,Joueurs tabjoueur[],Cases plateau[taillePlateauX][taillePlateauY], double* debut,pics Image)
{
    ///fonction qui gère la totalité des déplacements d'un joueur
    int mouv_animation;//variable pour animer les déplacement
    //taille d'une case du plateau
    int taillex = 33;
    int tailley = 22;
    int delay = 25;
    int cible[taillePlateauX][taillePlateauY]={{0}}; //permet de lister les couts en PM de chaque déplacement

    int x=actjoueur->posX;
    int y=actjoueur->posY;

    calculCoordPossible(*actjoueur, plateau, cible, x, y, 0); //lance le calcul des positions possibles
    cible[x][y]=-1; // position du joueur


    int destX=-1;
    int destY=-1;

    /// affichage et lecture des choix pour l'utilisateur
    do
    {
        clear_bitmap(backscreen);
        clear_bitmap(buffer);

        if (delay > 0) {
            delay -= 1;
        }

        affichePlateau(backscreen, buffer, plateau, &destX, &destY,Image);

        int nextcible[taillePlateauX][taillePlateauY]={{0}};
        if(destX!=-1 && cible[destX][destY]>0)
        {
            //calcule les prochains déplacement possible
            calculCoordPossible(*actjoueur, plateau, nextcible, destX, destY, cible[destX][destY]); //lance le calcul des positions possibles
            nextcible[destX][destY]=-1; // position du joueur



            ///affiche les déplacements possible
            for(int i=0; i<taillePlateauY; i++)
            {
                for(int j=0; j<taillePlateauX; j++)
                {
                    if (nextcible[j][i]>0) {
                        int xcase = plateau[j][i].posX - taillex;
                        int ycase = plateau[j][i].posY - tailley;
                        masked_blit(Image.pblock[1],backscreen,0,0,xcase,ycase, Image.pblock[1]->w, Image.pblock[1]->h);
                        line(backscreen, xcase,ycase+tailley,xcase+taillex,ycase+(tailley*2)              ,colblack);
                        line(backscreen, xcase,ycase+tailley,xcase+taillex,ycase                          ,colblack);
                        line(backscreen, xcase+taillex,ycase+(tailley*2),xcase+(taillex*2),ycase+tailley  ,colblack);
                        line(backscreen, xcase+taillex,ycase,xcase+(taillex*2),ycase+tailley              ,colblack);
                    }
                    else if(nextcible[j][i] ==-1) {

                        int xcase = plateau[j][i].posX - taillex;
                        int ycase = plateau[j][i].posY - tailley;
                        masked_blit(Image.pblock[2],backscreen,0,0,xcase,ycase, Image.pblock[2]->w, Image.pblock[2]->h);
                        line(backscreen, xcase,ycase+tailley,xcase+taillex,ycase+(tailley*2)              ,colblack);
                        line(backscreen, xcase,ycase+tailley,xcase+taillex,ycase                          ,colblack);
                        line(backscreen, xcase+taillex,ycase+(tailley*2),xcase+(taillex*2),ycase+tailley  ,colblack);
                        line(backscreen, xcase+taillex,ycase,xcase+(taillex*2),ycase+tailley              ,colblack);
                    }
                }
            }
            ///etapes de déplacement
            int nbetapes=cible[destX][destY];
            int etapes[taillePlateauX][2]={{0}}; //sauvegarde les etapes du chemin le plus court
            calculTrajet(etapes, cible, destX, destY);
            //affichage du trajet pour aller vers la sourie de l'utilisateur
            for(int i=0; i<nbetapes; i++)
            {
                int xcase = plateau[etapes[i][0]][etapes[i][1]].posX - taillex;
                int ycase = plateau[etapes[i][0]][etapes[i][1]].posY - tailley;
                masked_blit(Image.pblock[2],backscreen,0,0,xcase,ycase, Image.pblock[2]->w, Image.pblock[2]->h);
                line(backscreen, xcase,ycase+tailley,xcase+taillex,ycase+(tailley*2)              ,colblack);
                line(backscreen, xcase,ycase+tailley,xcase+taillex,ycase                          ,colblack);
                line(backscreen, xcase+taillex,ycase+(tailley*2),xcase+(taillex*2),ycase+tailley  ,colblack);
                line(backscreen, xcase+taillex,ycase,xcase+(taillex*2),ycase+tailley              ,colblack);
            }
        }
        else
        {
            //met les coord par default
            destX=-1;
            destY=-1;
            //affiche les déplacement possible depuis la position d'origine
            for(int i=0; i<taillePlateauY; i++)
            {
                for(int j=0; j<taillePlateauX; j++)
                {
                    if (cible[j][i] > 0) {
                        int xcase = plateau[j][i].posX - taillex;
                        int ycase = plateau[j][i].posY - tailley;
                        masked_blit(Image.pblock[1],backscreen,0,0,xcase,ycase, Image.pblock[1]->w, Image.pblock[1]->h);
                        line(backscreen, xcase,ycase+tailley,xcase+taillex,ycase+(tailley*2)              ,colblack);
                        line(backscreen, xcase,ycase+tailley,xcase+taillex,ycase                          ,colblack);
                        line(backscreen, xcase+taillex,ycase+(tailley*2),xcase+(taillex*2),ycase+tailley  ,colblack);
                        line(backscreen, xcase+taillex,ycase,xcase+(taillex*2),ycase+tailley              ,colblack);
                    }
                }
            }

        }
        int ciblenul[taillePlateauX][taillePlateauY]={{0}};
        afficheTerrain(backscreen, buffer, tabjoueur,plateau,0,Image, ciblenul);


        //Affichage souris
        masked_blit(Image.pmenu[92],backscreen,0,0,mouse_x,mouse_y,Image.pmenu[92]->w, Image.pmenu[92]->h);

        blit(backscreen,screen,0,0,0,0,SCREEN_W,SCREEN_H);
        rest(1);

        //test fin de tour
        if((clock()-*debut)/CLOCKS_PER_SEC>=timeMaxTour){
            destX=-1;
            destY=-1;
            break;
        }
    }
    while((mouse_b!=1 || delay !=0) && !key[KEY_ESC]);



    if(destX!=-1 && cible[destX][destY]!=-1) //déplace seulement si la position d'arriver est differente de l'actuelle
    {
        double t1=clock();

        ///etapes pour animation de déplacement
        int nbetapes=cible[destX][destY];
        int etapes[taillePlateauX][2]={{0}}; //sauvegarde les etapes du chemin le plus court
        calculTrajet(etapes, cible, destX, destY);

        const int ipc=25; ///Image Par Case
        float tmpxf,tmpyf, fractx,fracty;
        for(int n=0; n<nbetapes; n++)   //tant que l'animation n'est pas fini
        {
            //calcul vecteur déplacement
            actjoueur->ecranX=plateau[ etapes[n][0] ][ etapes[n][1] ].posX;
            actjoueur->ecranY=plateau[ etapes[n][0] ][ etapes[n][1] ].posY;
            tmpxf=plateau[ etapes[n+1][0] ][ etapes[n+1][1] ].posX;
            tmpyf=plateau[ etapes[n+1][0] ][ etapes[n+1][1] ].posY;
            fractx=(tmpxf-actjoueur->ecranX)/ipc;
            fracty=(tmpyf-actjoueur->ecranY)/ipc;

            if(fractx<0 && fracty<0) actjoueur->sens=1; //haut gauche
            if(fractx>0 && fracty<0) actjoueur->sens=2; //haut droite
            if(fractx<0 && fracty>0) actjoueur->sens=3; //bas  gauche
            if(fractx>0 && fracty>0) actjoueur->sens=4; //bas  droite

            mouv_animation = 15;
            //déplacement d'une case
            for(int i=0; i<ipc; i++)
            {
                mouv_animation--;
                clear_bitmap(backscreen);
                clear_bitmap(buffer);

                int a,b; //récupère les coordonnées mais les utilises pas
                affichePlateau(backscreen, buffer, plateau, &a, &b,Image);

                actjoueur->ecranX+=fractx;
                actjoueur->ecranY+=fracty;

                int ciblenul[taillePlateauX][taillePlateauY]={{0}};
                afficheTerrain(backscreen, buffer, tabjoueur, plateau,mouv_animation,Image, ciblenul);

                //Affichage souris
                masked_blit(Image.pmenu[92],backscreen,0,0,mouse_x,mouse_y,Image.pmenu[92]->w, Image.pmenu[92]->h);

                blit(backscreen,screen,0,0,0,0,SCREEN_W,SCREEN_H);
                //on déplace le joueur dans le tableau pour gerer l'ordre d'affichage
                if(i==ipc/2) {
                    actjoueur->posX=etapes[n+1][0];
                    actjoueur->posY=etapes[n+1][1];
                }
            }
        }
        actjoueur->ecranX=-1;
        actjoueur->ecranY=-1;

        //deplace le pointeur de joueur dans le plateau
        plateau[x][y].ptjoueur=NULL;
        plateau[destX][destY].ptjoueur=actjoueur;
        //deplace le joueur et reduit ses PM
        actjoueur->posX=destX;
        actjoueur->posY=destY;
        actjoueur->PM-=cible[destX][destY];

        //effet de terrain : exemple le feu
        effetPlateau(plateau,tabjoueur, actjoueur,0);

        //on retire le temps de l'animation au temps du tour du joueur
        double t2=clock();
        *debut+=t2-t1;
    }
}

void deplacement_auto(BITMAP* backscreen, BITMAP* buffer, Joueurs* actjoueur,Joueurs tabjoueur[],Cases plateau[taillePlateauX][taillePlateauY],pics Image)
{
    ///fonction qui gère la totalité des déplacements des bots et leurs attaques
    int mouv_animation;
    int taillex = 33;
    int tailley = 22;

    int cible[taillePlateauX][taillePlateauY]={{0}}; //permet de lister les couts en PM de chaque déplacement

    int x=actjoueur->posX;
    int y=actjoueur->posY;
    //on lui donne bcp de PM pour q'il puisse voir les joueurs même derriere plusieur obstacle
    actjoueur->PM=taillePlateauX+taillePlateauY+10000;

    calculCoordPossible(*actjoueur, plateau, cible, x, y, 0); //lance le calcul des positions possibles
    cible[x][y]=-1; // position du joueur

    actjoueur->PM=actjoueur->PMmax;
    if(actjoueur->lenteur>0) {
        actjoueur->lenteur-=1;
        actjoueur->PM-=1;
    }

    ///on cherche le plus proche
    int destX=x;
    int destY=y;
    int PMmin=taillePlateauX+taillePlateauY+10000;
    int cible_ob=0;
    for(int i=0; i<taillePlateauX; i++)
    {
        for(int j=0; j<taillePlateauY; j++)
        {
            //on cherche un joueur
            if(plateau[i][j].ptjoueur!=NULL && plateau[i][j].ptjoueur->IDteam!=actjoueur->IDteam)
            {
                //on cherche la case qui coute le moin de PM autour du joueur
                int mintmp=taillePlateauX+taillePlateauY+10000;
                if(i-1>=0 && cible[i-1][j]<mintmp && cible[i-1][j]!=0) mintmp=cible[i-1][j];
                if(j-1>=0 && cible[i][j-1]<mintmp && cible[i][j-1]!=0) mintmp=cible[i][j-1];
                if(i+1<=taillePlateauX-1 && cible[i+1][j]<mintmp && cible[i+1][j]!=0) mintmp=cible[i+1][j];
                if(j+1<=taillePlateauY-1 && cible[i][j+1]<mintmp && cible[i][j+1]!=0) mintmp=cible[i][j+1];
                cible[i][j]=(mintmp<0? 0 :mintmp)+1;
                //on la compare aux autres joueur
                if(cible[i][j]<PMmin)
                {
                    destX=i;
                    destY=j;
                    PMmin=cible[i][j];
                }
            }
        }
    }



    if(destX!=x || destY!=y)
    {
        ///etapes pour animation de déplacement
        int nbetapes=cible[destX][destY]%100;
        int etapes[taillePlateauX+taillePlateauY][2]={{0}}; //sauvegarde les etapes du chemin le plus court
        calculTrajet(etapes, cible, destX, destY);

        const int ipc=25; ///Image Par Case
        float tmpxf,tmpyf, fractx,fracty;
        for(int n=0; n<(nbetapes < actjoueur->PM+1? nbetapes : actjoueur->PM+1); n++)   //tant que l'animation n'est pas fini
        {
            //calcul vecteur déplacement
            actjoueur->ecranX=plateau[ etapes[n][0] ][ etapes[n][1] ].posX;
            actjoueur->ecranY=plateau[ etapes[n][0] ][ etapes[n][1] ].posY;
            tmpxf=plateau[ etapes[n+1][0] ][ etapes[n+1][1] ].posX;
            tmpyf=plateau[ etapes[n+1][0] ][ etapes[n+1][1] ].posY;
            fractx=(tmpxf-actjoueur->ecranX)/ipc;
            fracty=(tmpyf-actjoueur->ecranY)/ipc;

            if(fractx<0 && fracty<0) actjoueur->sens=1; //haut gauche
            if(fractx>0 && fracty<0) actjoueur->sens=2; //haut droite
            if(fractx<0 && fracty>0) actjoueur->sens=3; //bas  gauche
            if(fractx>0 && fracty>0) actjoueur->sens=4; //bas  droite

            ///on test si il y a un joueur ou un obstacle devant
            if(plateau[ etapes[n+1][0] ][ etapes[n+1][1] ].ptjoueur!=NULL || plateau[ etapes[n+1][0] ][ etapes[n+1][1] ].PVobstacle>0)
            {
                int missed=0;
                float degat[2][3]={{0}};
                degat[1][0]=-1;
                if(rand()%100 >= 10)
                {
                    const int ATK=50;
                    ///attaque un joueur ou un bot
                    if(plateau[ etapes[n+1][0] ][ etapes[n+1][1] ].ptjoueur!=NULL)
                    {
                        ///degats bot
                        int degatMin=ATK- (int)(ATK*0.2);
                        int degatMax=ATK+ (int)(ATK*0.2)+1;// degat € [degatmin ; degatmax[
                        int degatup=(actjoueur->force>0 ? 20 : 0); //effet force
                        float fractdegat=(plateau[ etapes[n+1][0] ][ etapes[n+1][1] ].ptjoueur->def>0 ? 3.0/4.0 : 1.0); //effet resistance
                        degat[0][0]=(int)round((rand()%(degatMax-degatMin)+degatMin + degatup)*fractdegat);
                        plateau[ etapes[n+1][0] ][ etapes[n+1][1] ].ptjoueur->PV-=degat[0][0];
                        degat[0][1]=plateau[ etapes[n+1][0] ][ etapes[n+1][1] ].posX;
                        degat[0][2]=plateau[ etapes[n+1][0] ][ etapes[n+1][1] ].posY;

                        if(plateau[ etapes[n+1][0] ][ etapes[n+1][1] ].ptjoueur->PV<=0)
                        {
                            plateau[ etapes[n+1][0] ][ etapes[n+1][1] ].ptjoueur->PV=0;
                            if(plateau[ etapes[n+1][0] ][ etapes[n+1][1] ].ptjoueur->IDcadavre<0){
                                plateau[ etapes[n+1][0] ][ etapes[n+1][1] ].ptjoueur->IDcadavre=-plateau[ etapes[n+1][0] ][ etapes[n+1][1] ].ptjoueur->IDcadavre;
                                for(int i=0; i<nbJoueurMax; i++){
                                    if(tabjoueur[i].IDcadavre<0) tabjoueur[i].IDcadavre--;
                                }
                            }
                            plateau[ etapes[n+1][0] ][ etapes[n+1][1] ].ptjoueur=NULL;
                            supp_bot(plateau, tabjoueur);
                        }
                    }
                    //attaque un obstacle
                    else if(plateau[ etapes[n+1][0] ][ etapes[n+1][1] ].PVobstacle>0)
                    {
                        ///degats obstacle
                        int degatMin=ATK- (int)(ATK*0.2);
                        int degatMax=ATK+ (int)(ATK*0.2)+1;// degat € [degatmin ; degatmax[
                        int degatup=(actjoueur->force>0 ? 20 : 0); //effet force
                        degat[0][0]=(int)round(rand()%(degatMax-degatMin)+degatMin + degatup);
                        plateau[ etapes[n+1][0] ][ etapes[n+1][1] ].PVobstacle-=degat[0][0];
                        degat[0][1]=plateau[ etapes[n+1][0] ][ etapes[n+1][1] ].posX;
                        degat[0][2]=plateau[ etapes[n+1][0] ][ etapes[n+1][1] ].posY;


                        if(plateau[ etapes[n+1][0] ][ etapes[n+1][1] ].PVobstacle<=0)
                        {
                            plateau[ etapes[n+1][0] ][ etapes[n+1][1] ].PVobstacle=-1;
                            plateau[ etapes[n+1][0] ][ etapes[n+1][1] ].tourobstacle=-1;
                            plateau[ etapes[n+1][0] ][ etapes[n+1][1] ].type=0;
                            plateau[ etapes[n+1][0] ][ etapes[n+1][1] ].sortbloque=0;
                        }
                    }
                }
                else {
                    missed=1;
                }

                ///animation de l'attaque
                Sorts spell;
                spell.typesort=0;
                int x1=plateau[actjoueur->posX][actjoueur->posY].posX;
                int y1=plateau[actjoueur->posX][actjoueur->posY].posY;
                int x2=plateau[ etapes[n+1][0] ][ etapes[n+1][1] ].posX;
                int y2=plateau[ etapes[n+1][0] ][ etapes[n+1][1] ].posY;
                sortAnimation(backscreen, buffer, plateau, tabjoueur, x1, y1, x2, y2, spell, degat, missed, Image);
                if(plateau[ etapes[n+1][0] ][ etapes[n+1][1] ].ptjoueur==NULL)
                    break;
            }

            ///si il n'a pas réussi a le détruire on stop le déplacement
            if(plateau[ etapes[n+1][0] ][ etapes[n+1][1] ].ptjoueur!=NULL || plateau[ etapes[n+1][0] ][ etapes[n+1][1] ].PVobstacle>0)
                break;

            ///déplacement du bot
            if(n<actjoueur->PM)
            {
                mouv_animation = 18;
                //déplacement d'une case
                for(int i=0; i<ipc; i++)
                {
                    mouv_animation--;
                    clear_bitmap(backscreen);
                    clear_bitmap(buffer);

                    int a,b; //récupère les coordonnées mais les utilises pas
                    affichePlateau(backscreen, buffer, plateau, &a, &b,Image);

                    actjoueur->ecranX+=fractx;
                    actjoueur->ecranY+=fracty;
                    //rectfill(backscreen, tmpx-8, tmpy-26, tmpx+8, tmpy+8,colwhite);
                    //circlefill(backscreen, tmpx, tmpy-32,6,colwhite);

                    int ciblenul[taillePlateauX][taillePlateauY]={{0}};
                    afficheTerrain(backscreen, buffer, tabjoueur, plateau, mouv_animation,Image, ciblenul);

                    //Affichage souris
                    masked_blit(Image.pmenu[92],backscreen,0,0,mouse_x,mouse_y,Image.pmenu[92]->w, Image.pmenu[92]->h);

                    blit(backscreen,screen,0,0,0,0,SCREEN_W,SCREEN_H);
                    if(i==ipc/2) {
                        actjoueur->posX=etapes[n+1][0];
                        actjoueur->posY=etapes[n+1][1];
                        plateau[etapes[n][0]][etapes[n][1]].ptjoueur=NULL;
                        plateau[etapes[n+1][0]][etapes[n+1][1]].ptjoueur=actjoueur;
                    }
                }
            }
        }
        actjoueur->ecranX=-1;
        actjoueur->ecranY=-1;
        effetPlateau(plateau,tabjoueur, actjoueur, 0);
    }
}
