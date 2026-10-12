#ifndef USUARIO_H
#define USUARIO_H

#include "Cadena.h"

class Usuario {
private:
    Cadena nombreUsuario;
public:
    Usuario() {}
    Usuario(const Cadena& n) : nombreUsuario(n) {}
    Cadena getNombreUsuario() const { return nombreUsuario; }
};

#endif