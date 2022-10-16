#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include <time.h>
#include <math.h>
#include "head.h"


void effetTour(BITMAP* backscreen, BITMAP* buffer, Cases plateau[taillePlateauX][taillePlateauY], Joueurs tabjoueur[], int tour,pics Image)
{
    ///fonction qui active tous les effets liés aux tours
    ///(par exemple le poison infligue 10 dégâts au joueurs empoisonnés à la fin de son tour)

    //active les effet de poison et wither et réduit les temps d'effet de tous les effet
    if(tabjoueur[tour].classe.type>0){
        if(tabjoueur[tour].poison>0)
        {
            tabjoueur[tour].poison-=1;
            tabjoueur[tour].PV-=10;
        }
        if(tabjoueur[tour].wither>0)
        {
            tabjoueur[tour].wither-=1;
            tabjoueur[tour].PV-=20;
        }
        if(tabjoueur[tour].force>0) tabjoueur[tour].force-=1;
        if(tabjoueur[tour].def>0) tabjoueur[tour].def-=1;

        ///test si le joueur est mort
        if(tabjoueur[tour].PV<=0)
        {
            tabjoueur[tour].PV=0;
            if(tabjoueur[tour].IDcadavre<0){
                tabjoueur[tour].IDcadavre=-tabjoueur[tour].IDcadavre;
                for(int i=0; i<nbJoueurMax; i++){
                    if(tabjoueur[i].IDcadavre<0) tabjoueur[i].IDcadavre--;
                }
            }
            plateau[tabjoueur[tour].posX][tabjoueur[tour].posY].ptjoueur=NULL;
        }
    }
    ///reduit la durées de vie des obstacle affecter par les tours
    for(int x=0; x<taillePlateauX; x++)
    {
        for(int y=0; y<taillePlateauY; y++)
        {
            if(plateau[x][y].IDcreateur==tour)
            {
                if(plateau[x][y].tourobstacle>0) plateau[x][y].tourobstacle-=1;
                //test si l'obstacle est détruit
                if(plateau[x][y].tourobstacle==0)
                {
                    plateau[x][y].type=0;
                    plateau[x][y].PVobstacle=-1;
                    plateau[x][y].tourobstacle=-1;
                    plateau[x][y].IDcreateur=-1;
                    plateau[x][y].sortbloque=0;
                }

            }
        }
    }

    ///bots
    //gere les effet des bots
    for(int b=0; b<tabjoueur[tour].nbbots; b++)
    {
        deplacement_auto(backscreen, buffer, tabjoueur[tour].ptbots[b],tabjoueur, plateau,Image);

        if(tabjoueur[tour].ptbots[b]->poison>0)
        {
            tabjoueur[tour].ptbots[b]->poison-=1;
            tabjoueur[tour].ptbots[b]->PV-=10;
        }
        if(tabjoueur[tour].ptbots[b]->wither>0)
        {
            tabjoueur[tour].ptbots[b]->wither-=1;
            tabjoueur[tour].ptbots[b]->PV-=20;
        }
        if(tabjoueur[tour].ptbots[b]->force>0) tabjoueur[tour].force-=1;
        if(tabjoueur[tour].ptbots[b]->def>0) tabjoueur[tour].def-=1;

        if(tabjoueur[tour].ptbots[b]->PV<=0)
        {
            tabjoueur[tour].ptbots[b]->PV=0;
            plateau[tabjoueur[tour].ptbots[b]->posX][tabjoueur[tour].ptbots[b]->posY].ptjoueur=NULL;
        }
    }
}

void effetPlateau(Cases plateau[taillePlateauX][taillePlateauY], Joueurs tabjoueur[], Joueurs* actjoueur, int effetbot)
{
    ///fonction qui active tous les effet de plateau (exemple : un joueur est dans une flamme, donc il prend des dégâts

    if(actjoueur->classe.type>0){
        if(plateau[actjoueur->posX][actjoueur->posY].type==-1 && actjoueur->classe.type!=3)  //Toile d'araignée
        {
            actjoueur->PM=(actjoueur->PM>=1 ? 1 : 0);
        }
        else if(plateau[actjoueur->posX][actjoueur->posY].type==-2 && actjoueur->classe.type!=4) //Feu
        {
            actjoueur->PV-=50;
            if(actjoueur->PV<=0)
            {
                actjoueur->PV=0;
                if(actjoueur->IDcadavre<0){
                    actjoueur->IDcadavre=-actjoueur->IDcadavre;
                    for(int i=0; i<nbJoueurMax; i++){
                        if(tabjoueur[i].IDcadavre<0) tabjoueur[i].IDcadavre--;
                    }
                }
                plateau[actjoueur->posX][actjoueur->posY].ptjoueur=NULL;
            }
        }
    }
    ///bots
    //effet de plateau pour les bots
    if(effetbot){
        for(int b; b<actjoueur->nbbots; b++)
        {
            if(plateau[actjoueur->ptbots[b]->posX][actjoueur->ptbots[b]->posY].type==-1)  //Toile d'araignée
            {
                actjoueur->ptbots[b]->PM=(actjoueur->ptbots[b]->PM>=1 ? 1 : 0);
            }
            else if(plateau[actjoueur->ptbots[b]->posX][actjoueur->ptbots[b]->posY].type==-2) //Feu
            {
                actjoueur->ptbots[b]->PV-=50;
                if(actjoueur->ptbots[b]->PV<=0)
                {
                    actjoueur->ptbots[b]->PV=0;
                    plateau[actjoueur->ptbots[b]->posX][actjoueur->ptbots[b]->posY].ptjoueur=NULL;
                    supp_bot(plateau, tabjoueur);
                }
            }
        }
    }
}

