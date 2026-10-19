// RedSocial.cpp
#include "RedSocial.h"
#include "MedidorRecursos.h"
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cstring>

using namespace std;

RedSocial::RedSocial()
    : usuarios(), mensajes(), relaciones(0), tabla(2003),
    usuarioActual(-1), ultimosAmigosMostrados(), refrescoUsado(false),
    mensajesAlCargar(0), huboCambioEstadoLeido(false),
    nuevasAmistadesA(), nuevasAmistadesB() {}

bool RedSocial::leerLineaArchivo(istream& entrada, Cadena& linea) {
    char buffer[1024];
    if (!entrada.getline(buffer, 1024)) {
        return false;
    }
    int len = 0;
    while (buffer[len] != '\0') {
        if (buffer[len] == '\r' || buffer[len] == '\n') {
            buffer[len] = '\0';
            break;
        }
        len++;
    }
    linea = Cadena(buffer);
    return true;
}

// ============================================================
// FUNCIONALIDAD I: CARGA Y PERSISTENCIA
// ============================================================

bool RedSocial::cargarDatos(const char* rutaUsuarios, const char* rutaAmistades,
                            const char* rutaSolicitudes, const char* rutaMensajes) {
    ifstream archUsuarios(rutaUsuarios);
    if (!archUsuarios.is_open()) {
        cout << "[Aviso] No se encontro " << rutaUsuarios << ". Iniciando red vacia." << endl;
        return false;
    }

    Cadena linea;
    // Formato esperado usuarios.txt: id;nombreUsuario;fechaNac;correo;foto;pass;ciudad;pais;fechaInsc
    while (leerLineaArchivo(archUsuarios, linea)) {
        MedidorRecursos::contarIteracion();
        if (linea.getLongitud() == 0) continue;
        if (linea.cantidadCampos(';') < 9) continue; // Ignora líneas malformadas

        int id = 0;
        Fecha fNac, fInsc;
        if (!linea.campo(0, ';').aEntero(id)) continue;
        Cadena nombre = linea.campo(1, ';');
        if (nombre.getLongitud() == 0) continue;
        if (!Fecha::desdeCadena(linea.campo(2, ';'), fNac)) fNac = Fecha(1, 1, 2000);
        Cadena correo = linea.campo(3, ';');
        Cadena foto = linea.campo(4, ';');
        Cadena pass = linea.campo(5, ';');
        Cadena ciudad = linea.campo(6, ';');
        Cadena pais = linea.campo(7, ';');
        if (!Fecha::desdeCadena(linea.campo(8, ';'), fInsc)) fInsc = Fecha(1, 1, 2024);

        Usuario u(id, nombre, fNac, correo, foto, pass, ciudad, pais, fInsc);
        usuarios.agregar(u);
        tabla.insertar(nombre, usuarios.getTamano() - 1);
    }
    archUsuarios.close();

    relaciones.redimensionar(usuarios.getTamano());

    // Cargar amistades.txt (Formato: idA;idB)
    ifstream archAmistades(rutaAmistades);
    if (archAmistades.is_open()) {
        while (leerLineaArchivo(archAmistades, linea)) {
            MedidorRecursos::contarIteracion();
            if (linea.cantidadCampos(';') < 2) continue;
            int a = -1, b = -1;
            if (linea.campo(0, ';').aEntero(a) && linea.campo(1, ';').aEntero(b)) {
                relaciones.agregarAmistad(a, b);
            }
        }
        archAmistades.close();
    }

    // Cargar solicitudes.txt (Formato: idRemitente;idReceptor)
    ifstream archSolicitudes(rutaSolicitudes);
    if (archSolicitudes.is_open()) {
        while (leerLineaArchivo(archSolicitudes, linea)) {
            MedidorRecursos::contarIteracion();
            if (linea.cantidadCampos(';') < 2) continue;
            int rem = -1, rec = -1;
            if (linea.campo(0, ';').aEntero(rem) && linea.campo(1, ';').aEntero(rec)) {
                if (rec >= 0 && rec < usuarios.getTamano() && rem >= 0 && rem < usuarios.getTamano()) {
                    usuarios[rec].agregarSolicitud(rem);
                }
            }
        }
        archSolicitudes.close();
    }

    // Cargar mensajes.txt (Formato: idEmisor;idReceptor;fecha;leido;texto)
    ifstream archMensajes(rutaMensajes);
    if (archMensajes.is_open()) {
        while (leerLineaArchivo(archMensajes, linea)) {
            MedidorRecursos::contarIteracion();
            if (linea.cantidadCampos(';') < 5) continue;
            int emisor = -1, receptor = -1, leidoInt = 0;
            Fecha fEnvio;
            if (!linea.campo(0, ';').aEntero(emisor) || !linea.campo(1, ';').aEntero(receptor)) continue;
            if (!Fecha::desdeCadena(linea.campo(2, ';'), fEnvio)) continue;
            linea.campo(3, ';').aEntero(leidoInt);
            Cadena texto = linea.desdeCampo(4, ';');

            if (emisor >= 0 && emisor < usuarios.getTamano() &&
                receptor >= 0 && receptor < usuarios.getTamano()) {
                Mensaje m(emisor, receptor, texto, fEnvio, leidoInt != 0);
                mensajes.agregar(m);
                int idxMsg = mensajes.getTamano() - 1;
                usuarios[emisor].registrarMensajeEnviado(idxMsg);
                usuarios[receptor].registrarMensajeRecibido(idxMsg);
            }
        }
        archMensajes.close();
    }

    mensajesAlCargar = mensajes.getTamano();
    cout << ">>> Usuarios cargados en memoria: " << usuarios.getTamano() << " <<<" << endl;
    return true;
}

