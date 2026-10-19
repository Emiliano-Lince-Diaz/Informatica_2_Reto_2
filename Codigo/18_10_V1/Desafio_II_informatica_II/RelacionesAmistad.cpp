// RelacionesAmistad.cpp
#include "RelacionesAmistad.h"
#include "MedidorRecursos.h"

RelacionesAmistad::RelacionesAmistad(int n)
    : matrizBits(nullptr), cantidadUsuarios(0), bloquesPorFila(0) {
    if (n > 0) {
        redimensionar(n);
    }
}

RelacionesAmistad::RelacionesAmistad(const RelacionesAmistad& otra)
    : matrizBits(nullptr), cantidadUsuarios(0), bloquesPorFila(0) {
    copiarDesde(otra);
}

RelacionesAmistad::~RelacionesAmistad() {
    liberar();
}

RelacionesAmistad& RelacionesAmistad::operator=(const RelacionesAmistad& otra) {
    if (this != &otra) {
        liberar();
        copiarDesde(otra);
    }
    return *this;
}

void RelacionesAmistad::liberar() {
    if (matrizBits != nullptr) {
        for (int i = 0; i < cantidadUsuarios; i++) {
            delete[] matrizBits[i];
        }
        delete[] matrizBits;
        matrizBits = nullptr;
    }
    cantidadUsuarios = 0;
    bloquesPorFila = 0;
}

void RelacionesAmistad::copiarDesde(const RelacionesAmistad& otra) {
    if (otra.cantidadUsuarios <= 0) return;
    cantidadUsuarios = otra.cantidadUsuarios;
    bloquesPorFila = otra.bloquesPorFila;
    matrizBits = new unsigned long long*[cantidadUsuarios];
    for (int i = 0; i < cantidadUsuarios; i++) {
        matrizBits[i] = new unsigned long long[bloquesPorFila];
        for (int b = 0; b < bloquesPorFila; b++) {
            MedidorRecursos::contarIteracion();
            matrizBits[i][b] = otra.matrizBits[i][b];
        }
    }
}

void RelacionesAmistad::redimensionar(int nuevoN) {
    if (nuevoN <= 0) {
        liberar();
        return;
    }
    int nuevosBloques = (nuevoN + 63) / 64;
    unsigned long long** nuevaMatriz = new unsigned long long*[nuevoN];
    for (int i = 0; i < nuevoN; i++) {
        nuevaMatriz[i] = new unsigned long long[nuevosBloques];
        for (int b = 0; b < nuevosBloques; b++) {
            MedidorRecursos::contarIteracion();
            nuevaMatriz[i][b] = 0ULL;
        }
    }

    int minUsuarios = (cantidadUsuarios < nuevoN) ? cantidadUsuarios : nuevoN;
    int minBloques = (bloquesPorFila < nuevosBloques) ? bloquesPorFila : nuevosBloques;
    for (int i = 0; i < minUsuarios; i++) {
        for (int b = 0; b < minBloques; b++) {
            MedidorRecursos::contarIteracion();
            nuevaMatriz[i][b] = matrizBits[i][b];
        }
    }

    liberar();
    matrizBits = nuevaMatriz;
    cantidadUsuarios = nuevoN;
    bloquesPorFila = nuevosBloques;
}

void RelacionesAmistad::agregarAmistad(int a, int b) {
    if (a < 0 || b < 0 || a >= cantidadUsuarios || b >= cantidadUsuarios || a == b) {
        return;
    }
    matrizBits[a][b / 64] |= (1ULL << (b % 64));
    matrizBits[b][a / 64] |= (1ULL << (a % 64));
}

bool RelacionesAmistad::sonAmigos(int a, int b) const {
    if (a < 0 || b < 0 || a >= cantidadUsuarios || b >= cantidadUsuarios) {
        return false;
    }
    return ((matrizBits[a][b / 64] >> (b % 64)) & 1ULL) != 0ULL;
}

int RelacionesAmistad::contarAmigos(int a) const {
    if (a < 0 || a >= cantidadUsuarios) return 0;
    int total = 0;
    for (int b = 0; b < bloquesPorFila; b++) {
        unsigned long long bloque = matrizBits[a][b];
        // Algoritmo de Brian Kernighan: solo itera por cada bit encendido (1)
        while (bloque != 0ULL) {
            MedidorRecursos::contarIteracion();
            bloque &= (bloque - 1ULL);
            total++;
        }
    }
    return total;
}

int RelacionesAmistad::contarEnComun(int a, int b) const {
    if (a < 0 || b < 0 || a >= cantidadUsuarios || b >= cantidadUsuarios || a == b) {
        return 0;
    }
    int comun = 0;
    for (int k = 0; k < bloquesPorFila; k++) {
        MedidorRecursos::contarIteracion();
        unsigned long long interseccion = matrizBits[a][k] & matrizBits[b][k];
        while (interseccion != 0ULL) {
            MedidorRecursos::contarIteracion();
            interseccion &= (interseccion - 1ULL);
            comun++;
        }
    }
    return comun;
}

ListaDinamica<int> RelacionesAmistad::obtenerAmigos(int a) const {
    ListaDinamica<int> amigos;
    if (a < 0 || a >= cantidadUsuarios) return amigos;

    for (int b = 0; b < bloquesPorFila; b++) {
        unsigned long long bloque = matrizBits[a][b];
        if (bloque == 0ULL) {
            MedidorRecursos::contarIteracion();
            continue;
        }
        for (int bit = 0; bit < 64; bit++) {
            int idAmigo = b * 64 + bit;
            if (idAmigo >= cantidadUsuarios) break;
            MedidorRecursos::contarIteracion();
            if ((bloque >> bit) & 1ULL) {
                amigos.agregar(idAmigo);
            }
        }
    }
    return amigos;
}

int RelacionesAmistad::getCantidadUsuarios() const {
    return cantidadUsuarios;
}

long RelacionesAmistad::bytesDinamicos() const {
    if (matrizBits == nullptr) return 0;
    long ptrs = static_cast<long>(cantidadUsuarios) * static_cast<long>(sizeof(unsigned long long*));
    long datos = static_cast<long>(cantidadUsuarios) * static_cast<long>(bloquesPorFila) *
                 static_cast<long>(sizeof(unsigned long long));
    return ptrs + datos;
}