#ifndef BUSCADORUSUARIOS_H
#define BUSCADORUSUARIOS_H

#include "ListaDinamica.h"
#include "Usuario.h"
#include "Cadena.h"

// Busqueda binaria por nombre. Es la ALTERNATIVA DESCARTADA del informe (la elegida es
// TablaHashUsuarios): se conserva para comparar iteraciones. No usarla dentro de RedSocial.
class BuscadorUsuarios {
public:
    BuscadorUsuarios(); // Constructor

    // Precondicion: la lista debe estar ORDENADA por nombre de usuario (orden de Cadena::operator<).
    // Con la lista en el orden del archivo (por id) el resultado no es fiable.
    // Recibe la lista por referencia constante para no duplicar memoria.
    int buscarPorNombre(const ListaDinamica<Usuario>& usuarios, const Cadena& nombreBuscado) const;
};

#endif // BUSCADORUSUARIOS_H
