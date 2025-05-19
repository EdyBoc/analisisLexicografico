#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "regex_parser.h"

int prioridad(char c) {
    switch (c) {
        case '*': return 3;
        case '.': return 2;
        case '|': return 1;
        default: return 0;
    }
}

int esOperador(char c) {
    return c == '*' || c == '.' || c == '|';
}

int esSimbolo(char c) {
    return isalnum(c) || c == ')';
}

int esInicioDeGrupo(char c) {
    return isalnum(c) || c == '(';
}

// Inserta concatenaciones explícitas con '.'
char* insertarPuntosConcatenacion(const char* expr) {
    size_t len = strlen(expr);
    char* salida = malloc(len * 2); // espacio extra por posibles puntos
    int j = 0;

    for (int i = 0; i < len; i++) {
        char c1 = expr[i];
        salida[j++] = c1;

        if (i + 1 < len) {
            char c2 = expr[i + 1];

           // Condiciones para insertar punto de concatenación
            if (
                (isalnum(c1) && (isalnum(c2) || c2 == '(')) ||
                (c1 == '*' && (isalnum(c2) || c2 == '(')) ||
                (c1 == ')' && (isalnum(c2) || c2 == '('))
            ) {
                salida[j++] = '.';
            }
        }
    }

    salida[j] = '\0';
    return salida;
}

// Conversión infija a postfija usando Shunting Yard
char* infijoAPostfijo(const char* expresion) {
    char* conPuntos = insertarPuntosConcatenacion(expresion);
    char* salida = malloc(strlen(conPuntos) * 2);
    char pila[256];
    int top = -1, j = 0;

    for (int i = 0; conPuntos[i]; i++) {
        char c = conPuntos[i];
        if (isalnum(c)) {
            salida[j++] = c;
        } else if (c == '(') {
            pila[++top] = c;
        } else if (c == ')') {
            while (top >= 0 && pila[top] != '(')
                salida[j++] = pila[top--];
            top--; // eliminar '('
        } else if (esOperador(c)) {
            while (top >= 0 && prioridad(pila[top]) >= prioridad(c))
                salida[j++] = pila[top--];
            pila[++top] = c;
        }
    }

    while (top >= 0)
        salida[j++] = pila[top--];
    salida[j] = '\0';

    free(conPuntos);
    return salida;
}
