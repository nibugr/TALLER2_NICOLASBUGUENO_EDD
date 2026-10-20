#ifndef COLA_H
#define COLA_H

#include "Paciente.h"

struct NodoCola {
    Paciente dato;
    NodoCola* sig;
};

class Cola {
private:
    NodoCola* frente;
    NodoCola* final;
    int tam;

    // sin copias, para no liberar dos veces los mismos nodos
    Cola(const Cola& otra);
    Cola& operator=(const Cola& otra);

public:
    Cola();
    ~Cola();

    void encolar(const Paciente& p);
    bool desencolar(Paciente& salida);
    bool estaVacia() const;
    int getTam() const;
    bool contieneId(int id) const;
};

#endif
