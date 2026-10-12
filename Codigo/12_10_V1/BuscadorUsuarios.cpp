#include "BuscadorUsuarios.h"

BuscadorUsuarios::BuscadorUsuarios() {
    // Constructor vacío
}

int BuscadorUsuarios::buscarPorNombre(const ListaDinamica<Usuario>& usuarios, const Cadena& nombreBuscado) const {
    int inicio = 0;
    int fin = usuarios.getTamano() - 1;

    while (inicio <= fin) {
        int medio = inicio + (fin - inicio) / 2;

        if (usuarios[medio].getNombreUsuario() == nombreBuscado) {
            return medio; // Retorna el índice exacto
        }
        if (usuarios[medio].getNombreUsuario() < nombreBuscado) {
            inicio = medio + 1;
        } else {
            fin = medio - 1;
        }
    }
    return -1; // Retorna -1 si el usuario no existe
}