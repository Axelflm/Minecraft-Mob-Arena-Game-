#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include <time.h>
#include <math.h>
#include "head.h"

int AfficheGagnant (BITMAP *backscreen,BITMAP *buffer,Joueurs tabjoueur[],Cases plateau[taillePlateauX][taillePlateauY],int nbJoueur,leaf l[], int tour, int team[], pics Image) {

    ///Lorsque qu'une seule team est vivante, c'est la fin du Jeu : --> AfficheGagnant --> MenuFin
    ///Sous-programme qui affiche le classement de chaque joueur (L'ordre dans lequel ils sont morts)
    ///Affiche la team qui a gagné la parti et tous les vainqueurs (joueur de la team)

    Joueurs tmp;
    Joueurs classement[nbJoueur];
    int max,idmax;
    for(int i=0; i<nbJoueur; i++) classement[i]=tabjoueur[i];
    for(int i=0; i<nbJoueur; i++)
    {
        idmax=i;
        max=abs(classement[i].IDcadavre);
        for(int j=i; j<nbJoueur; j++)
        {
            if(max<abs(classement[j].IDcadavre))
            {
                idmax=j;
                max=abs(classement[j].IDcadavre);
            }
        }
        tmp=classement[idmax];
        for(int k=idmax; k>i; k--)
        {
            classement[k]=classement[k-1];
        }
        classement[i]=tmp;
    }

    int xclic, yclic;
    int teamwinner;
    int col;
    for (int i=0;i<4;i++) {
        if (team[i] > 0) {
            teamwinner = i;
        }
    }
    for (int i=0;i<4;i++) {
        if (team[i] > 0) {
            teamwinner = i;
        }
    }
    while (teamwinner != 10) {
        clear_bitmap(backscreen);
        clear_bitmap(buffer);

        affichePlateau(backscreen,buffer,plateau,&xclic,&yclic,Image);
        int ciblenul[taillePlateauX][taillePlateauY]={{0}};
        afficheTerrain(backscreen,buffer,tabjoueur,plateau,0,Image, ciblenul);

        for(int x=300; x<500; x++){
            for(int y=150; y<325; y++){
                int C=getpixel(backscreen, x,y);
                putpixel(backscreen, x,y, makecol(getr(C)/2, getg(C)/2, getb(C)/2));
            }
        }
        textprintf_centre_ex(backscreen, font, 396, 200, colwhite, -1, "Classement :");
        for(int i=0; i<nbJoueur; i++){
            textprintf_centre_ex(backscreen, font, 396, 200+(i+1)*25, colwhite, -1, "%de - %s", i+1, classement[i].nom);
        }

        masked_blit(Image.pmenu[106],backscreen,0,0,0,0,Image.pmenu[106]->w, Image.pmenu[106]->h);
        textprintf_ex(backscreen,font,36,34,colwhite,-1,"La team ");
        if (teamwinner == 0) {
            textprintf_ex(backscreen,font,102,34,colteam1,-1,"Diamond");
            col = colteam1;
            masked_blit(Image.pmenu[80],backscreen,0,0,266,18,Image.pmenu[80]->w, Image.pmenu[80]->h);
        }
        if (teamwinner == 1) {
            textprintf_ex(backscreen,font,102,34,colteam2,-1,"Emerald");
            col = colteam2;
            masked_blit(Image.pmenu[81],backscreen,0,0,266,18,Image.pmenu[80]->w, Image.pmenu[80]->h);
        }
        if (teamwinner == 2) {
            textprintf_ex(backscreen,font,102,34,colteam3,-1,"Gold");
            col = colteam3;
            masked_blit(Image.pmenu[82],backscreen,0,0,266,18,Image.pmenu[80]->w, Image.pmenu[80]->h);
        }
        if (teamwinner == 3) {
            textprintf_ex(backscreen,font,102,34,colteam4,-1,"Redstone");
            col = colteam4;
            masked_blit(Image.pmenu[83],backscreen,0,0,266,18,Image.pmenu[80]->w, Image.pmenu[80]->h);
        }
        textprintf_ex(backscreen,font,175,34,colwhite,-1,"l'emporte!");

        if (key[KEY_ESC] == -1) {
            return 1;
        }

        textprintf_ex(backscreen,font,36,63,colwhite,-1,"Vainqueur(s) :");
        int j=0;
        for (int i=0;i<nbJoueur;i++) {
            if (tabjoueur[i].IDteam == teamwinner+1) {
                textprintf_ex(backscreen,font,36,73+(j*11),col,-1,"-%s",tabjoueur[i].nom);
                j++;
            }
        }

        masked_blit(Image.pmenu[53],backscreen,0,0,630,480,Image.pmenu[53]->w, Image.pmenu[53]->h); //ok
        rectfill(buffer,    630,480,630+Image.pmenu[54]->w,480+Image.pmenu[54]->h,bcolbutton3);
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton3) { //Si on touche le bouton OK
            masked_blit(Image.pmenu[54],backscreen,0,0,630,480,Image.pmenu[54]->w, Image.pmenu[54]->h);

            if (mouse_b == 1) {
                return 0;
            }
        }

        DecorsFeuilles(backscreen,l,Image,1);

        //Affichage souris
        masked_blit(Image.pmenu[92],backscreen,0,0,mouse_x,mouse_y,Image.pmenu[92]->w, Image.pmenu[92]->h);

        masked_blit(Image.pmenu[55],backscreen,0,0,0,0,Image.pmenu[55]->w, Image.pmenu[55]->h); //border
        blit(backscreen,screen,0,0,0,0,SCREEN_W,SCREEN_H);
        rest(1);
    }
    return 0;
}

///==========================================================================================================///

int MenuFin (BITMAP *backscreen,BITMAP *buffer,Joueurs tabjoueur[],Cases plateau[taillePlateauX][taillePlateauY],int nbJoueur, leaf l[], int tour, int team[], pics Image) {

    ///Menu de fin du jeu qui donne 3 Options :
    /// 1 -Rejouer : relance immédiatement une partie avec les mêmes paramètres (mêmes classes, teams, nb de joueurs, choix du terrain, etc..)
    /// 2 -Nouvelle partie : relance une partie mais en redemandant les paramètres (nb de joueurs, leur classes, etc...)
    /// 3 -Retour : Retourne au menu principale

    int val=0;
    int delay = 0;
    for (int i=0;i<NbLeafMax;i++) {
        l[0].tabx[i] = -1;
        l[0].taby[i] = -1;
        l[0].speed[i] = 1;
    }
    while (!key[KEY_ESC] && val == 0) {
        clear_bitmap(backscreen);
        clear_bitmap(buffer);

        masked_blit(Image.pmenu[43],backscreen,0,0,0,0,Image.pmenu[43]->w, Image.pmenu[43]->h);
        DecorsFeuilles(backscreen,l,Image, 1);
        masked_blit(Image.pmenu[55],backscreen,0,0,0,0,Image.pmenu[55]->w, Image.pmenu[55]->h); //border

        masked_blit(Image.pmenu[46],backscreen,0,0,650,10,Image.pmenu[46]->w, Image.pmenu[46]->h);     //boutonLEAVE
        rectfill(buffer,    650,10,650+Image.pmenu[46]->w,10+Image.pmenu[46]->h,bcolbutton1);
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton1) { //Si on touche le bouton Retour
            masked_blit(Image.pmenu[47],backscreen,0,0,650,10,Image.pmenu[47]->w, Image.pmenu[47]->h);

            if (mouse_b == 1 && delay==0) {
                return 1;
            }
        }

        masked_blit(Image.pmenu[102],backscreen,0,0,250,200,Image.pmenu[102]->w, Image.pmenu[102]->h);     //bouton rejouer
        rectfill(buffer,    250,200,250+Image.pmenu[102]->w,200+Image.pmenu[102]->h,bcolbutton2);
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton2) {
            masked_blit(Image.pmenu[103],backscreen,0,0,250,200,Image.pmenu[103]->w, Image.pmenu[103]->h);

            if (mouse_b == 1 && delay==0) {
                return 3;
            }
        }
        masked_blit(Image.pmenu[104],backscreen,0,0,250,320,Image.pmenu[102]->w, Image.pmenu[102]->h);     //bouton newgame again
        rectfill(buffer,    250,320,250+Image.pmenu[102]->w,320+Image.pmenu[102]->h,bcolbutton3);
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton3) {
            masked_blit(Image.pmenu[105],backscreen,0,0,250,320,Image.pmenu[103]->w, Image.pmenu[103]->h);

            if (mouse_b == 1 && delay==0) {
                return 4;
            }
        }

        //Affichage souris
        masked_blit(Image.pmenu[92],backscreen,0,0,mouse_x,mouse_y,Image.pmenu[92]->w, Image.pmenu[92]->h);
        blit(backscreen,screen,0,0,0,0,SCREEN_W,SCREEN_H);
        rest(1);
    }
    return -1;
}

void MenuTransition (BITMAP *backscreen) {
    ///affiche un écran gris entre 2 menu
    rectfill(backscreen,0,0,800,600,makecol(200,200,200));
    blit(backscreen,screen,0,0,0,0,SCREEN_W,SCREEN_H);
    rest(2);
}

///==========================================================================================================///
void DecorsFeuilles(BITMAP *backscreen, leaf l[],pics Image, int confetti) {
    ///fait defiler des feuilles a l'écrans (pour les menus)
        if (l[0].delay > 0) {
            l[0].delay--;
        }
        else {
            int u=0;
            while (l[0].tabx[u] != -1 && u != 39){
                u++;
            }
            if (confetti == 1) {
                l[0].color[u] = makecol(rand()%(204)+50,rand()%(204)+50,rand()%(204)+50);
            }
            l[0].tabx[u] = rand()%(800);
            l[0].taby[u] = rand()%(600);
            l[0].speed[u] = rand()%(30)+10;
            l[0].speed[u] = l[0].speed[u]/10;
            if (l[0].tabx[u] > l[0].taby[u]) {
                l[0].taby[u]=-10;
            }
            else {
                l[0].tabx[u]=810;
            }

            if (confetti == 0) {
                l[0].delay = 5;
            }
            else {
                l[0].delay = 2;
            }
        }
        for (int i=0;i<NbLeafMax;i++) {
            if (l[0].tabx[i] != -1) {
                if (confetti == 0) {
                    masked_blit(Image.panime[24],backscreen,0,0,l[0].tabx[i],l[0].taby[i],Image.panime[24]->w, Image.panime[24]->h);
                }
                else {
                    rectfill(backscreen,l[0].tabx[i],l[0].taby[i],l[0].tabx[i]+5,l[0].taby[i]+8,l[0].color[i]);
                }
                l[0].tabx[i]-=l[0].speed[i];
                l[0].taby[i]+=1;
                if (l[0].tabx[i] < -10 || l[0].taby[i] > 610) {
                    l[0].tabx[i]=-1;
                    l[0].taby[i]=-1;
                }
            }
        }
}

int rules(BITMAP *backscreen,BITMAP *buffer,pics Image,leaf l[]) {
    ///affiche les règles du jeu

    int page = 1;
    int delay = 10;
    while (!key[KEY_ESC]) {
        clear_bitmap(backscreen);
        clear_bitmap(buffer);
        ///affiche image de fond et bordure d'écrans
        masked_blit(Image.pmenu[84+page],backscreen,0,0,0,0,Image.pmenu[84+page]->w, Image.pmenu[84+page]->h);
        masked_blit(Image.pmenu[55],backscreen,0,0,0,0,Image.pmenu[55]->w, Image.pmenu[55]->h); //border

        if (delay != 0) {
            delay--;
        }
        masked_blit(Image.pmenu[9],backscreen,0,0,10,0,Image.pmenu[9]->w, Image.pmenu[9]->h);     //boutonLEAVE
        rectfill(buffer,    10,0,10+Image.pmenu[9]->w,Image.pmenu[9]->h,bcolbutton4);
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton4) {                                      //Si on touche le bouton Retour
            masked_blit(Image.pmenu[10],backscreen,0,0,10,0,Image.pmenu[10]->w, Image.pmenu[10]->h);
            if (mouse_b == 1) {
                return 0;
            }
        }

        masked_blit(Image.pmenu[88],backscreen,0,0,660,25,Image.pmenu[88]->w, Image.pmenu[88]->h);     //<---
        rectfill(buffer,    660,25,660+Image.pmenu[88]->w,25+Image.pmenu[88]->h,bcolbutton1);
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton1) {
            masked_blit(Image.pmenu[89],backscreen,0,0,660,25,Image.pmenu[88]->w, Image.pmenu[89]->h);
            if (mouse_b == 1 && page != 1 && delay == 0) {
                page--;
                delay = 20;
            }
        }
        masked_blit(Image.pmenu[90],backscreen,0,0,700,25,Image.pmenu[88]->w, Image.pmenu[88]->h);     //--->
        rectfill(buffer,    700,25,700+Image.pmenu[88]->w,25+Image.pmenu[88]->h,bcolbutton2);
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton2) {
            masked_blit(Image.pmenu[91],backscreen,0,0,700,25,Image.pmenu[88]->w, Image.pmenu[89]->h);
            if (mouse_b == 1 && page != 3 && delay == 0) {
                page++;
                delay = 20;
            }
        }

        //Affichage souris
        masked_blit(Image.pmenu[92],backscreen,0,0,mouse_x,mouse_y,Image.pmenu[92]->w, Image.pmenu[92]->h);
        blit(backscreen,screen,0,0,0,0,SCREEN_W,SCREEN_H);
        rest(1);
    }
    return -1;
}

int credits(BITMAP* backscreen, BITMAP* buffer, leaf l[], pics Image)
{
    ///affiche les membres du projet
    int choix = 0;

    while (!key[KEY_ESC] && choix == 0) {
        clear_bitmap(backscreen);
        clear_bitmap(buffer);
        ///affiche le fond d'écrans et les bordures
        masked_blit(Image.pmenu[30],backscreen,0,0,-200,0,Image.pmenu[30]->w, Image.pmenu[30]->h);
        DecorsFeuilles(backscreen,l,Image,0);
        masked_blit(Image.pmenu[55],backscreen,0,0,0,0,Image.pmenu[55]->w, Image.pmenu[55]->h); //border

                ///Affichage du menu
        masked_blit(Image.pmenu[41],backscreen,0,0,240,10,Image.pmenu[31]->w, Image.pmenu[31]->h);

        ///effet de transparence
        for(int x=240; x<=560; x++){
            for(int y=150; y<400; y++){
                int C= getpixel(backscreen, x, y);
                putpixel(backscreen, x, y, makecol(getr(C)/2, getg(C)/2, getb(C)/2));
            }
        }
        textprintf_centre_ex(backscreen, font, 396, 196, colwhite, -1, "FLAMENT Axel");
        textprintf_centre_ex(backscreen, font, 396, 246, colwhite, -1, "GUIGNON Pierre");
        textprintf_centre_ex(backscreen, font, 396, 296, colwhite, -1, "LEE kyungmin");
        textprintf_centre_ex(backscreen, font, 396, 346, colwhite, -1, "LIMOUSIN Antoine");


        masked_blit(Image.pmenu[9],backscreen,0,0,10,0,Image.pmenu[9]->w, Image.pmenu[9]->h);     //boutonLEAVE
        rectfill(buffer,    10,0,10+Image.pmenu[9]->w,Image.pmenu[9]->h,bcolbutton4);
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton4) {                                      //Si on touche le bouton Retour
            masked_blit(Image.pmenu[10],backscreen,0,0,10,0,Image.pmenu[10]->w, Image.pmenu[10]->h);
            if (mouse_b == 1) {
                return 0;
            }
        }

        //Affichage souris
        masked_blit(Image.pmenu[92],backscreen,0,0,mouse_x,mouse_y,Image.pmenu[92]->w, Image.pmenu[92]->h);
        blit(backscreen,screen,0,0,0,0,SCREEN_W,SCREEN_H);
        rest(1);
    }
    return -1;
}

