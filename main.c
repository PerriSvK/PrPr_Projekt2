#include <stdio.h>

typedef struct Auto
{
    char kategoria[50], znacka[50], predajca[100], stav_vozidla[200];
    int id, cena, rok_vyroby;
    struct Auto *child;
} AUTO;

int main()
{
    int exit = 1;
    char prikaz;

    do // Zaciatok loopu
    {
        scanf("%1c", &prikaz);

        switch(prikaz) // Deli prikazy
            // nie je cas hladat lepsi sposob
        {
            case 'n': break; // TODO nacitanie suboru
            case 'v': break; // TODO vypis suboru
            case 'h': break; // TODO vyhladavanie
            case 'p': break; // TODO pridavanie
            case 'z': break; // TODO mazanie
            case 'a': break; // TODO aktualizacia zaznamu
            case 'k':  exit = 0; break; // TODO uvolnenie pamate
        }
    }
    while(exit); // Koniec loopu -- Opakuje sa okial nie je zadane "k"

    return 0;
}