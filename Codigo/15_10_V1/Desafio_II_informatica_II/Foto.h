// Foto.h
#ifndef FOTO_H
#define FOTO_H

#include "Fecha.h"
#include "Cadena.h"

class Foto {
private:
    Fecha fechaCaptura;
    int tamano;
    Cadena ruta;
    int likes;

public:
    Foto() : fechaCaptura(), tamano(0), ruta(""), likes(0) {}
    Foto(const Fecha& f, int tam, const Cadena& r, int l = 0)
        : fechaCaptura(f), tamano(tam), ruta(r), likes(l) {}
    Foto(const Foto& otra)
        : fechaCaptura(otra.fechaCaptura), tamano(otra.tamano), ruta(otra.ruta), likes(otra.likes) {}

    const Cadena& getRuta() const { return ruta; }
    int getLikes() const { return likes; }
    void darLike() { likes++; }
    void quitarLike() { if (likes > 0) likes--; }
    long bytesDinamicos() const { return ruta.bytesDinamicos(); }
};

#endif // FOTO_H