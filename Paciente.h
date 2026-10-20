#ifndef PACIENTE_H
#define PACIENTE_H

#include <string>

struct Paciente {
    int id;
    std::string nombre;
    int edad;
    float temperatura;
    int presionArterial;       // sistolica
    int dolor;                 // 0 a 10
    std::string tipoDolor;     // pecho, cabeza, abdomen, extremidad u otro
    int trauma;                // 0 o 1
    int sintomasNeurologicos;  // 0 o 1
    int requiereCirugia;       // 0 o 1
    int diasHospitalizacion;
};

#endif