void lancerSorts(BITMAP* backscreen, BITMAP* buffer, Cases plateau[taillePlateauX][taillePlateauY], Joueurs tabjoueur[], int tour, Sorts spell, double* debut, pics Image)
{
    ///fonction qui gère la totalité des sorts en fonction des paramètres de la structure Sorts
    /// Ex : soins = 15, alors le sort regénère 15 PV   /   lenteur = 3 : il mets un effet de lenteur qui dure 3 secondes

    int missed = 0;     // passe à 1 si l'attaque rate
    int cible[taillePlateauX][taillePlateauY]={{0}};
    cible[tabjoueur[tour].posX][tabjoueur[tour].posY]=-1;
    int destX,destY;
    int delay=25;
    int taillex = 33;
    int tailley = 22;

    ///calcul les cases possible pour le lancer de sort
    if(spell.formePorte==0) // forme de cercle (losange)
    {
        int borneXmin=(tabjoueur[tour].posX-spell.porteMax>0 ?              tabjoueur[tour].posX-spell.porteMax : 0);
        int borneXmax=(tabjoueur[tour].posX+spell.porteMax<taillePlateauX ? tabjoueur[tour].posX+spell.porteMax : taillePlateauX-1);
        int borneYmin=(tabjoueur[tour].posY-spell.porteMax>0 ?              tabjoueur[tour].posY-spell.porteMax : 0);
        int borneYmax=(tabjoueur[tour].posY+spell.porteMax<taillePlateauY ? tabjoueur[tour].posY+spell.porteMax : taillePlateauY-1);

        int delta;
        for(int x=borneXmin; x <= borneXmax; x++)
        {
            for(int y=borneYmin; y <= borneYmax; y++)
            {
                delta=abs(tabjoueur[tour].posX-x)+abs(tabjoueur[tour].posY-y);
                if(spell.porteMin<=delta && delta<=spell.porteMax)
                {
                    cible[x][y]=1;
                }
            }
        }
    }
    else if(spell.formePorte==1) //forme de ligne (4 directions)
    {
        for(int i=spell.porteMin; i <= spell.porteMax; i++)
        {
            if(tabjoueur[tour].posX-i>=0)              cible[tabjoueur[tour].posX-i][tabjoueur[tour].posY]=1;
            if(tabjoueur[tour].posX+i<taillePlateauX) cible[tabjoueur[tour].posX+i][tabjoueur[tour].posY]=1;
            if(tabjoueur[tour].posY-i>=0)              cible[tabjoueur[tour].posX][tabjoueur[tour].posY-i]=1;
            if(tabjoueur[tour].posY+i<taillePlateauY) cible[tabjoueur[tour].posX][tabjoueur[tour].posY+i]=1;
        }
    }

    ///condition des sorts
    for(int x=0; x<taillePlateauX; x++)
    {
        for(int y=0; y<taillePlateauY; y++)
        {

            if(cible[x][y]==1)
            {
                ///condition cas part cas
                int cond_totem=(spell.typesort != 20 || plateau[x][y].ptjoueur->PV < 201);
                int cond_POIhealh=(spell.typesort != 20 || plateau[x][y].ptjoueur->PV < 201) && (spell.typesort != 3 || plateau[x][y].ptjoueur->lenteur>0 || plateau[x][y].ptjoueur->poison>0 || plateau[x][y].ptjoueur->wither>0);
                ///condition générale
                int cond_joueur=(plateau[x][y].ptjoueur!=NULL);
                int cond_sort0=(spell.type==0);
                int cond_sort1=0;
                if(cond_joueur)
                {
                    cond_sort1=(spell.type==1 && plateau[x][y].ptjoueur->IDteam == tabjoueur[tour].IDteam);
                }
                int cond_sort2=(spell.type==2 && plateau[x][y].type==0 && plateau[x][y].ptjoueur==NULL);
                int cond_sort3=(spell.type==3 && plateau[x][y].type==0 && plateau[x][y].ptjoueur==NULL && tabjoueur[tour].nbbots<3);

                ///si consigne pas respecter on grise la case
                if(!(cond_sort0 || cond_sort1 || cond_sort2 || cond_sort3) || !cond_totem || !cond_POIhealh)
                {
                    cible[x][y]=-2;
                    continue;
                }

                if(spell.obstacle)
                {
                    ///si le sort peut etre bloquer par les obstacles on calcul si il l'es ou pas
                    const float precision=100;
                    float tmpx=tabjoueur[tour].posX;
                    float tmpy=tabjoueur[tour].posY;
                    float fractx=(x-tmpx)/precision;
                    float fracty=(y-tmpy)/precision;

                    //trace une ligne droite entre la case actuel et le joueur et test si il y a un obstacle dessu
                    for(int i=0; i<precision; i++)
                    {
                        tmpx+=fractx;
                        tmpy+=fracty;
                        ///condition pour détecter les sorts bloqués
                        int cond_obstacle=(plateau[(int)round(tmpx)][(int)round(tmpy)].sortbloque==1);
                        int cond_cible=(!((int)round(tmpx)==x && (int)round(tmpy)==y));
                        int cond_cible_j=(!((int)round(tmpx)==tabjoueur[tour].posX && (int)round(tmpy)==tabjoueur[tour].posY));

                        //test pour les diagonales 1
                        int cond_diag_p=(plateau[(int)round(tmpx+0.05)][(int)round(tmpy)].sortbloque==1);
                        int cond_cible_p=(!((int)round(tmpx+0.05)==x && (int)round(tmpy)==y));
                        int cond_cible_j_p=(!((int)round(tmpx+0.05)==tabjoueur[tour].posX && (int)round(tmpy)==tabjoueur[tour].posY));
                        int cond_ob_p=(cond_diag_p && cond_cible_p && cond_cible_j_p);

                        //test pour les diagonales 2
                        int cond_diag_m=(plateau[(int)round(tmpx-0.05)][(int)round(tmpy)].sortbloque==1);
                        int cond_cible_m=(!((int)round(tmpx-0.05)==x && (int)round(tmpy)==y));
                        int cond_cible_j_m=(!((int)round(tmpx-0.05)==tabjoueur[tour].posX && (int)round(tmpy)==tabjoueur[tour].posY));
                        int cond_ob_m=(cond_diag_m && cond_cible_m && cond_cible_j_m);

                        if(plateau[(int)round(tmpx)][(int)round(tmpy)].ptjoueur!=NULL)
                            cond_joueur=(plateau[(int)round(tmpx)][(int)round(tmpy)].ptjoueur->IDteam != tabjoueur[tour].IDteam);

                        if(((cond_obstacle|| cond_joueur) && cond_cible && cond_cible_j ) || cond_ob_p || cond_ob_m )
                        {
                            ///grise la cases si les condition ne sont pas respecter
                            cible[x][y]=-2;
                            break;
                        }
                    }
                }
            }
        }
    }

    ///affichage et choix joueur
    do
    {
        clear_bitmap(backscreen);
        clear_bitmap(buffer);

        if (delay > 0) {
            delay -= 1;
        }


        affichePlateau(backscreen, buffer, plateau, &destX, &destY, Image);

        ///calcule de la zone de dégat si la sourie est a porté du sort
        ///effet de zone
        int taillecible[taillePlateauX][taillePlateauY]={{0}};
        if(cible[destX][destY]==1)
        {
            int borneXmin=(destX-spell.taille>0 ?              destX-spell.taille : 0);
            int borneXmax=(destX+spell.taille<taillePlateauX ? destX+spell.taille : taillePlateauX-1);
            int borneYmin=(destY-spell.taille>0 ?              destY-spell.taille : 0);
            int borneYmax=(destY+spell.taille<taillePlateauY ? destY+spell.taille : taillePlateauY-1);

            int delta;
            for(int x=borneXmin; x <= borneXmax; x++)
            {
                for(int y=borneYmin; y <= borneYmax; y++)
                {
                    delta=abs(destX-x)+abs(destY-y);
                    if(delta<=spell.taille)
                    {
                        taillecible[x][y]=1;
                    }

                }
            }
        }

        ///transmet toute les info dans un tableau et l'envoie dans affiche terrain
        int affcible[taillePlateauX][taillePlateauY]={0};
        for(int y=0; y<taillePlateauY; y++)
        {
            for(int x=0; x<taillePlateauX; x++)
            {
                if(taillecible[x][y]==1) affcible[x][y]=2;
                else affcible[x][y]=cible[x][y];
            }
        }
        afficheTerrain(backscreen, buffer, tabjoueur,plateau, 0, Image, affcible);


        //Affichage souris
        masked_blit(Image.pmenu[92],backscreen,0,0,mouse_x,mouse_y,Image.pmenu[92]->w, Image.pmenu[92]->h);

        blit(backscreen,screen,0,0,0,0,SCREEN_W,SCREEN_H);
        rest(1);

        ///test de fin de tour
        if((clock()-*debut)/CLOCKS_PER_SEC>=timeMaxTour)
        {
            destX=-1;
            destY=-1;
            break;
        }
    }
    while((mouse_b!=1 || delay!=0) && !key[KEY_ESC]);



    ///modification des stats ou annulation
    if(destX!=-1 && cible[destX][destY]==1) /// on annule si le clique est invalide ou en dehors des zone accessible
    {
        double t1=clock();
        //on parcour la zone de dégat
        int borneXmin=(destX-spell.taille>0 ?              destX-spell.taille : 0);
        int borneXmax=(destX+spell.taille<taillePlateauX ? destX+spell.taille : taillePlateauX-1);
        int borneYmin=(destY-spell.taille>0 ?              destY-spell.taille : 0);
        int borneYmax=(destY+spell.taille<taillePlateauY ? destY+spell.taille : taillePlateauY-1);

        int postPA=tabjoueur[tour].PA-spell.PA; //pour évité de deduire reduire 2 fois les PA

        int degatMin, degatMax, degatup;
        float degat[30][3]={{0}};//stock tous les dégat infliger avec leur coordonné (pour les dégat de zone)
        int id_d=0;

        int delta;

        ///sauvegarde les donner du tableau avant les modification pour l'animation
        Cases platprec[taillePlateauX][taillePlateauY];
        for(int x=0; x<taillePlateauX; x++)
        {
            for(int y=0; y<taillePlateauY; y++)
            {
                platprec[x][y].type=plateau[x][y].type;
                platprec[x][y].PVobstacle=plateau[x][y].PVobstacle;
                platprec[x][y].PVMAXobstacle=plateau[x][y].PVMAXobstacle;
                platprec[x][y].posX=plateau[x][y].posX;
                platprec[x][y].posY=plateau[x][y].posY;
                platprec[x][y].blocktype=plateau[x][y].blocktype;
                platprec[x][y].ptjoueur=plateau[x][y].ptjoueur;
            }
        }


        ///modification des stats
        for(int x=borneXmin; x <= borneXmax; x++)
        {
            for(int y=borneYmin; y <= borneYmax; y++)
            {
                delta=abs(destX-x)+abs(destY-y);
                if(delta<=spell.taille)
                {
                    ///tir sur joueur
                    if(plateau[x][y].ptjoueur!=NULL)
                    {
                        ///type 0 : offensif
                        if(spell.type==0 && plateau[x][y].ptjoueur->IDteam != tabjoueur[tour].IDteam)// pas de tir offensif allié
                        {
                            tabjoueur[tour].PA=postPA;

                            if(rand()%100 >= spell.probDegatNul)
                            {
                                ///degats
                                degatMin=spell.degat- (int)(spell.degat*0.2);
                                degatMax=spell.degat+ (int)(spell.degat*0.2)+1;// degat € [degatmin ; degatmax[
                                degatup=(tabjoueur[tour].force>0 ? 20 : 0); //effet force
                                float fractdegat=(plateau[x][y].ptjoueur->def>0 ? 3.0/4.0 : 1.0); //effet resistance
                                degat[id_d][0]=(int)round((rand()%(degatMax-degatMin)+degatMin + degatup)*fractdegat);
                                plateau[x][y].ptjoueur->PV -= degat[id_d][0];
                                degat[id_d][1]=plateau[x][y].posX;
                                degat[id_d][2]=plateau[x][y].posY;
                                id_d++;

                                ///buff
                                if(spell.poison>plateau[x][y].ptjoueur->poison) plateau[x][y].ptjoueur->poison=spell.poison;
                                if(spell.wither>plateau[x][y].ptjoueur->wither) plateau[x][y].ptjoueur->wither=spell.wither;
                                if(spell.lenteur>plateau[x][y].ptjoueur->lenteur) plateau[x][y].ptjoueur->lenteur=spell.lenteur;
                                tabjoueur[tour].PV+=spell.soins; // sort qui soigne l'attaquant

                                if(plateau[x][y].ptjoueur->PV<=0)
                                {
                                    plateau[x][y].ptjoueur->PV=0;
                                    if(plateau[x][y].ptjoueur->IDcadavre<0){
                                        plateau[x][y].ptjoueur->IDcadavre=-plateau[x][y].ptjoueur->IDcadavre;
                                        for(int i=0; i<nbJoueurMax; i++){
                                            if(tabjoueur[i].IDcadavre<0) tabjoueur[i].IDcadavre--;
                                        }
                                    }
                                    plateau[x][y].ptjoueur=NULL;
                                    supp_bot(plateau, tabjoueur);
                                }
                            }
                            else {
                                missed = 1;
                                degat[id_d][0] = 0;
                                degat[id_d][1]=plateau[x][y].posX;
                                degat[id_d][2]=plateau[x][y].posY;
                                id_d++;
                            }
                        }
                        ///type 1 : soutient
                        else if(spell.type==1 && plateau[x][y].ptjoueur->IDteam == tabjoueur[tour].IDteam) //tire de soutient alié uniquement
                        {
                            tabjoueur[tour].PA=postPA;

                            if(rand()%100 >= spell.probDegatNul)
                            {
                                ///buff
                                plateau[x][y].ptjoueur->PV+=spell.soins; // sort qui soigne l'attaquant
                                plateau[x][y].ptjoueur->PM+=spell.PMup;
                                if(spell.force>plateau[x][y].ptjoueur->force) plateau[x][y].ptjoueur->force=spell.force;
                                if(spell.def>plateau[x][y].ptjoueur->def) plateau[x][y].ptjoueur->def=spell.def;
                                if(spell.debuff)
                                {
                                    plateau[x][y].ptjoueur->poison=0;
                                    plateau[x][y].ptjoueur->wither=0;
                                    plateau[x][y].ptjoueur->lenteur=0;
                                }
                            }
                            else {
                                missed = 1;
                            }
                        }
                    }
                    ///type 2 : terrain
                    if(spell.type==2 && plateau[x][y].type==0 && plateau[x][y].ptjoueur==NULL)
                    {
                        tabjoueur[tour].PA=postPA;

                        if(rand()%100 >= spell.probDegatNul)
                        {
                            plateau[x][y].type=spell.Obtype;
                            if(spell.ObPV>0) plateau[x][y].PVobstacle=spell.ObPV;
                            if(spell.ObPV>0) plateau[x][y].PVMAXobstacle=spell.ObPV;
                            if(spell.Obtour>0) plateau[x][y].tourobstacle=spell.Obtour;
                            plateau[x][y].IDcreateur=tour;
                            plateau[x][y].sortbloque=spell.Obbloque;
                        }
                        else {
                            missed = 1;
                        }
                    }
                    ///type 3 : invocation mini spider
                    if(spell.type==3 && plateau[x][y].type==0 && plateau[x][y].ptjoueur==NULL && tabjoueur[tour].nbbots<nbBotsMax)
                    {
                        tabjoueur[tour].PA=postPA;

                        Joueurs* bot=(Joueurs*)malloc(sizeof(Joueurs));
                        bot->PVmax=110;
                        bot->PV=bot->PVmax;
                        bot->PMmax=3;
                        bot->PM=bot->PMmax;
                        bot->IDteam=tabjoueur[tour].IDteam;
                        bot->classe.type=-1;

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

                        tabjoueur[tour].ptbots[tabjoueur[tour].nbbots]=bot;
                        plateau[x][y].ptjoueur=bot;
                        tabjoueur[tour].nbbots+=1;

                        deplacement_auto(backscreen, buffer, tabjoueur[tour].ptbots[tabjoueur[tour].nbbots-1],tabjoueur, plateau, Image);
                    }


                    ///tir sur obstacle
                    if(plateau[x][y].PVobstacle>0 && spell.degat>0)
                    {
                        tabjoueur[tour].PA=postPA;


                        if(rand()%100 >= spell.probDegatNul)
                        {
                            ///degats
                            degatMin=spell.degat- (int)(spell.degat*0.2);
                            degatMax=spell.degat+ (int)(spell.degat*0.2);
                            degatup=(tabjoueur[tour].force>0 ? 20 : 0); //effet force
                            degat[id_d][0]=(rand()%(degatMax-degatMin)+degatMin + degatup);
                            plateau[x][y].PVobstacle -= degat[id_d][0];
                            degat[id_d][1]=plateau[x][y].posX;
                            degat[id_d][2]=plateau[x][y].posY;
                            id_d++;

                            ///transformation si détruit
                            if(plateau[x][y].PVobstacle<=0)
                            {
                                plateau[x][y].type=0;
                                plateau[x][y].PVobstacle=-1;
                                plateau[x][y].PVMAXobstacle=-1;
                                plateau[x][y].tourobstacle=-1;
                                plateau[x][y].IDcreateur=-1;
                                plateau[x][y].sortbloque=0;
                            }
                        }
                        else {
                            missed = 1;
                            degat[id_d][0]=0;
                            degat[id_d][1]=plateau[x][y].posX;
                            degat[id_d][2]=plateau[x][y].posY;
                            id_d++;
                        }
                    }
                }
            }//fin for
        }//fin for


        ///si le sort touche on lance l'animation
        if(tabjoueur[tour].PA==postPA)
        {
            degat[id_d][0]=-1;
            ///on tourne le joueur dans le sens du sort
            int fractx=(plateau[destX][destY].posX-plateau[tabjoueur[tour].posX][tabjoueur[tour].posY].posX);
            int fracty=(plateau[destX][destY].posY-plateau[tabjoueur[tour].posX][tabjoueur[tour].posY].posY);

            if(fractx<0 && fracty<0) tabjoueur[tour].sens=1; //haut gauche
            if(fractx>0 && fracty<0) tabjoueur[tour].sens=2; //haut droite
            if(fractx<0 && fracty>0) tabjoueur[tour].sens=3; //bas  gauche
            if(fractx>0 && fracty>0) tabjoueur[tour].sens=4; //bas  droite


            //animation
            int jx=plateau[tabjoueur[tour].posX][tabjoueur[tour].posY].posX;
            int jy=plateau[tabjoueur[tour].posX][tabjoueur[tour].posY].posY;
            sortAnimation(backscreen, buffer, platprec, tabjoueur, jx, jy, plateau[destX][destY].posX, plateau[destX][destY].posY, spell,degat,missed, Image);
        }
        ///on retire le temps de l'animation sur le temps de jeu du joueur
        double t2=clock();
        *debut+=t2-t1;
    }//fin if
}

