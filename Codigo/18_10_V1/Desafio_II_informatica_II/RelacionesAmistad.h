// RelacionesAmistad.h
#ifndef RELACIONESAMISTAD_H
#define RELACIONESAMISTAD_H

#include "ListaDinamica.h"

class RelacionesAmistad {
private:
    unsigned long long** matrizBits;
    int cantidadUsuarios;
    int bloquesPorFila;

    void liberar();
    void copiarDesde(const RelacionesAmistad& otra);

public:
    RelacionesAmistad(int n = 0);
    RelacionesAmistad(const RelacionesAmistad& otra);
    ~RelacionesAmistad();
    RelacionesAmistad& operator=(const RelacionesAmistad& otra);

    void redimensionar(int nuevoN);
    void agregarAmistad(int a, int b);
    bool sonAmigos(int a, int b) const;
    int contarAmigos(int a) const;
    int contarEnComun(int a, int b) const;
    ListaDinamica<int> obtenerAmigos(int a) const;

    int getCantidadUsuarios() const;
    long bytesDinamicos() const;
};

#endif // RELACIONESAMISTAD_H