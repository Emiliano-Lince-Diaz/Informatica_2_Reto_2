#include "BuscadorUsuarios.h"
#include "MedidorRecursos.h"

BuscadorUsuarios::BuscadorUsuarios() {
    // Constructor vacío
}

int BuscadorUsuarios::buscarPorNombre(const ListaDinamica<Usuario>& usuarios, const Cadena& nombreBuscado) const {
    int inicio = 0;
    int fin = usuarios.getTamano() - 1;

    while (inicio <= fin) {
        MedidorRecursos::contarIteracion();
        int medio = inicio + (fin - inicio) / 2;
        const Cadena& actual = usuarios[medio].getNombreUsuario(); // sin copias

        if (actual == nombreBuscado) {
            return medio; // Retorna el índice exacto
        }
        if (actual < nombreBuscado) {
            inicio = medio + 1;
        } else {
            fin = medio - 1;
        }
    }
    return -1; // Retorna -1 si el usuario no existe
}