void rotationBITMAP(BITMAP* source, BITMAP* dest, double angle)
{
    /// fonction pour tourner une bitmap sur elle même
    /// IN  : bitmap a tourner ; bitmap résultat ; angle de rotation
    /// OUT : bitmap résultat par adresse

    ///on la transforme en bitmap carré
    //on calcul le coté le plus grand
    int cote=(source->h>=source->w ? source->h : source->w);
    //creation bitmap carre magenta
    BITMAP* carre=create_bitmap(cote, cote);
    rectfill(carre, 0,0, cote,cote, makecol(255,0,255));
    //on colle l'image au milieu
    int w=0, h=0;
    if(cote==source->w) h=(cote-source->h)/2;
    else               w=(cote-source->w)/2;
    blit(source, carre, 0, 0, w, h, source->w, source->h);


    ///centre
    int Cx=carre->w/2;
    int Cy=carre->h/2;

    double mod, arg;
    int col;
    int xget,yget;
    for(int x=0; x<carre->w; x++)
    {
        for(int y=0; y<carre->h; y++)
        {
            mod=sqrt(pow(x-Cx, 2)+pow(y-Cy, 2));     //module
            arg=atan((double)(Cy-y)/(double)(Cx-x)); //argument
            if(Cx-x>=0) arg=pi+arg;

            ///calcul coordonné selecteur de couleur
            xget=Cx+round(mod*cos(arg+angle));
            yget=Cy+round(mod*sin(arg+angle));

            ///gestions des bordures
            if(xget>0 && xget <carre->w && yget>0 && yget <carre->h)
                col=getpixel(carre, xget, yget);
            else col=makecol(255,0,255);

            putpixel(dest,x,y,col);
        }
    }
}

