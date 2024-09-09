#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>

typedef struct _list
{
    void *info;
    struct _list *urm;
}Celula, *Lista;

Lista initL();
void elibL(Lista *l, void (*elib)(void*));
int inserare(Lista *l, void *info);
int inserareO(Lista *l, void *info, int (*cmp)(void*, void*));
void inserareP(Lista *l, void *info, int poz);
void stergeNod(Lista *l, void *info, int (*cmp)(void*, void*));
void* caut(Lista *l, void *info, int (*cmp)(void*, void*));
void printL(Lista l, void (*print)(void*, FILE*), FILE* f);
#endif // LISTA_H_INCLUDED
