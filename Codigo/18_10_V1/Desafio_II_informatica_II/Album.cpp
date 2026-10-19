// Album.cpp
#include "Album.h"

using namespace std;

Album::Album() : nombre(""), ruta(""), privacidad(0), fotos() {}

Album::Album(const Cadena& n, const Cadena& r, int priv)
    : nombre(n), ruta(r), privacidad(priv), fotos() {}

Album::Album(const Album& otra)
    : nombre(otra.nombre), ruta(otra.ruta), privacidad(otra.privacidad), fotos(otra.fotos) {}

const Cadena& Album::getNombre() const {
    return nombre;
}

const Cadena& Album::getRuta() const {
    return ruta;
}

int Album::getPrivacidad() const {
    return privacidad;
}

int Album::getCantidadFotos() const {
    return fotos.getTamano();
}

Foto& Album::getFoto(int i) {
    return fotos[i];
}

const Foto& Album::getFoto(int i) const {
    return fotos[i];
}

void Album::agregarFoto(const Foto& f) {
    fotos.agregar(f);
}

bool Album::esVisiblePara(bool esAmigo) const {
    // 0: Publico, 1: Amigos, 2: Privado
    return privacidad == 0 || (privacidad == 1 && esAmigo);
}

long Album::bytesDinamicos() const {
    long b = nombre.bytesDinamicos() + ruta.bytesDinamicos() + fotos.bytesDinamicos();
    // Recorre toda la capacidad porque new Foto[capacidad] construye por defecto todos los slots
    for (int i = 0; i < fotos.getCapacidad(); i++) {
        b += fotos.getElementoCapacidad(i).bytesDinamicos();
    }
    return b;
}