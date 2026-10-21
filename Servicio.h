#ifndef SERVICIO_H
#define SERVICIO_H

enum Servicio {
    URGENCIAS = 0,
    MEDICINA_GENERAL,
    CARDIOLOGIA,
    NEUROLOGIA,
    TRAUMATOLOGIA,
    CIRUGIA,
    PEDIATRIA,
    HOSPITALIZACION,
    NUM_SERVICIOS
};

inline const char* nombreServicio(int s) {
    switch (s) {
        case URGENCIAS:        return "Urgencias";
        case MEDICINA_GENERAL: return "Medicina General";
        case CARDIOLOGIA:      return "Cardiologia";
        case NEUROLOGIA:       return "Neurologia";
        case TRAUMATOLOGIA:    return "Traumatologia";
        case CIRUGIA:          return "Cirugia";
        case PEDIATRIA:        return "Pediatria";
        case HOSPITALIZACION:  return "Hospitalizacion";
        default:               return "Desconocido";
    }
}

#endif
