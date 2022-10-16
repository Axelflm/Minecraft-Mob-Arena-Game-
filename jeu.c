#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include <time.h>
#include <math.h>
#include "head.h"

void affichePlateau(BITMAP *backscreen,BITMAP *buffer,Cases plateau[taillePlateauX][taillePlateauY],int* xclic, int* yclic,pics Image) {

    ///Affichage du plateau à l'écran (plateau sans obtacles ni joueur au dessus, seulement les cases)
    ///Double buffer permettant de savoir sur quel case se situe la souris (plateau[xclic][yclic])

    BITMAP *bufferplateau;
    bufferplateau = create_bitmap(SCREEN_W,SCREEN_H);
    masked_blit(Image.pmenu[101],backscreen,0,0,5,0,Image.pmenu[101]->w, Image.pmenu[101]->h);
    masked_blit(Image.pmenu[55],backscreen,0,0,0,0,Image.pmenu[55]->w, Image.pmenu[55]->h); //border
                        ///taille d'une case : 66x44 (si on prend en forme de rectangle)
    int taillex = 33;   //taille entre le centre de la case et le bord sur l'axe X
    int tailley = 22;   //taille entre le centre de la case et le bord sur l'axe Y
    int xcase,ycase;    //position en haut à gauche de la case
    int coldetectbuffer;

    ///Affichage plateau
    for (int x=taillePlateauX-1 ; x>0-1 ; x--) {    //Affichage isométrique

        xcase=4+(x*taillex);
        ycase=270-(x*tailley);

        for (int y=0 ; y<taillePlateauY ; y++) {

            plateau[x][y].posX = xcase+taillex;
            plateau[x][y].posY = ycase+tailley;

            int sommet[8]={ xcase,ycase+tailley,                xcase+taillex,ycase,
                            xcase+(taillex*2),ycase+tailley,    xcase+taillex,ycase+(tailley*2)};
            polygon(bufferplateau, 4, sommet, makecol(x, y, 1));


            ///Affichage d'une case
            if (plateau[x][y].blocktype == 0) { //blocktype : type du bloc (mousse, pierre, roche, roche mousseuse, ..)
                masked_blit(Image.pblock[7],backscreen,0,0,xcase,ycase, Image.pblock[7]->w, Image.pblock[7]->h);
            }
            else if (plateau[x][y].blocktype == 1) {
                masked_blit(Image.pblock[6],backscreen,0,0,xcase,ycase, Image.pblock[6]->w, Image.pblock[6]->h);
            }
            else if (plateau[x][y].blocktype == 2) {
                masked_blit(Image.pblock[14],backscreen,0,0,xcase,ycase, Image.pblock[6]->w, Image.pblock[6]->h);
            }
            else if (plateau[x][y].blocktype == 3) {
                masked_blit(Image.pblock[15],backscreen,0,0,xcase,ycase, Image.pblock[6]->w, Image.pblock[6]->h);
            }
            line(backscreen, xcase,ycase+tailley,xcase+taillex,ycase+(tailley*2)              ,colblack);
            line(backscreen, xcase,ycase+tailley,xcase+taillex,ycase                          ,colblack);
            line(backscreen, xcase+taillex,ycase+(tailley*2),xcase+(taillex*2),ycase+tailley  ,colblack);
            line(backscreen, xcase+taillex,ycase,xcase+(taillex*2),ycase+tailley              ,colblack);

            xcase += taillex;
            ycase += tailley;
        }
    }

    ///plateau[xclic][yclic] donne la case que la souris touche
    coldetectbuffer = getpixel(bufferplateau,mouse_x,mouse_y);
    if (coldetectbuffer != colwhite && coldetectbuffer != colblack) {
        *xclic = getr(coldetectbuffer);
        *yclic = getg(coldetectbuffer);

        xcase = plateau[*xclic][*yclic].posX-taillex;
        ycase = plateau[*xclic][*yclic].posY-tailley;
        masked_blit(Image.pblock[0],backscreen,0,0,xcase,ycase, Image.pblock[0]->w, Image.pblock[0]->h);
        line(backscreen, xcase,ycase+tailley,xcase+taillex,ycase+(tailley*2)              ,colblack);
        line(backscreen, xcase,ycase+tailley,xcase+taillex,ycase                          ,colblack);
        line(backscreen, xcase+taillex,ycase+(tailley*2),xcase+(taillex*2),ycase+tailley  ,colblack);
        line(backscreen, xcase+taillex,ycase,xcase+(taillex*2),ycase+tailley              ,colblack);

    }
    else {  ///-1 si la souris est en dehors du plateau
        *xclic = -1;
        *yclic = -1;
    }

    destroy_bitmap(bufferplateau);
}

