#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "thompson.h"

int estadoID = 0;

Estado* nuevoEstado(char simbolo) {
    Estado* e = malloc(sizeof(Estado));
    e->id = estadoID++;
    e->e1 = e->e2 = NULL;
    e->simbolo = simbolo;
    return e;
}

AFN* construirAFN(const char* postfija) {
    AFN* pila[256];
    int top = -1;

    for (int i = 0; postfija[i]; i++) {
        char c = postfija[i];
        if (isalnum(c)) {
            Estado* s1 = nuevoEstado(0);
            Estado* s2 = nuevoEstado(0);
            s1->simbolo = c;
            s1->e1 = s2;
            AFN* afn = malloc(sizeof(AFN));
            afn->inicio = s1;
            afn->fin = s2;
            pila[++top] = afn;
        } else if (c == '*') {
            AFN* a = pila[top--];
            Estado* s1 = nuevoEstado(0);
            Estado* s2 = nuevoEstado(0);
            s1->e1 = a->inicio;
            s1->e2 = s2;
            a->fin->e1 = a->inicio;
            a->fin->e2 = s2;
            AFN* afn = malloc(sizeof(AFN));
            afn->inicio = s1;
            afn->fin = s2;
            pila[++top] = afn;
        } else if (c == '.') {
            AFN* b = pila[top--];
            AFN* a = pila[top--];
            a->fin->e1 = b->inicio;
            AFN* afn = malloc(sizeof(AFN));
            afn->inicio = a->inicio;
            afn->fin = b->fin;
            pila[++top] = afn;
        } else if (c == '|') {
            AFN* b = pila[top--];
            AFN* a = pila[top--];
            Estado* s1 = nuevoEstado(0);
            Estado* s2 = nuevoEstado(0);
            s1->e1 = a->inicio;
            s1->e2 = b->inicio;
            a->fin->e1 = s2;
            b->fin->e1 = s2;
            AFN* afn = malloc(sizeof(AFN));
            afn->inicio = s1;
            afn->fin = s2;
            pila[++top] = afn;
        }
    }
    return pila[top];
}

void liberarAFN(AFN* afn) {
    // Liberación simplificada para demostración
    free(afn);
}
