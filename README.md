# Hospital Marmaja EXTENSIÓN - Taller N°2 Estructura de Datos

Clasificacion de pacientes con un arbol de decision y almacenamiento en arboles AVL
segun el servicio asignado.

Flujo: `Archivo -> Cola -> Arbol de decision -> Servicio -> AVL`

## Integrantes

| Nombre | RUT | Usuario GitHub | Carrera | Paralelo |
|--------|-----|----------------|---------|----------|
| Nicolas Bugueño Rementería | 20.007.300-2 | nibugr | ICCI | C2 |

## Compilacion y ejecucion (Codespace / Linux)

```bash
make
./hospital                 # usa pacientes.txt
./hospital otro_archivo.txt
```

Para limpiar: `make clean`

Tambien se puede compilar a mano:

```bash
g++ -std=c++11 -Wall -Wextra -o hospital main.cpp Cola.cpp Lector.cpp
```

## Formato del archivo de pacientes

Los campos se separan con `;`. Una linea por paciente. Las lineas vacias y las que
empiezan con `#` se ignoran.

```
ID;Nombre;Edad;Temperatura;PresionArterial;Dolor;TipoDolor;Trauma;SintomasNeurologicos;RequiereCirugia;DiasHospitalizacion
```

Validaciones al cargar (si falla, la linea se omite y se informa el motivo):

- Exactamente 11 campos.
- ID entero positivo y no repetido.
- Edad entre 0 y 120, temperatura entre 30 y 45, presion arterial entre 40 y 250.
- Dolor entre 0 y 10. TipoDolor: pecho, cabeza, abdomen, extremidad u otro.
- Trauma, SintomasNeurologicos y RequiereCirugia solo 0 o 1.
- DiasHospitalizacion mayor o igual a 0.
- Si el archivo no existe se informa el error y el programa termina.

## Estructura del proyecto

| Archivo | Contenido |
|---------|-----------|
| `Paciente.h` | struct con los datos del paciente |
| `Cola.h / Cola.cpp` | Cola FIFO con nodos y punteros |
| `Lector.h / Lector.cpp` | Lectura y validacion del archivo |
| `main.cpp` | Programa principal (por ahora solo prueba la cola) |
| `pacientes.txt` | Datos de ejemplo |
| `pacientes_invalidos.txt` | Pruebas de errores y duplicados |

## Avance

- [x] Cola FIFO y carga del archivo
- [ ] Arbol de decision
- [ ] Arboles AVL (insercion, rotaciones, busqueda, eliminacion)
- [ ] Menu, consultas y documentacion final
