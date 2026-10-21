#include "ArbolDecision.h"

#include <iostream>

using namespace std;

ArbolDecision::ArbolDecision() {
    raiz = NULL;
}

ArbolDecision::~ArbolDecision() {
    liberar(raiz);
    raiz = NULL;
}

// postorden: primero los hijos y despues el nodo
void ArbolDecision::liberar(NodoDecision* n) {
    if (n == NULL) {
        return;
    }
    liberar(n->si);
    liberar(n->no);
    delete n;
}

NodoDecision* ArbolDecision::crearHoja(Servicio s) {
    NodoDecision* n = new NodoDecision;
    n->esHoja = true;
    n->atributo = ATR_EDAD;
    n->op = OP_IGUAL;
    n->umbral = 0;
    n->servicio = s;
    n->si = NULL;
    n->no = NULL;
    return n;
}

NodoDecision* ArbolDecision::crearPregunta(Atributo a, Operador op, float umbral,
                                           const string& texto,
                                           const string& etiquetaSi,
                                           const string& etiquetaNo,
                                           NodoDecision* si, NodoDecision* no) {
    NodoDecision* n = new NodoDecision;
    n->esHoja = false;
    n->atributo = a;
    n->op = op;
    n->umbral = umbral;
    n->texto = texto;
    n->etiquetaSi = etiquetaSi;
    n->etiquetaNo = etiquetaNo;
    n->servicio = MEDICINA_GENERAL;
    n->si = si;
    n->no = no;
    return n;
}

// Se arma desde abajo hacia arriba. El orden de las preguntas es
// la prioridad: edad, trauma, neuro, pecho, fiebre, dolor, presion, dias.
void ArbolDecision::construir() {
    liberar(raiz);
    raiz = NULL;

    NodoDecision* pDias = crearPregunta(ATR_DIAS, OP_MAYOR_IGUAL, 2, "",
                                        "DiasHospitalizacion >= 2",
                                        "DiasHospitalizacion < 2",
                                        crearHoja(HOSPITALIZACION),
                                        crearHoja(MEDICINA_GENERAL));

    NodoDecision* pPresion = crearPregunta(ATR_PRESION, OP_MAYOR_IGUAL, 180, "",
                                           "PresionArterial >= 180",
                                           "PresionArterial < 180",
                                           crearHoja(URGENCIAS), pDias);

    NodoDecision* pDolor = crearPregunta(ATR_DOLOR, OP_MAYOR_IGUAL, 8, "",
                                         "Dolor >= 8", "Dolor < 8",
                                         crearHoja(URGENCIAS), pPresion);

    NodoDecision* pTemp = crearPregunta(ATR_TEMPERATURA, OP_MAYOR_IGUAL, 38.5f, "",
                                        "Temperatura >= 38.5", "Temperatura < 38.5",
                                        crearHoja(URGENCIAS), pDolor);

    NodoDecision* pPecho = crearPregunta(ATR_TIPO_DOLOR, OP_IGUAL, 0, "pecho",
                                         "TipoDolor == pecho", "TipoDolor != pecho",
                                         crearHoja(CARDIOLOGIA), pTemp);

    NodoDecision* pNeuro = crearPregunta(ATR_NEURO, OP_IGUAL, 1, "",
                                         "SintomasNeurologicos == 1",
                                         "SintomasNeurologicos == 0",
                                         crearHoja(NEUROLOGIA), pPecho);

    NodoDecision* pCirugia = crearPregunta(ATR_CIRUGIA, OP_IGUAL, 1, "",
                                           "RequiereCirugia == 1",
                                           "RequiereCirugia == 0",
                                           crearHoja(CIRUGIA),
                                           crearHoja(TRAUMATOLOGIA));

    NodoDecision* pTrauma = crearPregunta(ATR_TRAUMA, OP_IGUAL, 1, "",
                                          "Trauma == 1", "Trauma == 0",
                                          pCirugia, pNeuro);

    raiz = crearPregunta(ATR_EDAD, OP_MENOR, 14, "",
                         "Edad < 14", "Edad >= 14",
                         crearHoja(PEDIATRIA), pTrauma);
}

bool ArbolDecision::datosValidos(const Paciente& p) {
    if (p.edad < 0 || p.edad > 120) return false;
    if (p.temperatura < 30.0f || p.temperatura > 45.0f) return false;
    if (p.presionArterial < 40 || p.presionArterial > 250) return false;
    if (p.dolor < 0 || p.dolor > 10) return false;
    if (p.trauma != 0 && p.trauma != 1) return false;
    if (p.sintomasNeurologicos != 0 && p.sintomasNeurologicos != 1) return false;
    if (p.requiereCirugia != 0 && p.requiereCirugia != 1) return false;
    if (p.diasHospitalizacion < 0) return false;
    if (p.tipoDolor != "pecho" && p.tipoDolor != "cabeza" && p.tipoDolor != "abdomen" &&
        p.tipoDolor != "extremidad" && p.tipoDolor != "otro") return false;
    return true;
}

