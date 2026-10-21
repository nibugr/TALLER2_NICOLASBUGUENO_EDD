// main provisorio: carga el archivo, arma el arbol de decision y clasifica
// a los pacientes en orden de llegada. El AVL y el menu vienen despues.

#include <iostream>
#include <iomanip>
#include <string>
#include "Cola.h"
#include "Lector.h"
#include "ArbolDecision.h"
#include "Servicio.h"

using namespace std;

static void mostrarPaciente(const Paciente& p) {
    cout << "ID: " << setw(3) << setfill('0') << p.id << setfill(' ') << endl;
    cout << "Nombre : " << p.nombre << endl;
    cout << "Edad: " << p.edad << endl;
    cout << "Temperatura : " << fixed << setprecision(1) << p.temperatura << endl;
    cout << "PresionArterial : " << p.presionArterial << endl;
    cout << "Dolor : " << p.dolor << endl;
    cout << "TipoDolor : " << p.tipoDolor << endl;
    cout << "Trauma : " << p.trauma << endl;
    cout << "SintomasNeurologicos : " << p.sintomasNeurologicos << endl;
    cout << "RequiereCirugia : " << p.requiereCirugia << endl;
    cout << "DiasHospitalizacion : " << p.diasHospitalizacion << endl;
}

int main(int argc, char* argv[]) {
    string ruta = "pacientes.txt";
    if (argc > 1) {
        ruta = argv[1];
    }

    Cola cola;
    int cargados = cargarPacientes(ruta, cola);
    if (cargados < 0) {
        return 1;
    }

    ArbolDecision arbol;
    arbol.construir();

    cout << endl;
    arbol.mostrarArbol();
    cout << "Nodos: " << arbol.getCantidadNodos()
         << " | Altura: " << arbol.getAltura() << endl;
    cout << endl;
    arbol.mostrarPreorden();
    cout << endl;
    arbol.mostrarInorden();
    cout << endl;
    arbol.mostrarPostorden();

    cout << endl << "=== ATENDIENDO PACIENTES ===" << endl;

    int contador[NUM_SERVICIOS];
    for (int i = 0; i < NUM_SERVICIOS; i++) {
        contador[i] = 0;
    }

    Paciente p;
    while (cola.desencolar(p)) {
        cout << endl;
        mostrarPaciente(p);

        string rutaDecision;
        Servicio s = arbol.evaluar(p, rutaDecision);
        contador[s]++;

        cout << "Ruta: " << rutaDecision << endl;
        cout << "Servicio asignado : " << nombreServicio(s) << endl;
    }

    cout << endl << "=== RESUMEN POR SERVICIO ===" << endl;
    for (int i = 0; i < NUM_SERVICIOS; i++) {
        cout << nombreServicio(i) << ": " << contador[i] << endl;
    }

    return 0;
}