///==========================================================================================================///
int MenuChoixClasse (BITMAP *backscreen,BITMAP *buffer,Joueurs tabjoueur[],int i,int nbJoueur,pics Image,leaf l[]) {

    tabjoueur[i].classe.type = 0;
    tabjoueur[i].IDteam = 0;
    int delay = 0;
    int val = -1;   //val=-1: le joueur n'a pas encore fait son choix / 0: retour menu
    int xbutton = 0;  //Si xbutton=0 : montre les 3 1ere classes, si xbutton=1, montre la 2eme, 3eme et 4eme classe, etc..
    //chaque boutons pour choisir sa classe est espacé de 200pixel en coord x
    int ecranX=0;
    BITMAP* affclasse=create_sub_bitmap(backscreen, 105, 120, 600,Image.pmenu[15]->h);

    while (!key[KEY_ESC] && val == -1) {
        clear_bitmap(backscreen);
        clear_bitmap(buffer);
        //rectfill(affclasse, 0, 0, affclasse->w, affclasse->h, makecol(255,0,255));
        clear_bitmap(affclasse);

        masked_blit(Image.pmenu[43],backscreen,0,0,0,0,Image.pmenu[43]->w, Image.pmenu[43]->h); //image de fond
        DecorsFeuilles(backscreen,l,Image,0);
        masked_blit(Image.pmenu[55],backscreen,0,0,0,0,Image.pmenu[55]->w, Image.pmenu[55]->h); //border
                    ///Affichage du menu

        masked_blit(Image.pmenu[52],backscreen,0,0,0,0,Image.pmenu[52]->w, Image.pmenu[52]->h);
        textprintf_ex(backscreen,font,33,33,colwhite,-1,"Choix du joueur:");   //i+1
        textprintf_ex(backscreen,font,46,46,colhealh,-1,"%s",tabjoueur[i].nom);

                                                            //Affichage des boutons :
        masked_blit(Image.pmenu[48],backscreen,0,0,30,200,Image.pmenu[48]->w, Image.pmenu[48]->h);
        rectfill(buffer,    30,200,30+Image.pmenu[48]->w,200+Image.pmenu[48]->h,bcolbutton1);

        masked_blit(Image.pmenu[50],backscreen,0,0,705,200,Image.pmenu[50]->w, Image.pmenu[50]->h);
        rectfill(buffer,    705,200,705+Image.pmenu[50]->w,200+Image.pmenu[50]->h,bcolbutton2);

                                            ///boutonOK
        masked_blit(Image.pmenu[53],backscreen,0,0,520,450,Image.pmenu[53]->w, Image.pmenu[53]->h);
        rectfill(buffer,    520,450,520+Image.pmenu[54]->w,450+Image.pmenu[54]->h,bcolbutton3);

        masked_blit(Image.pmenu[46],backscreen,0,0,650,10,Image.pmenu[46]->w, Image.pmenu[46]->h);     //boutonLEAVE
        rectfill(buffer,    650,10,650+Image.pmenu[46]->w,10+Image.pmenu[46]->h,bcolbutton4);

        if(ecranX<200) {
            masked_blit(Image.pmenu[15],affclasse,0,0,-ecranX,0,Image.pmenu[15]->w, Image.pmenu[15]->h);
                rectfill(buffer,    -(ecranX<0? ecranX:0)+105,120,-(ecranX>-400? ecranX:-400)+105+Image.pmenu[15]->w,120+Image.pmenu[15]->h,bcolbutton5);
            if (tabjoueur[i].classe.type == 1) {
                masked_blit(Image.pmenu[17],affclasse,0,0,-ecranX,0,Image.pmenu[17]->w, Image.pmenu[17]->h);
            }
        }
        if(ecranX<400) {
            masked_blit(Image.pmenu[18],affclasse,0,0,-ecranX+200,0,Image.pmenu[18]->w, Image.pmenu[18]->h);
                rectfill(buffer,    -(ecranX<200? ecranX:200)+305,120,-(ecranX>-200? ecranX:-200)+305+Image.pmenu[15]->w,120+Image.pmenu[15]->h,bcolbutton6);
            if (tabjoueur[i].classe.type == 2) {
                masked_blit(Image.pmenu[20],affclasse,0,0,-ecranX+200,0,Image.pmenu[20]->w, Image.pmenu[20]->h);
            }
        }
        if(ecranX<600) {
            masked_blit(Image.pmenu[21],affclasse,0,0,-ecranX+400,0,Image.pmenu[21]->w, Image.pmenu[21]->h);
                rectfill(buffer,    -(ecranX<400? ecranX:400)+505,120,-(ecranX>0? ecranX:0)+505+Image.pmenu[15]->w,120+Image.pmenu[15]->h,bcolbutton7);
            if (tabjoueur[i].classe.type == 3) {
                masked_blit(Image.pmenu[23],affclasse,0,0,-ecranX+400,0,Image.pmenu[23]->w, Image.pmenu[23]->h);
            }
        }
        if(ecranX>0) {
            masked_blit(Image.pmenu[24],affclasse,0,0,-ecranX+600,0,Image.pmenu[24]->w, Image.pmenu[24]->h);
                rectfill(buffer,    -(ecranX<600? ecranX:600)+705,120,-(ecranX>200? ecranX:200)+705+Image.pmenu[15]->w,120+Image.pmenu[15]->h,bcolbutton8);
            if (tabjoueur[i].classe.type == 4) {
                masked_blit(Image.pmenu[26],affclasse,0,0,-ecranX+600,0,Image.pmenu[26]->w, Image.pmenu[26]->h);
            }
        }
        if(ecranX>200) {
            masked_blit(Image.pmenu[27],affclasse,0,0,-ecranX+800,0,Image.pmenu[27]->w, Image.pmenu[27]->h);
                rectfill(buffer,    -(ecranX<800? ecranX:800)+905,120,-(ecranX>400? ecranX:400)+905+Image.pmenu[15]->w,120+Image.pmenu[15]->h,bcolbutton9);
            if (tabjoueur[i].classe.type == 5) {
                masked_blit(Image.pmenu[29],affclasse,0,0,-ecranX+800,0,Image.pmenu[29]->w, Image.pmenu[29]->h);
            }
        }

            ///boutons Choix team
        textprintf_ex(backscreen,font,130,460,colwhite,-1,"Choisissez votre team");

        masked_blit(Image.pmenu[72],backscreen,0,0,80,496,Image.pmenu[72]->w, Image.pmenu[72]->h);      ///TEAM
        rectfill(buffer,    80,496,80+Image.pmenu[72]->w,496+Image.pmenu[72]->h,bcolbutton10);

        masked_blit(Image.pmenu[74],backscreen,0,0,80+62,496,Image.pmenu[74]->w, Image.pmenu[74]->h);
        rectfill(buffer,    80+62,496,80+62+Image.pmenu[72]->w,496+Image.pmenu[72]->h,bcolbutton11);

        masked_blit(Image.pmenu[76],backscreen,0,0,80+124,496,Image.pmenu[76]->w, Image.pmenu[76]->h);
        rectfill(buffer,    80+124,496,80+124+Image.pmenu[72]->w,496+Image.pmenu[72]->h,bcolbutton12);

        masked_blit(Image.pmenu[78],backscreen,0,0,80+186,496,Image.pmenu[78]->w, Image.pmenu[78]->h);
        rectfill(buffer,    80+186,496,80+186+Image.pmenu[72]->w,496+Image.pmenu[72]->h,bcolbutton13);

        if (tabjoueur[i].IDteam == 1) {
            masked_blit(Image.pmenu[42],backscreen,0,0,80,496,Image.pmenu[42]->w, Image.pmenu[42]->h);
        }
        else if (tabjoueur[i].IDteam == 2) {
            masked_blit(Image.pmenu[42],backscreen,0,0,80+62,496,Image.pmenu[42]->w, Image.pmenu[42]->h);
        }
        else if (tabjoueur[i].IDteam == 3) {
            masked_blit(Image.pmenu[42],backscreen,0,0,80+124,496,Image.pmenu[42]->w, Image.pmenu[42]->h);
        }
        else if (tabjoueur[i].IDteam == 4) {
            masked_blit(Image.pmenu[42],backscreen,0,0,80+186,496,Image.pmenu[42]->w, Image.pmenu[42]->h);
        }
        ///Programme du Menu (fonctionnement des boutons)
        if (delay > 0) {
            delay--;
            if(ecranX<xbutton*200) ecranX+=20;
            if(ecranX>xbutton*200) ecranX-=20;
        }
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton1) { //Si on touche le bouton LEFT (-)
            masked_blit(Image.pmenu[49],backscreen,0,0,30,200,Image.pmenu[49]->w, Image.pmenu[49]->h);

            if (mouse_b == 1 && xbutton != 0 && delay == 0) {
                xbutton -= 1;
                delay = 10;
            }
        }
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton2) { //Si on touche le bouton RIGHT (+)
            masked_blit(Image.pmenu[51],backscreen,0,0,705,200,Image.pmenu[51]->w, Image.pmenu[51]->h);

            if (mouse_b == 1 && xbutton != 2 && delay == 0) {
                xbutton += 1;
                delay = 10;
            }
        }
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton3) { //Si on touche le bouton OK
            masked_blit(Image.pmenu[54],backscreen,0,0,520,450,Image.pmenu[54]->w, Image.pmenu[54]->h);

            if (mouse_b == 1 && tabjoueur[i].classe.type != 0 && tabjoueur[i].IDteam != 0) {

                if (nbJoueur == i+1) {  //Verifie si les joueurs sont pas tous dans 1 seul team
                    val = -1;
                    int tmp = tabjoueur[0].IDteam;
                    for (int j=1 ;j<nbJoueur; j++) {
                        if (tmp != tabjoueur[j].IDteam) {
                            val = 1;
                        }
                        else {
                            if (j == nbJoueur-1) {
                                textprintf_ex(backscreen,font,430,570,colwhite,-1,"Tous les joueurs sont dans la meme team !");
                            }
                        }
                    }
                }
                else {
                    val = 1;
                }
            }
        }
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton4) { //Si on touche le bouton Retour
            masked_blit(Image.pmenu[47],backscreen,0,0,650,10,Image.pmenu[47]->w, Image.pmenu[47]->h);

            if (mouse_b == 1) {
                val = 0;
            }
        }
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton5) { //Si on touche le bouton Zombie
            masked_blit(Image.pmenu[45],backscreen,0,0,200,0,Image.pmenu[45]->w, Image.pmenu[45]->h);
            textprintf_ex(backscreen,font,267,32,makecol(255,61,0),-1,"850PV");
            textprintf_ex(backscreen,font,270,61,makecol(159,215,94),-1,"4PM");
            textprintf_ex(backscreen,font,340,31,colzombie,-1,"ZOMBIE : Beaucoup de PV,");
            textprintf_ex(backscreen,font,340,42,colzombie,-1,"degat de melee puissant,");
            textprintf_ex(backscreen,font,340,53,colzombie,-1,"Se soigne avec les effets");
            textprintf_ex(backscreen,font,340,64,colzombie,-1,"negatif, mais est lent");

            if (mouse_b == 1) {
                tabjoueur[i].classe.type = 1;
            }
            if (tabjoueur[i].classe.type != 1) {
                masked_blit(Image.pmenu[16],backscreen,0,0,(xbutton*200)+105,120,Image.pmenu[16]->w, Image.pmenu[16]->h);
            }
        }
        else if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton6) { //Si on touche le bouton Squelette
            masked_blit(Image.pmenu[45],backscreen,0,0,200,0,Image.pmenu[45]->w, Image.pmenu[45]->h);
            textprintf_ex(backscreen,font,267,32,makecol(255,61,0),-1,"650PV");
            textprintf_ex(backscreen,font,270,61,makecol(159,215,94),-1,"6PM");
            textprintf_ex(backscreen,font,340,31,colskeleton,-1,"SQUELETTE : Parfait pour,");
            textprintf_ex(backscreen,font,340,42,colskeleton,-1,"des parties rapides, tres");
            textprintf_ex(backscreen,font,340,53,colskeleton,-1,"aggressif, et maitrise");
            textprintf_ex(backscreen,font,340,64,colskeleton,-1,"plein d'effets differents");

            if (mouse_b == 1) {
                tabjoueur[i].classe.type = 2;
            }
            if (tabjoueur[i].classe.type != 2) {
                masked_blit(Image.pmenu[19],backscreen,0,0,-(xbutton*200)+305,120,Image.pmenu[19]->w, Image.pmenu[19]->h);
            }
        }
        else if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton7) { //Si on touche le bouton Spider
            masked_blit(Image.pmenu[45],backscreen,0,0,200,0,Image.pmenu[45]->w, Image.pmenu[45]->h);
            textprintf_ex(backscreen,font,267,32,makecol(255,61,0),-1,"600PV");
            textprintf_ex(backscreen,font,270,61,makecol(159,215,94),-1,"8PM");
            textprintf_ex(backscreen,font,340,31,colspider,-1,"ARAIGNEE: Tres rapide, pose");
            textprintf_ex(backscreen,font,340,42,colspider,-1,"des toiles sur le terrain,");
            textprintf_ex(backscreen,font,340,53,colspider,-1,"peu invoquer des allies");
            textprintf_ex(backscreen,font,340,64,colspider,-1,"et empoisonner en melee");

            if (mouse_b == 1) {
                tabjoueur[i].classe.type = 3;
            }
            if (tabjoueur[i].classe.type != 3) {
                masked_blit(Image.pmenu[22],backscreen,0,0,-(xbutton*200)+505,120,Image.pmenu[22]->w, Image.pmenu[22]->h);
            }
        }
        else if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton8) { //Si on touche le bouton Blaze
            masked_blit(Image.pmenu[45],backscreen,0,0,200,0,Image.pmenu[45]->w, Image.pmenu[45]->h);
            textprintf_ex(backscreen,font,267,32,makecol(255,61,0),-1,"700PV");
            textprintf_ex(backscreen,font,270,61,makecol(159,215,94),-1,"4PM");
            textprintf_ex(backscreen,font,340,31,colblaze,-1,"BLAZE : Tres longue portee,");
            textprintf_ex(backscreen,font,340,42,colblaze,-1,"Excellent pour detruire les");
            textprintf_ex(backscreen,font,340,53,colblaze,-1,"obtacles sur le terrain et");
            textprintf_ex(backscreen,font,340,64,colblaze,-1,"peut aussi buff ses allies");

            if (mouse_b == 1) {
                tabjoueur[i].classe.type = 4;
            }
            if (tabjoueur[i].classe.type != 4) {
                masked_blit(Image.pmenu[25],backscreen,0,0,-(xbutton*200)+705,120,Image.pmenu[25]->w, Image.pmenu[25]->h);
            }
        }
        else if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton9) { //Si on touche le bouton Pillager
            masked_blit(Image.pmenu[45],backscreen,0,0,200,0,Image.pmenu[45]->w, Image.pmenu[45]->h);
            textprintf_ex(backscreen,font,267,32,makecol(255,61,0),-1,"800PV");
            textprintf_ex(backscreen,font,270,61,makecol(159,215,94),-1,"5PM");
            textprintf_ex(backscreen,font,340,31,colpillager,-1,"PILLAGER: Grande portee,");
            textprintf_ex(backscreen,font,340,42,colpillager,-1,"peut tirer a travers les");
            textprintf_ex(backscreen,font,340,53,colpillager,-1,"obtacles et en construire,");
            textprintf_ex(backscreen,font,340,64,colpillager,-1,"il est aussi tres resistant");

            if (mouse_b == 1) {
                tabjoueur[i].classe.type = 5;
            }
            if (tabjoueur[i].classe.type != 5) {
                masked_blit(Image.pmenu[28],backscreen,0,0,-(xbutton*200)+905,120,Image.pmenu[28]->w, Image.pmenu[28]->h);
            }
        }
        else {
            masked_blit(Image.pmenu[44],backscreen,0,0,200,0,Image.pmenu[44]->w, Image.pmenu[44]->h);
            textprintf_ex(backscreen,font,246,34,colwhite,-1,"Choisissez votre CLASSE et votre EQUIPE");
        }
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton10 && tabjoueur[i].IDteam != 1) { //Si on touche le bouton TeamGreen
            masked_blit(Image.pmenu[73],backscreen,0,0,80,496,Image.pmenu[72]->w, Image.pmenu[72]->h);

            if (mouse_b == 1) {
                tabjoueur[i].IDteam = 1;
            }
        }
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton11 && tabjoueur[i].IDteam != 2) { //Si on touche le bouton TeamCyan
            masked_blit(Image.pmenu[75],backscreen,0,0,80+62,496,Image.pmenu[74]->w, Image.pmenu[74]->h);

            if (mouse_b == 1) {
                tabjoueur[i].IDteam = 2;
            }
        }
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton12 && tabjoueur[i].IDteam != 3) { //Si on touche le bouton TeamBleu
            masked_blit(Image.pmenu[77],backscreen,0,0,80+124,496,Image.pmenu[76]->w, Image.pmenu[76]->h);

            if (mouse_b == 1) {
                tabjoueur[i].IDteam = 3;
            }
        }
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton13 && tabjoueur[i].IDteam != 4) { //Si on touche le bouton TeamViolet
            masked_blit(Image.pmenu[79],backscreen,0,0,80+186,496,Image.pmenu[78]->w, Image.pmenu[78]->h);

            if (mouse_b == 1) {
                tabjoueur[i].IDteam = 4;
            }
        }



        //Affichage souris
        masked_blit(Image.pmenu[92],backscreen,0,0,mouse_x,mouse_y,Image.pmenu[92]->w, Image.pmenu[92]->h);
        blit(backscreen,screen,0,0,0,0,SCREEN_W,SCREEN_H);
        rest(1);
    }
    destroy_bitmap(affclasse);
    return val;
}

