#include <stdio.h>

#include "saisie.h"
#include "alertes.h"
#include "affichage.h"
#include "capteur.h"
#include "menu_actions.h"

static action_t actions[6] = {
    action_case_0,
    action_case_1,
    action_case_2,
    action_case_3,
    action_case_4,
    action_case_5
};

int main(void)
{
    int choix;
    do
    {

        afficher_menu();
        scanf("%d", &choix);
        printf("\n");
        
        if (choix >= 0 && choix <= 5) actions[choix]();
        else printf("ERREUR !! La valeur doit etre entre 0 et 6\n");

        printf("\n");
    } while(choix != 0);

    return 0;
}