bool ArbolDecision::responder(const NodoDecision* n, const Paciente& p) const {
    if (n->atributo == ATR_TIPO_DOLOR) {
        return p.tipoDolor == n->texto;
    }

    float valor = 0;
    switch (n->atributo) {
        case ATR_EDAD:        valor = (float)p.edad; break;
        case ATR_TEMPERATURA: valor = p.temperatura; break;
        case ATR_PRESION:     valor = (float)p.presionArterial; break;
        case ATR_DOLOR:       valor = (float)p.dolor; break;
        case ATR_TRAUMA:      valor = (float)p.trauma; break;
        case ATR_NEURO:       valor = (float)p.sintomasNeurologicos; break;
        case ATR_CIRUGIA:     valor = (float)p.requiereCirugia; break;
        case ATR_DIAS:        valor = (float)p.diasHospitalizacion; break;
        default: break;
    }

    switch (n->op) {
        case OP_MENOR:       return valor < n->umbral;
        case OP_MAYOR_IGUAL: return valor >= n->umbral;
        case OP_IGUAL:       return valor == n->umbral;
    }
    return false;
}

Servicio ArbolDecision::evaluar(const Paciente& p, string& ruta) const {
    ruta = "";

    // datos invalidos van a Urgencias (esta explicado en el README)
    if (!datosValidos(p) || raiz == NULL) {
        ruta = "Datos invalidos o insuficientes";
        return URGENCIAS;
    }

    const NodoDecision* actual = raiz;
    while (!actual->esHoja) {
        bool respuesta = responder(actual, p);
        if (!ruta.empty()) {
            ruta += " -> ";
        }
        ruta += respuesta ? actual->etiquetaSi : actual->etiquetaNo;
        actual = respuesta ? actual->si : actual->no;
    }
    return actual->servicio;
}

string ArbolDecision::descripcion(const NodoDecision* n) const {
    if (n->esHoja) {
        return string("Servicio: ") + nombreServicio(n->servicio);
    }
    return "Pregunta: " + n->etiquetaSi;
}

void ArbolDecision::preorden(const NodoDecision* n) const {
    if (n == NULL) return;
    cout << "  - " << descripcion(n) << endl;
    preorden(n->si);
    preorden(n->no);
}

void ArbolDecision::inorden(const NodoDecision* n) const {
    if (n == NULL) return;
    inorden(n->si);
    cout << "  - " << descripcion(n) << endl;
    inorden(n->no);
}

void ArbolDecision::postorden(const NodoDecision* n) const {
    if (n == NULL) return;
    postorden(n->si);
    postorden(n->no);
    cout << "  - " << descripcion(n) << endl;
}

void ArbolDecision::imprimirIndentado(const NodoDecision* n, int nivel, const string& rama) const {
    if (n == NULL) return;
    for (int i = 0; i < nivel; i++) {
        cout << "    ";
    }
    cout << rama << descripcion(n) << endl;
    imprimirIndentado(n->si, nivel + 1, "[Si] ");
    imprimirIndentado(n->no, nivel + 1, "[No] ");
}

int ArbolDecision::contarNodos(const NodoDecision* n) const {
    if (n == NULL) return 0;
    return 1 + contarNodos(n->si) + contarNodos(n->no);
}

int ArbolDecision::altura(const NodoDecision* n) const {
    if (n == NULL) return 0;
    int hs = altura(n->si);
    int hn = altura(n->no);
    return 1 + (hs > hn ? hs : hn);
}

void ArbolDecision::mostrarPreorden() const {
    cout << "Recorrido preorden del arbol de decision:" << endl;
    preorden(raiz);
}

void ArbolDecision::mostrarInorden() const {
    cout << "Recorrido inorden del arbol de decision:" << endl;
    inorden(raiz);
}

void ArbolDecision::mostrarPostorden() const {
    cout << "Recorrido postorden del arbol de decision:" << endl;
    postorden(raiz);
}

void ArbolDecision::mostrarArbol() const {
    cout << "Arbol de decision:" << endl;
    imprimirIndentado(raiz, 0, "");
}

int ArbolDecision::getCantidadNodos() const {
    return contarNodos(raiz);
}

int ArbolDecision::getAltura() const {
    return altura(raiz);
}
