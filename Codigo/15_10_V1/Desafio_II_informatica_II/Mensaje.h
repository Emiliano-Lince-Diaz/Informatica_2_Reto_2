// Mensaje.h
#ifndef MENSAJE_H
#define MENSAJE_H

#include "Cadena.h"
#include "Fecha.h"

class Mensaje {
private:
    int idEmisor;
    int idReceptor;
    Cadena texto;
    Fecha fechaEnvio;
    bool leido;

public:
    Mensaje();
    Mensaje(int emisor, int receptor, const Cadena& txt, const Fecha& fecha, bool estaLeido = false);
    Mensaje(const Mensaje& otro);

    int getIdEmisor() const;
    int getIdReceptor() const;
    const Cadena& getTexto() const;
    const Fecha& getFechaEnvio() const;
    bool estaLeido() const;
    void marcarLeido();

    long bytesDinamicos() const;
};

#endif // MENSAJE_H