///==========================================================================================================///

int MenuChoixTerrain (BITMAP *backscreen,BITMAP *buffer,Joueurs tabjoueur[], pics Image,leaf l[]) {

    int choix = 0;
    int delay = 20; //Le delay empeche l'utilisateur de cliquÃ© sur le bouton 1000 fois par seconde
    while (!key[KEY_ESC] && choix == 0) {
        clear_bitmap(backscreen);
        clear_bitmap(buffer);

                    ///Affichage du menu pour choisir le nb de joueurs (Temporaire)
        masked_blit(Image.pmenu[43],backscreen,0,0,0,0,Image.pmenu[43]->w, Image.pmenu[43]->h); //image de fond
        DecorsFeuilles(backscreen,l,Image,0);
        masked_blit(Image.pmenu[55],backscreen,0,0,0,0,Image.pmenu[55]->w, Image.pmenu[55]->h); //border

        masked_blit(Image.pmenu[44],backscreen,0,0,200,0,Image.pmenu[44]->w, Image.pmenu[44]->h);
        textprintf_ex(backscreen,font,245,34,colwhite,-1,"Choisissez le Terrain que vous preferez");
                                                            //Affichage des boutons :

        masked_blit(Image.pmenu[93],backscreen,0,0,50,150,Image.pmenu[93]->w, Image.pmenu[93]->h);  //aleatoire facile
        rectfill(buffer,    50,150,50+Image.pmenu[93]->w,150+Image.pmenu[93]->h,bcolbutton1);

        masked_blit(Image.pmenu[95],backscreen,0,0,290,150,Image.pmenu[93]->w, Image.pmenu[93]->h);  //aleatoire difficile
        rectfill(buffer,    290,150,290+Image.pmenu[93]->w,150+Image.pmenu[93]->h,bcolbutton2);

        masked_blit(Image.pmenu[97],backscreen,0,0,530,150,Image.pmenu[93]->w, Image.pmenu[93]->h);  //charger partie
        rectfill(buffer,    530,150,530+Image.pmenu[93]->w,150+Image.pmenu[93]->h,bcolbutton3);

        masked_blit(Image.pmenu[46],backscreen,0,0,650,10,Image.pmenu[46]->w, Image.pmenu[46]->h);     //boutonLEAVE
        rectfill(buffer,    650,10,650+Image.pmenu[46]->w,10+Image.pmenu[46]->h,bcolbutton4);


        ///Programme du Menu (fonctionnement des boutons)
        if (delay > 0) {
            delay--;
        }
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton1) { //Si on touche le bouton facile
            masked_blit(Image.pmenu[94],backscreen,0,0,50,150,Image.pmenu[93]->w, Image.pmenu[93]->h);

            if (mouse_b == 1 && delay == 0) {
                return 1;
            }
        }
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton2) { //Si on touche le bouton difficile
            masked_blit(Image.pmenu[96],backscreen,0,0,290,150,Image.pmenu[93]->w, Image.pmenu[93]->h);

            if (mouse_b == 1 && delay == 0) {
                return 2;
            }
        }
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton3) { //Si on touche le bouton charge
            masked_blit(Image.pmenu[98],backscreen,0,0,530,150,Image.pmenu[93]->w, Image.pmenu[93]->h);

            if (mouse_b == 1 && delay == 0) {
                return 3;
            }
        }
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton4) { //Si on touche le bouton Retour
            masked_blit(Image.pmenu[47],backscreen,0,0,650,10,Image.pmenu[47]->w, Image.pmenu[47]->h);

            if (mouse_b == 1) {
                return 0;
            }
        }

        //Affichage souris
        masked_blit(Image.pmenu[92],backscreen,0,0,mouse_x,mouse_y,Image.pmenu[92]->w, Image.pmenu[92]->h);
        blit(backscreen,screen,0,0,0,0,SCREEN_W,SCREEN_H);
        rest(1);
    }
    return -1;  //Si l'utilisateur appuie sur un bouton "retour au menu" ou echap
}

int MenuNBJoueur (BITMAP *backscreen,BITMAP *buffer,Joueurs tabjoueur[],pics Image,leaf l[]) {

    int nbJoueur = 2;
    int choix = 0;
    int delay = 0; //Le delay empeche l'utilisateur de cliquÃ© sur le bouton 1000 fois par seconde
    int ecranX = 0, ecranY=0;
    BITMAP* bitnbJoueur= create_sub_bitmap(backscreen, 120, 220, Image.pmenu[56]->w, Image.pmenu[56]->h);

    ///nom
    int id_c[nbJoueurMax]={0};
    char c;
    int j=-1;
    int maj=0;
    int verrmaj=0;
    int x0=100, y0=0;
    const char alph[72]={'1','2','3','4','5','6','7','8','9','0',
                        'a','z','e','r','t','y','u','i','o','p',
                        'q','s','d','f','g','h','j','k','l','m',
                        'w','x','c','v','b','n',
                        '1','2','3','4','5','6','7','8','9','0',
                        'A','Z','E','R','T','Y','U','I','O','P',
                        'Q','S','D','F','G','H','J','K','L','M',
                        'W','X','C','V','B','N'};
    strcpy(tabjoueur[0].nom, "player1");
    strcpy(tabjoueur[1].nom, "player2");
    strcpy(tabjoueur[2].nom, "player3");
    strcpy(tabjoueur[3].nom, "player4");
    const char defnom[10]="player";

    while (!key[KEY_ESC] && choix == 0) {
        clear_bitmap(backscreen);
        clear_bitmap(buffer);
        clear_bitmap(bitnbJoueur);
        masked_blit(Image.pmenu[43],backscreen,0,0,0,0,Image.pmenu[43]->w, Image.pmenu[43]->h);
        DecorsFeuilles(backscreen,l,Image,0);
        masked_blit(Image.pmenu[55],backscreen,0,0,0,0,Image.pmenu[55]->w, Image.pmenu[55]->h); //border
                    ///Affichage du menu pour choisir le nb de joueurs (Temporaire)

        masked_blit(Image.pmenu[44],backscreen,0,0,200,0,Image.pmenu[44]->w, Image.pmenu[44]->h);
        textprintf_ex(backscreen,font,250,34,colwhite,-1,"Choisissez le nombre de Joueurs (2-4)");
                                                            //Affichage des boutons :
        masked_blit(Image.pmenu[48],backscreen,0,0,30,250,Image.pmenu[48]->w, Image.pmenu[48]->h); //left
        rectfill(buffer,    30,250,30+Image.pmenu[48]->w,250+Image.pmenu[48]->h,bcolbutton1);

        masked_blit(Image.pmenu[50],backscreen,0,0,305,250,Image.pmenu[50]->w, Image.pmenu[50]->h); //right
        rectfill(buffer,    305,250,305+Image.pmenu[50]->w,250+Image.pmenu[50]->h,bcolbutton2);

        masked_blit(Image.pmenu[53],backscreen,0,0,135,420,Image.pmenu[53]->w, Image.pmenu[53]->h); //ok
        rectfill(buffer,    135,420,135+Image.pmenu[54]->w,420+Image.pmenu[54]->h,bcolbutton3);

        masked_blit(Image.pmenu[46],backscreen,0,0,650,10,Image.pmenu[46]->w, Image.pmenu[46]->h);     //boutonLEAVE
        rectfill(buffer,    650,10,650+Image.pmenu[46]->w,10+Image.pmenu[46]->h,bcolbutton4);

        ///affichage du nombre de joueur
        masked_blit(Image.pmenu[56],bitnbJoueur,0,0,-ecranX+(0)*160,0,Image.pmenu[56]->w, Image.pmenu[56]->h);
        masked_blit(Image.pmenu[57],bitnbJoueur,0,0,-ecranX+(1)*160,0,Image.pmenu[57]->w, Image.pmenu[57]->h);
        masked_blit(Image.pmenu[58],bitnbJoueur,0,0,-ecranX+(2)*160,0,Image.pmenu[58]->w, Image.pmenu[58]->h);

        ///affichages des noms des joueurs
        rectfill(backscreen,    400,220-ecranY/3,750,220+40*nbJoueur-ecranY/3, colblack);
        for(int i=0; i<nbJoueur; i++)
        {
            textprintf_ex(backscreen,font,416,236+40*i-ecranY/3,colwhite,-1,"%d -  %s",i+1, tabjoueur[i].nom);
            rectfill(buffer, 400,220+40*i-ecranY/3,750,220+40*(i+1)-ecranY/3, makecol(1,1,i));
            if(j==i)rect(backscreen, 400,220+40*i-ecranY/3,750,220+40*(i+1)-ecranY/3, colwhite);
        }

        ///Programme du Menu (fonctionnement des boutons)
        if (delay > 0) {
            delay--;
            if(ecranX>(nbJoueur-2)*160) ecranX-=16;
            if(ecranX<(nbJoueur-2)*160) ecranX+=16;

            if(ecranY>0 && j<0) ecranY-=20;
            if(ecranY<250 && j>-1) ecranY+=20;
        }
        ///sort le clavier si on cclique sur un joueur
        if(getr(getpixel(buffer,mouse_x,mouse_y))==1){
            rect(backscreen, 400,220+40*getb(getpixel(buffer,mouse_x,mouse_y))-ecranY/3,750,220+40*(getb(getpixel(buffer,mouse_x,mouse_y))+1)-ecranY/3, colwhite);
            if(mouse_b==1 && delay==0){
                j=getb(getpixel(buffer,mouse_x,mouse_y));
                delay=15;
            }
        }
        //coordonnée clavier
        y0=SCREEN_H-ecranY;
        if(j>-1)
        {
            ///fond du clavier transparent
            for(int x=0; x<SCREEN_W; x++){
                for(int y=y0-20; y<SCREEN_H; y++){
                    int col=getpixel(backscreen, x, y);
                    putpixel(backscreen, x,y,makecol(getr(col)/2,getg(col)/2,getb(col)/2));
                }
            }
            rectfill(buffer, 0, y0-20, 800, 600, colblack);

            ///gestion du clavier physique
            if(keypressed()){
                c=readkey();
                if(key[KEY_ESC]) break;

                if(key[KEY_ENTER]){ //entrer
                    if(delay==0) {
                        if(id_c[j]==0){
                            for(int i=0; i<6; i++){
                                tabjoueur[j].nom[i]=defnom[i];
                            }
                            tabjoueur[j].nom[6]=(char)(j+1+48);
                            tabjoueur[j].nom[7]='\0';
                        }
                        j=-1;
                        delay=15;
                        continue;
                    }
                }
                else if(key[KEY_BACKSPACE]){ //retour en arrière
                    if(id_c[j]!=0){
                        id_c[j]--;
                        tabjoueur[j].nom[id_c[j]]='\0';
                    }
                    else tabjoueur[j].nom[0]='\0';
                }
                else if(id_c[j]<7 && c!=8 && c!=' '){ // autre
                    tabjoueur[j].nom[id_c[j]]=c;
                    id_c[j]++;
                    tabjoueur[j].nom[id_c[j]]='\0';
                }
            }

            ///affichage du clavier
            for(int i=0; i<4; i++)
            {
                for(int j=0; j<(i==3?6:10); j++)
                {
                    rectfill(backscreen, x0+60*j+20*i ,y0+60*i, x0+60*j+20*i+50 ,y0+60*i+50, makecol(100, 100, 100));
                    rectfill(buffer    , x0+60*j+20*i ,y0+60*i, x0+60*j+20*i+50 ,y0+60*i+50, makecol(100, j, i));
                    textprintf_centre_ex(backscreen, font, x0+26+60*j+20*i ,y0+26+60*i, colwhite, -1, "%c", alph[j+10*i+abs(maj-verrmaj)*36]);
                }
            }
            //bouton MAJ
            rectfill(backscreen, x0-60 ,y0+60*3, x0+50 ,y0+60*3+50, makecol(100, 100, 100));
            rectfill(buffer    , x0-60 ,y0+60*3, x0+50 ,y0+60*3+50, makecol(1, 100,100));
            textprintf_centre_ex(backscreen, font, x0-4 ,y0+26+60*3, colwhite, -1, "MAJ");
            if(maj) rectfill(backscreen, x0-60 ,y0+60*3, x0+50 ,y0+60*3+50, makecol(200, 200, 200));

            //bouton VERR MAJ
            rectfill(backscreen, x0-60 ,y0+60*2, x0+30 ,y0+60*2+50, makecol(100, 100, 100));
            rectfill(buffer    , x0-60 ,y0+60*2, x0+30 ,y0+60*2+50, makecol(2, 100,100));
            textprintf_centre_ex(backscreen, font, x0-40+26 ,y0+26+60*2, colwhite, -1, "VERR MAJ");
            if(verrmaj) rectfill(backscreen, x0-60 ,y0+60*2, x0+30 ,y0+60*2+50, makecol(200, 200, 200));

            //bouton backspace
            rectfill(backscreen, x0+60*10 ,y0, x0+60*10+80 ,y0+50, makecol(100, 100, 100));
            rectfill(buffer    , x0+60*10 ,y0, x0+60*10+80 ,y0+50, makecol(3, 100,100));
            textprintf_ex(backscreen, font, x0+60*10+10 ,y0+22, colwhite, -1, "<--");

            //bouton entrée
            rectfill(backscreen, x0+60*10+20 ,y0+60, x0+60*10+80 ,y0+60+50, makecol(100, 100, 100));
            rectfill(buffer    , x0+60*10+20 ,y0+60, x0+60*10+80 ,y0+60+50, makecol(4, 100,100));
            textprintf_ex(backscreen, font, x0+60*10+31 ,y0+60+22, colwhite, -1, "ENTREE");
            rectfill(backscreen, x0+60*10+40 ,y0+60+50, x0+60*10+80 ,y0+120+50, makecol(100, 100, 100));
            rectfill(buffer    , x0+60*10+40 ,y0+60+50, x0+60*10+80 ,y0+120+50, makecol(4, 100,100));
            textprintf_ex(backscreen, font, x0+60*10+50 ,y0+120+22, colwhite, -1, "<--");


            //fonctionnement du clavier numérique
            int C=getpixel(buffer, mouse_x, mouse_y);
            if(C==makecol(1, 100,100)){///MAJ
                rectfill(backscreen, x0-60 ,y0+60*3, x0+50 ,y0+60*3+50, makecol(150, 150, 150));
                if(mouse_b==1 && delay==0){
                    maj=(maj+1)%2;
                    delay=5;
                }
            }
            if(C==makecol(2, 100,100) && delay==0){///VERR MAJ
                rectfill(backscreen, x0-60 ,y0+60*2, x0+30 ,y0+60*2+50, makecol(150, 150, 150));
                if(mouse_b==1){
                    verrmaj=(verrmaj+1)%2;
                    delay=5;
                }
            }
            if(C==makecol(3, 100,100)){///BACKSPACE
                rectfill(backscreen, x0+60*10 ,y0, x0+60*10+80 ,y0+50, makecol(150, 150, 150));
                if(mouse_b==1 && delay==0 && id_c[j]!=0){
                    id_c[j]--;
                    tabjoueur[j].nom[id_c[j]]='\0';
                    delay=5;
                }
                else if(mouse_b==1 && delay==0 && id_c[j]==0) tabjoueur[j].nom[0]='\0';
            }
            if(C==makecol(4, 100,100) || (C==colblack && mouse_b==1)){///ENTRER
                rectfill(backscreen, x0+60*10+20 ,y0+60, x0+60*10+80 ,y0+60+50, makecol(150, 150, 150));
                rectfill(backscreen, x0+60*10+40 ,y0+60+50, x0+60*10+80 ,y0+120+50, makecol(150, 150,150));
                if(mouse_b==1 && delay==0){
                    if(id_c[j]==0){
                        for(int i=0; i<6; i++){
                            tabjoueur[j].nom[i]=defnom[i];
                        }
                        tabjoueur[j].nom[6]=(char)(j+1+48);
                        tabjoueur[j].nom[7]='\0';
                    }
                    j=-1;
                    delay=5;
                    continue;
                }
            }
            if(getr(C)==100){///AUTRE
                rectfill(backscreen, x0+60*getg(C)+20*getb(C) ,y0+60*getb(C), x0+60*getg(C)+20*getb(C)+50 ,y0+60*getb(C)+50, makecol(150, 150, 150));
                if(mouse_b==1 && delay==0 && id_c[j]<7){
                    tabjoueur[j].nom[id_c[j]]=alph[getg(C)+getb(C)*10+abs(maj-verrmaj)*36];
                    id_c[j]++;
                    tabjoueur[j].nom[id_c[j]]='\0';
                    maj=0;
                    delay=5;
                }
            }
        }


        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton1) { //Si on touche le bouton LEFT (-)
            masked_blit(Image.pmenu[49],backscreen,0,0,30,250,Image.pmenu[49]->w, Image.pmenu[49]->h);

            if (mouse_b == 1 && nbJoueur != 2 && delay == 0) {
                nbJoueur -= 1;
                delay = 15;
            }
        }
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton2) { //Si on touche le bouton RIGHT (+)
            masked_blit(Image.pmenu[51],backscreen,0,0,305,250,Image.pmenu[51]->w, Image.pmenu[51]->h);

            if (mouse_b == 1 && nbJoueur != 4 && delay == 0) {
                nbJoueur += 1;
                delay = 15;
            }
        }
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton3) { //Si on touche le bouton OK
            masked_blit(Image.pmenu[54],backscreen,0,0,135,420,Image.pmenu[54]->w, Image.pmenu[54]->h);

            if (mouse_b == 1 && delay==0) {
                return nbJoueur;
            }
        }
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton4) { //Si on touche le bouton Retour
            masked_blit(Image.pmenu[47],backscreen,0,0,650,10,Image.pmenu[47]->w, Image.pmenu[47]->h);

            if (mouse_b == 1 && delay==0) {
                return 0;
            }
        }


        //Affichage souris
        masked_blit(Image.pmenu[92],backscreen,0,0,mouse_x,mouse_y,Image.pmenu[92]->w, Image.pmenu[92]->h);
        blit(backscreen,screen,0,0,0,0,SCREEN_W,SCREEN_H);
        rest(1);
    }
    return -1;  //Si l'utilisateur appuie sur un bouton "retour au menu" ou echap
}


