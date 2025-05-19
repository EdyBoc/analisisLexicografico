
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "afd.h"

// --- Estructuras auxiliares para determinización ---
typedef struct NodoEstado {
    Estado* estado;
    struct NodoEstado* siguiente;
} NodoEstado;

typedef struct ConjuntoEstados {
    Estado* estados[256];
    int cantidad;
} ConjuntoEstados;

void agregarEstado(ConjuntoEstados* conjunto, Estado* estado) {
    for (int i = 0; i < conjunto->cantidad; i++) {
        if (conjunto->estados[i] == estado)
            return;
    }
    conjunto->estados[conjunto->cantidad++] = estado;
}

void epsilonCerradura(Estado* estado, ConjuntoEstados* conjunto) {
    if (estado == NULL) return;
    agregarEstado(conjunto, estado);
    if (estado->simbolo == 0) {
        if (estado->e1) epsilonCerradura(estado->e1, conjunto);
        if (estado->e2) epsilonCerradura(estado->e2, conjunto);
    }
}

void mover(ConjuntoEstados* origen, char simbolo, ConjuntoEstados* destino) {
    for (int i = 0; i < origen->cantidad; i++) {
        Estado* e = origen->estados[i];
        if (e->simbolo == simbolo && e->e1)
            epsilonCerradura(e->e1, destino);
    }
}

int compararConjuntos(ConjuntoEstados* a, ConjuntoEstados* b) {
    if (a->cantidad != b->cantidad) return 0;
    for (int i = 0; i < a->cantidad; i++) {
        int encontrado = 0;
        for (int j = 0; j < b->cantidad; j++) {
            if (a->estados[i] == b->estados[j]) {
                encontrado = 1;
                break;
            }
        }
        if (!encontrado) return 0;
    }
    return 1;
}

int conjuntoExiste(ConjuntoEstados conjuntos[], int total, ConjuntoEstados* nuevo) {
    for (int i = 0; i < total; i++) {
        if (compararConjuntos(&conjuntos[i], nuevo))
            return i;
    }
    return -1;
}

AFD* convertirAFN_AFD(AFN* afn) {
    AFD* afd = malloc(sizeof(AFD));
    memset(afd->transiciones, -1, sizeof(afd->transiciones));
    memset(afd->estadosFinales, 0, sizeof(afd->estadosFinales));

    ConjuntoEstados conjuntos[256];
    int totalConjuntos = 0;

    ConjuntoEstados inicial = {.cantidad = 0};
    epsilonCerradura(afn->inicio, &inicial);
    conjuntos[totalConjuntos++] = inicial;

    int procesados = 0;
    afd->estadoInicial = 0;

    while (procesados < totalConjuntos) {
        ConjuntoEstados actual = conjuntos[procesados];
        for (char c = 'a'; c <= 'b'; c++) {
            ConjuntoEstados destino = {.cantidad = 0};
            mover(&actual, c, &destino);
            if (destino.cantidad == 0) continue;
            int index = conjuntoExiste(conjuntos, totalConjuntos, &destino);
            if (index == -1) {
                conjuntos[totalConjuntos] = destino;
                index = totalConjuntos;
                totalConjuntos++;
            }
            afd->transiciones[procesados][(int)c] = index;
        }
        for (int i = 0; i < actual.cantidad; i++) {
            if (actual.estados[i] == afn->fin) {
                afd->estadosFinales[procesados] = 1;
                break;
            }
        }
        procesados++;
    }

    return afd;
}

void liberarAFD(AFD* afd) {
    free(afd);
}
