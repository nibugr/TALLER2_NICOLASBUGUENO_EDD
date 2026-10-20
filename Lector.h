#ifndef LECTOR_H
#define LECTOR_H

#include <string>
#include "Cola.h"

// Lee el archivo (campos separados por ';') y encola los pacientes en orden.
// Devuelve cuantos cargo, o -1 si no pudo abrir el archivo.
int cargarPacientes(const std::string& ruta, Cola& cola);

#endif
