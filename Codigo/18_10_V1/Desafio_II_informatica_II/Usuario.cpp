// Usuario.cpp
#include "Usuario.h"
#include "MedidorRecursos.h"

using namespace std;

Usuario::Usuario() : id(-1), nombreUsuario(""), fechaNacimiento(), correo(""),
    rutaFotoPerfil(""), contrasena(""), ciudad(""), pais(""), fechaInscripcion() {}

Usuario::Usuario(const Cadena& n) : id(-1), nombreUsuario(n), fechaNacimiento(), correo(""),
    rutaFotoPerfil(""), contrasena(""), ciudad(""), pais(""), fechaInscripcion() {}

Usuario::Usuario(int idUsuario, const Cadena& nombre, const Fecha& nacimiento,
                 const Cadena& mail, const Cadena& foto, const Cadena& pass,
                 const Cadena& ciu, const Cadena& pa, const Fecha& inscripcion)
    : id(idUsuario), nombreUsuario(nombre), fechaNacimiento(nacimiento),
    correo(mail), rutaFotoPerfil(foto), contrasena(pass),
    ciudad(ciu), pais(pa), fechaInscripcion(inscripcion) {}

Usuario::Usuario(const Usuario& otro)
    : id(otro.id), nombreUsuario(otro.nombreUsuario), fechaNacimiento(otro.fechaNacimiento),
    correo(otro.correo), rutaFotoPerfil(otro.rutaFotoPerfil), contrasena(otro.contrasena),
    ciudad(otro.ciudad), pais(otro.pais), fechaInscripcion(otro.fechaInscripcion),
    albumes(otro.albumes), mensajesEnviados(otro.mensajesEnviados),
    mensajesRecibidos(otro.mensajesRecibidos), solicitudesRecibidas(otro.solicitudesRecibidas) {}

int Usuario::getId() const { return id; }
const Cadena& Usuario::getNombreUsuario() const { return nombreUsuario; }
const Cadena& Usuario::getRutaFotoPerfil() const { return rutaFotoPerfil; }
const Cadena& Usuario::getCiudad() const { return ciudad; }
const Cadena& Usuario::getPais() const { return pais; }

bool Usuario::validarContrasena(const Cadena& c) const {
    return contrasena == c;
}

void Usuario::agregarAlbum(const Album& a) {
    albumes.agregar(a);
}

void Usuario::agregarSolicitud(int idRemitente) {
    if (!tieneSolicitudDe(idRemitente)) {
        solicitudesRecibidas.agregar(idRemitente);
    }
}

void Usuario::quitarSolicitud(int idRemitente) {
    ListaDinamica<int> nuevas;
    for (int i = 0; i < solicitudesRecibidas.getTamano(); i++) {
        MedidorRecursos::contarIteracion();
        if (solicitudesRecibidas[i] != idRemitente) {
            nuevas.agregar(solicitudesRecibidas[i]);
        }
    }
    solicitudesRecibidas = nuevas;
}

bool Usuario::tieneSolicitudDe(int idRemitente) const {
    for (int i = 0; i < solicitudesRecibidas.getTamano(); i++) {
        MedidorRecursos::contarIteracion();
        if (solicitudesRecibidas[i] == idRemitente) {
            return true;
        }
    }
    return false;
}

void Usuario::registrarMensajeEnviado(int idMensaje) {
    mensajesEnviados.agregar(idMensaje);
}

void Usuario::registrarMensajeRecibido(int idMensaje) {
    mensajesRecibidos.agregar(idMensaje);
}

const ListaDinamica<int>& Usuario::getMensajesEnviados() const { return mensajesEnviados; }
const ListaDinamica<int>& Usuario::getMensajesRecibidos() const { return mensajesRecibidos; }
const ListaDinamica<int>& Usuario::getSolicitudesRecibidas() const { return solicitudesRecibidas; }

ListaDinamica<Album>& Usuario::getAlbumes() { return albumes; }
const ListaDinamica<Album>& Usuario::getAlbumes() const { return albumes; }

long Usuario::bytesDinamicos() const {
    long total = nombreUsuario.bytesDinamicos() +
                 correo.bytesDinamicos() +
                 rutaFotoPerfil.bytesDinamicos() +
                 contrasena.bytesDinamicos() +
                 ciudad.bytesDinamicos() +
                 pais.bytesDinamicos() +
                 albumes.bytesDinamicos() +
                 mensajesEnviados.bytesDinamicos() +
                 mensajesRecibidos.bytesDinamicos() +
                 solicitudesRecibidas.bytesDinamicos();

    // Se recorre toda la capacidad porque cada slot no usado tambien tiene un Album construido por defecto
    for (int i = 0; i < albumes.getCapacidad(); i++) {
        total += albumes.getElementoCapacidad(i).bytesDinamicos();
    }
    return total;
}