///==========================================================================================================///
void InitialisationClasses(Joueurs tabjoueur[],int i) {


                for (int j=0 ;j<nbSortMax ;j++) {   /// Init par defaut
                        tabjoueur[i].sort[j].debuff = 0;
                        tabjoueur[i].sort[j].poison = 0;
                        tabjoueur[i].sort[j].PMup = 0;
                        tabjoueur[i].sort[j].force = 0;
                        tabjoueur[i].sort[j].wither = 0;
                        tabjoueur[i].sort[j].taille = 0;
                        tabjoueur[i].sort[j].lenteur = 0;
                        tabjoueur[i].sort[j].soins = 0;
                        tabjoueur[i].sort[j].ObPV = 0;
                        tabjoueur[i].sort[j].Obtour = 0;
                        tabjoueur[i].sort[j].def = 0;
                        tabjoueur[i].sort[j].Obtype = 0;
                        tabjoueur[i].sort[j].probDegatNul = 8;
                        tabjoueur[i].sort[j].formePorte = 0;
                        tabjoueur[i].sort[j].porteMin = 0;
                        tabjoueur[i].sort[j].porteMax = 0;
                        tabjoueur[i].sort[j].obstacle = 1;
                        tabjoueur[i].sort[j].degat = 0;
                        tabjoueur[i].sort[j].Obbloque = 0;
                }

                ///ATTAQUE 0 : CAC
                tabjoueur[i].sort[4].type = 0; //Offensif
                tabjoueur[i].sort[4].degat = 50;
                tabjoueur[i].sort[4].porteMin = 1;
                tabjoueur[i].sort[4].porteMax = 1;
                tabjoueur[i].sort[4].PA = 2;
                tabjoueur[i].sort[4].typesort = 0;


                if (tabjoueur[i].classe.type == 1) {    ///ZOMBIE
                    tabjoueur[i].PMmax = 4;
                    tabjoueur[i].PAmax = 10;
                    tabjoueur[i].PVmax = 850;

                    ///ATTAQUE 1 : CONTAMINATION
                    tabjoueur[i].sort[0].type = 0; //Offensif
                    tabjoueur[i].sort[0].degat = 80;
                    tabjoueur[i].sort[0].soins = 15;
                    tabjoueur[i].sort[0].porteMin = 1;
                    tabjoueur[i].sort[0].porteMax = 4;
                    tabjoueur[i].sort[0].PA = 3;
                    tabjoueur[i].sort[0].typesort = 1;

                    ///ATTAQUE 2 : CRAFT TOOL
                    tabjoueur[i].sort[1].type = 1; //Effet
                    tabjoueur[i].sort[1].force = 3;
                    tabjoueur[i].sort[1].PA = 4;
                    tabjoueur[i].sort[1].porteMin = 0;
                    tabjoueur[i].sort[1].porteMax = 1;
                    tabjoueur[i].sort[1].typesort = 2;

                    ///ATTAQUE 3 : POISON HEALH
                    tabjoueur[i].sort[2].type = 1; //Effet
                    tabjoueur[i].sort[2].debuff = 1;
                    tabjoueur[i].sort[2].soins = 40;
                    tabjoueur[i].sort[2].PA = 2;
                    tabjoueur[i].sort[2].typesort = 3;

                    ///ATTAQUE 4 : DROWNED
                    tabjoueur[i].sort[3].type = 0; //Offensif
                    tabjoueur[i].sort[3].degat = 165;
                    tabjoueur[i].sort[3].taille = 1;
                    tabjoueur[i].sort[3].formePorte = 1;
                    tabjoueur[i].sort[3].porteMin = 1;
                    tabjoueur[i].sort[3].porteMax = 6;
                    tabjoueur[i].sort[3].PA = 8;
                    tabjoueur[i].sort[3].lenteur = 1;
                    tabjoueur[i].sort[3].typesort = 4;
                }
                if (tabjoueur[i].classe.type == 2) {    ///SKELETON
                    tabjoueur[i].PMmax = 6;
                    tabjoueur[i].PAmax = 10;
                    tabjoueur[i].PVmax = 650;

                    ///ATTAQUE 1 : QUICK ARROW
                    tabjoueur[i].sort[0].type = 0; //Offensif
                    tabjoueur[i].sort[0].degat = 85;
                    tabjoueur[i].sort[0].porteMin = 1;
                    tabjoueur[i].sort[0].porteMax = 5;
                    tabjoueur[i].sort[0].PA = 3;
                    tabjoueur[i].sort[0].typesort = 5;

                    ///ATTAQUE 2 : ICE ARROW
                    tabjoueur[i].sort[1].type = 0; //Offensif
                    tabjoueur[i].sort[1].porteMin = 4;
                    tabjoueur[i].sort[1].porteMax = 5;
                    tabjoueur[i].sort[1].degat = 80;
                    tabjoueur[i].sort[1].lenteur = 2;
                    tabjoueur[i].sort[1].PA = 4;
                    tabjoueur[i].sort[1].typesort = 6;

                    ///ATTAQUE 3 : WITHER EFFECT
                    tabjoueur[i].sort[2].type = 0; //Offensif
                    tabjoueur[i].sort[2].degat = 110;
                    tabjoueur[i].sort[2].porteMin = 1;
                    tabjoueur[i].sort[2].porteMax = 2;
                    tabjoueur[i].sort[2].wither = 3;
                    tabjoueur[i].sort[2].PA = 6;
                    tabjoueur[i].sort[2].typesort = 7;

                    ///ATTAQUE 4 : ARMOR
                    tabjoueur[i].sort[3].type = 1; //Effet
                    tabjoueur[i].sort[3].def = 3;
                    tabjoueur[i].sort[3].PA = 2;
                    tabjoueur[i].sort[3].typesort = 8;
                }
                if (tabjoueur[i].classe.type == 3) {    ///SPIDER
                    tabjoueur[i].PMmax = 8;
                    tabjoueur[i].PAmax = 10;
                    tabjoueur[i].PVmax = 600;

                    ///ATTAQUE 1 : SNICKY ATTACK
                    tabjoueur[i].sort[0].type = 0; //Offensif
                    tabjoueur[i].sort[0].degat = 55;
                    tabjoueur[i].sort[0].porteMin = 1;
                    tabjoueur[i].sort[0].porteMax = 3;
                    tabjoueur[i].sort[0].poison = 4;
                    tabjoueur[i].sort[0].PA = 3;
                    tabjoueur[i].sort[0].typesort = 9;

                    ///ATTAQUE 2 : COBWEB
                    tabjoueur[i].sort[1].type = 2; //Effet
                    tabjoueur[i].sort[1].porteMin = 2;
                    tabjoueur[i].sort[1].porteMax = 4;
                    tabjoueur[i].sort[1].PA = 4;
                    tabjoueur[i].sort[1].typesort = 10;
                    tabjoueur[i].sort[1].Obtype = -1;
                    tabjoueur[i].sort[1].ObPV = -1;
                    tabjoueur[i].sort[1].Obtour = 8;
                    tabjoueur[i].sort[1].Obbloque = 1;

                    ///ATTAQUE 3 : CALL FOR HELP
                    tabjoueur[i].sort[2].type = 3; //Effet
                    tabjoueur[i].sort[2].porteMin = 1;
                    tabjoueur[i].sort[2].porteMax = 1;
                    tabjoueur[i].sort[2].PA = 6;
                    tabjoueur[i].sort[2].typesort = 11;

                    ///ATTAQUE 4 : REST
                    tabjoueur[i].sort[3].type = 1; //Effet
                    tabjoueur[i].sort[3].soins = 10;
                    tabjoueur[i].sort[3].PA = 2;
                    tabjoueur[i].sort[2].porteMin = 0;
                    tabjoueur[i].sort[2].porteMax = 1;
                    tabjoueur[i].sort[3].typesort = 12;
                }
                if (tabjoueur[i].classe.type == 4) {    ///BLAZE
                    tabjoueur[i].PMmax = 5;
                    tabjoueur[i].PAmax = 10;
                    tabjoueur[i].PVmax = 700;

                    ///ATTAQUE 1 : FIREBALL
                    tabjoueur[i].sort[0].type = 0; //Offensif   //moins de degat mais plus grande range que les fleches du skeleton
                    tabjoueur[i].sort[0].degat = 75;
                    tabjoueur[i].sort[0].porteMin = 4;
                    tabjoueur[i].sort[0].porteMax = 7;
                    tabjoueur[i].sort[0].PA = 3;
                    tabjoueur[i].sort[0].typesort = 13;

                    ///ATTAQUE 2 : FIREWALL
                    tabjoueur[i].sort[1].type = 2; //Effet
                    tabjoueur[i].sort[1].porteMin = 2;
                    tabjoueur[i].sort[1].porteMax = 5;
                    tabjoueur[i].sort[1].taille = 1;
                    tabjoueur[i].sort[1].PA = 2;
                    tabjoueur[i].sort[1].typesort = 14;
                    tabjoueur[i].sort[1].Obtype = -2;
                    tabjoueur[i].sort[1].ObPV = -1;
                    tabjoueur[i].sort[1].Obtour = 5;

                    ///ATTAQUE 3 : ERUPTION
                    tabjoueur[i].sort[2].type = 0; //Offensif
                    tabjoueur[i].sort[2].degat = 140;
                    tabjoueur[i].sort[2].porteMin = 3;
                    tabjoueur[i].sort[2].porteMax = 4;
                    tabjoueur[i].sort[2].taille = 2;
                    tabjoueur[i].sort[2].PA = 7;
                    tabjoueur[i].sort[2].typesort = 15;

                    ///ATTAQUE 4 : ALCHEMY
                    tabjoueur[i].sort[3].type = 1; //Effet
                    tabjoueur[i].sort[3].force = 3;
                    tabjoueur[i].sort[3].def = 3;
                    tabjoueur[i].sort[3].PA = 4;
                    tabjoueur[i].sort[3].porteMin = 1;  //Ne peut pas l'appliquer sur lui, seulement un allié
                    tabjoueur[i].sort[3].porteMax = 3;
                    tabjoueur[i].sort[3].typesort = 16;
                }
                if (tabjoueur[i].classe.type == 5) {    ///PILLAGER
                    tabjoueur[i].PMmax = 5;
                    tabjoueur[i].PAmax = 10;
                    tabjoueur[i].PVmax = 800;

                    ///ATTAQUE 1 : QUICK SHOT
                    tabjoueur[i].sort[0].type = 0; //Offensif
                    tabjoueur[i].sort[0].degat = 110;
                    tabjoueur[i].sort[0].porteMin = 3;
                    tabjoueur[i].sort[0].porteMax = 6;
                    tabjoueur[i].sort[0].PA = 5;
                    tabjoueur[i].sort[0].typesort = 17;

                    ///ATTAQUE 2 : PILLAGER OUTPOST
                    tabjoueur[i].sort[1].type = 2; //Offensif
                    tabjoueur[i].sort[1].porteMin = 2;
                    tabjoueur[i].sort[1].porteMax = 5;
                    tabjoueur[i].sort[1].Obtype = 3;
                    tabjoueur[i].sort[1].ObPV = 200;
                    tabjoueur[i].sort[1].Obtour = -1;
                    tabjoueur[i].sort[1].PA = 4;
                    tabjoueur[i].sort[1].typesort = 18;
                    tabjoueur[i].sort[1].Obbloque = 1;

                    ///ATTAQUE 3 : PIERCING
                    tabjoueur[i].sort[2].type = 0; //Offensif
                    tabjoueur[i].sort[2].formePorte = 1;
                    tabjoueur[i].sort[2].obstacle = 0;
                    tabjoueur[i].sort[2].degat = 145;
                    tabjoueur[i].sort[2].porteMin = 3;
                    tabjoueur[i].sort[2].porteMax = 7;
                    tabjoueur[i].sort[2].PA = 6;
                    tabjoueur[i].sort[2].typesort = 19;

                    ///ATTAQUE 4 : TOTEM
                    tabjoueur[i].sort[3].type = 1; //Effet
                    tabjoueur[i].sort[3].def = 4;
                    tabjoueur[i].sort[3].soins = 100;
                    tabjoueur[i].sort[3].PA = 5;
                    tabjoueur[i].sort[3].typesort = 20;
                }
}

