#include "TablaHashUsuarios.h"

// Constructor
TablaHashUsuarios::TablaHashUsuarios(int cap) {
    capacidad = cap;
    tamano = 0;
    indices = new int[capacidad];
    nombres = new Cadena[capacidad];

    // Inicializa los índices en -1 indicando que la casilla está vacía
    for (int i = 0; i < capacidad; i++) {
        indices[i] = -1;
    }
}

// Destructor
TablaHashUsuarios::~TablaHashUsuarios() {
    delete[] indices;
    delete[] nombres;
}

// Convierte el texto en un número
unsigned long TablaHashUsuarios::calcularHash(const Cadena& str) const {
    unsigned long hash = 5381;
    const char* ptr = str.obtenerTexto();

    while (*ptr) {
        hash = ((hash << 5) + hash) + *ptr;
        ptr++;
    }
    return hash % capacidad;
}

// Inserta un usuario usando Linear Probing para colisiones
void TablaHashUsuarios::insertar(const Cadena& nombre, int idx) {
    // Evitar saturación
    if (tamano >= capacidad / 2) return;

    int pos = calcularHash(nombre);

    // Si hay colisión, buscar la siguiente libre
    while (indices[pos] != -1) {
        if (nombres[pos] == nombre) {
            indices[pos] = idx; // Actualizar el índice si la llave ya existe
            return;
        }
        pos = (pos + 1) % capacidad;
    }

    // Insertar en la casilla libre encontrada
    nombres[pos] = nombre;
    indices[pos] = idx;
    tamano++;
}

// Busca el índice del usuario en la tabla
int TablaHashUsuarios::buscar(const Cadena& nombre) const {
    int pos = calcularHash(nombre);
    int posInicial = pos;

    while (indices[pos] != -1) {
        if (nombres[pos] == nombre) {
            return indices[pos]; // Usuario encontrado
        }
        pos = (pos + 1) % capacidad;

        // Si da la vuelta completa y vuelve al inicio, no está
        if (pos == posInicial) {
            break;
        }
    }

    return -1; // Retorna -1 si no se encuentra
}