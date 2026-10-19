// RedSocial.h
#ifndef REDSOCIAL_H
#define REDSOCIAL_H

#include "ListaDinamica.h"
#include "Usuario.h"
#include "Mensaje.h"
#include "RelacionesAmistad.h"
#include "TablaHashUsuarios.h"
#include "Fecha.h"
#include "Cadena.h"

using namespace std;

class RedSocial {
private:
    ListaDinamica<Usuario> usuarios;
    ListaDinamica<Mensaje> mensajes;
    RelacionesAmistad relaciones;
    TablaHashUsuarios tabla;
    int usuarioActual;

    // Control para Funcionalidad II (un solo refresco con grupo distinto)
    ListaDinamica<int> ultimosAmigosMostrados;
    bool refrescoUsado;

    // Control de persistencia incremental (Persona A y Persona B)
    int mensajesAlCargar;
    bool huboCambioEstadoLeido;
    bool huboCambioLikes;
    ListaDinamica<int> nuevasAmistadesA;
    ListaDinamica<int> nuevasAmistadesB;

    bool leerLineaArchivo(istream& entrada, Cadena& linea);

public:
    RedSocial();

    // --- Funcionalidad I: Carga y Persistencia (delimitador '|') ---
    bool cargarDatos(const char* rutaUsuarios = "usuarios.txt",
                     const char* rutaAmistades = "amistades.txt",
                     const char* rutaSolicitudes = "solicitudes.txt",
                     const char* rutaAlbumes = "albumes.txt",
                     const char* rutaFotos = "fotos.txt",
                     const char* rutaMensajes = "mensajes.txt");
    void guardarCambiosRed(const char* rutaMensajes = "mensajes.txt",
                           const char* rutaAmistades = "amistades.txt",
                           const char* rutaSolicitudes = "solicitudes.txt",
                           const char* rutaFotos = "fotos.txt");

    // --- Funcionalidad II: Ingreso y Fotos Ponderadas ---
    bool iniciarSesion(const Cadena& u, const Cadena& c);
    void cerrarSesion();
    bool haySesionActiva() const;
    int getUsuarioActual() const;
    const Usuario& getUsuario(int idx) const;
    void mostrarFotosAmigos(bool esRefresco);

    // --- Funcionalidad III: Administrar Amistades (Persona A) ---
    bool enviarSolicitud(const Cadena& destino);
    void aceptarSolicitud(int idRemitente);

    // --- Funcionalidad IV: Visitar Perfil y Carrusel Circular (Persona A) ---
    void visitarPerfilAmigo(const Cadena& amigo);

    // --- Funcionalidad V: Top-K Sugerencias (Persona B) ---
    void sugerirAmigos(int k);

    // --- Funcionalidad VI: Mensajería (Persona B) ---
    bool enviarMensaje(const Cadena& amigo, const Cadena& texto, const Fecha& fechaActual = Fecha(15, 10, 2026));
    void leerMensajesNuevos();
    void buscarMensajesPorFecha(const Fecha& ini, const Fecha& fin);

    // --- Consulta para MedidorRecursos (Funcionalidad VII) ---
    long bytesDinamicos() const;
};

#endif // REDSOCIAL_H