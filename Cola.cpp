#include "Cola.h"

Cola::Cola() {
    frente = NULL;
    final = NULL;
    tam = 0;
}

Cola::~Cola() {
    while (frente != NULL) {
        NodoCola* aux = frente;
        frente = frente->sig;
        delete aux;
    }
    final = NULL;
    tam = 0;
}

void Cola::encolar(const Paciente& p) {
    NodoCola* nuevo = new NodoCola;
    nuevo->dato = p;
    nuevo->sig = NULL;

    if (estaVacia()) {
        frente = nuevo;
    } else {
        final->sig = nuevo;
    }
    final = nuevo;
    tam++;
}

bool Cola::desencolar(Paciente& salida) {
    if (estaVacia()) {
        return false;
    }
    NodoCola* aux = frente;
    salida = aux->dato;
    frente = frente->sig;

    if (frente == NULL) {
        final = NULL;
    }
    delete aux;
    tam--;
    return true;
}

bool Cola::estaVacia() const {
    return frente == NULL;
}

int Cola::getTam() const {
    return tam;
}

bool Cola::contieneId(int id) const {
    NodoCola* actual = frente;
    while (actual != NULL) {
        if (actual->dato.id == id) {
            return true;
        }
        actual = actual->sig;
    }
    return false;
}