void sortAnimation_projectile(BITMAP* backscreen, BITMAP* buffer, Cases plateau[taillePlateauX][taillePlateauY], Joueurs tabjoueur[], int x1, int y1, int x2, int y2, Sorts spell, BITMAP* image,pics Image) {

    ///Animation des sorts lançants des projectiles (flèche, boule de feu,..)
    ///Pour cela, on utilise la fonction "rotationBITMAP" pour tourner le projectile dans le bon sens, puis on calcule la trajectoire qu'il
    ///doit effectuer pour aller jusqu'à la cible

    //on creer une bitmap resultat
    int cote=(image->h>=image->w ? image->h : image->w);
    BITMAP* IMGrot=create_bitmap(cote, cote);
    rectfill(IMGrot, 0,0, cote,cote, makecol(255,0,255));

    double mod=sqrt(pow(x2-x1, 2)+pow(y2-y1, 2));     //module
    double arg= -atan((double)(y2-y1)/(double)(x2-x1)); //argument
    if(x2-x1<0) arg=pi+arg;
    rotationBITMAP(image, IMGrot, arg);

    float fps=mod /2;
    float tmpx=x1;
    float tmpy=y1;
    float fractx=(x2-x1)/fps;
    float fracty=(y2-y1)/fps;


    for(int i=0; i<fps; i++)
    {
        clear_bitmap(backscreen);
        clear_bitmap(buffer);

        tmpx+=fractx;
        tmpy+=fracty;

        float h=mod/5*sin(i/fps*pi);

        int a,b;
        affichePlateau(backscreen, buffer, plateau, &a, &b,Image);
        int ciblenul[taillePlateauX][taillePlateauY]={{0}};
        afficheTerrain(backscreen, buffer, tabjoueur,plateau,0,Image, ciblenul);

        masked_blit(IMGrot, backscreen, 0,0, tmpx-image->w/2,tmpy-image->h/2 - h - 30, cote, cote);

        //Affichage souris
        masked_blit(Image.pmenu[92],backscreen,0,0,mouse_x,mouse_y,Image.pmenu[92]->w, Image.pmenu[92]->h);

        blit(backscreen,screen,0,0,0,0,SCREEN_W,SCREEN_H);
    }
    destroy_bitmap(IMGrot);
}

