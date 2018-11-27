#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

typedef struct Auto
{
    char kategoria[50], znacka[50], predajca[100], stav_vozidla[200];
    int cena, rok_vyroby;
    struct Auto *child;
} AUTO;

void uvolniPamat(AUTO **a)
{
    if((*a)->child != NULL)
    {
        uvolniPamat(&(*a)->child);
    }

    free(*a);
    *a = NULL;
}

void nacitajSubor(AUTO **list)
{
    uvolniPamat(list);

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
        AUTO *a = (AUTO *) malloc(sizeof(AUTO));

        // Nacitanie dat zo suboru do premennych
        fscanf(subor, "$\n%[^\n]\n%[^\n]\n%[^\n]\n%d\n%d\n%[^\n]\n",
               a->kategoria, a->znacka, a->predajca, &(a->cena), &(a->rok_vyroby), a->stav_vozidla);

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

void vypisAut(AUTO *list)
{
    if(list == NULL) // Nechceme null pointer
    {
        printf("NULL\n");
        return;
    }

    int id = 1; // Cislovanie zoznamu

    AUTO *a = list;

    while(a != NULL) // Ak existuje aktualny prvok
    {
        printf("%d.\nkategoria: %s\nznacka: %s\npredajca: %s\ncena: %d\nrok_vyroby: %d\nstav_vozidla: %s\n",
                id++, a->kategoria, a->znacka, a->predajca, a->cena, a->rok_vyroby, a->stav_vozidla);

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
                v->child = NULL;
            }
            else
            {
                AUTO *t = v; // Testovacia premenna
                while(t->child != NULL) // Ak existuje
                {
                    t = t->child; // posunieme sa dalej
                }

                AUTO cp = *a; // Vytvorime kopiu auta
                t->child = &cp; // Pridame ju na koniec vysledku
                t->child->child = NULL;
            }
        }

        a = a->child; // Posunieme sa dalej
    }

    if(v == NULL)
    {
        printf("V ponuke nie su pozadovane auta.\n");
        return;
    }

    vypisAut(v);
}

void pridajAuto(AUTO **list)
{
    AUTO *nove = (AUTO *) malloc(sizeof(AUTO));
    int poradie;
    scanf(" %d\n", &poradie); // Nacitanie udajov
    scanf("%[^\n]\n", nove->kategoria);
    scanf("%[^\n]\n", nove->znacka);
    scanf("%[^\n]\n", nove->predajca);
    scanf("%d\n", &(nove->cena));
    scanf("%d\n", &(nove->rok_vyroby));
    scanf("%[^\n]\n", nove->stav_vozidla);

    if(*list == NULL || poradie <= 1) // Ak mam prazdny zoznam alebo auto ide na zaciatok
    {
        nove->child = *list; // Tak mu nastavim "chvost"
        *list = nove; // A dam ho na zaciatok
        return;
    }

    AUTO *a = *list;
    int depth = 2;
    while( a->child != NULL && depth < poradie) // Kontrolujem, ci nie som na konci alebo ci nie som dost hlboko
    {
        a = a->child; // Ak nie, pokracujem
        depth++;
    }

    nove->child = a->child; // Nastavim "chvost"
    a->child = nove; // A priradim na danu poziciu
}

void vymazAuto(AUTO **list)
{
    char znacka[50];
    scanf(" %[^\n]", znacka); // Nacitanie

    // Na velke pismena
    for(int i = 0; znacka[i]; i++)
    {
        znacka[i] = (char) (znacka[i] >= 'a' ?  znacka[i] -'a'+'A' : znacka[i]);
    }

    int pocet = 0;

    AUTO *last = NULL; // Posledny "zijuci" zaznam
    AUTO *a = *list;
    while(a != NULL)
    {
        char velke[50];
        strcpy(velke, a->znacka);

        // Znacka na velke pismena
        for(int i = 0; velke[i]; i++)
        {
            velke[i] = (char) (velke[i] >= 'a' ?  velke[i] -'a'+'A' : velke[i]);
        }

        if(strstr(velke, znacka) == NULL) // Ak znacka neobsahuje zadanu cast
        {
            if(last != NULL) // Ak uz mame zaznam, tak aktualne auto napiseme na koniec
            {
                last->child = a; // Tak ho nastavime
            }

            last = a; // Novy poslendy "zijuci" prvok
        }
        else
        {
            pocet++; // Inak mazeme
        }
        a = a->child;
    }

    printf("Vymazalo sa %d zaznamov\n", pocet);
}

void aktualizujAuto(AUTO *list)
{
    char znacka[50];
    scanf(" %[^\n]\n", znacka);
    int rok, c = 0;
    scanf(" %d\n", &rok);

    // Vsetko na velke pismena
    for(int i = 0; znacka[i]; i++)
    {
        znacka[i] = (char) (znacka[i] >= 'a' ? znacka[i] - 'a' + 'A' : znacka[i]);
    }

    AUTO *a = list;
    while(a != NULL)
    {
        char velke[50];
        int vel;
        // Vsetko na velke pismena
        for(vel = 0; a->znacka[vel]; vel++)
        {
            velke[vel] = (char) (velke[vel] >= 'a' ? velke[vel] - 'a' + 'A' : velke[vel]);
        }
        velke[vel] = '\0';

        if(strcmp(velke, znacka) == 0 && rok == a->rok_vyroby) // Ak znacka aj rok sedi
        {
            a->cena = max(a->cena-100, 0); // Tak znizime cenu o 100, minimalna mozna hodnota je 0
            c++;
        }

        a = a->child;
    }

    printf("Aktualizovalo sa %d zaznamov\n", c);
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
            case 'h': hladajAuto(list); break; // vyhladavanie
            case 'p': pridajAuto(&list); break; // pridavanie
            case 'z': vymazAuto(&list); break; // mazanie
            case 'a': aktualizujAuto(list); break; // aktualizacia zaznamu
            case 'k': uvolniPamat(&list); exit = 0; break; // uvolnenie pamate a ukoncenie
        }
    }
    while(exit); // Koniec loopu -- Opakuje sa okial nie je zadane "k"

    return 0;
}