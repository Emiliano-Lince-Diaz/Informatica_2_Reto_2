// Album.cpp
#include "Album.h"

Album::Album() : nombre(""), ruta(""), privacidad(0), fotos() {}

Album::Album(const Cadena& n, const Cadena& r, int priv)
    : nombre(n), ruta(r), privacidad(priv), fotos() {}

Album::Album(const Album& otra)
    : nombre(otra.nombre), ruta(otra.ruta), privacidad(otra.privacidad), fotos(otra.fotos) {}

const Cadena& Album::getNombre() const {
    return nombre;
}

int Album::getPrivacidad() const {
    return privacidad;
}

int Album::getCantidadFotos() const {
    return fotos.getTamano();
}

Foto Album::getFoto(int i) const {
    return fotos[i];
}

void Album::agregarFoto(const Foto& f) {
    fotos.agregar(f);
}

bool Album::esVisiblePara(bool esAmigo) const {
    return privacidad == 0 || (privacidad == 1 && esAmigo);
}

long Album::bytesDinamicos() const {
    long b = nombre.bytesDinamicos() + ruta.bytesDinamicos() + fotos.bytesDinamicos();
    for (int i = 0; i < fotos.getTamano(); i++) {
        b += fotos[i].bytesDinamicos();
    }
    return b;
}