void sortAnimation_hit(BITMAP* backscreen, BITMAP* buffer, Cases plateau[taillePlateauX][taillePlateauY], Joueurs tabjoueur[], Sorts spell,int x2, float y2, float degat[30][3],pics Image) {

    ///Animation des sorts causant des dégâts (affiche des particules de dégâts et le nombre de PV enlevé à la cible)

    int d=0;
    while(degat[d][0]!=-1){
        d++;
    }

    int nbparticule = 8;    //nombre de particule de dégât
    float tabx[d][nbparticule];
    float taby[d][nbparticule];
    for(int j=0; j<d; j++){
        for(int i=0; i<nbparticule; i++) {
            tabx[j][i] = degat[j][1]+rand()%(86)-45;
            taby[j][i] = degat[j][2]+rand()%(81)-80;
        }
    }
    float speed = 2;
    int red = 100;
    for(int i=0; i<25; i++)
    {
        clear_bitmap(backscreen);
        clear_bitmap(buffer);

        int a,b;
        affichePlateau(backscreen, buffer, plateau, &a, &b,Image);
        int ciblenul[taillePlateauX][taillePlateauY]={{0}};
        afficheTerrain(backscreen, buffer, tabjoueur,plateau,0,Image, ciblenul);

        for(int j=0; j<d; j++){
            if (degat[j][0] > 0) {
                for(int i=0; i<nbparticule; i++) {
                    masked_blit(Image.panime[0], backscreen, 0,0, tabx[j][i],taby[j][i],Image.panime[0]->w, Image.panime[0]->h);
                    taby[j][i] += 0.4;
                }

                textprintf_ex(backscreen,font,degat[j][1]-20,degat[j][2]-69,colblack,-1,"-%d PV",(int)degat[j][0]);
                textprintf_ex(backscreen,font,degat[j][1]-20,degat[j][2]-70,makecol(red,0,0),-1,"-%d PV",(int)degat[j][0]);
            }
            else {
                textprintf_ex(backscreen,font,degat[j][1]-20,degat[j][2]-69,colblack,-1,"* Miss *");
                textprintf_ex(backscreen,font,degat[j][1]-20,degat[j][2]-70,makecol(50+red,50+red,50+red),-1,"* Miss *");
            }
            if (i<15) {
                degat[j][2] -= speed;
                speed -= 0.2;
                red += 5;
            }
        }

        //Affichage souris
        masked_blit(Image.pmenu[92],backscreen,0,0,mouse_x,mouse_y,Image.pmenu[92]->w, Image.pmenu[92]->h);
        blit(backscreen,screen,0,0,0,0,SCREEN_W,SCREEN_H);
        rest(1);
    }
}

