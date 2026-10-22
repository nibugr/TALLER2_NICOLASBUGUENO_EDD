#include "ArbolAVL.h"

#include <iostream>
#include <cctype>

using namespace std;

ArbolAVL::ArbolAVL(const string& nombre) {
    raiz = NULL;
    cantidad = 0;
    this->nombre = nombre;
    mostrarRotaciones = false;
}

ArbolAVL::~ArbolAVL() {
    liberar(raiz);
    raiz = NULL;
}

// postorden: primero los hijos y despues el nodo
void ArbolAVL::liberar(NodoAVL* n) {
    if (n == NULL) {
        return;
    }
    liberar(n->izq);
    liberar(n->der);
    delete n;
}

void ArbolAVL::setMostrarRotaciones(bool valor) {
    mostrarRotaciones = valor;
}

// ---------- altura y balance ----------

int ArbolAVL::altura(NodoAVL* n) const {
    if (n == NULL) {
        return 0;
    }
    return n->altura;
}

void ArbolAVL::actualizarAltura(NodoAVL* n) {
    int hi = altura(n->izq);
    int hd = altura(n->der);
    n->altura = 1 + (hi > hd ? hi : hd);
}

// factor de balance = altura izquierda - altura derecha
int ArbolAVL::factorBalance(NodoAVL* n) const {
    if (n == NULL) {
        return 0;
    }
    return altura(n->izq) - altura(n->der);
}

// ---------- rotaciones ----------

NodoAVL* ArbolAVL::rotarDerecha(NodoAVL* y) {
    if (mostrarRotaciones) {
        cout << "  [" << nombre << "] Rotacion simple derecha en el nodo " << y->dato.id << endl;
    }
    NodoAVL* x = y->izq;
    y->izq = x->der;
    x->der = y;
    actualizarAltura(y);
    actualizarAltura(x);
    return x;
}

NodoAVL* ArbolAVL::rotarIzquierda(NodoAVL* x) {
    if (mostrarRotaciones) {
        cout << "  [" << nombre << "] Rotacion simple izquierda en el nodo " << x->dato.id << endl;
    }
    NodoAVL* y = x->der;
    x->der = y->izq;
    y->izq = x;
    actualizarAltura(x);
    actualizarAltura(y);
    return y;
}

// caso izquierda-derecha: primero se rota el hijo izquierdo a la izquierda
NodoAVL* ArbolAVL::rotarIzquierdaDerecha(NodoAVL* n) {
    if (mostrarRotaciones) {
        cout << "  [" << nombre << "] Rotacion doble izquierda-derecha en el nodo " << n->dato.id << endl;
    }
    bool aux = mostrarRotaciones;
    mostrarRotaciones = false;
    n->izq = rotarIzquierda(n->izq);
    n = rotarDerecha(n);
    mostrarRotaciones = aux;
    return n;
}

// caso derecha-izquierda: primero se rota el hijo derecho a la derecha
NodoAVL* ArbolAVL::rotarDerechaIzquierda(NodoAVL* n) {
    if (mostrarRotaciones) {
        cout << "  [" << nombre << "] Rotacion doble derecha-izquierda en el nodo " << n->dato.id << endl;
    }
    bool aux = mostrarRotaciones;
    mostrarRotaciones = false;
    n->der = rotarDerecha(n->der);
    n = rotarIzquierda(n);
    mostrarRotaciones = aux;
    return n;
}

// Revisa el nodo y rota si el factor de balance se salio de [-1, 1]
NodoAVL* ArbolAVL::balancear(NodoAVL* n) {
    actualizarAltura(n);
    int fb = factorBalance(n);

    if (fb > 1) {
        if (factorBalance(n->izq) < 0) {
            return rotarIzquierdaDerecha(n);
        }
        return rotarDerecha(n);
    }
    if (fb < -1) {
        if (factorBalance(n->der) > 0) {
            return rotarDerechaIzquierda(n);
        }
        return rotarIzquierda(n);
    }
    return n;
}

// ---------- insertar ----------

NodoAVL* ArbolAVL::insertar(NodoAVL* n, const Paciente& p, bool& ok) {
    if (n == NULL) {
        NodoAVL* nuevo = new NodoAVL;
        nuevo->dato = p;
        nuevo->altura = 1;
        nuevo->izq = NULL;
        nuevo->der = NULL;
        ok = true;
        return nuevo;
    }

    if (p.id < n->dato.id) {
        n->izq = insertar(n->izq, p, ok);
    } else if (p.id > n->dato.id) {
        n->der = insertar(n->der, p, ok);
    } else {
        ok = false;      // ID repetido
        return n;
    }
    return balancear(n);
}

bool ArbolAVL::insertar(const Paciente& p) {
    bool ok = false;
    raiz = insertar(raiz, p, ok);
    if (ok) {
        cantidad++;
    }
    return ok;
}

// ---------- eliminar ----------