///==========================================================================================================///
void afficheTerrain(BITMAP *backscreen,BITMAP *buffer,Joueurs tabjoueur[],Cases plateau[taillePlateauX][taillePlateauY],int mouv_animation,pics Image, int cible[taillePlateauX][taillePlateauY]) {

    ///Affichage des obtacles/joueurs/bots (tout ce qu'il y a SUR le plateau)

    int c,percent;  //c = valeur random qui permet de faire un calcul

    for (int x=taillePlateauX-1 ; x>0-1 ; x--) {
        for (int y=0 ; y<taillePlateauY ; y++) {

            int xobstacle = plateau[x][y].posX-(Image.pblock[6]->w/2);
            int yobstacle = plateau[x][y].posY-(Image.pblock[6]->h/2)-14;
            //Si il y a un obtacle sur la case, on l'affiche
            if (plateau[x][y].type != 0) {   //supérieur à 0 : Obtacle infranchissable


                if (plateau[x][y].type == 3) {///pillager outpost
                    masked_blit(Image.pblock[8],backscreen,0,0,xobstacle,yobstacle, Image.pblock[8]->w, Image.pblock[8]->h);
                }
                else if (plateau[x][y].type == -1) { ///cobweb
                    masked_blit(Image.pblock[9],backscreen,0,0,xobstacle,yobstacle, Image.pblock[9]->w, Image.pblock[9]->h);
                }
                else if (plateau[x][y].type == -2) { ///feu
                    masked_blit(Image.pblock[11],backscreen,0,0,xobstacle,yobstacle, Image.pblock[11]->w, Image.pblock[11]->h);
                }
                else if (plateau[x][y].type == 1) {  ///cobblestone
                    masked_blit(Image.pblock[7],backscreen,0,0,xobstacle,yobstacle, Image.pblock[7]->w, Image.pblock[7]->h);
                }
                else if (plateau[x][y].type == 2) {   ///stone
                    masked_blit(Image.pblock[6],backscreen,0,0,xobstacle,yobstacle, Image.pblock[6]->w, Image.pblock[6]->h);
                }
                else if (plateau[x][y].type == 4) {   ///leaves
                    masked_blit(Image.pblock[18],backscreen,0,0,xobstacle,yobstacle, Image.pblock[18]->w, Image.pblock[18]->h);
                }
                else if (plateau[x][y].type == 5) {
                    masked_blit(Image.pblock[6],backscreen,0,0,xobstacle,yobstacle, Image.pblock[6]->w, Image.pblock[6]->h);
                    masked_blit(Image.pblock[6],backscreen,0,0,xobstacle,yobstacle-28, Image.pblock[6]->w, Image.pblock[6]->h);
                }

                if (plateau[x][y].type > 0) {
                    if (plateau[x][y].type==5) yobstacle-=28;
                    line(backscreen, xobstacle,yobstacle+22,xobstacle+33,yobstacle+(44)              ,colblack);
                    line(backscreen, xobstacle,yobstacle+22,xobstacle+33,yobstacle                   ,colblack);
                    line(backscreen, xobstacle+33,yobstacle+(44),xobstacle+(66),yobstacle+22         ,colblack);
                    line(backscreen, xobstacle+33,yobstacle,xobstacle+(66),yobstacle+22              ,colblack);

                    if (plateau[x][y].PVobstacle != plateau[x][y].PVMAXobstacle) {
                        percent = (plateau[x][y].PVobstacle*100)/plateau[x][y].PVMAXobstacle;
                        rectfill(backscreen,plateau[x][y].posX-14,plateau[x][y].posY-2-32,plateau[x][y].posX+13,plateau[x][y].posY+2-32,colblack);
                        if (plateau[x][y].type == 3) {
                            rectfill(backscreen,plateau[x][y].posX-13,plateau[x][y].posY-1-32,plateau[x][y].posX-13+(percent*0.25),plateau[x][y].posY+1-32,colpillager);
                        }
                        else {
                            rectfill(backscreen,plateau[x][y].posX-13,plateau[x][y].posY-1-32,plateau[x][y].posX-13+(percent*0.25),plateau[x][y].posY+1-32,colskeleton);
                        }
                    }
                }
            }

            int taillex = 33;
            int tailley = 22;
            int xcase = plateau[x][y].posX - taillex;
            int ycase = plateau[x][y].posY - tailley;
            if (cible[x][y]==2 && plateau[x][y].type!=0) {  ///Affichage deplacement possible
                masked_blit(Image.pblock[17],backscreen,0,0,xcase,ycase, Image.pblock[17]->w, Image.pblock[17]->h);  //moveblock2
            }
            else if (cible[x][y] == 1 && plateau[x][y].type!=0) {
                masked_blit(Image.pblock[16],backscreen,0,0,xcase,ycase, Image.pblock[16]->w, Image.pblock[16]->h);  //moveblock
            }
            else if (cible[x][y]==2 && plateau[x][y].type==0) {
                masked_blit(Image.pblock[4],backscreen,0,0,xcase,ycase, Image.pblock[4]->w, Image.pblock[4]->h);  //moveblock2
                line(backscreen, xcase,ycase+tailley,xcase+taillex,ycase+(tailley*2)              ,colblack);
                line(backscreen, xcase,ycase+tailley,xcase+taillex,ycase                          ,colblack);
                line(backscreen, xcase+taillex,ycase+(tailley*2),xcase+(taillex*2),ycase+tailley  ,colblack);
                line(backscreen, xcase+taillex,ycase,xcase+(taillex*2),ycase+tailley              ,colblack);
            }
            else if (cible[x][y] == 1 && plateau[x][y].type==0) {
                masked_blit(Image.pblock[3],backscreen,0,0,xcase,ycase, Image.pblock[3]->w, Image.pblock[3]->h);  //moveblock
                line(backscreen, xcase,ycase+tailley,xcase+taillex,ycase+(tailley*2)              ,colblack);
                line(backscreen, xcase,ycase+tailley,xcase+taillex,ycase                          ,colblack);
                line(backscreen, xcase+taillex,ycase+(tailley*2),xcase+(taillex*2),ycase+tailley  ,colblack);
                line(backscreen, xcase+taillex,ycase,xcase+(taillex*2),ycase+tailley              ,colblack);
            }
            else if (cible[x][y] == -2 && plateau[x][y].type==0) {
                masked_blit(Image.pblock[5],backscreen,0,0,xcase,ycase, Image.pblock[5]->w, Image.pblock[5]->h); //moveblock3
                line(backscreen, xcase,ycase+tailley,xcase+taillex,ycase+(tailley*2)              ,colblack);
                line(backscreen, xcase,ycase+tailley,xcase+taillex,ycase                          ,colblack);
                line(backscreen, xcase+taillex,ycase+(tailley*2),xcase+(taillex*2),ycase+tailley  ,colblack);
                line(backscreen, xcase+taillex,ycase,xcase+(taillex*2),ycase+tailley              ,colblack);
            }


            for(int i=0; i<nbJoueurMax+1; i++)  ///Affichage des entités sur le plateau (bot et joueur)
            {
                int col;
                if (tabjoueur[i].IDteam == 1) {
                    col = colteam1;
                }
                if (tabjoueur[i].IDteam == 2) {
                    col = colteam2;
                }
                if (tabjoueur[i].IDteam == 3) {
                    col = colteam3;
                }
                if (tabjoueur[i].IDteam == 4) {
                    col = colteam4;
                }
                if(tabjoueur[i].ecranX!=-1 && tabjoueur[i].posX==x && tabjoueur[i].posY==y)// && tabjoueur[i].ecranY>=plateau[x][y].posY)
                {
                    ellipsefill(backscreen, tabjoueur[i].ecranX, tabjoueur[i].ecranY+1, 20, 13, col);   //affichage cercle couleur de la type
                    masked_blit(Image.pblock[13],backscreen,0,0,tabjoueur[i].ecranX-33,tabjoueur[i].ecranY-22, Image.pblock[13]->w, Image.pblock[13]->h);

                    c = ((tabjoueur[i].sens*2)-2)+((tabjoueur[i].classe.type-1)*8);
                    if (mouv_animation < 1) {
                        c++;
                    }
                    masked_blit(Image.pmob[c],backscreen,0,0,tabjoueur[i].ecranX-50,tabjoueur[i].ecranY-70, Image.pmob[c]->w, Image.pmob[c]->h);
                }
                ///affichage des bots
                for(int b=0; b<tabjoueur[i].nbbots; b++)
                {
                    if(tabjoueur[i].ptbots[b]->posX==x && tabjoueur[i].ptbots[b]->posY==y)  //si il y a un bot
                    {
                        if(tabjoueur[i].ptbots[b]->IDteam!=-1){ //bot cave spider
                            if(tabjoueur[i].ptbots[b]->ecranX==-1 && tabjoueur[i].ptbots[b]->PV>0)
                            {
                                ellipsefill(backscreen, plateau[x][y].posX, plateau[x][y].posY+1, 20, 13, col);
                                masked_blit(Image.pblock[13],backscreen,0,0,plateau[x][y].posX-33,plateau[x][y].posY-22, Image.pblock[13]->w, Image.pblock[13]->h);

                                c = ((tabjoueur[i].ptbots[b]->sens*2)-2)+40;
                                masked_blit(Image.pmob[c],backscreen,0,0,plateau[x][y].posX-50,plateau[x][y].posY-70, Image.pmob[c]->w, Image.pmob[c]->h);
                                textprintf_centre_ex(backscreen, font, plateau[x][y].posX, plateau[x][y].posY-40, colblack, -1, "bot");
                                textprintf_centre_ex(backscreen, font, plateau[x][y].posX, plateau[x][y].posY-41, col, -1, "bot");

                                percent = (tabjoueur[i].ptbots[b]->PV*100)/tabjoueur[i].ptbots[b]->PVmax;
                                rectfill(backscreen,plateau[x][y].posX-26,plateau[x][y].posY-3-30,plateau[x][y].posX+26,plateau[x][y].posY+3-30,colwither);  //159
                                rectfill(backscreen,plateau[x][y].posX-25,plateau[x][y].posY-2-30,plateau[x][y].posX-25+(percent*0.5),plateau[x][y].posY+2-30,col);  //159
                            }
                            else
                            {       //mouvement du bot (cave spider)
                                ellipsefill(backscreen, tabjoueur[i].ptbots[b]->ecranX, tabjoueur[i].ptbots[b]->ecranY+1, 20, 13, col);
                                masked_blit(Image.pblock[13],backscreen,0,0,tabjoueur[i].ptbots[b]->ecranX-33,tabjoueur[i].ptbots[b]->ecranY-22, Image.pblock[13]->w, Image.pblock[13]->h);
                                c = ((tabjoueur[i].ptbots[b]->sens*2)-2)+40;
                                if (mouv_animation < 1) {
                                    c++;
                                }
                                masked_blit(Image.pmob[c],backscreen,0,0,tabjoueur[i].ptbots[b]->ecranX-50,tabjoueur[i].ptbots[b]->ecranY-70, Image.pmob[c]->w, Image.pmob[c]->h);
                            }
                        }
                        else{ ///bots naturel
                            if(tabjoueur[i].ptbots[b]->ecranX==-1 && tabjoueur[i].ptbots[b]->PV>0)
                            {
                                masked_blit(Image.pblock[13],backscreen,0,0,plateau[x][y].posX-33,plateau[x][y].posY-22, Image.pblock[13]->w, Image.pblock[13]->h);

                                c = ((tabjoueur[i].ptbots[b]->sens*2)-2)+48;
                                masked_blit(Image.pmob[c],backscreen,0,0,plateau[x][y].posX-50,plateau[x][y].posY-65, Image.pmob[c]->w, Image.pmob[c]->h);
                                textprintf_centre_ex(backscreen, font, plateau[x][y].posX, plateau[x][y].posY-40, colblack, -1, "bot");
                                textprintf_centre_ex(backscreen, font, plateau[x][y].posX, plateau[x][y].posY-41, makecol(200,200,200), -1, "bot");

                                percent = (tabjoueur[i].ptbots[b]->PV*100)/tabjoueur[i].ptbots[b]->PVmax;
                                rectfill(backscreen,plateau[x][y].posX-26,plateau[x][y].posY-3-30,plateau[x][y].posX+26,plateau[x][y].posY+3-30,colwither);  //159
                                rectfill(backscreen,plateau[x][y].posX-25,plateau[x][y].posY-2-30,plateau[x][y].posX-25+(percent*0.5),plateau[x][y].posY+2-30,makecol(150,250,0));  //159
                            }
                            else
                            {       //mouvement du bot (silverfish)
                                masked_blit(Image.pblock[13],backscreen,0,0,tabjoueur[i].ptbots[b]->ecranX-33,tabjoueur[i].ptbots[b]->ecranY-22, Image.pblock[13]->w, Image.pblock[13]->h);
                                c = ((tabjoueur[i].ptbots[b]->sens*2)-2)+48;
                                if (mouv_animation < 1) {
                                    c++;
                                }
                                masked_blit(Image.pmob[c],backscreen,0,0,tabjoueur[i].ptbots[b]->ecranX-50,tabjoueur[i].ptbots[b]->ecranY-65, Image.pmob[c]->w, Image.pmob[c]->h);
                            }
                        }
                    }
                }
            }

            ///Si il y a un joueur sur la case, on l'affiche
            if (plateau[x][y].ptjoueur != NULL && plateau[x][y].ptjoueur->ecranX==-1 && plateau[x][y].ptjoueur->classe.type>0) {

                //Affichage J1 / J2 au dessus de la tete de la couleur de la team

                int actjoueur;
                if  (plateau[x][y].ptjoueur == &tabjoueur[0]) {
                    actjoueur = 1;
                }
                else if  (plateau[x][y].ptjoueur == &tabjoueur[1]) {
                    actjoueur = 2;
                }
                else if  (plateau[x][y].ptjoueur == &tabjoueur[2]) {
                    actjoueur = 3;
                }
                else if  (plateau[x][y].ptjoueur == &tabjoueur[3]) {
                    actjoueur = 4;
                }

                int col;
                if (tabjoueur[actjoueur-1].IDteam == 1) {
                    col = colteam1;
                }
                if (tabjoueur[actjoueur-1].IDteam == 2) {
                    col = colteam2;
                }
                if (tabjoueur[actjoueur-1].IDteam == 3) {
                    col = colteam3;
                }
                if (tabjoueur[actjoueur-1].IDteam == 4) {
                    col = colteam4;
                }

                ellipsefill(backscreen, plateau[x][y].posX, plateau[x][y].posY+1, 20, 13, col);
                masked_blit(Image.pblock[13],backscreen,0,0,plateau[x][y].posX-33,plateau[x][y].posY-22, Image.pblock[13]->w, Image.pblock[13]->h);


                c = ((tabjoueur[actjoueur-1].sens*2)-2)+((tabjoueur[actjoueur-1].classe.type-1)*8);
                masked_blit(Image.pmob[c],backscreen,0,0,plateau[x][y].posX-50,plateau[x][y].posY-72, Image.pmob[c]->w, Image.pmob[c]->h);
                if (tabjoueur[actjoueur-1].classe.type != 3) {
                    textprintf_centre_ex(backscreen, font, plateau[x][y].posX, plateau[x][y].posY-80, colblack, -1, "%s", tabjoueur[actjoueur-1].nom);
                    textprintf_centre_ex(backscreen, font, plateau[x][y].posX, plateau[x][y].posY-81, col, -1, "%s", tabjoueur[actjoueur-1].nom);
                }
                else {
                    textprintf_centre_ex(backscreen, font, plateau[x][y].posX, plateau[x][y].posY-50, colblack, -1, "%s", tabjoueur[actjoueur-1].nom);
                    textprintf_centre_ex(backscreen, font, plateau[x][y].posX, plateau[x][y].posY-51, col, -1, "%s", tabjoueur[actjoueur-1].nom);
                }
            }
            if (plateau[x][y].type == -1) { ///toile d'araignée
                masked_blit(Image.pblock[10],backscreen,0,0,xobstacle,yobstacle, Image.pblock[10]->w, Image.pblock[10]->h);
            }
            if (plateau[x][y].type == -2) { ///feu
                masked_blit(Image.pblock[12],backscreen,0,0,xobstacle,yobstacle, Image.pblock[12]->w, Image.pblock[12]->h);
            }
        }
    }
}