void sortAnimation_explosion(BITMAP* backscreen, BITMAP* buffer, Cases plateau[taillePlateauX][taillePlateauY], Joueurs tabjoueur[],int x2, float y2,Sorts spell,float degat[30][3],int nbparticule,pics Image) {

    ///Animation pour les sorts qui font des dégâts de zones, affichage de particules d'explosion

    if (degat[0][0] > 0) {
            float tabx[nbparticule];
            float taby[nbparticule];
            int nb = (nbparticule*6)+1;
            int nb2 = (nbparticule*4);
            for(int i=0; i<nbparticule; i++) {
                tabx[i] = x2+rand()%(nb)-(nb2);
                taby[i] = y2+rand()%(nb)-(nb2);
            }
            for(int i=0; i<15; i++) {
                clear_bitmap(backscreen);
                clear_bitmap(buffer);
                int a,b;
                affichePlateau(backscreen, buffer, plateau, &a, &b,Image);
                int ciblenul[taillePlateauX][taillePlateauY]={{0}};
                afficheTerrain(backscreen, buffer, tabjoueur,plateau,0,Image, ciblenul);

                if (i > 5 && i < 11) {
                    for(int j=0; j<nbparticule; j++) {
                        masked_blit(Image.panime[20], backscreen, 0,0, tabx[j],taby[j],Image.panime[20]->w, Image.panime[20]->h);
                    }
                }
                else if (i > 10) {
                    for(int j=0; j<nbparticule; j++) {
                        masked_blit(Image.panime[21], backscreen, 0,0, tabx[j],taby[j],Image.panime[21]->w, Image.panime[21]->h);
                    }
                }
                else {
                    for(int j=0; j<nbparticule; j++) {
                        masked_blit(Image.panime[19], backscreen, 0,0, tabx[j],taby[j],Image.panime[19]->w, Image.panime[19]->h);
                    }
                }
                //Affichage souris
                masked_blit(Image.pmenu[92],backscreen,0,0,mouse_x,mouse_y,Image.pmenu[92]->w, Image.pmenu[92]->h);
                blit(backscreen,screen,0,0,0,0,SCREEN_W,SCREEN_H);
                rest(1);
            }
    }
}

