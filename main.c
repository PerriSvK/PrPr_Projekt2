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

        a->id = i+1; // priradime mu id (cislujeme od nuly)
        a->child = NULL;

        // Priradime do zoznamu
        if(*list == NULL) // Ak este nemame prvy prvok, tak ho nastavime
        {
            *list = a;
            continue;
        }

        AUTO *parrent = *list;
        while(parrent->child != NULL) // Ak nas aktualny prvok uz ma v sebe dalsi
        {
            parrent = parrent->child; // Tak sa posunieme
        }

        parrent->child = a; // Ak nie, priradime mu dieta
    }

    fclose(subor); // Zavrieme subor

    printf("Nacitalo sa %d zaznamov\n", size);
}

void vypisAut(AUTO *list, int keepId)
{
    if(list == NULL) // Nechceme null pointer
    {
        printf("NULL\n");
        return;
    }

    int id = 1; // Vlastne cislovanie zoznamu -> funkcia je pouzita aj inde ako vo "v"

    AUTO *a = list;

    while(a != NULL) // Ak existuje aktualny prvok
    {
        printf("%d.\nkategoria: %s\nznacka: %s\npredajca: %s\ncena: %d\nrok_vyroby: %d\nstav_vozidla: %s\n",
               keepId ? a->id : id++, a->kategoria, a->znacka, a->predajca, a->cena, a->rok_vyroby, a->stav_vozidla);

        // Tak ho vypiseme a ideme dalej
        a = a->child;
    }
}

void hladajAuto(AUTO *list)
{
    char znacka[50], male[50];
    int cena;

    scanf(" %[^\n]\n", znacka);
    scanf("%d", &cena);

    for(int i = 0; znacka[i]; i++)
    {
        znacka[i] = (char) (znacka[i] <= 'Z' ? znacka[i] - 'A' + 'a' : znacka[i]);
    }

    AUTO *v = NULL; // Vysledny zoznam
    AUTO *a = list; // Aktualne auto

    while(a != NULL) // Ak aktualne auto existuje
    {
        // Zmenime si vsetko na male pismena
        int vel = 0;
        for(vel = 0; a->znacka[vel]; vel++)
        {
            male[vel] = (char) (a->znacka[vel] <= 'Z' ? (a->znacka[vel] - 'A' + 'a') : a->znacka[vel]);
        }
        male[vel] = '\0'; // Ukoncime string

        if(strcmp(male, znacka) == 0 && a->cena <= cena) // Ak sedi znacka aj cena
        {
            // Mame zhodu pridavame
            if(v == NULL) // Ak vo vysledku nic nie je
            {
                AUTO cp = *a; // Vytvorime kopiu auta
                v = &cp; // Nastavime pointer vysledku
                v->child = NULL; // Vynulujeme nepravdivy dalsi prvok
            }
            else
            {
                AUTO *t = v; // Testovacia premenna
                while(t->child != NULL) // Ak existuje
                {
                    t = t->child; // posunieme sa dalej
                }

                AUTO cp = *a; // Vytvorime si kopiu
                t->child = &cp; // Pridame ju na koniec vysledku
                t->child->child = NULL; // Vynulujeme dalsi prvok
            }
        }

        a = a->child; // Posunieme sa dalej
    }

    if(v == NULL)
    {
        printf("V ponuke nie su pozadovane auta.\n");
        return;
    }

    vypisAut(v, 0);
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
            case 'v': vypisAut(list, 1); break; // vypis suboru
            case 'h': hladajAuto(list); break; // TODO vyhladavanie
            case 'p': break; // TODO pridavanie
            case 'z': break; // TODO mazanie
            case 'a': break; // TODO aktualizacia zaznamu
            case 'k':  exit = 0; break; // TODO uvolnenie pamate
        }
    }
    while(exit); // Koniec loopu -- Opakuje sa okial nie je zadane "k"

    return 0;
}