pics InitialisationImages(pics Image) {      ///INITIALISATION Bitmaps
    //images des sorts
        Image.psort[0] = load_bitmap("pics/sorts/cac.bmp",NULL);
        Image.psort[1] = load_bitmap("pics/sorts/cac+.bmp",NULL);
        Image.psort[2] = load_bitmap("pics/sorts/sortzombie0.bmp",NULL);
        Image.psort[3] = load_bitmap("pics/sorts/sortzombie0+.bmp",NULL);
        Image.psort[4] = load_bitmap("pics/sorts/sortzombie1.bmp",NULL);
        Image.psort[5] = load_bitmap("pics/sorts/sortzombie1+.bmp",NULL);
        Image.psort[6] = load_bitmap("pics/sorts/sortzombie2.bmp",NULL);
        Image.psort[7] = load_bitmap("pics/sorts/sortzombie2+.bmp",NULL);
        Image.psort[8] = load_bitmap("pics/sorts/sortzombie3.bmp",NULL);
        Image.psort[9] = load_bitmap("pics/sorts/sortzombie3+.bmp",NULL);
        Image.psort[10] = load_bitmap("pics/sorts/sortskeleton0.bmp",NULL);
        Image.psort[11] = load_bitmap("pics/sorts/sortskeleton0+.bmp",NULL);
        Image.psort[12] = load_bitmap("pics/sorts/sortskeleton1.bmp",NULL);
        Image.psort[13] = load_bitmap("pics/sorts/sortskeleton1+.bmp",NULL);
        Image.psort[14] = load_bitmap("pics/sorts/sortskeleton2.bmp",NULL);
        Image.psort[15] = load_bitmap("pics/sorts/sortskeleton2+.bmp",NULL);
        Image.psort[16] = load_bitmap("pics/sorts/sortskeleton3.bmp",NULL);
        Image.psort[17] = load_bitmap("pics/sorts/sortskeleton3+.bmp",NULL);
        Image.psort[18] = load_bitmap("pics/sorts/sortspider0.bmp",NULL);
        Image.psort[19] = load_bitmap("pics/sorts/sortspider0+.bmp",NULL);
        Image.psort[20] = load_bitmap("pics/sorts/sortspider1.bmp",NULL);
        Image.psort[21] = load_bitmap("pics/sorts/sortspider1+.bmp",NULL);
        Image.psort[22] = load_bitmap("pics/sorts/sortspider2.bmp",NULL);
        Image.psort[23] = load_bitmap("pics/sorts/sortspider2+.bmp",NULL);
        Image.psort[24] = load_bitmap("pics/sorts/sortspider3.bmp",NULL);
        Image.psort[25] = load_bitmap("pics/sorts/sortspider3+.bmp",NULL);
        Image.psort[26] = load_bitmap("pics/sorts/sortblaze0.bmp",NULL);
        Image.psort[27] = load_bitmap("pics/sorts/sortblaze0+.bmp",NULL);
        Image.psort[28] = load_bitmap("pics/sorts/sortblaze1.bmp",NULL);
        Image.psort[29] = load_bitmap("pics/sorts/sortblaze1+.bmp",NULL);
        Image.psort[30] = load_bitmap("pics/sorts/sortblaze2.bmp",NULL);
        Image.psort[31] = load_bitmap("pics/sorts/sortblaze2+.bmp",NULL);
        Image.psort[32] = load_bitmap("pics/sorts/sortblaze3.bmp",NULL);
        Image.psort[33] = load_bitmap("pics/sorts/sortblaze3+.bmp",NULL);
        Image.psort[34] = load_bitmap("pics/sorts/sortpillager0.bmp",NULL);
        Image.psort[35] = load_bitmap("pics/sorts/sortpillager0+.bmp",NULL);
        Image.psort[36] = load_bitmap("pics/sorts/sortpillager1.bmp",NULL);
        Image.psort[37] = load_bitmap("pics/sorts/sortpillager1+.bmp",NULL);
        Image.psort[38] = load_bitmap("pics/sorts/sortpillager2.bmp",NULL);
        Image.psort[39] = load_bitmap("pics/sorts/sortpillager2+.bmp",NULL);
        Image.psort[40] = load_bitmap("pics/sorts/sortpillager3.bmp",NULL);
        Image.psort[41] = load_bitmap("pics/sorts/sortpillager3+.bmp",NULL);
        for(int i=0; i<=41; i++){
            if(Image.psort[i]==NULL) printf("Erreur : image psort[%d] non trouvee\n", i);
        }

        //images des obstacle et des cases surlignée
        Image.pblock[0] = load_bitmap("pics/blocks/mouseblock.bmp",NULL);
        Image.pblock[1] = load_bitmap("pics/blocks/moveblock.bmp",NULL);
        Image.pblock[2] = load_bitmap("pics/blocks/moveblock+.bmp",NULL);
        Image.pblock[3] = load_bitmap("pics/blocks/sortblock.bmp",NULL);
        Image.pblock[4] = load_bitmap("pics/blocks/sortblock+.bmp",NULL);
        Image.pblock[5] = load_bitmap("pics/blocks/sortbloquee.bmp",NULL);
        Image.pblock[6] = load_bitmap("pics/blocks/stoneblock.bmp",NULL);
        Image.pblock[7] = load_bitmap("pics/blocks/cobblestoneblock.bmp",NULL);
        Image.pblock[8] = load_bitmap("pics/blocks/logblock.bmp",NULL);
        Image.pblock[9] = load_bitmap("pics/blocks/cobwebblock.bmp",NULL);
        Image.pblock[10] = load_bitmap("pics/blocks/cobwebblock+.bmp",NULL);
        Image.pblock[11] = load_bitmap("pics/blocks/flameblock.bmp",NULL);
        Image.pblock[12] = load_bitmap("pics/blocks/flameblock+.bmp",NULL);
        Image.pblock[13] = load_bitmap("pics/blocks/ombre.bmp",NULL);
        Image.pblock[14] = load_bitmap("pics/blocks/mossycobblestoneblock.bmp",NULL);
        Image.pblock[15] = load_bitmap("pics/blocks/mouss.bmp",NULL);
        Image.pblock[16] = load_bitmap("pics/blocks/sortonblock.bmp",NULL);
        Image.pblock[17] = load_bitmap("pics/blocks/sortonblock+.bmp",NULL);
        Image.pblock[18] = load_bitmap("pics/blocks/leaves.bmp",NULL);
        for(int i=0; i<=18; i++){
            if(Image.pblock[i]==NULL) printf("Erreur : image pblock[%d] non trouvee\n", i);
        }

        //image des personnages et des bots
        Image.pmob[0] = load_bitmap("pics/mobs/zombie1.bmp",NULL);
        Image.pmob[1] = load_bitmap("pics/mobs/zombie1+.bmp",NULL);
        Image.pmob[2] = load_bitmap("pics/mobs/zombie2.bmp",NULL);
        Image.pmob[3] = load_bitmap("pics/mobs/zombie2+.bmp",NULL);
        Image.pmob[4] = load_bitmap("pics/mobs/zombie3.bmp",NULL);
        Image.pmob[5] = load_bitmap("pics/mobs/zombie3+.bmp",NULL);
        Image.pmob[6] = load_bitmap("pics/mobs/zombie4.bmp",NULL);
        Image.pmob[7] = load_bitmap("pics/mobs/zombie4+.bmp",NULL);
        Image.pmob[8] = load_bitmap("pics/mobs/skeleton1.bmp",NULL);
        Image.pmob[9] = load_bitmap("pics/mobs/skeleton1+.bmp",NULL);
        Image.pmob[10] = load_bitmap("pics/mobs/skeleton2.bmp",NULL);
        Image.pmob[11] = load_bitmap("pics/mobs/skeleton2+.bmp",NULL);
        Image.pmob[12] = load_bitmap("pics/mobs/skeleton3.bmp",NULL);
        Image.pmob[13] = load_bitmap("pics/mobs/skeleton3+.bmp",NULL);
        Image.pmob[14] = load_bitmap("pics/mobs/skeleton4.bmp",NULL);
        Image.pmob[15] = load_bitmap("pics/mobs/skeleton4+.bmp",NULL);
        Image.pmob[16] = load_bitmap("pics/mobs/spider1.bmp",NULL);
        Image.pmob[17] = load_bitmap("pics/mobs/spider1+.bmp",NULL);
        Image.pmob[18] = load_bitmap("pics/mobs/spider2.bmp",NULL);
        Image.pmob[19] = load_bitmap("pics/mobs/spider2+.bmp",NULL);
        Image.pmob[20] = load_bitmap("pics/mobs/spider3.bmp",NULL);
        Image.pmob[21] = load_bitmap("pics/mobs/spider3+.bmp",NULL);
        Image.pmob[22] = load_bitmap("pics/mobs/spider4.bmp",NULL);
        Image.pmob[23] = load_bitmap("pics/mobs/spider4+.bmp",NULL);
        Image.pmob[24] = load_bitmap("pics/mobs/blaze1.bmp",NULL);
        Image.pmob[25] = load_bitmap("pics/mobs/blaze1+.bmp",NULL);
        Image.pmob[26] = load_bitmap("pics/mobs/blaze2.bmp",NULL);
        Image.pmob[27] = load_bitmap("pics/mobs/blaze2+.bmp",NULL);
        Image.pmob[28] = load_bitmap("pics/mobs/blaze3.bmp",NULL);
        Image.pmob[29] = load_bitmap("pics/mobs/blaze3+.bmp",NULL);
        Image.pmob[30] = load_bitmap("pics/mobs/blaze4.bmp",NULL);
        Image.pmob[31] = load_bitmap("pics/mobs/blaze4+.bmp",NULL);
        Image.pmob[32] = load_bitmap("pics/mobs/pillager1.bmp",NULL);
        Image.pmob[33] = load_bitmap("pics/mobs/pillager1+.bmp",NULL);
        Image.pmob[34] = load_bitmap("pics/mobs/pillager2.bmp",NULL);
        Image.pmob[35] = load_bitmap("pics/mobs/pillager2+.bmp",NULL);
        Image.pmob[36] = load_bitmap("pics/mobs/pillager3.bmp",NULL);
        Image.pmob[37] = load_bitmap("pics/mobs/pillager3+.bmp",NULL);
        Image.pmob[38] = load_bitmap("pics/mobs/pillager4.bmp",NULL);
        Image.pmob[39] = load_bitmap("pics/mobs/pillager4+.bmp",NULL);
        Image.pmob[40] = load_bitmap("pics/mobs/cavespider1.bmp",NULL);
        Image.pmob[41] = load_bitmap("pics/mobs/cavespider1+.bmp",NULL);
        Image.pmob[42] = load_bitmap("pics/mobs/cavespider2.bmp",NULL);
        Image.pmob[43] = load_bitmap("pics/mobs/cavespider2+.bmp",NULL);
        Image.pmob[44] = load_bitmap("pics/mobs/cavespider3.bmp",NULL);
        Image.pmob[45] = load_bitmap("pics/mobs/cavespider3+.bmp",NULL);
        Image.pmob[46] = load_bitmap("pics/mobs/cavespider4.bmp",NULL);
        Image.pmob[47] = load_bitmap("pics/mobs/cavespider4+.bmp",NULL);
        Image.pmob[48] = load_bitmap("pics/mobs/silverfish1.bmp",NULL);
        Image.pmob[49] = load_bitmap("pics/mobs/silverfish1+.bmp",NULL);
        Image.pmob[50] = load_bitmap("pics/mobs/silverfish2.bmp",NULL);
        Image.pmob[51] = load_bitmap("pics/mobs/silverfish2+.bmp",NULL);
        Image.pmob[52] = load_bitmap("pics/mobs/silverfish3.bmp",NULL);
        Image.pmob[53] = load_bitmap("pics/mobs/silverfish3+.bmp",NULL);
        Image.pmob[54] = load_bitmap("pics/mobs/silverfish4.bmp",NULL);
        Image.pmob[55] = load_bitmap("pics/mobs/silverfish4+.bmp",NULL);
        for(int i=0; i<=55; i++){
            if(Image.pmob[i]==NULL) printf("Erreur : image pmob[%d] non trouvee\n", i);
        }

        //images des menus
        Image.pmenu[0] = load_bitmap("pics/menu/menuplayer1.bmp",NULL);
        Image.pmenu[1] = load_bitmap("pics/menu/menuplayer2.bmp",NULL);
        Image.pmenu[2] = load_bitmap("pics/menu/menuplayer3.bmp",NULL);
        Image.pmenu[3] = load_bitmap("pics/menu/menuplayer4_2.bmp",NULL);
        Image.pmenu[4] = load_bitmap("pics/menu/menuplayer4_3.bmp",NULL);
        Image.pmenu[5] = load_bitmap("pics/menu/menuplayer4_4.bmp",NULL);
        Image.pmenu[6] = load_bitmap("pics/menu/menusort1.bmp",NULL);
        Image.pmenu[7] = load_bitmap("pics/menu/menusort2-.bmp",NULL);
        Image.pmenu[8] = load_bitmap("pics/menu/menusort2.bmp",NULL);
        Image.pmenu[9] = load_bitmap("pics/menu/menusortleave.bmp",NULL);
        Image.pmenu[10] = load_bitmap("pics/menu/menusortleave+.bmp",NULL);
        Image.pmenu[11] = load_bitmap("pics/menu/menuplayermove.bmp",NULL);
        Image.pmenu[12] = load_bitmap("pics/menu/menuplayermove+.bmp",NULL);
        Image.pmenu[13] = load_bitmap("pics/menu/menuplayersort.bmp",NULL);
        Image.pmenu[14] = load_bitmap("pics/menu/menuplayersort+.bmp",NULL);

        Image.pmenu[15] = load_bitmap("pics/menu/classezombie.bmp",NULL);
        Image.pmenu[16] = load_bitmap("pics/menu/classezombie+.bmp",NULL);
        Image.pmenu[17] = load_bitmap("pics/menu/classezombie++.bmp",NULL);
        Image.pmenu[18] = load_bitmap("pics/menu/classeskeleton.bmp",NULL);
        Image.pmenu[19] = load_bitmap("pics/menu/classeskeleton+.bmp",NULL);
        Image.pmenu[20] = load_bitmap("pics/menu/classeskeleton++.bmp",NULL);
        Image.pmenu[21] = load_bitmap("pics/menu/classespider.bmp",NULL);
        Image.pmenu[22] = load_bitmap("pics/menu/classespider+.bmp",NULL);
        Image.pmenu[23] = load_bitmap("pics/menu/classespider++.bmp",NULL);
        Image.pmenu[24] = load_bitmap("pics/menu/classesblaze.bmp",NULL);
        Image.pmenu[25] = load_bitmap("pics/menu/classesblaze+.bmp",NULL);
        Image.pmenu[26] = load_bitmap("pics/menu/classesblaze++.bmp",NULL);
        Image.pmenu[27] = load_bitmap("pics/menu/classepillager.bmp",NULL);
        Image.pmenu[28] = load_bitmap("pics/menu/classepillager+.bmp",NULL);
        Image.pmenu[29] = load_bitmap("pics/menu/classepillager++.bmp",NULL);

        Image.pmenu[30] = load_bitmap("pics/menu/background1.bmp",NULL);
        Image.pmenu[31] = load_bitmap("pics/menu/newgame.bmp",NULL);
        Image.pmenu[32] = load_bitmap("pics/menu/newgame+.bmp",NULL);
        Image.pmenu[33] = load_bitmap("pics/menu/loadsave.bmp",NULL);
        Image.pmenu[34] = load_bitmap("pics/menu/loadsave+.bmp",NULL);
        Image.pmenu[35] = load_bitmap("pics/menu/leave.bmp",NULL);
        Image.pmenu[36] = load_bitmap("pics/menu/leave+.bmp",NULL);
        Image.pmenu[37] = load_bitmap("pics/menu/credits.bmp",NULL);
        Image.pmenu[38] = load_bitmap("pics/menu/credits+.bmp",NULL);
        Image.pmenu[39] = load_bitmap("pics/menu/how.bmp",NULL);
        Image.pmenu[40] = load_bitmap("pics/menu/how+.bmp",NULL);
        Image.pmenu[41] = load_bitmap("pics/menu/minecraft.bmp",NULL);
        Image.pmenu[42] = load_bitmap("pics/menu/teamselect.bmp",NULL);
        Image.pmenu[43] = load_bitmap("pics/menu/background3.bmp",NULL);
        Image.pmenu[44] = load_bitmap("pics/menu/classeinfo-.bmp",NULL);
        Image.pmenu[45] = load_bitmap("pics/menu/classeinfo.bmp",NULL);
        Image.pmenu[46] = load_bitmap("pics/menu/retour.bmp",NULL);
        Image.pmenu[47] = load_bitmap("pics/menu/retour+.bmp",NULL);
        Image.pmenu[48] = load_bitmap("pics/menu/left.bmp",NULL);
        Image.pmenu[49] = load_bitmap("pics/menu/left+.bmp",NULL);
        Image.pmenu[50] = load_bitmap("pics/menu/right.bmp",NULL);
        Image.pmenu[51] = load_bitmap("pics/menu/right+.bmp",NULL);
        Image.pmenu[52] = load_bitmap("pics/menu/turntochoose.bmp",NULL);
        Image.pmenu[53] = load_bitmap("pics/menu/ok.bmp",NULL);
        Image.pmenu[54] = load_bitmap("pics/menu/ok+.bmp",NULL);
        Image.pmenu[55] = load_bitmap("pics/menu/border.bmp",NULL);
        Image.pmenu[56] = load_bitmap("pics/menu/nbjoueur2.bmp",NULL);
        Image.pmenu[57] = load_bitmap("pics/menu/nbjoueur3.bmp",NULL);
        Image.pmenu[58] = load_bitmap("pics/menu/nbjoueur4.bmp",NULL);
        Image.pmenu[59] = load_bitmap("pics/menu/v.bmp",NULL);
        Image.pmenu[60] = load_bitmap("pics/menu/v+.bmp",NULL);
        Image.pmenu[61] = load_bitmap("pics/menu/vv.bmp",NULL);
        Image.pmenu[62] = load_bitmap("pics/menu/vv+.bmp",NULL);
        Image.pmenu[63] = load_bitmap("pics/menu/vv+.bmp",NULL);
        Image.pmenu[64] = load_bitmap("pics/menu/regles.bmp",NULL);
        Image.pmenu[65] = load_bitmap("pics/menu/regles+.bmp",NULL);
        Image.pmenu[66] = load_bitmap("pics/menu/retourmenu.bmp",NULL);
        Image.pmenu[67] = load_bitmap("pics/menu/retourmenu+.bmp",NULL);
        Image.pmenu[68] = load_bitmap("pics/menu/save.bmp",NULL);
        Image.pmenu[69] = load_bitmap("pics/menu/save+.bmp",NULL);
        Image.pmenu[70] = load_bitmap("pics/menu/fintour.bmp",NULL);
        Image.pmenu[71] = load_bitmap("pics/menu/fintour+.bmp",NULL);
        Image.pmenu[72] = load_bitmap("pics/menu/team1.bmp",NULL);
        Image.pmenu[73] = load_bitmap("pics/menu/team1+.bmp",NULL);
        Image.pmenu[74] = load_bitmap("pics/menu/team2.bmp",NULL);
        Image.pmenu[75] = load_bitmap("pics/menu/team2+.bmp",NULL);
        Image.pmenu[76] = load_bitmap("pics/menu/team3.bmp",NULL);
        Image.pmenu[77] = load_bitmap("pics/menu/team3+.bmp",NULL);
        Image.pmenu[78] = load_bitmap("pics/menu/team4.bmp",NULL);
        Image.pmenu[79] = load_bitmap("pics/menu/team4+.bmp",NULL);
        Image.pmenu[80] = load_bitmap("pics/menu/afficheteam1.bmp",NULL);
        Image.pmenu[81] = load_bitmap("pics/menu/afficheteam2.bmp",NULL);
        Image.pmenu[82] = load_bitmap("pics/menu/afficheteam3.bmp",NULL);
        Image.pmenu[83] = load_bitmap("pics/menu/afficheteam4.bmp",NULL);
        Image.pmenu[84] = load_bitmap("pics/menu/save++.bmp",NULL);
        Image.pmenu[85] = load_bitmap("pics/menu/page1.bmp",NULL);
        Image.pmenu[86] = load_bitmap("pics/menu/page2.bmp",NULL);
        Image.pmenu[87] = load_bitmap("pics/menu/page3.bmp",NULL);
        Image.pmenu[88] = load_bitmap("pics/menu/infoarrowleft.bmp",NULL);
        Image.pmenu[89] = load_bitmap("pics/menu/infoarrowleft+.bmp",NULL);
        Image.pmenu[90] = load_bitmap("pics/menu/infoarrowright.bmp",NULL);
        Image.pmenu[91] = load_bitmap("pics/menu/infoarrowright+.bmp",NULL);
        Image.pmenu[92] = load_bitmap("pics/menu/mouse.bmp",NULL);
        Image.pmenu[93] = load_bitmap("pics/menu/terrainfacile.bmp",NULL);
        Image.pmenu[94] = load_bitmap("pics/menu/terrainfacile+.bmp",NULL);
        Image.pmenu[95] = load_bitmap("pics/menu/terraindifficile.bmp",NULL);
        Image.pmenu[96] = load_bitmap("pics/menu/terraindifficile+.bmp",NULL);
        Image.pmenu[97] = load_bitmap("pics/menu/terraincharge.bmp",NULL);
        Image.pmenu[98] = load_bitmap("pics/menu/terraincharge+.bmp",NULL);
        Image.pmenu[99] = load_bitmap("pics/menu/song.bmp",NULL);
        Image.pmenu[100] = load_bitmap("pics/menu/song-.bmp",NULL);
        Image.pmenu[101] = load_bitmap("pics/menu/backgroundjeu.bmp",NULL);
        Image.pmenu[102] = load_bitmap("pics/menu/replay.bmp",NULL);
        Image.pmenu[103] = load_bitmap("pics/menu/replay+.bmp",NULL);
        Image.pmenu[104] = load_bitmap("pics/menu/newgame2.bmp",NULL);
        Image.pmenu[105] = load_bitmap("pics/menu/newgame2+.bmp",NULL);
        Image.pmenu[106] = load_bitmap("pics/menu/vainqueur.bmp",NULL);
        Image.pmenu[107] = load_bitmap("pics/menu/affichezombie.bmp",NULL);
        Image.pmenu[108] = load_bitmap("pics/menu/afficheskeleton.bmp",NULL);
        Image.pmenu[109] = load_bitmap("pics/menu/affichespider.bmp",NULL);
        Image.pmenu[110] = load_bitmap("pics/menu/afficheblaze.bmp",NULL);
        Image.pmenu[111] = load_bitmap("pics/menu/affichepillager.bmp",NULL);
        for(int i=0; i<=111; i++){
            if(Image.pmenu[i]==NULL) printf("Erreur : image pmenu[%d] non trouvee\n", i);
        }

        //images des animations de sort
        Image.panime[0] = load_bitmap("pics/animation/hit.bmp",NULL);
        Image.panime[1] = load_bitmap("pics/animation/healh.bmp",NULL);
        Image.panime[2] = load_bitmap("pics/animation/healh2.bmp",NULL);
        Image.panime[3] = load_bitmap("pics/animation/poison.bmp",NULL);
        Image.panime[4] = load_bitmap("pics/animation/poison2.bmp",NULL);
        Image.panime[5] = load_bitmap("pics/animation/slow.bmp",NULL);
        Image.panime[6] = load_bitmap("pics/animation/slow2.bmp",NULL);
        Image.panime[7] = load_bitmap("pics/animation/def.bmp",NULL);
        Image.panime[8] = load_bitmap("pics/animation/def2.bmp",NULL);
        Image.panime[9] = load_bitmap("pics/animation/wither.bmp",NULL);
        Image.panime[10] = load_bitmap("pics/animation/wither2.bmp",NULL);
        Image.panime[11] = load_bitmap("pics/animation/force.bmp",NULL);
        Image.panime[12] = load_bitmap("pics/animation/force2.bmp",NULL);
        Image.panime[13] = load_bitmap("pics/animation/arrow.bmp",NULL);
        Image.panime[14] = load_bitmap("pics/animation/icearrow.bmp",NULL);
        Image.panime[15] = load_bitmap("pics/animation/spectralarrow.bmp",NULL);
        Image.panime[16] = load_bitmap("pics/animation/trident.bmp",NULL);
        Image.panime[17] = load_bitmap("pics/animation/fireball.bmp",NULL);
        Image.panime[18] = load_bitmap("pics/animation/bigfireball.bmp",NULL);
        Image.panime[19] = load_bitmap("pics/animation/explosion1.bmp",NULL);
        Image.panime[20] = load_bitmap("pics/animation/explosion2.bmp",NULL);
        Image.panime[21] = load_bitmap("pics/animation/explosion3.bmp",NULL);
        Image.panime[22] = load_bitmap("pics/animation/logblock.bmp",NULL);
        Image.panime[23] = load_bitmap("pics/animation/cobweb.bmp",NULL);
        Image.panime[24] = load_bitmap("pics/animation/leaf.bmp",NULL);
        for(int i=0; i<=24; i++){
            if(Image.panime[i]==NULL) printf("Erreur : image panime[%d] non trouvee\n", i);
        }

        return Image;
}

