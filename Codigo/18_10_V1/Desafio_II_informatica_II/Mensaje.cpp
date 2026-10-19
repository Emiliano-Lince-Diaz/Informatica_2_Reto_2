// Mensaje.cpp
#include "Mensaje.h"

Mensaje::Mensaje() : idEmisor(-1), idReceptor(-1), texto(""), fechaEnvio(), leido(false) {}

Mensaje::Mensaje(int emisor, int receptor, const Cadena& txt, const Fecha& fecha, bool estaLeido)
    : idEmisor(emisor), idReceptor(receptor), texto(txt), fechaEnvio(fecha), leido(estaLeido) {}

Mensaje::Mensaje(const Mensaje& otro)
    : idEmisor(otro.idEmisor), idReceptor(otro.idReceptor),
    texto(otro.texto), fechaEnvio(otro.fechaEnvio), leido(otro.leido) {}

int Mensaje::getIdEmisor() const { return idEmisor; }
int Mensaje::getIdReceptor() const { return idReceptor; }
const Cadena& Mensaje::getTexto() const { return texto; }
const Fecha& Mensaje::getFechaEnvio() const { return fechaEnvio; }
bool Mensaje::estaLeido() const { return leido; }
void Mensaje::marcarLeido() { leido = true; }

long Mensaje::bytesDinamicos() const {
    return texto.bytesDinamicos();
}