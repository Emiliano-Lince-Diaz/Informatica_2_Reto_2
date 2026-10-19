// Usuario.h
#ifndef USUARIO_H
#define USUARIO_H

#include "Cadena.h"
#include "Fecha.h"
#include "ListaDinamica.h"
#include "Album.h"

class Usuario {
private:
    int id;
    Cadena nombreUsuario;
    Fecha fechaNacimiento;
    Cadena correo;
    Cadena rutaFotoPerfil;
    Cadena contrasena;
    Cadena ciudad;
    Cadena pais;
    Fecha fechaInscripcion;
    ListaDinamica<Album> albumes;
    ListaDinamica<int> mensajesEnviados;
    ListaDinamica<int> mensajesRecibidos;
    ListaDinamica<int> solicitudesRecibidas;

public:
    Usuario();
    Usuario(const Cadena& n);
    Usuario(int idUsuario, const Cadena& nombre, const Fecha& nacimiento,
            const Cadena& mail, const Cadena& foto, const Cadena& pass,
            const Cadena& ciu, const Cadena& pa, const Fecha& inscripcion);
    Usuario(const Usuario& otro);

    int getId() const;
    const Cadena& getNombreUsuario() const;
    const Cadena& getRutaFotoPerfil() const;
    const Cadena& getCiudad() const;
    const Cadena& getPais() const;
    bool validarContrasena(const Cadena& c) const;

    void agregarAlbum(const Album& a);
    void agregarSolicitud(int idRemitente);
    void quitarSolicitud(int idRemitente);
    bool tieneSolicitudDe(int idRemitente) const;

    void registrarMensajeEnviado(int idMensaje);
    void registrarMensajeRecibido(int idMensaje);

    const ListaDinamica<int>& getMensajesEnviados() const;
    const ListaDinamica<int>& getMensajesRecibidos() const;
    const ListaDinamica<int>& getSolicitudesRecibidas() const;
    const ListaDinamica<Album>& getAlbumes() const;
    ListaDinamica<Album>& getAlbumesModificables();

    long bytesDinamicos() const;
};

#endif // USUARIO_H