int InitialisationDebutJeu(BITMAP *backscreen,BITMAP *buffer,Joueurs tabjoueur[],Cases plateau[taillePlateauX][taillePlateauY],int nbJoueur,pics Image) {

        for (int i=0; i<nbJoueur; i++) {
            tabjoueur[i].PV = tabjoueur[i].PVmax;
        }

        ///SWAP pour mélanger tabjoueur
        int magique1, magique2;
        Joueurs tmp;
        for (int i=0; i<50; i++) {
            magique1 = rand()%(nbJoueur);
            magique2 = rand()%(nbJoueur);
            tmp = tabjoueur[magique1];
            tabjoueur[magique1] = tabjoueur[magique2];
            tabjoueur[magique2] = tmp;
        }


        int j=0;
        int xclic=-1, yclic=-1;
        int delay=25;
        int cible[taillePlateauX][taillePlateauY]={{0}};
        ///trouve les positions possibles
        for(int x=0; x<taillePlateauX; x++){
            for(int y=0; y<taillePlateauY; y++){
                if(plateau[x][y].type==0 && plateau[x][y].ptjoueur==NULL) cible[x][y]=1;
                else cible[x][y]=0;
                plateau[x][y].ptjoueur=NULL;
            }
        }
        const int Dmin=5; //distance minimal entre les joueurs
        const int timemax=15; //en seconde
        double debut=clock();

        while(j<nbJoueur && !key[KEY_ESC]) {
            clear_bitmap(backscreen);
            clear_bitmap(buffer);

            if (delay > 0) {
                delay -= 1;
            }


            ///-----------------affichage-plateau------------------------------------------------
            affichePlateau(backscreen, buffer, plateau, &xclic, &yclic, Image);

            ///-----------------bouton-retour----------------------------------------------------
            rectfill(buffer,    630,10,770,80,bcolbutton1);
            masked_blit(Image.pmenu[46],backscreen,0,0,650,10,Image.pmenu[46]->w, Image.pmenu[46]->h);

            if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton1) { //Si on touche le bouton Retour
                masked_blit(Image.pmenu[47],backscreen,0,0,650,10,Image.pmenu[47]->w, Image.pmenu[47]->h);

                if (mouse_b == 1) {
                    return 1; ///retour au menu
                }
            }


            ///-----------------affichage-des-tours----------------------------------------------------
            textprintf_ex(backscreen,font,18,20,colwhite,-1,"TURN:%s",tabjoueur[j].nom);
            rectfill(backscreen, 17, 31, 19+100, 38, colwither);
            rectfill(backscreen, 18, 32, 18+(clock()-debut)/CLOCKS_PER_SEC*100/timemax, 37, colwhite);
            int i=0;
            int tmptour=j;
            do{
                i++;
                tmptour += 1;
                if (tmptour >= nbJoueur) {
                    tmptour = 0;
                }
            }while(tabjoueur[j].PV<=0 && i<nbJoueur);
            textprintf_ex(backscreen,font,18,50,colskeleton,-1,"NEXT:%s",tabjoueur[tmptour].nom);


            ///-----------------affichage-du-joueur-en-cour-de-placement--------------------------------------
            if(xclic!=-1 && cible[xclic][yclic]==1) {
                cible[xclic][yclic]=2;
                plateau[xclic][yclic].ptjoueur=&tabjoueur[j];
            }

            afficheTerrain(backscreen, buffer, tabjoueur, plateau, 0, Image, cible);
            if(xclic!=-1 && cible[xclic][yclic]==2) {
                cible[xclic][yclic]=1;
                plateau[xclic][yclic].ptjoueur=NULL;
            }


            ///-----------------initialisation-de-la-position-et-joueur-suivant----------------------------------------------------
            if(mouse_b==1 && delay==0 && cible[xclic][yclic]>0){
                tabjoueur[j].posX=xclic;
                tabjoueur[j].posY=yclic;
                plateau[xclic][yclic].ptjoueur=&tabjoueur[j];

                cible[xclic][yclic]=0;
                j++;
                delay=25;

                ///bloque les positions proches
                for(int x=0; x<taillePlateauX; x++){
                    for(int y=0; y<taillePlateauY; y++){
                        int dx=abs(xclic-x);
                        int dy=abs(yclic-y);
                        if(dx+dy<=Dmin && cible[x][y]>0) cible[x][y]=-2;
                    }
                }
                debut=clock();
            }

            ///si le temps est écouler place le joueur a une position possible aléatoire
            if((clock()-debut)/CLOCKS_PER_SEC>=timemax)
            {
                int mx,my;
                do{
                    mx=rand()%taillePlateauX;
                    my=rand()%taillePlateauY;
                }while(cible[mx][my]<1);

                tabjoueur[j].posX=mx;
                tabjoueur[j].posY=my;
                plateau[mx][my].ptjoueur=&tabjoueur[j];

                cible[mx][my]=0;
                j++;
                delay=25;

                ///bloque les positions proches
                for(int x=0; x<taillePlateauX; x++){
                    for(int y=0; y<taillePlateauY; y++){
                        int dx=abs(mx-x);
                        int dy=abs(my-y);
                        if(dx+dy<=Dmin && cible[x][y]>0) cible[x][y]=-2;
                    }
                }

                debut=clock();
            }

            //Affichage souris
            masked_blit(Image.pmenu[92],backscreen,0,0,mouse_x,mouse_y,Image.pmenu[92]->w, Image.pmenu[92]->h);

            blit(backscreen,screen,0,0,0,0,SCREEN_W,SCREEN_H);
            rest(1);
        }

        for(int b=0; b<tabjoueur[nbJoueurMax].nbbots; b++){
            plateau[tabjoueur[nbJoueurMax].ptbots[b]->posX][tabjoueur[nbJoueurMax].ptbots[b]->posY].ptjoueur=tabjoueur[nbJoueurMax].ptbots[b];
        }
    return 0;
}