void sortAnimation_effect(BITMAP* backscreen, BITMAP* buffer, Cases plateau[taillePlateauX][taillePlateauY], Joueurs tabjoueur[],int x2, float y2, Sorts spell, int missed,int effect,pics Image) {

    ///Animation pour les sorts appliquant des effects, affiche des particules, la couleur des particules est determiné par la variable "effect"
    ///6 particules possibles : Healh(Soins), Slowness(Lenteur), Poison, Strength(Force), Resistance, Wither

    for(int x=0; x<taillePlateauX; x++){
        for(int y=0; y<taillePlateauY; y++){
            if(plateau[x][y].posX==x2 && plateau[x][y].posY==y2 && plateau[x][y].ptjoueur==NULL)
                effect=-2;
        }
    }

    if (missed == 0 && effect > -1) {
            //effet 0: healh / 2:slow / 1:poison / 5:force / 3:def / 4:wither
            int nbparticule = 20;
            float tabx[nbparticule];
            float taby[nbparticule];
            int nb = (nbparticule*4)+1;
            int nb2 = (nbparticule*2);
            float speed = 4;
            for(int i=0; i<nbparticule; i++) {
                tabx[i] = x2+rand()%(nb)-(nb2);
                taby[i] = y2+rand()%(nb)-(nb2);
            }
            for(int i=0; i<40; i++) {
                clear_bitmap(backscreen);
                clear_bitmap(buffer);
                int a,b,im;
                affichePlateau(backscreen, buffer, plateau, &a, &b,Image);
                int ciblenul[taillePlateauX][taillePlateauY]={{0}};
                afficheTerrain(backscreen, buffer, tabjoueur,plateau,0,Image, ciblenul);
                im = (effect*2)+1;
                if ((i>10 && i<21) || (i>30 && i<41)) {
                    im++;
                }
                for(int j=0; j<nbparticule; j++) {
                    masked_blit(Image.panime[im], backscreen, 0,0, tabx[j],taby[j],Image.panime[im]->w, Image.panime[im]->h);
                    if (tabx[j] > x2) {
                        tabx[j] += 0.4;
                    }
                    if (tabx[j] < x2) {
                        tabx[j] -= 0.4;
                    }
                    taby[j] -= speed;
                }
                speed -= 0.2;
                //Affichage souris
                masked_blit(Image.pmenu[92],backscreen,0,0,mouse_x,mouse_y,Image.pmenu[92]->w, Image.pmenu[92]->h);
                blit(backscreen,screen,0,0,0,0,SCREEN_W,SCREEN_H);
                rest(1);
            }
    }
    else if (missed != 0) {
        for(int i=0; i<30; i++) {
            clear_bitmap(backscreen);
            clear_bitmap(buffer);
            int a,b;
            affichePlateau(backscreen, buffer, plateau, &a, &b,Image);
            int ciblenul[taillePlateauX][taillePlateauY]={{0}};
            afficheTerrain(backscreen, buffer, tabjoueur,plateau,0,Image, ciblenul);
            textprintf_ex(backscreen,font,x2-20,y2-69,colblack,-1,"* Miss *");
            textprintf_ex(backscreen,font,x2-20,y2-70,colwhite,-1,"* Miss *");
            //Affichage souris
            masked_blit(Image.pmenu[92],backscreen,0,0,mouse_x,mouse_y,Image.pmenu[92]->w, Image.pmenu[92]->h);
            blit(backscreen,screen,0,0,0,0,SCREEN_W,SCREEN_H);
            rest(1);
        }
    }
}