void RedSocial::guardarCambiosRed(const char* rutaMensajes, const char* rutaAmistades,
                                  const char* rutaSolicitudes) {
    // 1. Si solo se agregaron mensajes nuevos y no cambiaron estados previos: append incremental
    if (!huboCambioEstadoLeido && mensajes.getTamano() > mensajesAlCargar) {
        ofstream archMsg(rutaMensajes, ios::app);
        if (archMsg.is_open()) {
            for (int i = mensajesAlCargar; i < mensajes.getTamano(); i++) {
                MedidorRecursos::contarIteracion();
                const Mensaje& m = mensajes[i];
                archMsg << m.getIdEmisor() << ";"
                        << m.getIdReceptor() << ";"
                        << m.getFechaEnvio() << ";"
                        << (m.estaLeido() ? 1 : 0) << ";"
                        << m.getTexto() << "\n";
            }
            archMsg.close();
            cout << "[OK] Mensajes nuevos agregados (append): " << (mensajes.getTamano() - mensajesAlCargar) << " registros." << endl;
        } else {
            cout << "[Error] No se pudo abrir (append) el archivo de mensajes: " << rutaMensajes << endl;
        }
    } else if (huboCambioEstadoLeido) {
        // -- Reescritura segura del archivo de mensajes usando archivo temporal --
        char tmpMsgPath[1024];
        if (snprintf(tmpMsgPath, sizeof(tmpMsgPath), "%s.tmp", rutaMensajes) < 0) {
            cout << "[Error] No se pudo construir nombre temporal para mensajes." << endl;
        } else {
            ofstream archTmp(tmpMsgPath, ios::trunc);
            if (archTmp.is_open()) {
                for (int i = 0; i < mensajes.getTamano(); i++) {
                    MedidorRecursos::contarIteracion();
                    const Mensaje& m = mensajes[i];
                    archTmp << m.getIdEmisor() << ";"
                            << m.getIdReceptor() << ";"
                            << m.getFechaEnvio() << ";"
                            << (m.estaLeido() ? 1 : 0) << ";"
                            << m.getTexto() << "\n";
                }
                archTmp.close();
                if (rename(tmpMsgPath, rutaMensajes) != 0) {
                    cout << "[Error] No se pudo reemplazar el archivo de mensajes: " << rutaMensajes << endl;
                    // intentar limpiar temporal
                    remove(tmpMsgPath);
                } else {
                    cout << "[OK] Archivo de mensajes actualizado correctamente (reescritura segura)." << endl;
                }
            } else {
                cout << "[Error] No se pudo crear archivo temporal para mensajes: " << tmpMsgPath << endl;
            }
        }
    }

    // 2. Escritura incremental (deltas) de nuevas amistades aceptadas (append)
    if (nuevasAmistadesA.getTamano() > 0) {
        ofstream archAmi(rutaAmistades, ios::app);
        if (archAmi.is_open()) {
            for (int i = 0; i < nuevasAmistadesA.getTamano(); i++) {
                MedidorRecursos::contarIteracion();
                archAmi << nuevasAmistadesA[i] << ";" << nuevasAmistadesB[i] << "\n";
            }
            archAmi.close();
            cout << "[OK] Nuevas amistades persistidas (append): " << nuevasAmistadesA.getTamano() << " parejas." << endl;
        } else {
            cout << "[Error] No se pudo abrir (append) el archivo de amistades: " << rutaAmistades << endl;
        }
    }

    // 3. Persistir estado vigente de solicitudes pendientes -> reescritura segura
    {
        char tmpSolPath[1024];
        if (snprintf(tmpSolPath, sizeof(tmpSolPath), "%s.tmp", rutaSolicitudes) < 0) {
            cout << "[Error] No se pudo construir nombre temporal para solicitudes." << endl;
        } else {
            ofstream archTmpS(tmpSolPath, ios::trunc);
            if (archTmpS.is_open()) {
                for (int u = 0; u < usuarios.getTamano(); u++) {
                    const ListaDinamica<int>& sols = usuarios[u].getSolicitudesRecibidas();
                    for (int s = 0; s < sols.getTamano(); s++) {
                        MedidorRecursos::contarIteracion();
                        archTmpS << sols[s] << ";" << u << "\n";
                    }
                }
                archTmpS.close();
                if (rename(tmpSolPath, rutaSolicitudes) != 0) {
                    cout << "[Error] No se pudo reemplazar el archivo de solicitudes: " << rutaSolicitudes << endl;
                    remove(tmpSolPath);
                } else {
                    cout << "[OK] Archivo de solicitudes actualizado correctamente (reescritura segura)." << endl;
                }
            } else {
                cout << "[Error] No se pudo crear archivo temporal para solicitudes: " << tmpSolPath << endl;
            }
        }
    }

    // Actualizar controles internos
    mensajesAlCargar = mensajes.getTamano();
    huboCambioEstadoLeido = false;
    nuevasAmistadesA.vaciar();
    nuevasAmistadesB.vaciar();
}

