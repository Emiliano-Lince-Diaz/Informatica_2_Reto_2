// Album.h
#ifndef ALBUM_H
#define ALBUM_H

#include "Cadena.h"
#include "Foto.h"
#include "ListaDinamica.h"

class Album {
private:
    Cadena nombre;
    Cadena ruta;
    int privacidad;
    ListaDinamica<Foto> fotos;

public:
    Album();
    Album(const Cadena& n, const Cadena& r, int priv);
    Album(const Album& otra);

    const Cadena& getNombre() const;
    int getPrivacidad() const;
    int getCantidadFotos() const;
    Foto getFoto(int i) const;
    void agregarFoto(const Foto& f);
    bool esVisiblePara(bool esAmigo) const;

    long bytesDinamicos() const;
};

#endif // ALBUM_H