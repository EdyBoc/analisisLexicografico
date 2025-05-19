#include <stdio.h>
#include <string.h>
#include "validacion.h"

void validarCadena(const char* cadena, AFD* afd) {
    int estado = afd->estadoInicial;
    for (int i = 0; cadena[i]; i++) {
        char c = cadena[i];

         // Validacion si el caracter es valido (solo 'a' o 'b')
        if (c != 'a' && c != 'b') {
            printf("ERROR: Caracter '%c' no esperado en la posicion %d.\n", c, i + 1);
            return;
        }

        int sig = afd->transiciones[estado][(int)c];
        if (sig == -1) {
            printf("Cadena RECHAZADA: Falta transicion en el caracter %d (caracter '%c').\n", i + 1, c);
            return;
        }
        estado = sig;
    }
    if (afd->estadosFinales[estado]) {
        printf("Cadena ACEPTADA.\n");
    } else {
        int len = strlen(cadena);
        printf("Cadena RECHAZADA: Falta transicion en el caracter %d (finaliza con '%c').\n",
            len, cadena[len - 1]);
    }
}
