#include "releves.h"
#include <stdio.h>

void saisir_releves(float * tab)
{
    float temp_user;
    int nb_releves = 0;

    while((nb_releves < 1) || (nb_releves > 24))
    {
        printf("Nombre de releves (24 max) ?\n");
        scanf("%d", &nb_releves);
    }

    for(int i = 0; i < nb_releves; i++)
    {
        printf("Heure %2d : ", i);
        scanf("%f", &temp_user);

        while((temp_user < -50.0) || (temp_user > 60.0))
        {
            printf("ERREUR : valeur hors plage [-50.0 ; 60.0] ! Veuillez resaisir : \n");

            scanf("%f", &temp_user);
        }

        tab[i] = temp_user;
    }

    printf("--- Releves enregistres---\n");
    for(int i = 0; i < nb_releves; i++)
    {
        printf("Heure %2d : %.2f C\n", i, tab[i]);
    }
}