///==========================================================================================================///
int Menusort(BITMAP *backscreen,BITMAP *buffer,Joueurs tabjoueur[],Joueurs actJoueur,int selectsort,pics Image) {

    ///Affichage des descriptions de chaque sorts + affichage des infos (nombre cout PA)
    ///Renvoie le sort selectionné à la fonction "MenuJeu"

            int taille = 60;
            textprintf_ex(backscreen,font,27,459,colwhite,-1,"Choisis un sort");
            textprintf_ex(backscreen,font,720,497,makecol(159,137,255),-1,"%d PA",actJoueur.PA);

                                     ///SORT CAC --------------------------------------------------
            masked_blit(Image.psort[0],backscreen,0,0,645,455,taille, taille);
            rectfill(buffer,645,455,645+taille,455+taille,bcolbutton10);

            if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton10 || key[clavCAC]) {
                rectfill(backscreen,175,520,285,590,colblack);
                rectfill(backscreen,20,450,160,470,colblack);
                textprintf_ex(backscreen,font,27,459,colwhite,-1,"Corps a corps");
                textprintf_ex(backscreen,font,177,530,colskeleton,-1,"Attaque sur");
                textprintf_ex(backscreen,font,177,541,colskeleton,-1,"les cases");
                textprintf_ex(backscreen,font,177,552,colskeleton,-1,"voisine");
                rectfill(backscreen,50,540,160,590,colblack);
                textprintf_ex(backscreen,font,67,563,colnone,-1,"---");

                //sort4=load_bitmap("pics/sorts/cac+.bmp",NULL);
                masked_blit(Image.psort[1],backscreen,0,0,645,455,taille,taille);
                selectsort = 4;
            }
            if(key[showClav]) {
                circlefill(backscreen, 645+Image.psort[0]->w-8, 455+8, 10, colwhite);
                circlefill(backscreen, 645+Image.psort[0]->w-8, 455+8, 9, colblack);
                textprintf_ex(backscreen, font, 645+Image.psort[0]->w-11, 455+5, colwhite, -1, "I");
            }


            if (actJoueur.classe.type == 1) {  ///ZOMBIE --------------------------------------------------

                                     ///SORT 1 DU ZOMBIE
                masked_blit(Image.psort[2],backscreen,0,0,525,525,taille, taille);
                rectfill(buffer,525,525,525+taille,525+taille,bcolbutton6);

                if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton6 || key[clavSort1]) {
                    rectfill(backscreen,175,520,285,590,colblack);
                    rectfill(backscreen,20,450,160,470,colblack);
                    textprintf_ex(backscreen,font,27,459,colwhite,-1,"Contamination");
                    textprintf_ex(backscreen,font,177,530,colzombie,-1,"Attaque a");
                    textprintf_ex(backscreen,font,177,541,colzombie,-1,"Courte portee");
                    textprintf_ex(backscreen,font,177,552,colzombie,-1,"Soigne 10 PV");
                    rectfill(backscreen,50,540,160,590,colblack);
                    textprintf_ex(backscreen,font,67,563,colhealh,-1,"+15 PV");

                    masked_blit(Image.psort[3],backscreen,0,0,525,525,taille, taille);
                    selectsort = 0;
                }
                                     ///SORT 2 DU ZOMBIE
                masked_blit(Image.psort[4],backscreen,0,0,590,525,taille, taille);
                rectfill(buffer,590,525,590+taille,525+taille,bcolbutton7);

                if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton7 || key[clavSort2]) {
                    rectfill(backscreen,175,520,285,590,colblack);
                    rectfill(backscreen,20,450,160,470,colblack);
                    textprintf_ex(backscreen,font,27,459,colwhite,-1,"Craft Tool");
                    textprintf_ex(backscreen,font,177,530,colzombie,-1,"Creer");
                    textprintf_ex(backscreen,font,177,541,colzombie,-1,"un outil,");
                    textprintf_ex(backscreen,font,177,552,colzombie,-1,"Effet Force");
                    textprintf_ex(backscreen,font,177,563,colzombie,-1,"(3 tours)");
                    rectfill(backscreen,50,540,160,590,colblack);
                    textprintf_ex(backscreen,font,67,563,colhealh,-1,"Force 3tours");
                    textprintf_ex(backscreen,font,67,573,colhealh,-1,"+20 Degats");

                    masked_blit(Image.psort[5],backscreen,0,0,590,525,taille, taille);
                    selectsort = 1;
                }
                                  ///SORT 3 DU ZOMBIE
                masked_blit(Image.psort[6],backscreen,0,0,655,525,taille, taille);
                rectfill(buffer,655,525,655+taille,525+taille,bcolbutton8);

                if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton8 || key[clavSort3]) {
                    rectfill(backscreen,175,520,285,590,colblack);
                    rectfill(backscreen,20,450,160,470,colblack);
                    textprintf_ex(backscreen,font,27,459,colwhite,-1,"Poison Health");
                    textprintf_ex(backscreen,font,177,530,colzombie,-1,"Retire tous");
                    textprintf_ex(backscreen,font,177,541,colzombie,-1,"les debuffs");
                    textprintf_ex(backscreen,font,177,552,colzombie,-1,"et se soigne");
                    rectfill(backscreen,50,540,160,590,colblack);
                    textprintf_ex(backscreen,font,67,563,colhealh,-1,"-debuffs");
                    textprintf_ex(backscreen,font,67,573,colhealh,-1,"+40 PV");

                    masked_blit(Image.psort[7],backscreen,0,0,655,525,taille,taille);
                    selectsort = 2;
                }
                               ///SORT 4 DU ZOMBIE
                masked_blit(Image.psort[8],backscreen,0,0,720,525,taille, taille);
                rectfill(buffer,720,525,720+taille,525+taille,bcolbutton9);

                if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton9 || key[clavSort4]) {
                    rectfill(backscreen,175,520,285,590,colblack);
                    rectfill(backscreen,20,450,160,470,colblack);
                    textprintf_ex(backscreen,font,27,459,colwhite,-1,"Drowned");
                    textprintf_ex(backscreen,font,177,530,colzombie,-1,"degat de zone");
                    textprintf_ex(backscreen,font,177,541,colzombie,-1,"puissant qui");
                    textprintf_ex(backscreen,font,177,552,colzombie,-1,"ralenti 1tour");
                    textprintf_ex(backscreen,font,177,563,colzombie,-1,"longue portee");
                    rectfill(backscreen,50,540,160,590,colblack);
                    textprintf_ex(backscreen,font,67,563,collenteur,-1,"Lenteur");
                    textprintf_ex(backscreen,font,67,573,collenteur,-1,"-1 PM");

                    masked_blit(Image.psort[9],backscreen,0,0,720,525,taille, taille);
                    selectsort = 3;
                }
            }
            else if (actJoueur.classe.type == 2) {  ///SKELETON --------------------------------------------------

                     ///SORT 1 DU SKELETON
                masked_blit(Image.psort[10],backscreen,0,0,525,525,taille, taille);
                rectfill(buffer,525,525,525+taille,525+taille,bcolbutton6);

                if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton6 || key[clavSort1]) {
                    rectfill(backscreen,175,520,285,590,colblack);
                    rectfill(backscreen,20,450,160,470,colblack);
                    textprintf_ex(backscreen,font,27,459,colwhite,-1,"Quick Arrow");
                    textprintf_ex(backscreen,font,177,530,colskeleton,-1,"Tire une");
                    textprintf_ex(backscreen,font,177,541,colskeleton,-1,"Fleche proche");
                    textprintf_ex(backscreen,font,177,552,colskeleton,-1,"de lui");
                    rectfill(backscreen,50,540,160,590,colblack);
                    textprintf_ex(backscreen,font,67,563,colnone,-1,"---");

                    masked_blit(Image.psort[11],backscreen,0,0,525,525,taille, taille);
                    selectsort = 0;
                }
                           ///SORT 2 DU SKELETON
                masked_blit(Image.psort[12],backscreen,0,0,590,525,taille, taille);
                rectfill(buffer,590,525,590+taille,525+taille,bcolbutton7);

                if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton7 || key[clavSort2]) {
                    rectfill(backscreen,175,520,285,590,colblack);
                    rectfill(backscreen,20,450,160,470,colblack);
                    textprintf_ex(backscreen,font,27,459,colwhite,-1,"Ice Arrow");
                    textprintf_ex(backscreen,font,177,530,colskeleton,-1,"La fleche");
                    textprintf_ex(backscreen,font,177,541,colskeleton,-1,"ralentit");
                    textprintf_ex(backscreen,font,177,552,colskeleton,-1,"l'ennemie");
                    textprintf_ex(backscreen,font,177,563,colskeleton,-1,"(2 tours)");
                    rectfill(backscreen,50,540,160,590,colblack);
                    textprintf_ex(backscreen,font,67,563,collenteur,-1,"Lenteur");
                    textprintf_ex(backscreen,font,67,573,collenteur,-1,"-1 PM");

                    masked_blit(Image.psort[13],backscreen,0,0,590,525,taille, taille);
                    selectsort = 1;
                }
                             ///SORT 3 DU SKELETON
                masked_blit(Image.psort[14],backscreen,0,0,655,525,taille, taille);
                rectfill(buffer,655,525,655+taille,525+taille,bcolbutton8);

                if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton8 || key[clavSort3]) {
                    rectfill(backscreen,175,520,285,590,colblack);
                    rectfill(backscreen,20,450,160,470,colblack);
                    textprintf_ex(backscreen,font,27,459,colwhite,-1,"Wither effect");
                    textprintf_ex(backscreen,font,177,530,colskeleton,-1,"Mets un coup");
                    textprintf_ex(backscreen,font,177,541,colskeleton,-1,"et l'effet");
                    textprintf_ex(backscreen,font,177,552,colskeleton,-1,"wither");
                    textprintf_ex(backscreen,font,177,563,colskeleton,-1,"(3 tours)");
                    rectfill(backscreen,50,540,160,590,colblack);
                    textprintf_ex(backscreen,font,67,563,colwither,-1,"Wither");
                    textprintf_ex(backscreen,font,67,573,colwither,-1,"-20 PV/tour");

                    masked_blit(Image.psort[15],backscreen,0,0,655,525,taille, taille);
                    selectsort = 2;
                }
                           ///SORT 4 DU SKELETON
                masked_blit(Image.psort[16],backscreen,0,0,720,525,taille, taille);
                rectfill(buffer,720,525,720+taille,525+taille,bcolbutton9);

                if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton9 || key[clavSort4]) {
                    rectfill(backscreen,175,520,285,590,colblack);
                    rectfill(backscreen,20,450,160,470,colblack);
                    textprintf_ex(backscreen,font,27,459,colwhite,-1,"Armor");
                    textprintf_ex(backscreen,font,177,530,colskeleton,-1,"Se protege");
                    textprintf_ex(backscreen,font,177,541,colskeleton,-1,"Effet");
                    textprintf_ex(backscreen,font,177,552,colskeleton,-1,"resistance");
                    textprintf_ex(backscreen,font,177,563,colskeleton,-1,"(2 tours)");
                    rectfill(backscreen,50,540,160,590,colblack);
                    textprintf_ex(backscreen,font,67,563,colskeleton,-1,"Resistance");
                    textprintf_ex(backscreen,font,67,573,colskeleton,-1,"(2 tours)");

                    masked_blit(Image.psort[17],backscreen,0,0,720,525,taille, taille);
                    selectsort = 3;
                }
            }
            else if (actJoueur.classe.type == 3) {  ///SPIDER --------------------------------------------------

                        ///SORT 1 SPIDER
                masked_blit(Image.psort[18],backscreen,0,0,525,525,taille, taille);
                rectfill(buffer,525,525,525+taille,525+taille,bcolbutton6);

                if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton6 || key[clavSort1]) {
                    rectfill(backscreen,175,520,285,590,colblack);
                    rectfill(backscreen,20,450,160,470,colblack);
                    textprintf_ex(backscreen,font,27,459,colwhite,-1,"Snicky Attack");
                    textprintf_ex(backscreen,font,177,530,colspider,-1,"Morsure");
                    textprintf_ex(backscreen,font,177,541,colspider,-1,"qui");
                    textprintf_ex(backscreen,font,177,552,colspider,-1,"empoisonne");
                    textprintf_ex(backscreen,font,177,563,colspider,-1,"4 tours");
                    rectfill(backscreen,50,540,160,590,colblack);
                    textprintf_ex(backscreen,font,67,563,colpoison,-1,"Poison");
                    textprintf_ex(backscreen,font,67,573,colpoison,-1,"-10 PV");

                    masked_blit(Image.psort[19],backscreen,0,0,525,525,taille, taille);
                    selectsort = 0;
                }
                           ///SORT 2 SPIDER
                masked_blit(Image.psort[20],backscreen,0,0,590,525,taille, taille);
                rectfill(buffer,590,525,590+taille,525+taille,bcolbutton7);

                if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton7 || key[clavSort2]) {
                    rectfill(backscreen,175,520,285,590,colblack);
                    rectfill(backscreen,20,450,160,470,colblack);
                    textprintf_ex(backscreen,font,27,459,colwhite,-1,"Cobweb");
                    textprintf_ex(backscreen,font,177,530,colspider,-1,"Pose une");
                    textprintf_ex(backscreen,font,177,541,colspider,-1,"toile sur");
                    textprintf_ex(backscreen,font,177,552,colspider,-1,"le terrain");
                    textprintf_ex(backscreen,font,177,563,colspider,-1,"(8 tours)");
                    rectfill(backscreen,50,540,160,590,colblack);
                    textprintf_ex(backscreen,font,67,563,colskeleton,-1,"Piege");
                    textprintf_ex(backscreen,font,67,573,colskeleton,-1,"PM = 1");

                    masked_blit(Image.psort[21],backscreen,0,0,590,525,taille, taille);
                    selectsort = 1;
                }
                                ///SORT 3 SPIDER
                masked_blit(Image.psort[22],backscreen,0,0,655,525,taille, taille);
                rectfill(buffer,655,525,655+taille,525+taille,bcolbutton8);

                if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton8 || key[clavSort3]) {
                    rectfill(backscreen,175,520,285,590,colblack);
                    rectfill(backscreen,20,450,160,470,colblack);
                    textprintf_ex(backscreen,font,27,459,colwhite,-1,"Call for help");
                    textprintf_ex(backscreen,font,177,530,colspider,-1,"Invoque");
                    textprintf_ex(backscreen,font,177,541,colspider,-1,"une petite");
                    textprintf_ex(backscreen,font,177,552,colspider,-1,"araignee");
                    textprintf_ex(backscreen,font,177,563,colspider,-1,"avec 110 PV");
                    rectfill(backscreen,50,540,160,590,colblack);
                    textprintf_ex(backscreen,font,67,563,colspider,-1,"Invocation");
                    textprintf_ex(backscreen,font,67,573,colspider,-1,"Degat CAC:50");

                    masked_blit(Image.psort[23],backscreen,0,0,655,525,taille, taille);
                    selectsort = 2;
                }

                                   ///SORT 4 SPIDER
                masked_blit(Image.psort[24],backscreen,0,0,720,525,taille, taille);
                rectfill(buffer,720,525,720+taille,525+taille,bcolbutton9);

                if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton9 || key[clavSort4]) {
                    rectfill(backscreen,175,520,285,590,colblack);
                    rectfill(backscreen,20,450,160,470,colblack);
                    textprintf_ex(backscreen,font,27,459,colwhite,-1,"Rest");
                    textprintf_ex(backscreen,font,177,530,colspider,-1,"Se soigne");
                    textprintf_ex(backscreen,font,177,541,colspider,-1,"legerement");
                    rectfill(backscreen,50,540,160,590,colblack);
                    textprintf_ex(backscreen,font,67,563,colhealh,-1,"+10 PV");

                    masked_blit(Image.psort[25],backscreen,0,0,720,525,taille, taille);
                    selectsort = 3;
                }
            }
            else if (actJoueur.classe.type == 4) {  ///BLAZE --------------------------------------------------

                    ///SORT 1 BLAZE
                masked_blit(Image.psort[26],backscreen,0,0,525,525,taille, taille);
                rectfill(buffer,525,525,525+taille,525+taille,bcolbutton6);

                if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton6 || key[clavSort1]) {
                    rectfill(backscreen,175,520,285,590,colblack);
                    rectfill(backscreen,20,450,160,470,colblack);
                    textprintf_ex(backscreen,font,27,459,colwhite,-1,"Fireball");
                    textprintf_ex(backscreen,font,177,530,colblaze,-1,"Tire une");
                    textprintf_ex(backscreen,font,177,541,colblaze,-1,"Boule de feu");
                    textprintf_ex(backscreen,font,177,552,colblaze,-1,"a longue");
                    textprintf_ex(backscreen,font,177,563,colblaze,-1,"portee");
                    rectfill(backscreen,50,540,160,590,colblack);
                    textprintf_ex(backscreen,font,67,563,colnone,-1,"---");

                    masked_blit(Image.psort[27],backscreen,0,0,525,525,taille, taille);
                    selectsort = 0;
                }
                      ///SORT 2 BLAZE
                masked_blit(Image.psort[28],backscreen,0,0,590,525,taille, taille);
                rectfill(buffer,590,525,590+taille,525+taille,bcolbutton7);

                if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton7 || key[clavSort2]) {
                    rectfill(backscreen,175,520,285,590,colblack);
                    rectfill(backscreen,20,450,160,470,colblack);
                    textprintf_ex(backscreen,font,27,459,colwhite,-1,"Firewall");
                    textprintf_ex(backscreen,font,177,530,colblaze,-1,"Pose une");
                    textprintf_ex(backscreen,font,177,541,colblaze,-1,"Flame sur");
                    textprintf_ex(backscreen,font,177,552,colblaze,-1,"le terrain");
                    textprintf_ex(backscreen,font,177,563,colblaze,-1,"(3 tours)");
                    rectfill(backscreen,50,540,160,590,colblack);
                    textprintf_ex(backscreen,font,67,563,colblaze,-1,"Piege");
                    textprintf_ex(backscreen,font,67,573,colblaze,-1,"-50 PV");

                    masked_blit(Image.psort[29],backscreen,0,0,590,525,taille, taille);
                    selectsort = 1;
                }
                        ///SORT 3 BLAZE
                masked_blit(Image.psort[30],backscreen,0,0,655,525,taille, taille);
                rectfill(buffer,655,525,655+taille,525+taille,bcolbutton8);

                if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton8 || key[clavSort3]) {
                    rectfill(backscreen,175,520,285,590,colblack);
                    rectfill(backscreen,20,450,160,470,colblack);
                    textprintf_ex(backscreen,font,27,459,colwhite,-1,"Eruption");
                    textprintf_ex(backscreen,font,177,530,colblaze,-1,"Creer une");
                    textprintf_ex(backscreen,font,177,541,colblaze,-1,"puissante");
                    textprintf_ex(backscreen,font,177,552,colblaze,-1,"explosion");
                    textprintf_ex(backscreen,font,177,563,colblaze,-1,"");
                    rectfill(backscreen,50,540,160,590,colblack);
                    textprintf_ex(backscreen,font,67,563,colblaze,-1,"Taille : 2");

                    masked_blit(Image.psort[31],backscreen,0,0,655,525,taille, taille);
                    selectsort = 2;
                }

                             ///SORT 4 BLAZE
                masked_blit(Image.psort[32],backscreen,0,0,720,525,taille, taille);
                rectfill(buffer,720,525,720+taille,525+taille,bcolbutton9);

                if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton9 || key[clavSort4]) {
                    rectfill(backscreen,175,520,285,590,colblack);
                    rectfill(backscreen,20,450,160,470,colblack);
                    textprintf_ex(backscreen,font,27,459,colwhite,-1,"Alchemy");
                    textprintf_ex(backscreen,font,177,530,colblaze,-1,"Boost un de");
                    textprintf_ex(backscreen,font,177,541,colblaze,-1,"ses allies");
                    textprintf_ex(backscreen,font,177,552,colblaze,-1,"proche");
                    textprintf_ex(backscreen,font,177,563,colblaze,-1,"(3 tours)");
                    rectfill(backscreen,50,540,160,590,colblack);
                    textprintf_ex(backscreen,font,67,563,colhealh,-1,"Resistance et");
                    textprintf_ex(backscreen,font,67,573,colhealh,-1,"Force 3tours");

                    masked_blit(Image.psort[33],backscreen,0,0,720,525,taille, taille);
                    selectsort = 3;
                }
            }
            else if (actJoueur.classe.type == 5) {  ///PILLAGER --------------------------------------------------
                 ///SORT 1 PILLAGER
                masked_blit(Image.psort[34],backscreen,0,0,525,525,taille, taille);
                rectfill(buffer,525,525,525+taille,525+taille,bcolbutton6);

                if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton6 || key[clavSort1]) {
                    rectfill(backscreen,175,520,285,590,colblack);
                    rectfill(backscreen,20,450,160,470,colblack);
                    textprintf_ex(backscreen,font,27,459,colwhite,-1,"Crossbow Shot");
                    textprintf_ex(backscreen,font,177,530,colpillager,-1,"Tire puissant");
                    textprintf_ex(backscreen,font,177,541,colpillager,-1,"et avec une");
                    textprintf_ex(backscreen,font,177,552,colpillager,-1,"excellente");
                    textprintf_ex(backscreen,font,177,563,colpillager,-1,"portee");
                    rectfill(backscreen,50,540,160,590,colblack);
                    textprintf_ex(backscreen,font,67,563,colnone,-1,"---");

                    masked_blit(Image.psort[35],backscreen,0,0,525,525,taille, taille);
                    selectsort = 0;
                }
                               ///SORT 2 PILLAGER
                masked_blit(Image.psort[36],backscreen,0,0,590,525,taille, taille);
                rectfill(buffer,590,525,590+taille,525+taille,bcolbutton7);

                if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton7 || key[clavSort2]) {
                    rectfill(backscreen,175,520,285,590,colblack);
                    rectfill(backscreen,20,450,160,470,colblack);
                    textprintf_ex(backscreen,font,27,459,colwhite,-1,"Pillager Outpost");
                    textprintf_ex(backscreen,font,177,530,colpillager,-1,"Se protege");
                    textprintf_ex(backscreen,font,177,541,colpillager,-1,"en posans");
                    textprintf_ex(backscreen,font,177,552,colpillager,-1,"un bloc sur");
                    textprintf_ex(backscreen,font,177,563,colpillager,-1,"le terrain");
                    rectfill(backscreen,50,540,160,590,colblack);
                    textprintf_ex(backscreen,font,67,563,colpillager,-1,"Obstacle");
                    textprintf_ex(backscreen,font,67,573,colpillager,-1,"PV = 200");

                    masked_blit(Image.psort[37],backscreen,0,0,590,525,taille, taille);
                    selectsort = 1;
                }
                          ///SORT 3 PILLAGER
                masked_blit(Image.psort[38],backscreen,0,0,655,525,taille, taille);
                rectfill(buffer,655,525,655+taille,525+taille,bcolbutton8);

                if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton8 || key[clavSort3]) {
                    rectfill(backscreen,175,520,285,590,colblack);
                    rectfill(backscreen,20,450,160,470,colblack);
                    textprintf_ex(backscreen,font,27,459,colwhite,-1,"Piercing");
                    textprintf_ex(backscreen,font,177,530,colpillager,-1,"Tire une fleche");
                    textprintf_ex(backscreen,font,177,541,colpillager,-1,"spectrale");
                    textprintf_ex(backscreen,font,177,552,colpillager,-1,"seulement en");
                    textprintf_ex(backscreen,font,177,563,colpillager,-1,"ligne droite");
                    rectfill(backscreen,50,540,160,590,colblack);
                    textprintf_ex(backscreen,font,67,563,colblaze,-1,"Traverse");
                    textprintf_ex(backscreen,font,67,573,colblaze,-1,"Obtacles");

                    masked_blit(Image.psort[39],backscreen,0,0,655,525,taille, taille);
                    selectsort = 2;
                }
                         ///SORT 4 PILLAGER
                masked_blit(Image.psort[40],backscreen,0,0,720,525,taille, taille);
                rectfill(buffer,720,525,720+taille,525+taille,bcolbutton9);

                if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton9 || key[clavSort4]) {
                    rectfill(backscreen,175,520,285,590,colblack);
                    rectfill(backscreen,20,450,160,470,colblack);
                    textprintf_ex(backscreen,font,27,459,colwhite,-1,"Totem of Undying");
                    textprintf_ex(backscreen,font,177,530,colpillager,-1,"Utilisable");
                    textprintf_ex(backscreen,font,177,541,colpillager,-1,"si le Joueur");
                    textprintf_ex(backscreen,font,177,552,colpillager,-1,"a moins de");
                    textprintf_ex(backscreen,font,177,563,colpillager,-1,"200 PV");
                    rectfill(backscreen,50,540,160,590,colblack);
                    textprintf_ex(backscreen,font,67,563,colhealh,-1,"+100 PV");
                    textprintf_ex(backscreen,font,67,573,colskeleton,-1,"Resistance");

                    masked_blit(Image.psort[41],backscreen,0,0,720,525,taille, taille);
                    selectsort = 3;
                }
            }

            if(key[showClav]) {
                ///sort 1
                circlefill(backscreen, 525+Image.psort[0]->w-8, 525+8, 10, colwhite);
                circlefill(backscreen, 525+Image.psort[0]->w-8, 525+8, 9, colblack);
                textprintf_ex(backscreen, font, 525+Image.psort[0]->w-11, 525+5, colwhite, -1, "J");
                ///sort 2
                circlefill(backscreen, 590+Image.psort[0]->w-8, 525+8, 10, colwhite);
                circlefill(backscreen, 590+Image.psort[0]->w-8, 525+8, 9, colblack);
                textprintf_ex(backscreen, font, 590+Image.psort[0]->w-11, 525+5, colwhite, -1, "K");
                ///sort 3
                circlefill(backscreen, 655+Image.psort[0]->w-8, 525+8, 10, colwhite);
                circlefill(backscreen, 655+Image.psort[0]->w-8, 525+8, 9, colblack);
                textprintf_ex(backscreen, font, 655+Image.psort[0]->w-11, 525+5, colwhite, -1, "L");
                ///sort 4
                circlefill(backscreen, 720+Image.psort[0]->w-8, 525+8, 10, colwhite);
                circlefill(backscreen, 720+Image.psort[0]->w-8, 525+8, 9, colblack);
                textprintf_ex(backscreen, font, 720+Image.psort[0]->w-11, 525+5, colwhite, -1, "M");
            }

            return selectsort;
}

