#ifndef THOMPSON_H
#define THOMPSON_H

typedef struct Estado {
    int id;
    struct Estado* e1;
    struct Estado* e2;
    char simbolo;
} Estado;

typedef struct {
    Estado* inicio;
    Estado* fin;
} AFN;

AFN* construirAFN(const char* postfija);
void liberarAFN(AFN* afn);

#endif
