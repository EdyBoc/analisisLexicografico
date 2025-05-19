#ifndef AFD_H
#define AFD_H

#include "thompson.h"

typedef struct {
    int transiciones[256][128];
    int estadosFinales[256];
    int estadoInicial;
} AFD;

AFD* convertirAFN_AFD(AFN* afn);
void liberarAFD(AFD* afd);

#endif
