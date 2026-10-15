#include "TablaHashUsuarios.h"
#include "MedidorRecursos.h"

void TablaHashUsuarios::reservar(int cap) {
    capacidad = cap;
    tamano = 0;
    indices = new int[capacidad];
    nombres = new Cadena*[capacidad];

    // -1 en indices y nullptr en nombres indican casilla vacia
    for (int i = 0; i < capacidad; i++) {
        MedidorRecursos::contarIteracion();
        indices[i] = -1;
        nombres[i] = nullptr;
    }
}

void TablaHashUsuarios::liberar() {
    for (int i = 0; i < capacidad; i++) {
        delete nombres[i];          // delete sobre nullptr es seguro
    }
    delete[] nombres;
    delete[] indices;
    nombres = nullptr;
    indices = nullptr;
    capacidad = 0;
    tamano = 0;
}

void TablaHashUsuarios::intercambiar(TablaHashUsuarios& otra) {
    int* tmpIndices = indices;     indices = otra.indices;     otra.indices = tmpIndices;
    Cadena** tmpNombres = nombres; nombres = otra.nombres;     otra.nombres = tmpNombres;
    int tmpCapacidad = capacidad;  capacidad = otra.capacidad; otra.capacidad = tmpCapacidad;
    int tmpTamano = tamano;        tamano = otra.tamano;       otra.tamano = tmpTamano;
}

// Constructor
TablaHashUsuarios::TablaHashUsuarios(int cap) : indices(nullptr), nombres(nullptr), capacidad(0), tamano(0) {
    reservar(cap < 4 ? 4 : cap);    // evita capacidades que no permiten ni una insercion
}

// Constructor de copia: copia profunda (la copia por defecto compartiria punteros)
TablaHashUsuarios::TablaHashUsuarios(const TablaHashUsuarios& otra)
    : indices(nullptr), nombres(nullptr), capacidad(0), tamano(0) {
    reservar(otra.capacidad);
    for (int i = 0; i < capacidad; i++) {
        MedidorRecursos::contarIteracion();
        indices[i] = otra.indices[i];
        if (otra.nombres[i] != nullptr) {
            nombres[i] = new Cadena(*otra.nombres[i]);
        }
    }
    tamano = otra.tamano;
}

TablaHashUsuarios& TablaHashUsuarios::operator=(const TablaHashUsuarios& otra) {
    if (this != &otra) {
        TablaHashUsuarios copia(otra);
        intercambiar(copia);        // copia libera lo anterior al salir de alcance
    }
    return *this;
}

// Destructor
TablaHashUsuarios::~TablaHashUsuarios() {
    liberar();
}

// Casilla donde esta la llave, o -1. El sondeo termina en la primera casilla vacia.
int TablaHashUsuarios::encontrarPosicion(const Cadena& nombre) const {
    int pos = static_cast<int>(nombre.hash() % static_cast<unsigned long>(capacidad));

    for (int pasos = 0; pasos < capacidad && indices[pos] != -1; pasos++) {
        MedidorRecursos::contarIteracion();
        if (*nombres[pos] == nombre) {
            return pos;
        }
        pos = (pos + 1) % capacidad;
    }
    return -1;
}

// Inserta una llave que NO existe, en la primera casilla libre del sondeo.
void TablaHashUsuarios::colocar(const Cadena& nombre, int idx) {
    int pos = static_cast<int>(nombre.hash() % static_cast<unsigned long>(capacidad));

    while (indices[pos] != -1) {
        MedidorRecursos::contarIteracion();
        pos = (pos + 1) % capacidad;
    }
    nombres[pos] = new Cadena(nombre);
    indices[pos] = idx;
    tamano++;
}

// Duplica la capacidad y vuelve a ubicar todas las llaves (rehash).
void TablaHashUsuarios::crecer() {
    TablaHashUsuarios nueva(capacidad * 2 + 1);
    for (int i = 0; i < capacidad; i++) {
        MedidorRecursos::contarIteracion();
        if (indices[i] != -1) {
            nueva.colocar(*nombres[i], indices[i]);
        }
    }
    intercambiar(nueva);            // nueva libera la tabla vieja al salir de alcance
}

bool TablaHashUsuarios::insertar(const Cadena& nombre, int idx) {
    if (idx < 0) {
        return false;
    }

    int existente = encontrarPosicion(nombre);
    if (existente >= 0) {
        indices[existente] = idx;   // llave repetida: se actualiza el indice
        return true;
    }

    if ((tamano + 1) * 2 > capacidad) {
        crecer();                   // mantiene la carga <= 50 %: nunca se rechaza un usuario
    }
    colocar(nombre, idx);
    return true;
}

// Busca el índice del usuario en la tabla
int TablaHashUsuarios::buscar(const Cadena& nombre) const {
    int pos = encontrarPosicion(nombre);
    return (pos < 0) ? -1 : indices[pos];
}

int TablaHashUsuarios::getTamano() const {
    return tamano;
}

int TablaHashUsuarios::getCapacidad() const {
    return capacidad;
}

long TablaHashUsuarios::bytesDinamicos() const {
    long bytes = static_cast<long>(capacidad) *
                 static_cast<long>(sizeof(int) + sizeof(Cadena*));
    for (int i = 0; i < capacidad; i++) {
        if (nombres[i] != nullptr) {
            bytes += static_cast<long>(sizeof(Cadena)) + nombres[i]->bytesDinamicos();
        }
    }
    return bytes;
}
