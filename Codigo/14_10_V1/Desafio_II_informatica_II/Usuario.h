#ifndef USUARIO_H
#define USUARIO_H

#include "Cadena.h"

class Usuario {
private:
    Cadena nombreUsuario;
public:
    Usuario() {}
    Usuario(const Cadena& n) : nombreUsuario(n) {}

    // Referencia constante: evita copiar (y reservar memoria) cada vez que se compara el nombre.
    const Cadena& getNombreUsuario() const { return nombreUsuario; }

    // Memoria dinamica que este usuario mantiene en el heap (para MedidorRecursos).
    long bytesDinamicos() const { return nombreUsuario.bytesDinamicos(); }
};

#endif