void sortAnimation(BITMAP* backscreen, BITMAP* buffer, Cases plateau[taillePlateauX][taillePlateauY], Joueurs tabjoueur[], int x1, int y1, int x2, int y2, Sorts spell,int degat[30][3],int missed,pics Image)
{
    ///Fonction que gère les animations de chacun des sorts individuellement en appelant les autres sous-programmes sortAnimation

    ///Ex: l'animation de la flèche de glace utilise la fonction "sortAnimation_projectile" pour la flèche, puis "sortAnimation_hit" pour
    ///montrer les dégâts causé par l'attaque, et enfin "sortAnimation_effect" pour les particules de glace/lenteur indiquant l'effet ajouté

    if (spell.typesort == 0) {  // CAC
        sortAnimation_hit(backscreen,buffer, plateau, tabjoueur,spell,x2,y2,degat,Image);
    }                              // ZOMBIE
    if (spell.typesort == 1) {      ///contamination
        sortAnimation_hit(backscreen,buffer, plateau, tabjoueur,spell,x2,y2,degat,Image);
        int effect=0;
        for(int x=0; x<taillePlateauX; x++){    //N'affiche pas de particule de soins si la cible est un bloc
            for(int y=0; y<taillePlateauY; y++){
                if(plateau[x][y].posX==x2 && plateau[x][y].posY==y2 && plateau[x][y].ptjoueur==NULL)
                    effect=-2;
            }
        }
        sortAnimation_effect(backscreen, buffer, plateau, tabjoueur, x1, y1, spell, missed,effect,Image);
    }
    else if (spell.typesort == 2) { ///craft (force)
        sortAnimation_effect(backscreen, buffer, plateau, tabjoueur, x2, y2, spell, missed,5,Image);
    }
    else if (spell.typesort == 3) { ///healh poison
        sortAnimation_effect(backscreen, buffer, plateau, tabjoueur, x2, y2, spell, missed,0,Image);
    }
    else if (spell.typesort == 4) { ///trident
        sortAnimation_projectile(backscreen, buffer, plateau, tabjoueur, x1, y1, x2, y2, spell, Image.panime[16],Image);
        sortAnimation_explosion(backscreen, buffer, plateau, tabjoueur, x2, y2, spell,degat, 10,Image);
        sortAnimation_hit(backscreen,buffer, plateau, tabjoueur,spell,x2,y2,degat,Image);
        sortAnimation_effect(backscreen, buffer, plateau, tabjoueur, x2, y2, spell, missed,2,Image);
    }
    else if (spell.typesort == 5) {  ///arrow
        sortAnimation_projectile(backscreen, buffer, plateau, tabjoueur, x1, y1, x2, y2, spell, Image.panime[13],Image);
        sortAnimation_hit(backscreen,buffer, plateau, tabjoueur,spell,x2,y2,degat,Image);
    }
    else if (spell.typesort == 6) { ///ice arrow
        sortAnimation_projectile(backscreen, buffer, plateau, tabjoueur, x1, y1, x2, y2, spell, Image.panime[14],Image);
        sortAnimation_hit(backscreen,buffer, plateau, tabjoueur,spell,x2,y2,degat,Image);
        sortAnimation_effect(backscreen, buffer, plateau, tabjoueur, x2, y2, spell, missed,2,Image);
    }
    else if (spell.typesort == 7) { ///wither
        sortAnimation_hit(backscreen,buffer, plateau, tabjoueur,spell,x2,y2,degat,Image);
        sortAnimation_effect(backscreen, buffer, plateau, tabjoueur, x2, y2, spell, missed,4,Image);
    }
    else if (spell.typesort == 8) { ///Armure (def)
        sortAnimation_effect(backscreen, buffer, plateau, tabjoueur, x1, y1, spell, missed,3,Image);
    }
    else if (spell.typesort == 9) {  /// Spider poison
        sortAnimation_hit(backscreen,buffer, plateau, tabjoueur,spell,x2,y2,degat,Image);
        sortAnimation_effect(backscreen, buffer, plateau, tabjoueur, x2, y2, spell, missed,1,Image);
    }
    else if (spell.typesort == 10) {  /// cobweb
        sortAnimation_projectile(backscreen, buffer, plateau, tabjoueur, x1, y1, x2, y2, spell, Image.panime[23],Image);
        sortAnimation_effect(backscreen, buffer, plateau, tabjoueur, x2, y2, spell, missed,-1,Image);
    }
    else if (spell.typesort == 12) {
        sortAnimation_effect(backscreen, buffer, plateau, tabjoueur, x2, y2, spell, missed,0,Image);
    }                                   // BLAZE
    else if (spell.typesort == 13) {  ///fireball
        sortAnimation_projectile(backscreen, buffer, plateau, tabjoueur, x1, y1, x2, y2, spell, Image.panime[17],Image);
        sortAnimation_hit(backscreen,buffer, plateau, tabjoueur,spell,x2,y2,degat,Image);
    }
    else if (spell.typesort == 14) { ///firewall
        sortAnimation_projectile(backscreen, buffer, plateau, tabjoueur, x1, y1, x2, y2, spell, Image.panime[17],Image);
        sortAnimation_effect(backscreen, buffer, plateau, tabjoueur, x2, y2, spell, missed,-1,Image);
    }
    else if (spell.typesort == 15) {  ///eruption
        sortAnimation_projectile(backscreen, buffer, plateau, tabjoueur, x1, y1, x2, y2, spell, Image.panime[18],Image);
        sortAnimation_explosion(backscreen, buffer, plateau, tabjoueur, x2, y2, spell,degat, 40,Image);
        sortAnimation_hit(backscreen,buffer, plateau, tabjoueur,spell,x2,y2,degat,Image);
    }
    else if (spell.typesort == 16) {  ///Alchimie
        sortAnimation_effect(backscreen, buffer, plateau, tabjoueur, x2, y2, spell, missed,5,Image);
        sortAnimation_effect(backscreen, buffer, plateau, tabjoueur, x2, y2, spell, missed,3,Image);
    }                                    // PILLAGER
    else if (spell.typesort == 17) { ///arrow arbalete
        sortAnimation_projectile(backscreen, buffer, plateau, tabjoueur, x1, y1, x2, y2, spell, Image.panime[13],Image);
        sortAnimation_hit(backscreen,buffer, plateau, tabjoueur,spell,x2,y2,degat,Image);
    }
    else if (spell.typesort == 18) {    ///Bloc log
        sortAnimation_projectile(backscreen, buffer, plateau, tabjoueur, x1, y1, x2, y2, spell, Image.panime[22],Image);
        sortAnimation_effect(backscreen, buffer, plateau, tabjoueur, x2, y2, spell, missed,-1,Image);
    }
    else if (spell.typesort == 19) {    ///spectral arrow
        sortAnimation_projectile(backscreen, buffer, plateau, tabjoueur, x1, y1, x2, y2, spell, Image.panime[15],Image);
        sortAnimation_hit(backscreen,buffer, plateau, tabjoueur,spell,x2,y2,degat,Image);
    }
    else if (spell.typesort == 20) {    ///Totem
        sortAnimation_effect(backscreen, buffer, plateau, tabjoueur, x2, y2, spell, missed,0,Image);
        sortAnimation_effect(backscreen, buffer, plateau, tabjoueur, x1, y1, spell, missed,3,Image);
    }
}
