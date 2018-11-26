#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Auto
{
    char kategoria[50], znacka[50], predajca[100], stav_vozidla[200];
    int id, cena, rok_vyroby;
    struct Auto *child;
} AUTO;

void nacitajSubor(AUTO **list)
{
    FILE *subor = fopen("auta.txt", "r"); // Nacitame si subor

    if(subor == NULL)
    {
        printf("Zaznamy neboli nacitane\n");
        return;
    }

    int size = 0; // pocet aut
    int s = 0; // pomocna premenna
    while((s = fgetc(subor)) != EOF) // Ak nie je koniec suboru
    {
        if(s == '$')   // Ak sme znak "$"
            size++;       // pripocitame ku poctu aut
    }

    fclose(subor); // Zavrieme subor
    subor = fopen("auta.txt", "r"); // Znova ho nacitame => pointer mame teraz znova na zaciatku suboru.

    for(int i = 0; i < size; i++) // Pre pocet vodicov
    {
        AUTO *a = malloc(sizeof(AUTO));

        // Nacitanie dat zo suboru do premennych
        fscanf(subor, "$\n%[^\n]\n%[^\n]\n%[^\n]\n%d\n%d\n%[^\n]\n",
               a->kategoria, a->znacka, a->predajca, &(a->cena), &(a->rok_vyroby), a->stav_vozidla);

        a->id = i+1;
        a->child = NULL;

        // Priradime do zoznamu
        if(*list == NULL)
        {
            *list = a;
            continue;
        }

        AUTO *parrent = *list;
        while(parrent->child != NULL)
        {
            parrent = parrent->child;
        }

        parrent->child = a;
    }

    fclose(subor); // Zavrieme subor

    printf("Nacitalo sa %d zaznamov\n", size);
}

void vypisAut(AUTO *list)
{
    if(list == NULL)
    {
        printf("NULL\n");
        return;
    }

    AUTO *a = list;

    while(a != NULL)
    {
        printf("%d.\nkategoria: %s\nznacka: %s\npredajca: %s\ncena: %d\nrok_vyroby: %d\nstav_vozidla: %s\n",
                a->id, a->kategoria, a->znacka, a->predajca, a->cena, a->rok_vyroby, a->stav_vozidla);

        a = a->child;
    }
}

int main()
{
    int exit = 1;
    char prikaz;
    AUTO *list = NULL;

    do // Zaciatok loopu
    {
        scanf("%1c", &prikaz);

        switch(prikaz) // Deli prikazy
            // nie je cas hladat lepsi sposob
        {
            case 'n': nacitajSubor(&list);break; // nacitanie suboru
            case 'v': vypisAut(list); break; // vypis suboru
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