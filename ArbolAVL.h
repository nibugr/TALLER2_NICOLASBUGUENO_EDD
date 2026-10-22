#ifndef ARBOLAVL_H
#define ARBOLAVL_H

#include <string>
#include "Paciente.h"

struct NodoAVL {
    Paciente dato;
    int altura;
    NodoAVL* izq;
    NodoAVL* der;
};

// AVL de pacientes ordenado por ID
class ArbolAVL {
private:
    NodoAVL* raiz;
    int cantidad;
    std::string nombre;       // nombre del servicio, solo para los mensajes
    bool mostrarRotaciones;

    ArbolAVL(const ArbolAVL& otro);
    ArbolAVL& operator=(const ArbolAVL& otro);

    int altura(NodoAVL* n) const;
    int factorBalance(NodoAVL* n) const;
    void actualizarAltura(NodoAVL* n);

    NodoAVL* rotarDerecha(NodoAVL* y);
    NodoAVL* rotarIzquierda(NodoAVL* x);
    NodoAVL* rotarIzquierdaDerecha(NodoAVL* n);
    NodoAVL* rotarDerechaIzquierda(NodoAVL* n);
    NodoAVL* balancear(NodoAVL* n);

    NodoAVL* insertar(NodoAVL* n, const Paciente& p, bool& ok);
    NodoAVL* eliminar(NodoAVL* n, int id, bool& ok);
    NodoAVL* minimo(NodoAVL* n) const;
    const Paciente* buscarNombre(NodoAVL* n, const std::string& nombre) const;

    void inorden(NodoAVL* n) const;
    void preorden(NodoAVL* n) const;
    void postorden(NodoAVL* n) const;
    void factores(NodoAVL* n) const;
    void liberar(NodoAVL* n);

public:
    ArbolAVL(const std::string& nombre);
    ~ArbolAVL();

    void setMostrarRotaciones(bool valor);

    bool insertar(const Paciente& p);      // false si el ID ya existe
    bool eliminar(int id);                 // false si no existe

    // Busca por ID usando el orden del arbol. En ruta deja los IDs visitados.
    const Paciente* buscarPorId(int id, std::string& ruta) const;
    // Busca por nombre recorriendo todo el arbol (no esta ordenado por nombre).
    const Paciente* buscarPorNombre(const std::string& nombre) const;

    void mostrarInorden() const;
    void mostrarPreorden() const;
    void mostrarPostorden() const;
    void mostrarFactores() const;          // altura y factor de balance de cada nodo
    bool getFactorBalance(int id, int& fb) const;

    int getAltura() const;
    int getCantidad() const;
    bool estaVacio() const;
};

#endif
