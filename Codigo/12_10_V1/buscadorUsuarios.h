#ifndef BUSCADORUSUARIOS_H
#define BUSCADORUSUARIOS_H

#include "ListaDinamica.h"
#include "Usuario.h"
#include "Cadena.h"

class BuscadorUsuarios {
public:
    BuscadorUsuarios(); // Constructor

    // Recibe la lista por referencia constante para no duplicar memoria
    int buscarPorNombre(const ListaDinamica<Usuario>& usuarios, const Cadena& nombreBuscado) const;
};

#endif // BUSCADORUSUARIOS_H
