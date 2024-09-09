#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Lista.h"

typedef struct _s
{
    int nr_ep;
    Lista ep;
}Sezon;

typedef struct _ss
{
    int id;
    char *nume;
    float rating;
    int nr_sez;
    Lista sez; //inserand la final, asiguram comportamentul unei cozi
    int timp;
}Serial;

void elibEp(void* a)
{
    free((int*)a);
}

void elibSez(void* a)
{
    Sezon* s = (Sezon*)a;
    elibL(&s->ep, elibEp);
    free(s);
}

void elibSer(void* a)
{
    Serial* s = (Serial*)a;
    free(s->nume);
    elibL(&s->sez, elibSez);
    free(s);
}

void printSer(void* a, FILE *f)
{
    Serial* s = (Serial*)a;
    fprintf(f, "(%s, %.1f)", s->nume, s->rating);
}

int cmpSer(void *a, void *b)
{
    Serial* sa = (Serial*)a;
    Serial* sb = (Serial*)b;

    if( sa->rating == sb->rating )
        return strcmp(sa->nume, sb->nume);
    else
    {
        if( sb->rating > sa->rating )
            return 1;
        return -1;
    }
}

int cmpSerNume(void *a, void *b)
{
    Serial* sa = (Serial*)a;
    Serial* sb = (Serial*)b;
    return strcmp(sa->nume, sb->nume);
}

void stergeDinTop(Lista top)
{
    int i = 0;
    while( i < 9 && top->urm != NULL )
    {
        top = top->urm;
        i++;
    }

    if( i == 9 && top->urm != NULL )
    {
        elibSer(top->urm->info);
        free(top->urm);
        top->urm = NULL;
    }
}

int main(int argc, char **argv)
{
    Lista tendinte = initL();
    Lista documentare = initL();
    Lista tutoriale = initL();
    Lista top10 = initL();
    Lista later = initL(); //o sa inseram la final ca la coada

    FILE *r = fopen(argv[1], "r");
    FILE *w = fopen(argv[2], "w");

    char c[20];
    while( fscanf(r, "%s", c) == 1 )
    {
        if( strcmp(c, "add") == 0 )
        {
            Serial *s = (Serial*)malloc(sizeof(Serial));
            s->nume = (char*)malloc(sizeof(char)*33);
            s->sez = initL();
            s->timp = 0;
            fscanf(r, "%d %s %f %d", &s->id, s->nume, &s->rating, &s->nr_sez);

            int i;
            for( i = 0; i < s->nr_sez; i++ )
            {
                Sezon *ss = (Sezon*)malloc(sizeof(Sezon));
                ss->ep = initL();
                fscanf(r, "%d", &ss->nr_ep);

                int j;
                for( j = 0; j < ss->nr_ep; j++ )
                {
                    int *ep = (int*)malloc(sizeof(int));
                    fscanf(r, "%d", ep);
                    inserare(&ss->ep, (void*)ep);
                }
                inserare(&s->sez, (void*)ss);
            }

            int poz;
            if( s->id == 1 )
                poz = inserareO(&tendinte, (void*)s, cmpSer);
            else if( s->id == 2 )
                poz = inserareO(&documentare, (void*)s, cmpSer);
            else if( s->id == 3 )
                poz = inserareO(&tutoriale, (void*)s, cmpSer);

            fprintf(w, "Serialul %s a fost adaugat la pozitia %d.\n", s->nume, poz);
        }
        else if( strcmp(c, "show") == 0 )
        {
            char x[10]; fscanf(r, "%s", x);

            if( strcmp(x, "1") == 0 )
            {
                fprintf(w, "Categoria 1: ");
                printL(tendinte, printSer, w);
            }
            else if( strcmp(x, "2") == 0 )
            {
                fprintf(w, "Categoria 2: ");
                printL(documentare, printSer, w);
            }
            else if( strcmp(x, "3") == 0 )
            {
                fprintf(w, "Categoria 3: ");
                printL(tutoriale, printSer, w);
            }
            else if( strcmp(x, "top10") == 0 )
            {
                fprintf(w, "Categoria top10: ");
                printL(top10, printSer, w);
            }
            else if( strcmp(x, "later") == 0 )
            {
                fprintf(w, "Categoria later: ");
                printL(later, printSer, w);
            }

            fprintf(w, ".\n");
        }
        else if( strcmp(c, "add_top") == 0 )
        {
            int poz; fscanf(r, "%d", &poz);

            Serial *s = (Serial*)malloc(sizeof(Serial));
            s->nume = (char*)malloc(sizeof(char)*33);
            s->sez = initL();
            s->timp = 0;
            s->id = 4;
            fscanf(r, "%s %f %d",s->nume, &s->rating, &s->nr_sez);

            int i;
            for( i = 0; i < s->nr_sez; i++ )
            {
                Sezon *ss = (Sezon*)malloc(sizeof(Sezon));
                ss->ep = initL();
                fscanf(r, "%d", &ss->nr_ep);

                int j;
                for( j = 0; j < ss->nr_ep; j++ )
                {
                    int *ep = (int*)malloc(sizeof(int));
                    fscanf(r, "%d", ep);
                    inserare(&ss->ep, (void*)ep);
                }
                inserare(&s->sez, (void*)ss);
            }

            inserareP(&top10, (void*)s, poz);
            stergeDinTop(top10);
            fprintf(w, "Categoria top10: ");
            printL(top10, printSer, w);
            fprintf(w, ".\n");
        }
        else if( strcmp(c, "later") == 0 )
        {
            Serial *s = (Serial*)malloc(sizeof(Serial));
            s->nume = (char*)malloc(sizeof(char)*33);
            fscanf(r, "%s", s->nume);

            int poz;
            void *a = caut(&tendinte, (void*)s, cmpSerNume);
            if( a != NULL )
                stergeNod(&tendinte, a, cmpSerNume);
            if( a == NULL )
            {
                a = caut(&documentare, (void*)s, cmpSerNume);
                if( a != NULL )
                    stergeNod(&documentare, a, cmpSerNume);
            }
            if( a == NULL )
            {
                a = caut(&tutoriale, (void*)s, cmpSerNume);
                if( a != NULL )
                    stergeNod(&tutoriale, a, cmpSerNume);
            }
            if( a == NULL )
            {
                a = caut(&top10, (void*)s, cmpSerNume);
                if( a != NULL )
                    stergeNod(&top10, a, cmpSerNume);
            }

            poz = inserare(&later, a);
            fprintf(w, "Serialul %s se afla in coada de asteptare pe pozitia %d.\n", s->nume, poz);

            free(s->nume);
            free(s);
        }
    }


    fclose(r);
    fclose(w);

    elibL(&tendinte, elibSer);
    elibL(&documentare, elibSer);
    elibL(&tutoriale, elibSer);
    elibL(&top10, elibSer);
    return 0;
}