///==========================================================================================================///
void MenuJeu(BITMAP *backscreen,BITMAP *buffer,Joueurs tabjoueur[],Cases plateau[taillePlateauX][taillePlateauY],int *tour, int *val,int xclic,int yclic,int *delay,int nbJoueur,int *menusort, double* debut, pics Image) {

    ///Sous-programme coeur du Jeu :
    ///Affichage de toutes les informations (Joueur dont c'est le tour, prochain tour, vie de tous les Joueurs, PA, PV, PM, effets, etc etc...)
    ///Et permet egalement au joueur dont c'est le tour de choisir ce qu'il souhaite faire
    ///(Deplacement, Sorts, Fin tour, Quitter le jeu, Sauvegarde, Regles du Jeu, ...)

    ///C'est elle qui appel les fonctions Deplacement, LanceSorts, et EffetTour.

    int col = makecol(0,0,0);
    if (*delay > 0) {
        *delay -= 1;
    }

    ///--------------------------------------------------------///Move (Si on clique le joueur dont c'est le tour)
    if (xclic != -1) {
        if (plateau[xclic][yclic].ptjoueur == &tabjoueur[*tour-1] && mouse_b == 1 && *delay == 0) {
            if(tabjoueur[*tour-1].PM <= 0) {
                blit(Image.pmenu[2],backscreen,0,0,SCREEN_W+5-Image.pmenu[2]->w,392,Image.pmenu[2]->w, Image.pmenu[2]->h);
                textprintf_ex(backscreen,font,SCREEN_W+5-Image.pmenu[2]->w+30,410,makecol(220,0,0),-1,"PM insuffisant");
            }
            else{
                deplacement(backscreen, buffer, &tabjoueur[*tour-1],tabjoueur, plateau, debut, Image);
                *delay = 15;
            }
        }
    }


    ///--------------------------------------------------------//menu info joueur

    int joueurclic; //Contient le joueur que la souris touche, si jamais la souris ne touche aucun joueur, alors clicjoueur = tour - 1
                    //Cette variable permet d'afficher les stats du joueurs choisis
    if (xclic != -1) {
        if  (plateau[xclic][yclic].ptjoueur == &tabjoueur[0]) {
            joueurclic = 0;
        }
        else if  (plateau[xclic][yclic].ptjoueur == &tabjoueur[1]) {
            joueurclic = 1;
        }
        else if  (plateau[xclic][yclic].ptjoueur == &tabjoueur[2]) {
            joueurclic = 2;
        }
        else if  (plateau[xclic][yclic].ptjoueur == &tabjoueur[3]) {
            joueurclic = 3;
        }
        else {
            joueurclic = *tour-1;
        }
    }
    if (xclic == -1) {
        joueurclic = *tour-1;
    }

    if (*menusort == 0 || joueurclic != *tour-1) {    //Si menusort=0, alors on affiche les infos du joueurs

    masked_blit(Image.pmenu[0],backscreen,0,0,800-345,600-170,Image.pmenu[0]->w, Image.pmenu[0]->h);    //affiche menu de base
    masked_blit(Image.pmenu[1],backscreen,0,0,-5,600-170,Image.pmenu[1]->w, Image.pmenu[1]->h);

    textprintf_ex(backscreen,font,691,463,makecol(255,61,0),-1,"%d PV",tabjoueur[joueurclic].PV);
    textprintf_ex(backscreen,font,695,494,makecol(159,137,255),-1,"%d PA",tabjoueur[joueurclic].PA);
    textprintf_ex(backscreen,font,701,526,makecol(159,215,94),-1,"%d PM",tabjoueur[joueurclic].PM);

    if (tabjoueur[joueurclic].force > 0) {
        textprintf_ex(backscreen,font,538,565,colforce,-1,"force:%d",tabjoueur[joueurclic].force);
    }
    if (tabjoueur[joueurclic].def > 0) {
        textprintf_ex(backscreen,font,630,555,colskeleton,-1,"resistance:%d",tabjoueur[joueurclic].def);
    }
    if (tabjoueur[joueurclic].poison > 0) {
        textprintf_ex(backscreen,font,605,565,colpoison,-1,"poison:%d",tabjoueur[joueurclic].poison);
    }
    if (tabjoueur[joueurclic].wither > 0) {
        textprintf_ex(backscreen,font,680,565,colwither,-1,"wither:%d",tabjoueur[joueurclic].wither);
    }
    if (tabjoueur[joueurclic].lenteur > 0) {
        textprintf_ex(backscreen,font,538,555,collenteur,-1,"lenteur:%d",tabjoueur[joueurclic].lenteur);
    }
    if (tabjoueur[joueurclic].force == 0 && tabjoueur[joueurclic].lenteur == 0 && tabjoueur[joueurclic].def == 0 && tabjoueur[joueurclic].poison == 0 && tabjoueur[joueurclic].wither == 0) {
        textprintf_ex(backscreen,font,645,560,makecol(190,190,210),-1,"aucun effet");
    }

    textprintf_ex(backscreen,font,27,459,colwhite,-1,"%s", tabjoueur[joueurclic].nom);
    textprintf_ex(backscreen,font,111,459,colwhite,-1,"Team%d",tabjoueur[joueurclic].IDteam);

    masked_blit(Image.pmenu[80+(tabjoueur[joueurclic].IDteam-1)],backscreen,0,0,110,476,Image.pmenu[80]->w, Image.pmenu[80]->h);   ///Affichage team
    masked_blit(Image.pmenu[107+(tabjoueur[joueurclic].classe.type-1)],backscreen,0,0,20,480,Image.pmenu[107]->w, Image.pmenu[107]->h);  ///Affichage classe

    ///--------------------------------------------------------//boutonAfficheMove
    if (joueurclic == *tour-1) {

        blit(Image.pmenu[11],backscreen,0,0,110,515,Image.pmenu[11]->w, Image.pmenu[11]->h);
        rectfill(buffer,110,515,110+80,515+80,bcolbutton4);


        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton4 || key[clavDeplacement]) { //Si on touche le bouton Move
            blit(Image.pmenu[12],backscreen,0,0,110,515,Image.pmenu[12]->w, Image.pmenu[12]->h);

            if(tabjoueur[*tour-1].PM <= 0) {
                blit(Image.pmenu[2],backscreen,0,0,SCREEN_W+5-Image.pmenu[2]->w,392,Image.pmenu[2]->w, Image.pmenu[2]->h);
                textprintf_ex(backscreen,font,SCREEN_W+5-Image.pmenu[2]->w+30,410,makecol(220,0,0),-1,"PM insuffisant");
            }
            else if ((mouse_b == 1 && *delay == 0) || key[clavDeplacement]) {
                deplacement(backscreen, buffer, &tabjoueur[*tour-1],tabjoueur, plateau, debut, Image);
                *delay = 15;
            }
        }
        if(key[showClav]) {
            circlefill(backscreen, 110+Image.pmenu[11]->w-8, 515+8, 10, colwhite);
            circlefill(backscreen, 110+Image.pmenu[11]->w-8, 515+8, 9, colblack);
            textprintf_ex(backscreen, font, 110+Image.pmenu[11]->w-11, 515+5, colwhite, -1, "D");
        }

    ///--------------------------------------------------------//boutonAfficheSorts

        blit(Image.pmenu[13],backscreen,0,0,200,515,Image.pmenu[13]->w, Image.pmenu[13]->h);
        rectfill(buffer,200,515,200+80,515+80,bcolbutton3);


        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton3 || key[clavAttaque]) { //Si on touche le bouton Move
            blit(Image.pmenu[14],backscreen,0,0,200,515,Image.pmenu[14]->w, Image.pmenu[14]->h);

            if (mouse_b == 1 || key[clavAttaque]) {
                *menusort = 1;
            }
        }
        if(key[showClav]) {
            circlefill(backscreen, 200+Image.pmenu[13]->w-8, 515+8, 10, colwhite);
            circlefill(backscreen, 200+Image.pmenu[13]->w-8, 515+8, 9, colblack);
            textprintf_ex(backscreen, font, 200+Image.pmenu[13]->w-11, 515+5, colwhite, -1, "Q");
        }
    }   //Fin du "if (joueurclic == *tour-1)"

    ///--------------------------------------------------------// Si le joueur regarde les info d'un autre joueur
    }   //Fin du "if (menusort == 0)"
    if (joueurclic != *tour-1) {
        blit(Image.pmenu[2],backscreen,0,0,-5,392,Image.pmenu[2]->w, Image.pmenu[2]->h);
        textprintf_ex(backscreen,font,25,410,colwhite,-1,"Info de %s",tabjoueur[joueurclic].nom);
    }


    ///--------------------------------------------------------// Menu de choix des sorts
    if (*menusort == 1) {

        if (joueurclic == *tour-1) {    //On affiche le menu des sorts sauf si on regarde les info d'un autre joueur
            masked_blit(Image.pmenu[6],backscreen,0,0,800-345,600-170,Image.pmenu[6]->w, Image.pmenu[6]->h);
            masked_blit(Image.pmenu[7],backscreen,0,0,-5,600-170,Image.pmenu[7]->w, Image.pmenu[7]->h);
            masked_blit(Image.pmenu[9],backscreen,0,0,0,390,Image.pmenu[9]->w, Image.pmenu[9]->h);
        }
        rectfill(buffer,0,390,Image.pmenu[9]->w,390+Image.pmenu[9]->h,bcolbutton5);

        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton5 || key[clavRetour]) { //Si on touche la fleche retour
            masked_blit(Image.pmenu[10],backscreen,0,0,0,390,Image.pmenu[10]->w, Image.pmenu[10]->h);

            if (mouse_b == 1 || key[clavRetour]) {
                *menusort = 0;
            }
        }
        if(key[showClav]) {
            circlefill(backscreen, 0+Image.pmenu[9]->w-8, 390+8, 10, colwhite);
            circlefill(backscreen, 0+Image.pmenu[9]->w-8, 390+8, 9, colblack);
            textprintf_ex(backscreen, font, 0+Image.pmenu[9]->w-11, 390+5, colwhite, -1, "W");
        }

        int selectsort = -1;   //-1 : rien / 0 : sort[0] / 1 : sort[1] / etc...

        ///------------------------------------- Sort de toutes les classes ----------------------------------------------///

        if (joueurclic == *tour-1) {    //Si on ne regarde pas les infos d'un autre joueur..
            selectsort = Menusort(backscreen,buffer,tabjoueur,tabjoueur[*tour-1],selectsort,Image);
        }
        ///---------------------------------------------------------------------------------------///Si il appuie sur un sort (et le lance dcp)
        if(tabjoueur[*tour-1].PA < tabjoueur[*tour-1].sort[selectsort].PA) {
            blit(Image.pmenu[2],backscreen,0,0,SCREEN_W+5-Image.pmenu[2]->w,392,Image.pmenu[2]->w, Image.pmenu[2]->h);
            textprintf_ex(backscreen,font,SCREEN_W+5-Image.pmenu[2]->w+30,410,makecol(220,0,0),-1,"PA insuffisant");
        }
        else if (((selectsort > -1 && mouse_b == 1 && *delay == 0) || key[clavCAC] || key[clavSort1] || key[clavSort2] || key[clavSort3] || key[clavSort4])) {
            lancerSorts(backscreen,buffer,plateau,tabjoueur,*tour-1,tabjoueur[*tour-1].sort[selectsort], debut, Image);
            *delay = 10;
        }

        if (selectsort != -1) {                                 ///Quand le joueur regarde les info d'un sort
            masked_blit(Image.pmenu[8],backscreen,0,0,-5,600-170,Image.pmenu[8]->w, Image.pmenu[8]->h);

            if (tabjoueur[*tour-1].sort[selectsort].degat > 0) {
                textprintf_ex(backscreen,font,69,494,makecol(255,55,55),-1,"Degat: %d",tabjoueur[*tour-1].sort[selectsort].degat);
            }
            else {
                textprintf_ex(backscreen,font,69,494,colnone,-1,"---");
            }
            if (tabjoueur[*tour-1].sort[selectsort].porteMin != 0) {
                textprintf_ex(backscreen,font,69,529,makecol(159,137,255),-1,"Zone: %d-%d",tabjoueur[*tour-1].sort[selectsort].porteMin,tabjoueur[*tour-1].sort[selectsort].porteMax);
            }
            else {
                textprintf_ex(backscreen,font,69,529,colnone,-1,"---");
            }
            textprintf_ex(backscreen,font,177,574,colwhite,-1,"Cout : %d PA",tabjoueur[*tour-1].sort[selectsort].PA);
        }
    }

    ///--------------------------------------------------------//Menuplayer4 (Healthbarre + tours)
    //Menu healthbar
    masked_blit(Image.pmenu[nbJoueur+1],backscreen,0,0,0,0,Image.pmenu[nbJoueur+1]->w, Image.pmenu[nbJoueur+1]->h);

    int percent;
    for (int i=0; i<nbJoueur; i++) {
        if (tabjoueur[i].PV > tabjoueur[i].PVmax) {
            tabjoueur[i].PV = tabjoueur[i].PVmax;
        }

        if (tabjoueur[i].IDteam == 1) {
            col = colteam1;
        }
        else if (tabjoueur[i].IDteam == 2) {
            col = colteam2;
        }
        else if (tabjoueur[i].IDteam == 3) {
            col = colteam3;
        }
        else if (tabjoueur[i].IDteam == 4) {
            col = colteam4;
        }
        percent = (tabjoueur[i].PV*100)/tabjoueur[i].PVmax;
        textprintf_ex(backscreen,font,13,53+(i*20),col,-1,"%s",tabjoueur[i].nom);
        rectfill(backscreen,75,50+(i*20),75+(percent*0.84),64+(i*20),makecol(100,250,0));  //159
        textprintf_ex(backscreen,font,106,54+(i*20),colblack,-1,"%d%%",percent);
        textprintf_ex(backscreen,font,105,54+(i*20),colwhite,-1,"%d%%",percent);

    }
    textprintf_ex(backscreen,font,50,24,colwhite,-1,"Healthbar");
    textprintf_ex(backscreen,font,180,24,colwhite,-1,"TURN:%s",tabjoueur[*tour-1].nom);
    int i=0;
    int tmptour=*tour-1;
    do{
        i++;
        tmptour += 1;
        if (tmptour >= nbJoueur) {
            tmptour = 0;
        }
    }while(tabjoueur[*tour-1].PV<=0 && i<nbJoueur);
    textprintf_ex(backscreen,font,180,54,colskeleton,-1,"NEXT:%s",tabjoueur[tmptour].nom);

    ///barre de temps
    rectfill(backscreen, 179, 36, 181+95, 42, colwither);
    rectfill(backscreen, 180, 37, 180+(clock()-*debut)/CLOCKS_PER_SEC*95/timeMaxTour, 41, colwhite);

    ///--------------------------------------------------------//boutonFinTour
    blit(Image.pmenu[70], backscreen,0,0, 550, 10, Image.pmenu[70]->w, Image.pmenu[70]->h);
    rectfill(buffer,    550,10,550+Image.pmenu[70]->w,10+Image.pmenu[70]->h,bcolbutton2);


    if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton2 || (clock()-*debut)/CLOCKS_PER_SEC>=timeMaxTour || key[clavFinTour]) { //Si on touche le bouton FinTour
        blit(Image.pmenu[71], backscreen,0,0, 550, 10, Image.pmenu[71]->w, Image.pmenu[71]->h);

        if ((mouse_b == 1 && *delay == 0) || (clock()-*debut)/CLOCKS_PER_SEC>=timeMaxTour || (key[clavFinTour] && *delay == 0)) {
            *debut=clock();
            *menusort = 0;
            effetPlateau(plateau,tabjoueur, &tabjoueur[*tour-1],1);

            int i=0;
            do{
                i++;
                effetTour(backscreen, buffer, plateau, tabjoueur, *tour-1,Image);
                *tour += 1;
                if (*tour > nbJoueur) {
                    effetPlateau(plateau,tabjoueur, &tabjoueur[nbJoueurMax],1);
                    effetTour(backscreen, buffer, plateau, tabjoueur, nbJoueurMax,Image);
                    *tour = 1;
                }
            }while(tabjoueur[*tour-1].PV<=0 && i<nbJoueur);

            if(plateau[tabjoueur[*tour-1].posX][tabjoueur[*tour-1].posY].type != -1 || tabjoueur[*tour-1].classe.type==3){ //Toile d'araignée
                tabjoueur[*tour-1].PM = tabjoueur[*tour-1].PMmax;
            }
            else {
                tabjoueur[*tour-1].PM = 1;
            }
            tabjoueur[*tour-1].PA = tabjoueur[*tour-1].PAmax;
            if(tabjoueur[*tour-1].lenteur>0) {
                tabjoueur[*tour-1].lenteur-=1;
                tabjoueur[*tour-1].PM-=1;
            }
            *delay = 15;
        }
    }
    if(key[showClav]) {
        circlefill(backscreen, 770-8, 150+8, 10, colwhite);
        circlefill(backscreen, 770-8, 150+8, 9, colblack);
        textprintf_ex(backscreen, font, 770-11, 150+5, colwhite, -1, "F");
    }
}
///==========================================================================================================///

