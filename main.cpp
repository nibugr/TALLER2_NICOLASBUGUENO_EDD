// main provisorio: clasifica a los pacientes y los guarda en un AVL por servicio.
// Despues hace unas pruebas del AVL. El menu final viene en el ultimo commit.

#include <iostream>
#include <string>
#include "Cola.h"
#include "Lector.h"
#include "ArbolDecision.h"
#include "ArbolAVL.h"
#include "Servicio.h"

using namespace std;

int main(int argc, char* argv[]) {
    string ruta = "pacientes.txt";
    if (argc > 1) {
        ruta = argv[1];
    }

    Cola cola;
    if (cargarPacientes(ruta, cola) < 0) {
        return 1;
    }

    ArbolDecision decision;
    decision.construir();

    // un AVL por servicio
    ArbolAVL* hospital[NUM_SERVICIOS];
    for (int i = 0; i < NUM_SERVICIOS; i++) {
        hospital[i] = new ArbolAVL(nombreServicio(i));
        hospital[i]->setMostrarRotaciones(true);
    }

    cout << endl << "=== ATENDIENDO PACIENTES ===" << endl;
    Paciente primero;
    bool hayPrimero = false;
    Servicio servicioPrimero = MEDICINA_GENERAL;

    Paciente p;
    while (cola.desencolar(p)) {
        string rutaDecision;
        Servicio s = decision.evaluar(p, rutaDecision);

        cout << p.id << " | " << p.nombre << " -> " << nombreServicio(s) << endl;
        if (!hospital[s]->insertar(p)) {
            cout << "  No se inserto: ya existe el ID " << p.id << endl;
        }

        if (!hayPrimero) {
            primero = p;
            servicioPrimero = s;
            hayPrimero = true;
        }
    }

    cout << endl << "=== ESTADO DE LOS SERVICIOS ===" << endl;
    for (int i = 0; i < NUM_SERVICIOS; i++) {
        cout << nombreServicio(i) << " (pacientes: " << hospital[i]->getCantidad()
             << ", altura: " << hospital[i]->getAltura() << ")" << endl;
        hospital[i]->mostrarInorden();
    }

    // pruebas sobre el servicio con mas pacientes
    int mayor = 0;
    for (int i = 1; i < NUM_SERVICIOS; i++) {
        if (hospital[i]->getCantidad() > hospital[mayor]->getCantidad()) {
            mayor = i;
        }
    }
    ArbolAVL* arbol = hospital[mayor];

    cout << endl << "=== PRUEBAS DEL AVL: " << nombreServicio(mayor) << " ===" << endl;
    cout << "Preorden:" << endl;
    arbol->mostrarPreorden();
    cout << "Postorden:" << endl;
    arbol->mostrarPostorden();
    cout << "Altura y factor de balance:" << endl;
    arbol->mostrarFactores();

    if (hayPrimero) {
        string rutaBusqueda;
        cout << endl << "Buscar por ID " << primero.id << " en " << nombreServicio(servicioPrimero) << ": ";
        const Paciente* r = hospital[servicioPrimero]->buscarPorId(primero.id, rutaBusqueda);
        if (r != NULL) {
            cout << "encontrado (ruta: " << rutaBusqueda << ")" << endl;
        } else {
            cout << "no encontrado" << endl;
        }

        cout << "Buscar por nombre '" << primero.nombre << "': ";
        r = hospital[servicioPrimero]->buscarPorNombre(primero.nombre);
        if (r != NULL) {
            cout << "encontrado (ID " << r->id << ")" << endl;
        } else {
            cout << "no encontrado" << endl;
        }

        cout << "Eliminar ID " << primero.id << ": ";
        if (hospital[servicioPrimero]->eliminar(primero.id)) {
            cout << "eliminado" << endl;
        } else {
            cout << "no existe" << endl;
        }
        cout << "Eliminar de nuevo el ID " << primero.id << ": ";
        if (hospital[servicioPrimero]->eliminar(primero.id)) {
            cout << "eliminado" << endl;
        } else {
            cout << "no existe" << endl;
        }
        cout << nombreServicio(servicioPrimero) << " despues de eliminar:" << endl;
        hospital[servicioPrimero]->mostrarInorden();
    }

    for (int i = 0; i < NUM_SERVICIOS; i++) {
        delete hospital[i];
    }
    return 0;
}
