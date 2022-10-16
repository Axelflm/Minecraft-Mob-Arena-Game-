#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include <time.h>
#include <math.h>
#include "head.h"

void initialisation_allegro() {    ///Initialisation pour Allegro
    allegro_init();
    install_keyboard();
    install_mouse();
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);

    set_color_depth(desktop_color_depth());
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED,800,600,0,0)!=0) {
        allegro_message("prb gfx mode");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
}


int main()
{
    srand(time(NULL));

    initialisation_allegro();         //Initialisation d'allegro et des buffers
    BITMAP *backscreen;
    backscreen = create_bitmap(SCREEN_W,SCREEN_H);
    BITMAP *buffer;
    buffer = create_bitmap(SCREEN_W,SCREEN_H);

    ///importation de la musique de fond
    SAMPLE* music=load_sample("pics/musique.wav");
    int volume=10; //40=max
    if(music==NULL){
        printf("Erreur le dossier << musique >> (format .wav) n'a pas ete trouvee");
        volume = 0;
    }

    ///Init

    int menu = 1;   //Menu = 1 : menu principale, Menu = 0 : quitte le programme
    Joueurs tabjoueur[nbJoueurMax+1]; //+1 : pour stocker bots naturel comme si il apartenait a un joueur
    tabjoueur[nbJoueurMax].classe.type=-1;
    Cases plateau[taillePlateauX][taillePlateauY];
    pics Image = InitialisationImages(Image);
    leaf l[1];
    l[0].delay =0;
    for (int i=0;i<NbLeafMax;i++) {
        l[0].tabx[i] = -1;
        l[0].taby[i] = -1;
        l[0].speed[i] = 1;
    }

    ///Go au Menu principale
    int nbJoueur;
    int choixterrain;
    int restart = 0;
    while (menu == 1) { //tant que le joueur n'a pas cliquer sur "quitter" ou fait echap, on le renvoie au menu principale, sinon on quitte le programme
        menu = MenuPrincipale(backscreen,buffer,tabjoueur,plateau,Image,l,&restart,&nbJoueur,&choixterrain, music, &volume);
    }

    destroy_sample(music);
    return 0;
} END_OF_MAIN() ;
