#include "Lista.h"

Lista initL()
{
    return NULL;
}

void elibL(Lista *l, void (*elib)(void*))
{
    Celula *crt = *l, *urm = NULL;
    while( crt != NULL )
    {
        urm = crt->urm;
        elib(crt->info);
        free(crt);
        crt = urm;
    }

    *l = NULL;
}

int inserare(Lista *l, void *info)
{
    Celula *c = (Celula*)malloc(sizeof(Celula));
    c->info = info;
    c->urm = NULL;

    if( *l == NULL )
    {
        *l = c;
        return 1;
    }
    else
    {
        Celula *aux = *l;
        int pos = 2;
        while( aux->urm != NULL )
        {
            aux = aux->urm;
            pos++;
        }
        aux->urm = c;
        return pos;
    }
}

int inserareO(Lista *l, void *info, int (*cmp)(void*, void*))
{
    Celula *c = (Celula*)malloc(sizeof(Celula));
    c->info = info;
    c->urm = NULL;

    if( *l == NULL )
    {
        *l = c;
        return 1;
    }
    else if( cmp(info, (*l)->info) < 0 )
    {
        c->urm = *l;
        *l = c;
        return 1;
    }
    else
    {
        Celula *aux = *l;
        int pos = 2;
        while( aux->urm != NULL && cmp(info, aux->urm->info) > 0 )
        {
            aux = aux->urm;
            pos++;
        }
        c->urm = aux->urm;
        aux->urm = c;
        return pos;
    }
}

void inserareP(Lista *l, void *info, int poz)
{
    Celula *c = (Celula*)malloc(sizeof(Celula));
    c->info = info;
    c->urm = NULL;

    if( *l == NULL )
        *l = c;
    else if( poz == 1 )
    {
        c->urm = *l;
        *l = c;
    }
    else
    {
        Celula *aux = *l;
        while( aux->urm != NULL && poz != 2 )
        {
            aux = aux->urm;
            poz--;
        }
        c->urm = aux->urm;
        aux->urm = c;
    }
}

void stergeNod(Lista *l, void *info, int (*cmp)(void*, void*))
{
    if( *l == NULL )
        return;

    if( cmp( (*l)->info, info ) == 0 )
    {
        Celula *c = *l;
        *l = (*l)->urm;
        free(c);
    }
    else
    {
        Celula *c = *l;
        while( c->urm != NULL && cmp(c->urm->info, info) != 0 )
            c = c->urm;

        if( c->urm != NULL )
        {
            Celula *a = c->urm;
            c->urm = a->urm;
            free(a);
        }
    }
}

void* caut(Lista *l, void *info, int (*cmp)(void*, void*))
{
    Celula *c = *l;
    while( c != NULL )
    {
        if( cmp(c->info, info) == 0 )
            return c->info;
        c = c->urm;
    }
    return NULL;
}

void printL(Lista l, void (*print)(void*, FILE*), FILE* f)
{
    fprintf(f, "[");

    Celula *c = l;
    while( c != NULL )
    {
        print(c->info, f);
        if( c->urm != NULL )
            fprintf(f, ", ");
        c = c->urm;
    }

    fprintf(f, "]");
}
