// Foto.cpp
#include "Foto.h"

Foto::Foto() : fechaCaptura(), tamano(0), ruta(""), likes(0) {}

Foto::Foto(const Fecha& f, int tam, const Cadena& r, int l)
    : fechaCaptura(f), tamano(tam), ruta(r), likes(l) {}

Foto::Foto(const Foto& otra)
    : fechaCaptura(otra.fechaCaptura), tamano(otra.tamano), ruta(otra.ruta), likes(otra.likes) {}

const Fecha& Foto::getFechaCaptura() const {
    return fechaCaptura;
}

int Foto::getTamano() const {
    return tamano;
}

const Cadena& Foto::getRuta() const {
    return ruta;
}

int Foto::getLikes() const {
    return likes;
}

void Foto::darLike() {
    ++likes;
}

void Foto::quitarLike() {
    if (likes > 0) --likes;
}

long Foto::bytesDinamicos() const {
    return ruta.bytesDinamicos();
}
