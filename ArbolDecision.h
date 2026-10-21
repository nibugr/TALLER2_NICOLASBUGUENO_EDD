#ifndef ARBOLDECISION_H
#define ARBOLDECISION_H

#include <string>
#include "Paciente.h"
#include "Servicio.h"

enum Atributo {
    ATR_EDAD,
    ATR_TEMPERATURA,
    ATR_PRESION,
    ATR_DOLOR,
    ATR_TIPO_DOLOR,
    ATR_TRAUMA,
    ATR_NEURO,
    ATR_CIRUGIA,
    ATR_DIAS
};

enum Operador {
    OP_MENOR,
    OP_MAYOR_IGUAL,
    OP_IGUAL
};

// Si esHoja es true el nodo guarda un servicio, si no es una pregunta.
struct NodoDecision {
    bool esHoja;

    Atributo atributo;
    Operador op;
    float umbral;
    std::string texto;        // solo para TipoDolor
    std::string etiquetaSi;   // texto para la ruta si la respuesta es si
    std::string etiquetaNo;   // texto para la ruta si la respuesta es no

    Servicio servicio;

    NodoDecision* si;
    NodoDecision* no;
};

class ArbolDecision {
private:
    NodoDecision* raiz;

    ArbolDecision(const ArbolDecision& otro);
    ArbolDecision& operator=(const ArbolDecision& otro);

    NodoDecision* crearHoja(Servicio s);
    NodoDecision* crearPregunta(Atributo a, Operador op, float umbral,
                                const std::string& texto,
                                const std::string& etiquetaSi,
                                const std::string& etiquetaNo,
                                NodoDecision* si, NodoDecision* no);

    bool responder(const NodoDecision* n, const Paciente& p) const;

    void preorden(const NodoDecision* n) const;
    void inorden(const NodoDecision* n) const;
    void postorden(const NodoDecision* n) const;
    void imprimirIndentado(const NodoDecision* n, int nivel, const std::string& rama) const;
    std::string descripcion(const NodoDecision* n) const;
    int contarNodos(const NodoDecision* n) const;
    int altura(const NodoDecision* n) const;
    void liberar(NodoDecision* n);

public:
    ArbolDecision();
    ~ArbolDecision();

    void construir();

    static bool datosValidos(const Paciente& p);

    // Devuelve el servicio y deja en ruta las preguntas que fue respondiendo.
    Servicio evaluar(const Paciente& p, std::string& ruta) const;

    void mostrarPreorden() const;
    void mostrarInorden() const;
    void mostrarPostorden() const;
    void mostrarArbol() const;
    int getCantidadNodos() const;
    int getAltura() const;
};

#endif
