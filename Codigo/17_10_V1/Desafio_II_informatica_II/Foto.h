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
    Foto();
    Foto(const Fecha& f, int tam, const Cadena& r, int l = 0);
    Foto(const Foto& otra);

    const Cadena& getRuta() const;
    int getLikes() const;
    void darLike();
    void quitarLike();
    long bytesDinamicos() const;
};

#endif // FOTO_H