int Jeu (BITMAP *backscreen,BITMAP *buffer,Joueurs tabjoueur[],Cases plateau[taillePlateauX][taillePlateauY],int nbJoueur, leaf l[], int tour, pics Image, SAMPLE* music, int* volume) {

    ///Sous-programme ou l'on reste dedans tant que la partie n'est pas fini (ou sauvegardé)
    ///C'est elle qui appelle les autres fonctions du jeu (AffichePlateau, MenuJeu ,AfficheTerrain notament)
    ///Et autres tâches secondaires (Gere le volume de la musique, supprime les bots morts sur le terrain, Fin du jeu...)

    //tour = 1 : c'est le tour du joueur[0] = joueur 1
    int xclic, yclic;
    int val = 0; ///Si val = 1 --> retour menu / val=2 --> sauvegarde    /  val=3 --> rejouer  /  val=4 --> new game
    int delay = 0;
    int menusort = 0;   //Booléen, si=0 on affiche les infos du joueurs, si=1 le menu des sort
    int info=0;
    double debut=clock();
    int sauf=0; //pour afficher si on a sauvegarder

    while (!key[KEY_ESC] && val == 0) {
        clear_bitmap(backscreen);
        clear_bitmap(buffer);


        ///Affichage du plateau (vide) + éclaire la case ou la souris se situe
        affichePlateau(backscreen,buffer,plateau,&xclic,&yclic,Image);

        ///BOUTONS ET MENU
        MenuJeu(backscreen,buffer,tabjoueur,plateau,&tour,&val,xclic,yclic,&delay,nbJoueur,&menusort, &debut, Image);

        ///Affichage du terrain sur le plateau (obstacle, joueur, etc...)
        int ciblenul[taillePlateauX][taillePlateauY]={{0}};
        afficheTerrain(backscreen,buffer,tabjoueur,plateau,0,Image, ciblenul);

        for(int i=0; i<nbJoueur; i++) {  ///Debugger
            for (int x=taillePlateauX-1 ; x>0-1 ; x--) {
                for (int y=0 ; y<taillePlateauY ; y++) {
                    if (tabjoueur[i].PV > 0 && x == tabjoueur[i].posX && y == tabjoueur[i].posY) { //TEST 1: si le joueur est en vie, on l'affiche sur le terrain
                        plateau[tabjoueur[i].posX][tabjoueur[i].posY].ptjoueur = &tabjoueur[i];
                    }
                    if (plateau[x][y].ptjoueur == &tabjoueur[i] && (x != tabjoueur[i].posX || y != tabjoueur[i].posY)) {
                        plateau[x][y].ptjoueur = 0;                                             //TEST 2 : si il y a un doublons, on le supprime
                    }
                }
            }
        }

        ///volume
        const int xs=691-(info<40? 0: (info-40)*3), ys=10+info*1.5;///coordonner bouton volume en fonction du bouton \/
        rectfill(buffer,xs,ys,xs+48,ys+50-5,bcolbutton15);
        rectfill(backscreen,xs,ys,xs+48,ys+50-5,colblack);

        rectfill(backscreen,xs,ys,xs+1+*volume,ys+50-5,makecol(40+(*volume*2),90+(*volume*4),0));
        if (*volume > 0) {
            masked_blit(Image.pmenu[99],backscreen,0,0,xs,ys,Image.pmenu[99]->w, Image.pmenu[99]->h); ///song
        }
        else {
            masked_blit(Image.pmenu[100],backscreen,0,0,xs,ys,Image.pmenu[100]->w, Image.pmenu[100]->h);
        }

        ///VOLUME : ENTRE 0 ET 40

        if(info>0 && info<60) info+=5;
        rectfill(buffer,690,10+info*1,790,60+info*1,bcolbutton12);
        blit(Image.pmenu[64], backscreen,0,0, 690, 10+info*1, Image.pmenu[64]->w, Image.pmenu[64]->h);
        rectfill(buffer,690,10+info*2,790,60+info*2,bcolbutton13);
        blit(Image.pmenu[68], backscreen,0,0, 690, 10+info*2, Image.pmenu[68]->w, Image.pmenu[68]->h);
        rectfill(buffer,690,10+info*3,790,60+info*3,bcolbutton14);
        blit(Image.pmenu[66], backscreen,0,0, 690, 10+info*3, Image.pmenu[66]->w, Image.pmenu[66]->h);
        rectfill(buffer,690,10,790,60,bcolbutton11);
        if(info<=0) blit(Image.pmenu[59], backscreen,0,0, 690, 10, Image.pmenu[59]->w, Image.pmenu[59]->h);
        if(info> 0) blit(Image.pmenu[61], backscreen,0,0, 690, 10, Image.pmenu[61]->w, Image.pmenu[61]->h);

        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton11){
            if(info<=0) blit(Image.pmenu[60], backscreen,0,0, 690, 10, Image.pmenu[60]->w, Image.pmenu[60]->h);
            if(info> 0) blit(Image.pmenu[62], backscreen,0,0, 690, 10, Image.pmenu[62]->w, Image.pmenu[62]->h);
            if (mouse_b == 1 && delay == 0){
                if(info<=0) info=1;
                else info=0;
                delay=15;
            }
        }
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton12){
            blit(Image.pmenu[65], backscreen,0,0, 690, 10+info*1, Image.pmenu[65]->w, Image.pmenu[65]->h);
            if (mouse_b == 1 && delay == 0){
                val = rules(backscreen, buffer, Image, l);
            }
        }
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton13){
            if(sauf==1) blit(Image.pmenu[84], backscreen,0,0, 690, 10+info*2, Image.pmenu[84]->w, Image.pmenu[84]->h);
            else blit(Image.pmenu[69], backscreen,0,0, 690, 10+info*2, Image.pmenu[69]->w, Image.pmenu[69]->h);
            if (mouse_b == 1 && delay == 0){
                sauvegarde(plateau, tabjoueur, nbJoueur, tour);
                sauf=1;
            }
        }else sauf=0;
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton14){
            blit(Image.pmenu[67], backscreen,0,0, 690, 10+info*3, Image.pmenu[67]->w, Image.pmenu[67]->h);
            if (mouse_b == 1 && delay == 0){
                val=1;
            }
        }
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton15 && mouse_b == 1) { ///volume
            *volume = mouse_x - xs;
            if (*volume < 8) {
                *volume = 0;
            }
            if (*volume > 40) {
                *volume = 40;
            }
            adjust_sample(music, (float)*volume*255.0/40.0, 0, 1000, 0);
        }


        //Affichage souris
        masked_blit(Image.pmenu[92],backscreen,0,0,mouse_x,mouse_y,Image.pmenu[92]->w, Image.pmenu[92]->h);

        masked_blit(Image.pmenu[55],backscreen,0,0,0,0,Image.pmenu[55]->w, Image.pmenu[55]->h); //border
        blit(backscreen,screen,0,0,0,0,SCREEN_W,SCREEN_H);

        supp_bot(plateau, tabjoueur);

        ///test fin de jeu
        int team[4]={0};
        for(int i=0; i<nbJoueur; i++)
        {
            if(tabjoueur[i].PV>0) team[tabjoueur[i].IDteam-1]++;
        }
        int nteam=0;
        for(int i=0; i<4; i++)
        {
            if(team[i]>0) nteam++;
        }
        if(nteam<=1) {
            for (int i=0;i<NbLeafMax;i++) {
                l[0].tabx[i] = -1;
                l[0].taby[i] = -1;
                l[0].speed[i] = 1;
            }
            val = AfficheGagnant(backscreen,buffer,tabjoueur,plateau,nbJoueur,l, tour,team, Image);
            MenuTransition(backscreen);
            val = MenuFin(backscreen,buffer,tabjoueur,plateau,nbJoueur, l, tour,team, Image);       /// MENU FIN
        }
        // vainqueur : tabjoueur[tour-1].IDteam = 1
        rest(1);
    }   //Fin du while

    /// FIN DU JEU
    for(int i=0; i<nbJoueur; i++)
    {
        for(int b=0; b<tabjoueur[i].nbbots; b++)
        {
            free(tabjoueur[i].ptbots[b]);
        }
        tabjoueur[i].nbbots=0;
    }

    for(int b=0; b<tabjoueur[nbJoueurMax].nbbots; b++)
    {
        free(tabjoueur[nbJoueurMax].ptbots[b]);
    }
    tabjoueur[nbJoueurMax].nbbots=0;
    return val;
}