///==========================================================================================================///
            ///MENU : lorsque le souus-programme est appelé, on est renvoyé au menu de base du jeu

int MenuPrincipale (BITMAP *backscreen,BITMAP *buffer,Joueurs tabjoueur[],Cases plateau[taillePlateauX][taillePlateauY],pics Image,leaf l[],int *restart,int*pnbJoueur,int*pchoixterrain, SAMPLE* music, int* volume) {

    int val;
    int othervalue=0;
    int delay = 50;
    int menu = 1;
    int choix = 0;
    int nbJoueur = *pnbJoueur;
    int choixterrain = *pchoixterrain;

    if (*restart == 1) choix=1;
    if (*restart == 2) choix=1;

    while (!key[KEY_ESC] && choix == 0) {
        clear_bitmap(backscreen);
        clear_bitmap(buffer);
        masked_blit(Image.pmenu[30],backscreen,0,0,-200,0,Image.pmenu[30]->w, Image.pmenu[30]->h);
        DecorsFeuilles(backscreen,l,Image,0);
        masked_blit(Image.pmenu[55],backscreen,0,0,0,0,Image.pmenu[55]->w, Image.pmenu[55]->h); //border

                ///Affichage du menu
        masked_blit(Image.pmenu[41],backscreen,0,0,240,10,Image.pmenu[31]->w, Image.pmenu[31]->h);

                                                            //Affichage des boutons :
        masked_blit(Image.pmenu[31],backscreen,0,0,200,120,Image.pmenu[31]->w, Image.pmenu[31]->h);
        rectfill(buffer,    200,120,200+Image.pmenu[33]->w,120+Image.pmenu[31]->h,bcolbutton1);

        masked_blit(Image.pmenu[33],backscreen,0,0,200,240,Image.pmenu[33]->w, Image.pmenu[33]->h);     //bouton2 : Load Save
        rectfill(buffer,    200,240,200+Image.pmenu[33]->w,240+Image.pmenu[33]->h,bcolbutton2);

        masked_blit(Image.pmenu[35],backscreen,0,0,200,360,Image.pmenu[35]->w, Image.pmenu[35]->h);     //bouton3 : Exit
        rectfill(buffer,    200,360,200+Image.pmenu[35]->w,360+Image.pmenu[35]->h,bcolbutton3);

        masked_blit(Image.pmenu[37],backscreen,0,0,200,480,Image.pmenu[37]->w, Image.pmenu[37]->h);      //bouton4 : How to play?
        rectfill(buffer,    200,480,200+Image.pmenu[37]->w,480+Image.pmenu[37]->h,bcolbutton4);

        masked_blit(Image.pmenu[39],backscreen,0,0,410,480,Image.pmenu[39]->w, Image.pmenu[39]->h);     //bouton5 : Credits
        rectfill(buffer,    410,480,410+Image.pmenu[37]->w,480+Image.pmenu[37]->h,bcolbutton5);

        ///volume
        const int xs=740, ys=10;///coordonner bouton volume
        rectfill(buffer,xs,ys,xs+48,ys+50-5,bcolbutton15);
        rectfill(backscreen,xs,ys,xs+48,ys+50-5,colblack);

        rectfill(backscreen,xs,ys,xs+1+*volume,ys+50-5,makecol(40+(*volume*2),90+(*volume*4),0));
        if (*volume > 0) {
            masked_blit(Image.pmenu[99],backscreen,0,0,xs,ys,Image.pmenu[99]->w, Image.pmenu[99]->h); ///song
        }
        else {
            masked_blit(Image.pmenu[100],backscreen,0,0,xs,ys,Image.pmenu[100]->w, Image.pmenu[100]->h);
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


        if (delay > 0) {
            delay--;
        }
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton1) { //Si on touche le bouton 1 (New Game)
            masked_blit(Image.pmenu[32],backscreen,0,0,200,120,Image.pmenu[32]->w, Image.pmenu[32]->h);
            if (mouse_b == 1) {
                choix = 1;
            }
        }
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton2) { //Si on touche le bouton 2 (Load Save)
            masked_blit(Image.pmenu[34],backscreen,0,0,200,240,Image.pmenu[34]->w, Image.pmenu[34]->h);
            if (mouse_b == 1) {
                choix = 2;
            }
        }
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton3) { //Si on touche le bouton 3 (Exit)
            masked_blit(Image.pmenu[36],backscreen,0,0,200,360,Image.pmenu[36]->w, Image.pmenu[36]->h);
            if (mouse_b == 1 && delay == 0) {
                choix = 3;
            }
        }
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton4) { //Si on touche le bouton 4 (How to play)
            masked_blit(Image.pmenu[38],backscreen,0,0,200,480,Image.pmenu[38]->w, Image.pmenu[38]->h);
            if (mouse_b == 1) {
                choix = 4;
            }
        }
        if (getpixel(buffer,mouse_x,mouse_y) == bcolbutton5) { //Si on touche le bouton 5 (Credits)
            masked_blit(Image.pmenu[40],backscreen,0,0,410,480,Image.pmenu[40]->w, Image.pmenu[40]->h);
            if (mouse_b == 1) {
                choix = 5;
            }
        }

        //Affichage souris
        masked_blit(Image.pmenu[92],backscreen,0,0,mouse_x,mouse_y,Image.pmenu[92]->w, Image.pmenu[92]->h);
        blit(backscreen,screen,0,0,0,0,SCREEN_W,SCREEN_H);
        rest(1);
    }


        ///Lorsqu'un bouton est cliqué, on sort du menu et on le redirige en fonction du choix
    switch (choix) {

        case 1:     ///NOUVELLE PARTIE

            if (*restart != 1) {    //si restart=1 alors on relance la partie avec les memes parametre que la partie précedente
                ///Etape1 : Choix du nombre de joueurs
            MenuTransition(backscreen);
            nbJoueur = MenuNBJoueur(backscreen,buffer,tabjoueur,Image,l);
            //nbJoueur = 2;
            MenuTransition(backscreen);
            if (nbJoueur == 0) {
                menu = 1;   //retour au menu
                break;
            }
            if (nbJoueur == -1) {
                menu = 0;   //quitte le programme
                break;
            }
                ///Etape2 : Choix des classes et team pour CHAQUE joueurs (individuellement)

            for (int i=0 ; i<nbJoueurMax+1; i++) {
                tabjoueur[i].ecranX=-1;
                tabjoueur[i].ecranY=-1;
                tabjoueur[i].IDcadavre=-1;
                tabjoueur[i].nbbots=0;
                for(int b=0; b<nbBotsMax; b++) tabjoueur[i].ptbots[b]=NULL;
            }
            for (int i=0 ; i<nbJoueur ; i++){
                val = MenuChoixClasse(backscreen,buffer,tabjoueur,i,nbJoueur,Image,l);
                //val = 1;

                MenuTransition(backscreen);
                if (val == 0) {
                    menu = 1;   //retour au menu
                    break;
                }
                if (val == -1) {
                    menu = 0;   //quitte le programme
                    break;
                }
            }
                    ///Etape3 : Choix du terrain

            val = MenuChoixTerrain(backscreen,buffer,tabjoueur,Image,l);
            MenuTransition(backscreen);
            if (val == 0) {
                menu = 1;   //retour au menu
                break;
            }
            else if (val == -1) {
                menu = 0;   //quitte le programme
                break;
            }

            choixterrain = val;
            }
            *restart=0;
                 ///Initialisation des autres variables du jeu..
            for (int i=0 ; i<nbJoueur ; i++){
                tabjoueur[i].sens=4;
                tabjoueur[i].lenteur = 0;
                tabjoueur[i].force = 0;
                tabjoueur[i].poison = 0;
                tabjoueur[i].wither = 0;
                tabjoueur[i].def = 0;
                tabjoueur[i].ecranX=-1;
                tabjoueur[i].ecranY=-1;
                tabjoueur[i].nbbots=0;

                InitialisationClasses(tabjoueur,i);
            }
            InitialisationPlateau(plateau, tabjoueur, choixterrain);

            val = InitialisationDebutJeu(backscreen, buffer, tabjoueur,plateau,nbJoueur,Image);
            if (val == 1) {
                menu = 1;   //retour au menu
                break;
            }
                ///Etape4 : Le jeu commence

            for (int i=0 ; i<nbJoueur ;i++) {
                tabjoueur[i].PM = tabjoueur[i].PMmax;
                tabjoueur[i].PA = tabjoueur[i].PAmax;
            }
            if(music!=NULL) {
                play_sample(music, 255, 0, 1000, PLAYMODE_LOOP);
                adjust_sample(music, (float)*volume*255.0/40.0, 0, 1000, 0);
            }
            val = Jeu(backscreen,buffer,tabjoueur,plateau,nbJoueur, l, 1, Image, music, volume);
            othervalue = val;
            if (val == 1) {
                menu = 1;   //retour au menu
                break;
            }
            if (val == 0) {
                menu = 0;   //quitte le programme
                break;
            }

            MenuTransition(backscreen);
            break;

        case 2: //Load Save
            {
                int tour;
                chargerparti(plateau, tabjoueur, &nbJoueur, &tour);
                if(music!=NULL) {
                    play_sample(music, 255, 0, 1000, PLAYMODE_LOOP);
                    adjust_sample(music, (float)*volume*255.0/40.0, 0, 1000, 0);
                }
                val = Jeu(backscreen,buffer,tabjoueur,plateau,nbJoueur, l, tour, Image, music, volume);
                othervalue = val;
            }

            break;

        case 4: //Credit
            val = credits(backscreen, buffer, l, Image);
            if(val==0){
                menu = 1; //retour au menu
                break;
            }
            if(val == -1){
                menu = 0; //quitte le programme
                break;
            }

            break;

        case 5: //How to play ?
            val = rules(backscreen, buffer, Image, l);
            if(val==0){
                menu = 1; //retour au menu
                break;
            }
            if(val == -1){
                menu = 0; //quitte le programme
                break;
            }
            break;

        default: //L'utilisateur a appuyé sur la touche ECHAP ou le bouton EXIT --> Quitte le programme
            menu = 0;
            return 0;
            break;
    }

    *restart = 0;
    if (othervalue == 3) {
        *restart = 1;
    }
    else if (othervalue == 4) {
        *restart = 2;
    }

    *pnbJoueur = nbJoueur;
    *pchoixterrain = choixterrain;

    stop_sample(music);
    return menu;
}
///==========================================================================================================///

