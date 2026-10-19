// Album.h
#ifndef ALBUM_H
#define ALBUM_H

#include "Cadena.h"
#include "Foto.h"
#include "ListaDinamica.h"

using namespace std;

class Album {
private:
    Cadena nombre;
    Cadena ruta;
    int privacidad; // 0 = Publico, 1 = Amigos, 2 = Privado
    ListaDinamica<Foto> fotos;

public:
    Album();
    Album(const Cadena& n, const Cadena& r, int priv);
    Album(const Album& otra);

    const Cadena& getNombre() const;
    const Cadena& getRuta() const;
    int getPrivacidad() const;
    int getCantidadFotos() const;

    // Referencia modificable (para dar/quitar like en el carrusel) y constante
    Foto& getFoto(int i);
    const Foto& getFoto(int i) const;

    void agregarFoto(const Foto& f);
    bool esVisiblePara(bool esAmigo) const;

    long bytesDinamicos() const;
};

#endif // ALBUM_H