void supp_bot(Cases plateau[taillePlateauX][taillePlateauY], Joueurs tabjoueur[]) {
    ///supprime les bots morts (cave spider)
    for(int i=0; i<nbJoueurMax; i++)
    {
        for(int b=tabjoueur[i].nbbots-1; b>=0; b--)
        {
            if(tabjoueur[i].ptbots[b]->PV<=0)
            {
                plateau[tabjoueur[i].ptbots[b]->posX][tabjoueur[i].ptbots[b]->posY].ptjoueur=NULL;
                free(tabjoueur[i].ptbots[b]);

                ///decalage des bots suivants
                for(int k=b; k<nbBotsMax-1; k++)
                {
                    tabjoueur[i].ptbots[k]=tabjoueur[i].ptbots[k+1];
                }
                tabjoueur[i].ptbots[nbBotsMax-1]=NULL;
                tabjoueur[i].nbbots-=1;
            }
        }
    }
    ///bot naturel (silverfish)
    for(int b=tabjoueur[nbJoueurMax].nbbots-1; b>=0; b--)
    {
        if(tabjoueur[nbJoueurMax].ptbots[b]->PV<=0)
        {
            plateau[tabjoueur[nbJoueurMax].ptbots[b]->posX][tabjoueur[nbJoueurMax].ptbots[b]->posY].ptjoueur=NULL;
            free(tabjoueur[nbJoueurMax].ptbots[b]);

            ///decalage des bots suivants
            for(int k=b; k<nbBotsMax-1; k++)
            {
                tabjoueur[nbJoueurMax].ptbots[k]=tabjoueur[nbJoueurMax].ptbots[k+1];
            }
            tabjoueur[nbJoueurMax].ptbots[nbBotsMax-1]=NULL;
            tabjoueur[nbJoueurMax].nbbots-=1;
        }
    }
}
///==========================================================================================================///