void InitialisationPlateau(Cases plateau[taillePlateauX][taillePlateauY], Joueurs tabjoueur[], int val)
{
    FILE* pf = fopen("fichier/blocktype.txt", "r");
    int bug=0;
    if(pf != NULL)
    {
        char tmpc;
        for (int y=0 ; y<taillePlateauY ; y++) {
            for (int x=0 ; x<taillePlateauX ; x++) {
                fscanf(pf, "%c", &tmpc);

                ///default
                plateau[x][y].ptjoueur=NULL;
                plateau[x][y].PVobstacle= -1;
                plateau[x][y].PVMAXobstacle= -1;
                plateau[x][y].tourobstacle=-1;
                plateau[x][y].IDcreateur=-1;
                plateau[x][y].sortbloque=0;
                if(tmpc =='0') {
                    plateau[x][y].blocktype=1;
                }
                if(tmpc =='1') {
                    plateau[x][y].blocktype=0;
                }
                if(tmpc == '2') {
                    plateau[x][y].blocktype=2;
                }
                if(tmpc == '3') {
                    plateau[x][y].blocktype=3;
                }
            }
            fscanf(pf, "%c", &tmpc); ///pour pas compter les \n
            if(tmpc != '\n' && y!=taillePlateauY-1) {
                bug = 1;
                printf("format de la map incorrecte\n");
            }
        }
    }
    else {
        printf("erreur fichier <<blocktype>> non trouve\n");
        bug = 1;
    }
    if (bug==1) {
        printf("generation du plateau aleatoire..\n");
        for (int y=0 ; y<taillePlateauY ; y++) {
            for (int x=0 ; x<taillePlateauX ; x++) {
                plateau[x][y].blocktype=rand()%(4);
            }
        }
    }
    fclose(pf);// fermeture du fichier
    pf = NULL;

    ///----------choix-----------------------------------------------------------------------------------------
    if (val == 1) {    ///Map random facile
        int magiquetype;
        for (int y=0 ; y<taillePlateauY ; y++) {
            for (int x=0 ; x<taillePlateauX ; x++) {
                magiquetype=rand()%100;

                if(magiquetype > 8) {  //cases vide
                    plateau[x][y].type=0;
                }
                else if (magiquetype > 5){
                    plateau[x][y].type=4;   //feuille
                    plateau[x][y].PVMAXobstacle=1;
                    plateau[x][y].PVobstacle=plateau[x][y].PVMAXobstacle;
                    plateau[x][y].sortbloque=1;
                }
                else if (magiquetype > 1){ //roche
                    plateau[x][y].type=rand()%2+1;
                    plateau[x][y].PVMAXobstacle=rand()%(100)+20; // 20 - 120
                    plateau[x][y].PVobstacle=plateau[x][y].PVMAXobstacle;
                    plateau[x][y].sortbloque=1;
                }
                else if (magiquetype > 0){ //roche haute
                    plateau[x][y].type=5;
                    plateau[x][y].PVMAXobstacle=rand()%(150)+30; // 30 - 180
                    plateau[x][y].PVobstacle=plateau[x][y].PVMAXobstacle;
                    plateau[x][y].sortbloque=1;
                }
                else {///1%  bot
                    plateau[x][y].type=0;
                    if(tabjoueur[nbJoueurMax].nbbots<nbBotsMax){
                        Joueurs* bot=(Joueurs*)malloc(sizeof(Joueurs));
                        bot->PVmax=110;
                        bot->PV=bot->PVmax;
                        bot->PMmax=3;
                        bot->PM=bot->PMmax;
                        bot->IDteam=-1;
                        bot->classe.type=-2;

                        bot->ecranX=-1;
                        bot->ecranY=-1;
                        bot->posX=x;
                        bot->posY=y;
                        bot->sens=4;

                        bot->lenteur = 0;
                        bot->force = 0;
                        bot->poison = 0;
                        bot->wither = 0;
                        bot->def = 0;

                        bot->nbbots=0;
                        bot->IDcadavre=0;

                        tabjoueur[nbJoueurMax].ptbots[tabjoueur[nbJoueurMax].nbbots]=bot;
                        plateau[x][y].ptjoueur=bot;
                        tabjoueur[nbJoueurMax].nbbots+=1;
                    }
                }
            }
        }
    }
    else if (val == 2) {    ///Map random difficile
        int magiquetype;
        for (int y=0 ; y<taillePlateauY ; y++) {
            for (int x=0 ; x<taillePlateauX ; x++) {
                magiquetype=rand()%100;

                if(magiquetype > 20) {
                    plateau[x][y].type=0;
                }
                else if (magiquetype > 18){
                    plateau[x][y].type=4;   //feuille
                    plateau[x][y].PVMAXobstacle=1;
                    plateau[x][y].PVobstacle=plateau[x][y].PVMAXobstacle;
                    plateau[x][y].sortbloque=1;
                }
                else if (magiquetype > 6){ //roche
                    plateau[x][y].type=rand()%2+1;
                    plateau[x][y].PVMAXobstacle=rand()%(100)+60;  // 60 - 160
                    plateau[x][y].PVobstacle=plateau[x][y].PVMAXobstacle;
                    plateau[x][y].sortbloque=1;
                }
                else if (magiquetype > 2){ //haute roche
                    plateau[x][y].type=5;
                    plateau[x][y].PVMAXobstacle=rand()%(150)+60; // 60 - 210
                    plateau[x][y].PVobstacle=plateau[x][y].PVMAXobstacle;
                    plateau[x][y].sortbloque=1;
                }
                else {//3%  bot
                    plateau[x][y].type=0;
                    if(tabjoueur[nbJoueurMax].nbbots<nbBotsMax){
                        Joueurs* bot=(Joueurs*)malloc(sizeof(Joueurs));
                        bot->PVmax=110;
                        bot->PV=bot->PVmax;
                        bot->PMmax=3;
                        bot->PM=bot->PMmax;
                        bot->IDteam=-1;
                        bot->classe.type=-2;

                        bot->ecranX=-1;
                        bot->ecranY=-1;
                        bot->posX=x;
                        bot->posY=y;
                        bot->sens=4;

                        bot->lenteur = 0;
                        bot->force = 0;
                        bot->poison = 0;
                        bot->wither = 0;
                        bot->def = 0;

                        bot->nbbots=0;
                        bot->IDcadavre=0;

                        tabjoueur[nbJoueurMax].ptbots[tabjoueur[nbJoueurMax].nbbots]=bot;
                        plateau[x][y].ptjoueur=bot;
                        tabjoueur[nbJoueurMax].nbbots+=1;
                    }
                }
            }
        }
    }
    else if (val == 3) {    ///Map chargé depuis txt
        FILE* pf = fopen("fichier/map.txt", "r");
        if(pf != NULL)
        {
            char tmpc;
            for (int y=0 ; y<taillePlateauY ; y++) {
                for (int x=0 ; x<taillePlateauX ; x++) {
                    fscanf(pf, "%c", &tmpc);

                    if(tmpc =='0') { //case normal
                        plateau[x][y].type=0;
                    }
                    else if(tmpc == '1') { //roche
                        if(rand()%8!=0){
                            plateau[x][y].type=rand()%2+1;
                            plateau[x][y].PVMAXobstacle=rand()%(100)+40;  // 40 - 140
                            plateau[x][y].PVobstacle=plateau[x][y].PVMAXobstacle;
                            plateau[x][y].sortbloque=1;
                        }
                        else{//haute roche
                            plateau[x][y].type=5;
                            plateau[x][y].PVMAXobstacle=rand()%(150)+60; // 60 - 210
                            plateau[x][y].PVobstacle=plateau[x][y].PVMAXobstacle;
                            plateau[x][y].sortbloque=1;
                        }
                    }
                    else if (tmpc == 'f') { //feuille
                        plateau[x][y].type=4;   //feuille
                        plateau[x][y].PVMAXobstacle=1;
                        plateau[x][y].PVobstacle=plateau[x][y].PVMAXobstacle;
                        plateau[x][y].sortbloque=1;
                    }
                    else if (tmpc == 'b'){ //bot
                        plateau[x][y].type=0;
                        if(tabjoueur[nbJoueurMax].nbbots<nbBotsMax){
                            Joueurs* bot=(Joueurs*)malloc(sizeof(Joueurs));
                            bot->PVmax=110;
                            bot->PV=bot->PVmax;
                            bot->PMmax=3;
                            bot->PM=bot->PMmax;
                            bot->IDteam=-1;
                            bot->classe.type=-2;

                            bot->ecranX=-1;
                            bot->ecranY=-1;
                            bot->posX=x;
                            bot->posY=y;
                            bot->sens=4;

                            bot->lenteur = 0;
                            bot->force = 0;
                            bot->poison = 0;
                            bot->wither = 0;
                            bot->def = 0;

                            bot->nbbots=0;
                            bot->IDcadavre=0;

                            tabjoueur[nbJoueurMax].ptbots[tabjoueur[nbJoueurMax].nbbots]=bot;
                            plateau[x][y].ptjoueur=bot;
                            tabjoueur[nbJoueurMax].nbbots+=1;
                        }
                    }
                }
                fscanf(pf, "%c", &tmpc); ///pour pas compter les \n
                if(tmpc != '\n' && y!=taillePlateauY-1) printf("format de la map incorrecte");
            }
            fclose(pf);// fermeture du fichier
            pf = NULL;
        }
        else printf("erreur fichier non trouvé");
    }
}

void sauvegarde(Cases plateau[taillePlateauX][taillePlateauY], Joueurs tabjoueur[nbJoueurMax], int nbjoueur, int tour)
{
    FILE* pt=fopen("fichier/save.txt", "w");
    if(pt!=NULL)
    {
        ///sauvegarde du plateau
        for(int x=0; x<taillePlateauX; x++){
            for(int y=0; y<taillePlateauY; y++){
                fprintf(pt, "%d %d %d %d %d %d %d\n", plateau[x][y].type, plateau[x][y].blocktype, plateau[x][y].PVobstacle, plateau[x][y].PVMAXobstacle, plateau[x][y].tourobstacle, plateau[x][y].IDcreateur, plateau[x][y].sortbloque);
            }
        }

        ///sauvegarde des joueurs
        fprintf(pt, "\n\n%d %d\n", nbjoueur, tour);
        for(int i=0; i<nbjoueur; i++)
        {
            fprintf(pt, "%s %d %d %d %f %f ", tabjoueur[i].nom, tabjoueur[i].posX, tabjoueur[i].posY, tabjoueur[i].sens, tabjoueur[i].ecranX, tabjoueur[i].ecranY);
            fprintf(pt, "%d %d %d %d %d %d ", tabjoueur[i].PV, tabjoueur[i].PA, tabjoueur[i].PM, tabjoueur[i].PVmax, tabjoueur[i].PAmax, tabjoueur[i].PMmax);
            fprintf(pt, "%d %d %d %d ", tabjoueur[i].classe.type, tabjoueur[i].IDteam, tabjoueur[i].poison, tabjoueur[i].wither);
            fprintf(pt, "%d %d %d %d %d\n", tabjoueur[i].lenteur, tabjoueur[i].force, tabjoueur[i].def, tabjoueur[i].nbbots, tabjoueur[i].IDcadavre);

            ///sauvegarde des bots de joueurs (cave spider)
            for(int b=0; b<tabjoueur[i].nbbots; b++)
            {
                fprintf(pt, "bot %d %d %d %f %f ", tabjoueur[i].ptbots[b]->posX, tabjoueur[i].ptbots[b]->posY, tabjoueur[i].ptbots[b]->sens, tabjoueur[i].ptbots[b]->ecranX, tabjoueur[i].ptbots[b]->ecranY);
                fprintf(pt, "%d %d %d ", tabjoueur[i].ptbots[b]->PV, tabjoueur[i].ptbots[b]->PVmax, tabjoueur[i].ptbots[b]->PMmax);
                fprintf(pt, "%d %d %d %d ", tabjoueur[i].ptbots[b]->classe.type, tabjoueur[i].ptbots[b]->IDteam, tabjoueur[i].ptbots[b]->poison, tabjoueur[i].ptbots[b]->wither);
                fprintf(pt, "%d %d %d %d\n", tabjoueur[i].ptbots[b]->lenteur, tabjoueur[i].ptbots[b]->force, tabjoueur[i].ptbots[b]->def, tabjoueur[i].ptbots[b]->nbbots);
            }
        }

        ///sauvegarde des bots naturel
        fprintf(pt, "\n\n%d\n", tabjoueur[nbJoueurMax].nbbots);
        for(int b=0; b<tabjoueur[nbJoueurMax].nbbots; b++)
        {
            fprintf(pt, "natBot %d %d %d %f %f ", tabjoueur[nbJoueurMax].ptbots[b]->posX, tabjoueur[nbJoueurMax].ptbots[b]->posY, tabjoueur[nbJoueurMax].ptbots[b]->sens, tabjoueur[nbJoueurMax].ptbots[b]->ecranX, tabjoueur[nbJoueurMax].ptbots[b]->ecranY);
            fprintf(pt, "%d %d %d ", tabjoueur[nbJoueurMax].ptbots[b]->PV, tabjoueur[nbJoueurMax].ptbots[b]->PVmax, tabjoueur[nbJoueurMax].ptbots[b]->PMmax);
            fprintf(pt, "%d %d %d %d ", tabjoueur[nbJoueurMax].ptbots[b]->classe.type, tabjoueur[nbJoueurMax].ptbots[b]->IDteam, tabjoueur[nbJoueurMax].ptbots[b]->poison, tabjoueur[nbJoueurMax].ptbots[b]->wither);
            fprintf(pt, "%d %d %d %d\n", tabjoueur[nbJoueurMax].ptbots[b]->lenteur, tabjoueur[nbJoueurMax].ptbots[b]->force, tabjoueur[nbJoueurMax].ptbots[b]->def, tabjoueur[nbJoueurMax].ptbots[b]->nbbots);
        }
        fclose(pt);
        pt=NULL;
    }
    else printf("Erreur fichier sauvegarde");
}

void chargerparti(Cases plateau[taillePlateauX][taillePlateauY], Joueurs tabjoueur[nbJoueurMax], int* nbjoueur, int* tour)
{
    FILE* pt=fopen("fichier/save.txt", "r");
    if(pt!=NULL)
    {
        ///initiaisation des variable obligatoire
        for(int i=0; i<nbJoueurMax+1; i++) {
            tabjoueur[i].nbbots=0;
            tabjoueur[i].ecranX=-1;
            tabjoueur[i].ecranY=-1;
        }
        ///lecture de la sauvegarde du plateau
        for(int x=0; x<taillePlateauX; x++){
            for(int y=0; y<taillePlateauY; y++){
                fscanf(pt, "%d %d %d %d %d %d %d\n", &plateau[x][y].type, &plateau[x][y].blocktype, &plateau[x][y].PVobstacle, &plateau[x][y].PVMAXobstacle, &plateau[x][y].tourobstacle, &plateau[x][y].IDcreateur, &plateau[x][y].sortbloque);
                plateau[x][y].ptjoueur=NULL;
            }
        }
        ///lecture de la sauvegarde des joueurs
        fscanf(pt, "\n\n%d %d\n", nbjoueur, tour);
        for(int i=0; i<*nbjoueur; i++)
        {
            fscanf(pt, "%s %d %d %d %f %f ", tabjoueur[i].nom, &tabjoueur[i].posX, &tabjoueur[i].posY, &tabjoueur[i].sens, &tabjoueur[i].ecranX, &tabjoueur[i].ecranY);
            fscanf(pt, "%d %d %d %d %d %d ", &tabjoueur[i].PV, &tabjoueur[i].PA, &tabjoueur[i].PM, &tabjoueur[i].PVmax, &tabjoueur[i].PAmax, &tabjoueur[i].PMmax);
            fscanf(pt, "%d %d %d %d ", &tabjoueur[i].classe.type, &tabjoueur[i].IDteam, &tabjoueur[i].poison, &tabjoueur[i].wither);
            fscanf(pt, "%d %d %d %d %d\n", &tabjoueur[i].lenteur, &tabjoueur[i].force, &tabjoueur[i].def, &tabjoueur[i].nbbots, &tabjoueur[i].IDcadavre);
            InitialisationClasses(tabjoueur, i);
            plateau[tabjoueur[i].posX][tabjoueur[i].posY].ptjoueur=&tabjoueur[i];

            ///lecture de la sauvegarde des bots
            for(int b=0; b<tabjoueur[i].nbbots; b++)
            {
                Joueurs* bot=(Joueurs*) malloc(sizeof(Joueurs));
                fscanf(pt, "%s %d %d %d %f %f ", bot->nom, &bot->posX, &bot->posY, &bot->sens, &bot->ecranX, &bot->ecranY);
                fscanf(pt, "%d %d %d ", &bot->PV, &bot->PVmax, &bot->PMmax);
                fscanf(pt, "%d %d %d %d ", &bot->classe.type, &bot->IDteam, &bot->poison, &bot->wither);
                fscanf(pt, "%d %d %d %d\n", &bot->lenteur, &bot->force, &bot->def, &bot->nbbots);

                tabjoueur[i].ptbots[b]=bot;
                plateau[tabjoueur[i].ptbots[b]->posX][tabjoueur[i].ptbots[b]->posY].ptjoueur=bot;
            }
        }
        ///lecture de la sauvegarde des bots naturel
        fscanf(pt, "\n\n%d\n", &tabjoueur[nbJoueurMax].nbbots);
        for(int b=0; b<tabjoueur[nbJoueurMax].nbbots; b++)
        {
            Joueurs* bot=(Joueurs*) malloc(sizeof(Joueurs));
            fscanf(pt, "%s %d %d %d %f %f ", bot->nom, &bot->posX, &bot->posY, &bot->sens, &bot->ecranX, &bot->ecranY);
            fscanf(pt, "%d %d %d ", &bot->PV, &bot->PVmax, &bot->PMmax);
            fscanf(pt, "%d %d %d %d ", &bot->classe.type, &bot->IDteam, &bot->poison, &bot->wither);
            fscanf(pt, "%d %d %d %d\n", &bot->lenteur, &bot->force, &bot->def, &bot->nbbots);

            tabjoueur[nbJoueurMax].ptbots[b]=bot;
            plateau[tabjoueur[nbJoueurMax].ptbots[b]->posX][tabjoueur[nbJoueurMax].ptbots[b]->posY].ptjoueur=bot;
        }
        fclose(pt);
        pt=NULL;
    }
    else printf("Erreur fichier sauvegarde");
}