NodoAVL* ArbolAVL::minimo(NodoAVL* n) const {
    while (n->izq != NULL) {
        n = n->izq;
    }
    return n;
}

NodoAVL* ArbolAVL::eliminar(NodoAVL* n, int id, bool& ok) {
    if (n == NULL) {
        ok = false;
        return NULL;
    }

    if (id < n->dato.id) {
        n->izq = eliminar(n->izq, id, ok);
    } else if (id > n->dato.id) {
        n->der = eliminar(n->der, id, ok);
    } else {
        ok = true;
        if (n->izq == NULL || n->der == NULL) {
            // 0 o 1 hijo: lo reemplaza su hijo
            NodoAVL* hijo = (n->izq != NULL) ? n->izq : n->der;
            delete n;
            return hijo;
        }
        // 2 hijos: se copia el sucesor (el menor del subarbol derecho) y se borra ese
        NodoAVL* suc = minimo(n->der);
        n->dato = suc->dato;
        n->der = eliminar(n->der, suc->dato.id, ok);
    }
    return balancear(n);
}

bool ArbolAVL::eliminar(int id) {
    bool ok = false;
    raiz = eliminar(raiz, id, ok);
    if (ok) {
        cantidad--;
    }
    return ok;
}

// ---------- busqueda ----------

const Paciente* ArbolAVL::buscarPorId(int id, string& ruta) const {
    ruta = "";
    NodoAVL* actual = raiz;
    while (actual != NULL) {
        if (!ruta.empty()) {
            ruta += " -> ";
        }
        ruta += to_string(actual->dato.id);

        if (id == actual->dato.id) {
            return &actual->dato;
        }
        if (id < actual->dato.id) {
            actual = actual->izq;
        } else {
            actual = actual->der;
        }
    }
    return NULL;
}

static string aMinusculas(string s) {
    for (size_t i = 0; i < s.size(); i++) {
        s[i] = (char)tolower((unsigned char)s[i]);
    }
    return s;
}

// preorden completo, porque el arbol esta ordenado por ID y no por nombre
const Paciente* ArbolAVL::buscarNombre(NodoAVL* n, const string& nombre) const {
    if (n == NULL) {
        return NULL;
    }
    if (aMinusculas(n->dato.nombre) == nombre) {
        return &n->dato;
    }
    const Paciente* r = buscarNombre(n->izq, nombre);
    if (r != NULL) {
        return r;
    }
    return buscarNombre(n->der, nombre);
}

const Paciente* ArbolAVL::buscarPorNombre(const string& nombre) const {
    return buscarNombre(raiz, aMinusculas(nombre));
}

// ---------- recorridos ----------

void ArbolAVL::inorden(NodoAVL* n) const {
    if (n == NULL) return;
    inorden(n->izq);
    cout << "  - " << n->dato.id << " | " << n->dato.nombre << endl;
    inorden(n->der);
}

void ArbolAVL::preorden(NodoAVL* n) const {
    if (n == NULL) return;
    cout << "  - " << n->dato.id << " | " << n->dato.nombre << endl;
    preorden(n->izq);
    preorden(n->der);
}

void ArbolAVL::postorden(NodoAVL* n) const {
    if (n == NULL) return;
    postorden(n->izq);
    postorden(n->der);
    cout << "  - " << n->dato.id << " | " << n->dato.nombre << endl;
}

void ArbolAVL::factores(NodoAVL* n) const {
    if (n == NULL) return;
    factores(n->izq);
    cout << "  - ID " << n->dato.id << " | altura " << n->altura
         << " | factor de balance " << factorBalance(n) << endl;
    factores(n->der);
}

void ArbolAVL::mostrarInorden() const {
    if (raiz == NULL) {
        cout << "  Sin pacientes" << endl;
        return;
    }
    inorden(raiz);
}

void ArbolAVL::mostrarPreorden() const {
    if (raiz == NULL) {
        cout << "  Sin pacientes" << endl;
        return;
    }
    preorden(raiz);
}

void ArbolAVL::mostrarPostorden() const {
    if (raiz == NULL) {
        cout << "  Sin pacientes" << endl;
        return;
    }
    postorden(raiz);
}

void ArbolAVL::mostrarFactores() const {
    if (raiz == NULL) {
        cout << "  Sin pacientes" << endl;
        return;
    }
    factores(raiz);
}

// busca el nodo por ID y entrega su factor de balance
bool ArbolAVL::getFactorBalance(int id, int& fb) const {
    NodoAVL* actual = raiz;
    while (actual != NULL) {
        if (id == actual->dato.id) {
            fb = factorBalance(actual);
            return true;
        }
        actual = (id < actual->dato.id) ? actual->izq : actual->der;
    }
    return false;
}

int ArbolAVL::getAltura() const {
    return altura(raiz);
}

int ArbolAVL::getCantidad() const {
    return cantidad;
}

bool ArbolAVL::estaVacio() const {
    return raiz == NULL;
}