// ============================================================
// FUNCIONALIDAD II: INGRESO Y FOTOS PONDERADAS CON 1 REFRESCO
// ============================================================

bool RedSocial::iniciarSesion(const Cadena& u, const Cadena& c) {
    int idx = tabla.buscar(u);
    if (idx < 0 || idx >= usuarios.getTamano()) {
        return false;
    }
    if (!usuarios[idx].validarContrasena(c)) {
        return false;
    }
    usuarioActual = idx;
    ultimosAmigosMostrados.vaciar();
    refrescoUsado = false;
    return true;
}

void RedSocial::cerrarSesion() {
    usuarioActual = -1;
    ultimosAmigosMostrados.vaciar();
    refrescoUsado = false;
}

bool RedSocial::haySesionActiva() const { return usuarioActual >= 0; }
int RedSocial::getUsuarioActual() const { return usuarioActual; }
const Usuario& RedSocial::getUsuario(int idx) const { return usuarios[idx]; }

void RedSocial::mostrarFotosAmigos(bool esRefresco) {
    if (!haySesionActiva()) return;

    if (esRefresco && refrescoUsado) {
        cout << "[Aviso] Solo se permite una (1) unica actualizacion de fotos de perfil por sesion." << endl;
        return;
    }

    ListaDinamica<int> amigos = relaciones.obtenerAmigos(usuarioActual);
    if (amigos.estaVacia()) {
        cout << "[Info] Aun no tienes amigos agregados para mostrar fotos de perfil." << endl;
        return;
    }

    // Filtrar candidatos (si es refresco, excluir el grupo mostrado anteriormente si alcanzan los amigos)
    ListaDinamica<int> candidatos;
    for (int i = 0; i < amigos.getTamano(); i++) {
        MedidorRecursos::contarIteracion();
        int idAmigo = amigos[i];
        bool yaMostrado = false;
        if (esRefresco && amigos.getTamano() > ultimosAmigosMostrados.getTamano()) {
            for (int j = 0; j < ultimosAmigosMostrados.getTamano(); j++) {
                MedidorRecursos::contarIteracion();
                if (ultimosAmigosMostrados[j] == idAmigo) {
                    yaMostrado = true;
                    break;
                }
            }
        }
        if (!yaMostrado) {
            candidatos.agregar(idAmigo);
        }
    }

    if (esRefresco) {
        refrescoUsado = true;
        ultimosAmigosMostrados.vaciar();
    }

    int aMostrar = (candidatos.getTamano() < 3) ? candidatos.getTamano() : 3;
    cout << "\n";

    bool* elegido = new bool[candidatos.getTamano()];
    for (int i = 0; i < candidatos.getTamano(); i++) elegido[i] = false;

    for (int k = 0; k < aMostrar; k++) {
        // Prioridad exacta según enunciado (escalada x2 para usar enteros):
        // <= 10 amigos en común -> 1.0x (peso 2)
        // 11 a 20 amigos en común -> 2.5x (peso 5)
        // > 20 amigos en común -> 3.5x (peso 7)
        long sumaPesos = 0;
        for (int i = 0; i < candidatos.getTamano(); i++) {
            MedidorRecursos::contarIteracion();
            if (!elegido[i]) {
                int comunes = relaciones.contarEnComun(usuarioActual, candidatos[i]);
                int peso = 2;
                if (comunes >= 11 && comunes <= 20) {
                    peso = 5;
                } else if (comunes > 20) {
                    peso = 7;
                }
                sumaPesos += peso;
            }
        }
        if (sumaPesos <= 0) break;

        long tiro = rand() % sumaPesos;
        long acumulado = 0;
        int idxGanador = -1;

        for (int i = 0; i < candidatos.getTamano(); i++) {
            MedidorRecursos::contarIteracion();
            if (!elegido[i]) {
                int comunes = relaciones.contarEnComun(usuarioActual, candidatos[i]);
                int peso = 2;
                if (comunes >= 11 && comunes <= 20) {
                    peso = 5;
                } else if (comunes > 20) {
                    peso = 7;
                }
                acumulado += peso;
                if (tiro < acumulado) {
                    idxGanador = i;
                    break;
                }
            }
        }

        if (idxGanador >= 0) {
            elegido[idxGanador] = true;
            int idAmi = candidatos[idxGanador];
            ultimosAmigosMostrados.agregar(idAmi);
            cout << "Foto perfil de amigo " << (k + 1) << ": "
                 << usuarios[idAmi].getRutaFotoPerfil()
                 << " (@" << usuarios[idAmi].getNombreUsuario() << ")" << endl;
        }
    }
    delete[] elegido;
}

// ============================================================
