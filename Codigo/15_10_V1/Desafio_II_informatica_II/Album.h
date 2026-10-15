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
    Album() : nombre(""), ruta(""), privacidad(0), fotos() {}
    Album(const Cadena& n, const Cadena& r, int priv)
        : nombre(n), ruta(r), privacidad(priv), fotos() {}
    Album(const Album& otra)
        : nombre(otra.nombre), ruta(otra.ruta), privacidad(otra.privacidad), fotos(otra.fotos) {}

    const Cadena& getNombre() const { return nombre; }
    int getPrivacidad() const { return privacidad; }
    int getCantidadFotos() const { return fotos.getTamano(); }
    Foto getFoto(int i) const { return fotos[i]; }
    void agregarFoto(const Foto& f) { fotos.agregar(f); }
    bool esVisiblePara(bool esAmigo) const { return privacidad == 0 || (privacidad == 1 && esAmigo); }

    long bytesDinamicos() const {
        long b = nombre.bytesDinamicos() + ruta.bytesDinamicos() + fotos.bytesDinamicos();
        for (int i = 0; i < fotos.getTamano(); i++) {
            b += fotos[i].bytesDinamicos();
        }
        return b;
    }
};

#endif // ALBUM_H