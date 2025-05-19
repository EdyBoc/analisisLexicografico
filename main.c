#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "regex_parser.h"
#include "thompson.h"
#include "afd.h"
#include "validacion.h"

#define MAX_INPUT 256

int main() {
    char regex[MAX_INPUT];
    char cadena[MAX_INPUT];

    printf("Ingrese una expresion regular:");
    fgets(regex, MAX_INPUT, stdin);
    regex[strcspn(regex, "\n")] = 0; // eliminar salto de línea

    char *postfija = infijoAPostfijo(regex);
    if (!postfija) {
        printf("Error al convertir a postfija.\n");
        return 1;
    }

    AFN *afn = construirAFN(postfija);
    AFD *afd = convertirAFN_AFD(afn);

    printf("AFD construido. Modo validacion activo:\n");

    while (1) {
        printf("Ingrese cadena: ");
        fgets(cadena, MAX_INPUT, stdin);
        cadena[strcspn(cadena, "\n")] = 0;

        if (strlen(cadena) == 0)
            break;

        validarCadena(cadena, afd);
    }

    liberarAFN(afn);
    liberarAFD(afd);
    free(postfija);
    return 0;
}
