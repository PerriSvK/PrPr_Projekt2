#include <stdio.h>

int main()
{
    int exit = 1;
    char prikaz;

    do
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
    while(exit);

    return 0;
}