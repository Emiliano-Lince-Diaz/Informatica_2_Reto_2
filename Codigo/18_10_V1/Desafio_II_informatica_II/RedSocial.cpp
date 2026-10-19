// RedSocial.cpp
#include "RedSocial.h"
#include "MedidorRecursos.h"
#include <fstream>
#include <cstdlib>

using namespace std;

RedSocial::RedSocial()
    : usuarios(), mensajes(), relaciones(0), tabla(2003),
    usuarioActual(-1), ultimosAmigosMostrados(), refrescoUsado(false),
    mensajesAlCargar(0), huboCambioEstadoLeido(false), huboCambioLikes(false),
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
                            const char* rutaSolicitudes, const char* rutaAlbumes,
                            const char* rutaFotos, const char* rutaMensajes) {
    // Registro de parametros por valor (6 punteros const char*) y buffer de lectura
    MedidorRecursos::registrarMemoriaLocal(
        static_cast<long>(6 * sizeof(const char*) + 1024 * sizeof(char) + sizeof(ifstream))
        );

    ifstream archUsuarios(rutaUsuarios);
    if (!archUsuarios.is_open()) {
        cout << "[Aviso] No se encontro " << rutaUsuarios << ". Iniciando red vacia." << endl;
        return false;
    }

    Cadena linea;
    // 1. Cargar usuarios.txt (Formato: id|nombreUsuario|fechaNac|correo|foto|pass|ciudad|pais|fechaInsc)
    while (leerLineaArchivo(archUsuarios, linea)) {
        MedidorRecursos::contarIteracion();
        if (linea.getLongitud() == 0) continue;
        if (linea.cantidadCampos('|') < 9) continue;

        int id = 0;
        Fecha fNac, fInsc;
        if (!linea.campo(0, '|').aEntero(id)) continue;
        Cadena nombre = linea.campo(1, '|');
        if (nombre.getLongitud() == 0) continue;
        if (!Fecha::desdeCadena(linea.campo(2, '|'), fNac)) fNac = Fecha(1, 1, 2000);
        Cadena correo = linea.campo(3, '|');
        Cadena foto = linea.campo(4, '|');
        Cadena pass = linea.campo(5, '|');
        Cadena ciudad = linea.campo(6, '|');
        Cadena pais = linea.campo(7, '|');
        if (!Fecha::desdeCadena(linea.campo(8, '|'), fInsc)) fInsc = Fecha(1, 1, 2024);

        Usuario u(id, nombre, fNac, correo, foto, pass, ciudad, pais, fInsc);
        usuarios.agregar(u);
        tabla.insertar(nombre, usuarios.getTamano() - 1);
    }
    archUsuarios.close();

    relaciones.redimensionar(usuarios.getTamano());

    // 2. Cargar amistades.txt (Formato: idA|idB)
    ifstream archAmistades(rutaAmistades);
    if (archAmistades.is_open()) {
        while (leerLineaArchivo(archAmistades, linea)) {
            MedidorRecursos::contarIteracion();
            if (linea.cantidadCampos('|') < 2) continue;
            int a = -1, b = -1;
            if (linea.campo(0, '|').aEntero(a) && linea.campo(1, '|').aEntero(b)) {
                relaciones.agregarAmistad(a, b);
            }
        }
        archAmistades.close();
    }

    // 3. Cargar solicitudes.txt (Formato: idRemitente|idReceptor)
    ifstream archSolicitudes(rutaSolicitudes);
    if (archSolicitudes.is_open()) {
        while (leerLineaArchivo(archSolicitudes, linea)) {
            MedidorRecursos::contarIteracion();
            if (linea.cantidadCampos('|') < 2) continue;
            int rem = -1, rec = -1;
            if (linea.campo(0, '|').aEntero(rem) && linea.campo(1, '|').aEntero(rec)) {
                if (rec >= 0 && rec < usuarios.getTamano() && rem >= 0 && rem < usuarios.getTamano()) {
                    usuarios[rec].agregarSolicitud(rem);
                }
            }
        }
        archSolicitudes.close();
    }

    // 4. Cargar albumes.txt (Formato: idUsuario|nombreAlbum|rutaAlbum|privacidad)
    ifstream archAlbumes(rutaAlbumes);
    if (archAlbumes.is_open()) {
        while (leerLineaArchivo(archAlbumes, linea)) {
            MedidorRecursos::contarIteracion();
            if (linea.cantidadCampos('|') < 4) continue;
            int idU = -1, priv = 0;
            if (!linea.campo(0, '|').aEntero(idU)) continue;
            Cadena nomAlb = linea.campo(1, '|');
            Cadena rutaAlb = linea.campo(2, '|');
            linea.campo(3, '|').aEntero(priv);

            if (idU >= 0 && idU < usuarios.getTamano() && nomAlb.getLongitud() > 0) {
                Album alb(nomAlb, rutaAlb, priv);
                usuarios[idU].agregarAlbum(alb);
            }
        }
        archAlbumes.close();
    }

    // 5. Cargar fotos.txt (Formato: idUsuario|nombreAlbum|fechaCaptura|tamano|rutaFoto|likes)
    ifstream archFotos(rutaFotos);
    if (archFotos.is_open()) {
        while (leerLineaArchivo(archFotos, linea)) {
            MedidorRecursos::contarIteracion();
            if (linea.cantidadCampos('|') < 6) continue;
            int idU = -1, tam = 0, lks = 0;
            Fecha fCap;
            if (!linea.campo(0, '|').aEntero(idU)) continue;
            Cadena nomAlb = linea.campo(1, '|');
            if (!Fecha::desdeCadena(linea.campo(2, '|'), fCap)) fCap = Fecha(1, 1, 2026);
            linea.campo(3, '|').aEntero(tam);
            Cadena rutaF = linea.campo(4, '|');
            linea.campo(5, '|').aEntero(lks);

            if (idU >= 0 && idU < usuarios.getTamano()) {
                ListaDinamica<Album>& albs = usuarios[idU].getAlbumes();
                for (int a = 0; a < albs.getTamano(); a++) {
                    MedidorRecursos::contarIteracion();
                    if (albs[a].getNombre() == nomAlb) {
                        Foto f(fCap, tam, rutaF, lks);
                        albs[a].agregarFoto(f);
                        break;
                    }
                }
            }
        }
        archFotos.close();
    }

    // 6. Cargar mensajes.txt (Formato: idEmisor|idReceptor|fecha|leido|texto)
    ifstream archMensajes(rutaMensajes);
    if (archMensajes.is_open()) {
        while (leerLineaArchivo(archMensajes, linea)) {
            MedidorRecursos::contarIteracion();
            if (linea.cantidadCampos('|') < 5) continue;
            int emisor = -1, receptor = -1, leidoInt = 0;
            Fecha fEnvio;
            if (!linea.campo(0, '|').aEntero(emisor) || !linea.campo(1, '|').aEntero(receptor)) continue;
            if (!Fecha::desdeCadena(linea.campo(2, '|'), fEnvio)) continue;
            linea.campo(3, '|').aEntero(leidoInt);
            Cadena texto = linea.desdeCampo(4, '|');

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

    MedidorRecursos::registrarMemoriaLocal(
        static_cast<long>(sizeof(linea)) + linea.bytesDinamicos() +
        static_cast<long>(sizeof(Usuario) + sizeof(Album) + sizeof(Foto) + sizeof(Mensaje))
        );

    mensajesAlCargar = mensajes.getTamano();
    cout << ">>> Usuarios cargados en memoria: " << usuarios.getTamano() << " <<<" << endl;
    return true;
}

void RedSocial::guardarCambiosRed(const char* rutaMensajes, const char* rutaAmistades,
                                  const char* rutaSolicitudes, const char* rutaFotos) {
    // Parametros por valor (4 punteros) + streams locales e indices de iteracion
    MedidorRecursos::registrarMemoriaLocal(
        static_cast<long>(4 * sizeof(const char*) + sizeof(ofstream) + 4 * sizeof(int))
        );

    // 1. Mensajes (incremental o actualización de leídos)
    if (!huboCambioEstadoLeido && mensajes.getTamano() > mensajesAlCargar) {
        ofstream archMsg(rutaMensajes, ios::app);
        if (archMsg.is_open()) {
            for (int i = mensajesAlCargar; i < mensajes.getTamano(); i++) {
                MedidorRecursos::contarIteracion();
                const Mensaje& m = mensajes[i];
                archMsg << m.getIdEmisor() << "|"
                        << m.getIdReceptor() << "|"
                        << m.getFechaEnvio() << "|"
                        << (m.estaLeido() ? 1 : 0) << "|"
                        << m.getTexto() << "\n";
            }
            archMsg.close();
        }
    } else if (huboCambioEstadoLeido) {
        ofstream archMsg(rutaMensajes, ios::trunc);
        if (archMsg.is_open()) {
            for (int i = 0; i < mensajes.getTamano(); i++) {
                MedidorRecursos::contarIteracion();
                const Mensaje& m = mensajes[i];
                archMsg << m.getIdEmisor() << "|"
                        << m.getIdReceptor() << "|"
                        << m.getFechaEnvio() << "|"
                        << (m.estaLeido() ? 1 : 0) << "|"
                        << m.getTexto() << "\n";
            }
            archMsg.close();
        }
    }

    // 2. Nuevas amistades aceptadas (append incremental)
    if (nuevasAmistadesA.getTamano() > 0) {
        ofstream archAmi(rutaAmistades, ios::app);
        if (archAmi.is_open()) {
            for (int i = 0; i < nuevasAmistadesA.getTamano(); i++) {
                MedidorRecursos::contarIteracion();
                archAmi << nuevasAmistadesA[i] << "|" << nuevasAmistadesB[i] << "\n";
            }
            archAmi.close();
        }
    }

    // 3. Estado vigente de solicitudes pendientes
    ofstream archSol(rutaSolicitudes, ios::trunc);
    if (archSol.is_open()) {
        for (int u = 0; u < usuarios.getTamano(); u++) {
            const ListaDinamica<int>& sols = usuarios[u].getSolicitudesRecibidas();
            for (int s = 0; s < sols.getTamano(); s++) {
                MedidorRecursos::contarIteracion();
                archSol << sols[s] << "|" << u << "\n";
            }
        }
        archSol.close();
    }

    // 4. Actualizar fotos.txt si hubo cambios de likes en el carrusel (Persona A)
    if (huboCambioLikes) {
        ofstream archFot(rutaFotos, ios::trunc);
        if (archFot.is_open()) {
            for (int u = 0; u < usuarios.getTamano(); u++) {
                const ListaDinamica<Album>& albs = usuarios[u].getAlbumes();
                for (int a = 0; a < albs.getTamano(); a++) {
                    const Album& alb = albs[a];
                    for (int f = 0; f < alb.getCantidadFotos(); f++) {
                        MedidorRecursos::contarIteracion();
                        const Foto& fot = alb.getFoto(f);
                        archFot << u << "|"
                                << alb.getNombre() << "|"
                                << fot.getFechaCaptura() << "|"
                                << fot.getTamano() << "|"
                                << fot.getRuta() << "|"
                                << fot.getLikes() << "\n";
                    }
                }
            }
            archFot.close();
        }
    }

    mensajesAlCargar = mensajes.getTamano();
    huboCambioEstadoLeido = false;
    huboCambioLikes = false;
    nuevasAmistadesA.vaciar();
    nuevasAmistadesB.vaciar();
}

// ============================================================
// FUNCIONALIDAD II: INGRESO Y FOTOS PONDERADAS CON 1 REFRESCO
// ============================================================

bool RedSocial::iniciarSesion(const Cadena& u, const Cadena& c) {
    int idx = tabla.buscar(u);
    MedidorRecursos::registrarMemoriaLocal(static_cast<long>(sizeof(idx)));

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
    // Parametro por valor: esRefresco
    MedidorRecursos::registrarMemoriaLocal(static_cast<long>(sizeof(esRefresco)));

    if (!haySesionActiva()) return;

    if (esRefresco && refrescoUsado) {
        cout << "[Aviso] Solo se permite una (1) unica actualizacion de fotos de perfil por sesion." << endl;
        return;
    }

    ListaDinamica<int> amigos = relaciones.obtenerAmigos(usuarioActual);
    if (amigos.estaVacia()) {
        MedidorRecursos::registrarMemoriaLocal(
            static_cast<long>(sizeof(amigos)) + amigos.bytesDinamicos()
            );
        cout << "[Info] Aun no tienes amigos agregados para mostrar fotos de perfil." << endl;
        return;
    }

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
        long sumaPesos = 0;
        for (int i = 0; i < candidatos.getTamano(); i++) {
            MedidorRecursos::contarIteracion();
            if (!elegido[i]) {
                int comunes = relaciones.contarEnComun(usuarioActual, candidatos[i]);
                int peso = 2; // <= 10 amigos en comun (1.0x)
                if (comunes >= 11 && comunes <= 20) {
                    peso = 5; // 11 a 20 amigos en comun (2.5x)
                } else if (comunes > 20) {
                    peso = 7; // > 20 amigos en comun (3.5x)
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

    // Registrar variables locales y estructuras temporales en el pico de la funcion (VII.b)
    long locales = static_cast<long>(sizeof(amigos)) + amigos.bytesDinamicos() +
                   static_cast<long>(sizeof(candidatos)) + candidatos.bytesDinamicos() +
                   static_cast<long>(sizeof(elegido)) +
                   static_cast<long>(candidatos.getTamano() * sizeof(bool)) +
                   static_cast<long>(5 * sizeof(int) + 3 * sizeof(long));
    MedidorRecursos::registrarMemoriaLocal(locales);

    delete[] elegido;
}

// ============================================================
// FUNCIONALIDAD III: ADMINISTRAR AMISTADES
// ============================================================

bool RedSocial::enviarSolicitud(const Cadena& destino) {
    if (!haySesionActiva()) return false;
    int idDestino = tabla.buscar(destino);
    MedidorRecursos::registrarMemoriaLocal(static_cast<long>(sizeof(idDestino)));

    if (idDestino < 0 || idDestino == usuarioActual) {
        cout << "[Error] Usuario destino invalido o eres tu mismo." << endl;
        return false;
    }
    if (relaciones.sonAmigos(usuarioActual, idDestino)) {
        cout << "[Aviso] Ya eres amigo de @" << destino << "." << endl;
        return false;
    }
    if (usuarios[idDestino].tieneSolicitudDe(usuarioActual) ||
        usuarios[usuarioActual].tieneSolicitudDe(idDestino)) {
        cout << "[Aviso] Ya existe una solicitud pendiente entre ambos." << endl;
        return false;
    }
    usuarios[idDestino].agregarSolicitud(usuarioActual);
    cout << "[OK] Solicitud enviada a @" << destino << "." << endl;
    return true;
}

void RedSocial::aceptarSolicitud(int idRemitente) {
    // Parametro por valor: idRemitente
    MedidorRecursos::registrarMemoriaLocal(static_cast<long>(sizeof(idRemitente)));

    if (!haySesionActiva()) return;
    if (!usuarios[usuarioActual].tieneSolicitudDe(idRemitente)) {
        cout << "[Error] No tienes una solicitud pendiente de ese usuario." << endl;
        return;
    }
    usuarios[usuarioActual].quitarSolicitud(idRemitente);
    relaciones.agregarAmistad(usuarioActual, idRemitente);
    nuevasAmistadesA.agregar(usuarioActual);
    nuevasAmistadesB.agregar(idRemitente);
    cout << "[OK] Ahora eres amigo de @" << usuarios[idRemitente].getNombreUsuario() << "." << endl;
}

// ============================================================
// FUNCIONALIDAD IV: VISITAR PERFIL Y CARRUSEL CIRCULAR
// ============================================================

void RedSocial::visitarPerfilAmigo(const Cadena& amigo) {
    if (!haySesionActiva()) return;

    int idAmigo = tabla.buscar(amigo);
    if (idAmigo < 0) {
        MedidorRecursos::registrarMemoriaLocal(static_cast<long>(sizeof(idAmigo)));
        cout << "[Error] El usuario @" << amigo << " no existe." << endl;
        return;
    }
    if (!relaciones.sonAmigos(usuarioActual, idAmigo)) {
        MedidorRecursos::registrarMemoriaLocal(static_cast<long>(sizeof(idAmigo)));
        cout << "[Error] @" << amigo << " no esta en tu lista de amigos." << endl;
        return;
    }

    Usuario& uAmigo = usuarios[idAmigo];
    ListaDinamica<Album>& albumes = uAmigo.getAlbumes();

    // Filtrar indices de albumes visibles para un amigo ("publico" = 0 o "amigos" = 1)
    ListaDinamica<int> indicesVisibles;
    for (int i = 0; i < albumes.getTamano(); i++) {
        MedidorRecursos::contarIteracion();
        if (albumes[i].esVisiblePara(true)) {
            indicesVisibles.agregar(i);
        }
    }

    // Registrar variables locales y lista temporal de indices visibles (VII.b)
    MedidorRecursos::registrarMemoriaLocal(
        static_cast<long>(sizeof(idAmigo) + sizeof(indicesVisibles)) +
        indicesVisibles.bytesDinamicos() +
        static_cast<long>(4 * sizeof(int) + 2 * sizeof(bool))
        );

    cout << "\n================ PERFIL DE @" << uAmigo.getNombreUsuario() << " ================" << endl;
    cout << "Ciudad: " << uAmigo.getCiudad() << " | Pais: " << uAmigo.getPais() << endl;
    cout << "Foto de perfil: " << uAmigo.getRutaFotoPerfil() << endl;

    if (indicesVisibles.estaVacia()) {
        cout << "[Info] Este amigo no tiene albumes visibles (publicos o para amigos)." << endl;
        return;
    }

    bool enPerfil = true;
    while (enPerfil) {
        cout << "\n--- ALBUMES DISPONIBLES DE @" << uAmigo.getNombreUsuario() << " ---" << endl;
        for (int i = 0; i < indicesVisibles.getTamano(); i++) {
            MedidorRecursos::contarIteracion();
            const Album& alb = albumes[indicesVisibles[i]];
            const char* etiqPriv = (alb.getPrivacidad() == 0) ? "Publico" : "Solo amigos";
            cout << (i + 1) << ". " << alb.getNombre()
                 << " [" << etiqPriv << "] (" << alb.getCantidadFotos() << " fotos)" << endl;
        }
        cout << "0. Volver al menu principal" << endl;
        cout << "Seleccione un album: ";

        int opAlb = -1;
        if (!(cin >> opAlb)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "[Error] Entrada invalida." << endl;
            continue;
        }
        cin.ignore(10000, '\n');

        if (opAlb == 0) {
            enPerfil = false;
            break;
        }
        if (opAlb < 1 || opAlb > indicesVisibles.getTamano()) {
            cout << "[Error] Numero de album fuera de rango." << endl;
            continue;
        }

        Album& albumSel = albumes[indicesVisibles[opAlb - 1]];
        int totalFotos = albumSel.getCantidadFotos();
        if (totalFotos == 0) {
            cout << "[Info] El album '" << albumSel.getNombre() << "' no contiene fotos." << endl;
            continue;
        }

        // Modo carrusel circular
        int posFoto = 0;
        bool enCarrusel = true;
        while (enCarrusel) {
            MedidorRecursos::contarIteracion();
            Foto& fotoActual = albumSel.getFoto(posFoto);

            cout << "\n--------------------------------------------------" << endl;
            cout << "Album: " << albumSel.getNombre()
                 << " | Foto " << (posFoto + 1) << " de " << totalFotos << endl;
            cout << "Ruta : " << fotoActual.getRuta() << endl;
            cout << "Fecha: " << fotoActual.getFechaCaptura()
                 << " | Tamano: " << fotoActual.getTamano() << " KB"
                 << " | Likes: " << fotoActual.getLikes() << endl;
            cout << "--------------------------------------------------" << endl;
            cout << "Opciones del carrusel:" << endl;
            cout << "1. Siguiente foto" << endl;
            cout << "2. Foto previa" << endl;
            cout << "3. Dar like (+1)" << endl;
            cout << "4. Quitar like (-1)" << endl;
            cout << "0. Salir del album" << endl;
            cout << "Accion: ";

            int acc = -1;
            if (!(cin >> acc)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "[Error] Entrada invalida." << endl;
                continue;
            }
            cin.ignore(10000, '\n');

            if (acc == 0) {
                enCarrusel = false;
            } else if (acc == 1) {
                posFoto = (posFoto + 1) % totalFotos; // Avance circular
            } else if (acc == 2) {
                posFoto = (posFoto - 1 + totalFotos) % totalFotos; // Retroceso circular
            } else if (acc == 3) {
                fotoActual.darLike();
                huboCambioLikes = true;
                cout << "[OK] Like registrado. Total likes: " << fotoActual.getLikes() << endl;
            } else if (acc == 4) {
                fotoActual.quitarLike();
                huboCambioLikes = true;
                cout << "[OK] Like retirado. Total likes: " << fotoActual.getLikes() << endl;
            } else {
                cout << "[Error] Opcion no valida." << endl;
            }
        }
    }
}

// ============================================================
// FUNCIONALIDAD V: SUGERENCIAS DE AMISTAD TOP-K
// ============================================================

void RedSocial::sugerirAmigos(int k) {
    // Parametro por valor: int k
    MedidorRecursos::registrarMemoriaLocal(static_cast<long>(sizeof(k)));

    if (!haySesionActiva()) return;

    if (k < 1 || k > 20) {
        cout << "[Error] El parametro K debe estar entre 1 y 20." << endl;
        return;
    }

    int* topIds = new int[k];
    int* topComunes = new int[k];
    int encontrados = 0;

    int totalUsuarios = usuarios.getTamano();

    // Registrar variables locales y arreglos dinamicos topIds/topComunes (VII.b)
    MedidorRecursos::registrarMemoriaLocal(
        static_cast<long>(sizeof(topIds) + sizeof(topComunes)) +
        static_cast<long>(2 * k * sizeof(int)) +
        static_cast<long>(6 * sizeof(int))
        );

    for (int u = 0; u < totalUsuarios; u++) {
        MedidorRecursos::contarIteracion();
        if (u == usuarioActual) continue;
        if (relaciones.sonAmigos(usuarioActual, u)) continue;
        if (usuarios[u].tieneSolicitudDe(usuarioActual) ||
            usuarios[usuarioActual].tieneSolicitudDe(u)) {
            continue;
        }

        int enComun = relaciones.contarEnComun(usuarioActual, u);
        if (enComun <= 0) continue;

        if (encontrados < k || enComun > topComunes[encontrados - 1]) {
            int pos = encontrados;
            if (encontrados < k) {
                encontrados++;
            } else {
                pos = k - 1;
            }
            while (pos > 0 && topComunes[pos - 1] < enComun) {
                MedidorRecursos::contarIteracion();
                topComunes[pos] = topComunes[pos - 1];
                topIds[pos] = topIds[pos - 1];
                pos--;
            }
            topComunes[pos] = enComun;
            topIds[pos] = u;
        }
    }

    cout << "\n================ SUGERENCIAS DE AMISTAD (TOP-" << k << ") ================" << endl;
    if (encontrados == 0) {
        cout << "No se encontraron sugerencias con amigos en comun en este momento." << endl;
        delete[] topIds;
        delete[] topComunes;
        return;
    }

    for (int i = 0; i < encontrados; i++) {
        int idCand = topIds[i];
        cout << (i + 1) << ". @" << usuarios[idCand].getNombreUsuario()
             << " (" << topComunes[i] << " amigos en comun) - "
             << usuarios[idCand].getCiudad() << ", " << usuarios[idCand].getPais() << endl;
    }
    cout << "==================================================================" << endl;

    cout << "\nDesea enviar solicitud a alguna sugerencia? (0 = No, o ingrese el numero 1.." << encontrados << "): ";
    int eleccion = 0;
    if (cin >> eleccion) {
        cin.ignore(10000, '\n');
        if (eleccion >= 1 && eleccion <= encontrados) {
            int idElegido = topIds[eleccion - 1];
            enviarSolicitud(usuarios[idElegido].getNombreUsuario());
        } else if (eleccion != 0) {
            cout << "[Aviso] Numero fuera del rango de sugerencias mostradas." << endl;
        }
    } else {
        cin.clear();
        cin.ignore(10000, '\n');
    }

    delete[] topIds;
    delete[] topComunes;
}

// ============================================================
// FUNCIONALIDAD VI: SISTEMA DE MENSAJERÍA
// ============================================================

bool RedSocial::enviarMensaje(const Cadena& amigo, const Cadena& texto, const Fecha& fechaActual) {
    if (!haySesionActiva()) return false;

    int lon = texto.getLongitud();
    if (lon == 0) {
        MedidorRecursos::registrarMemoriaLocal(static_cast<long>(sizeof(lon)));
        cout << "[Error] El mensaje no puede estar vacio." << endl;
        return false;
    }
    if (lon > 512) {
        MedidorRecursos::registrarMemoriaLocal(static_cast<long>(sizeof(lon)));
        cout << "[Error] El mensaje excede el limite maximo de 512 caracteres (" << lon << ")." << endl;
        return false;
    }

    int idReceptor = tabla.buscar(amigo);
    if (idReceptor < 0) {
        MedidorRecursos::registrarMemoriaLocal(static_cast<long>(sizeof(lon) + sizeof(idReceptor)));
        cout << "[Error] El usuario @" << amigo << " no existe." << endl;
        return false;
    }
    if (!relaciones.sonAmigos(usuarioActual, idReceptor)) {
        MedidorRecursos::registrarMemoriaLocal(static_cast<long>(sizeof(lon) + sizeof(idReceptor)));
        cout << "[Error] Solo puedes enviar mensajes a usuarios que sean tus amigos." << endl;
        return false;
    }

    Mensaje nuevo(usuarioActual, idReceptor, texto, fechaActual, false);
    // Registrar el objeto Mensaje local 'nuevo' y variables escalares antes de terminar
    MedidorRecursos::registrarMemoriaLocal(
        static_cast<long>(sizeof(lon) + sizeof(idReceptor) + sizeof(int) + sizeof(nuevo)) +
        nuevo.bytesDinamicos()
        );

    mensajes.agregar(nuevo);
    int idxMsg = mensajes.getTamano() - 1;

    usuarios[usuarioActual].registrarMensajeEnviado(idxMsg);
    usuarios[idReceptor].registrarMensajeRecibido(idxMsg);

    cout << "[OK] Mensaje enviado a @" << amigo << " con fecha " << fechaActual << "." << endl;
    return true;
}

void RedSocial::leerMensajesNuevos() {
    if (!haySesionActiva()) return;

    const ListaDinamica<int>& recibidos = usuarios[usuarioActual].getMensajesRecibidos();
    ListaDinamica<int> noLeidos;

    for (int i = 0; i < recibidos.getTamano(); i++) {
        MedidorRecursos::contarIteracion();
        int idxMsg = recibidos[i];
        if (!mensajes[idxMsg].estaLeido()) {
            noLeidos.agregar(idxMsg);
        }
    }

    // Registrar lista local noLeidos y variables del ordenamiento (VII.b)
    MedidorRecursos::registrarMemoriaLocal(
        static_cast<long>(sizeof(noLeidos)) + noLeidos.bytesDinamicos() +
        static_cast<long>(4 * sizeof(int) + sizeof(Fecha))
        );

    if (noLeidos.estaVacia()) {
        cout << "\n[Bandeja al dia] No tienes mensajes nuevos sin leer." << endl;
        return;
    }

    for (int i = 1; i < noLeidos.getTamano(); i++) {
        int claveIdx = noLeidos[i];
        Fecha fechaClave = mensajes[claveIdx].getFechaEnvio();
        int j = i - 1;
        while (j >= 0 && fechaClave < mensajes[noLeidos[j]].getFechaEnvio()) {
            MedidorRecursos::contarIteracion();
            noLeidos[j + 1] = noLeidos[j];
            j--;
        }
        noLeidos[j + 1] = claveIdx;
    }

    cout << "\n================ MENSAJES NUEVOS (" << noLeidos.getTamano() << ") ================" << endl;
    for (int i = 0; i < noLeidos.getTamano(); i++) {
        MedidorRecursos::contarIteracion();
        int idxMsg = noLeidos[i];
        Mensaje& m = mensajes[idxMsg];
        const Cadena& remitente = usuarios[m.getIdEmisor()].getNombreUsuario();

        cout << "[" << m.getFechaEnvio() << "] De @" << remitente << ": "
             << m.getTexto() << endl;

        m.marcarLeido();
        huboCambioEstadoLeido = true;
    }
    cout << "=========================================================" << endl;
}

void RedSocial::buscarMensajesPorFecha(const Fecha& ini, const Fecha& fin) {
    if (!haySesionActiva()) return;

    if (!ini.esValida() || !fin.esValida() || fin < ini) {
        cout << "[Error] Rango de fechas invalido. Verifique que fechaInicio <= fechaFin." << endl;
        return;
    }

    const ListaDinamica<int>& recibidos = usuarios[usuarioActual].getMensajesRecibidos();
    ListaDinamica<int> enRango;

    for (int i = 0; i < recibidos.getTamano(); i++) {
        MedidorRecursos::contarIteracion();
        int idx = recibidos[i];
        const Fecha& f = mensajes[idx].getFechaEnvio();
        if (ini <= f && f <= fin) {
            enRango.agregar(idx);
        }
    }

    // Registrar lista local enRango y variables del ordenamiento (VII.b)
    MedidorRecursos::registrarMemoriaLocal(
        static_cast<long>(sizeof(enRango)) + enRango.bytesDinamicos() +
        static_cast<long>(4 * sizeof(int) + sizeof(Fecha))
        );

    if (enRango.estaVacia()) {
        cout << "\nNo se encontraron mensajes recibidos entre " << ini
             << " y " << fin << "." << endl;
        return;
    }

    for (int i = 1; i < enRango.getTamano(); i++) {
        int clave = enRango[i];
        Fecha fClave = mensajes[clave].getFechaEnvio();
        int j = i - 1;
        while (j >= 0 && fClave < mensajes[enRango[j]].getFechaEnvio()) {
            MedidorRecursos::contarIteracion();
            enRango[j + 1] = enRango[j];
            j--;
        }
        enRango[j + 1] = clave;
    }

    cout << "\n=== MENSAJES RECIBIDOS ENTRE " << ini << " Y " << fin
         << " (" << enRango.getTamano() << ") ===" << endl;
    for (int i = 0; i < enRango.getTamano(); i++) {
        MedidorRecursos::contarIteracion();
        const Mensaje& m = mensajes[enRango[i]];
        cout << "[" << m.getFechaEnvio() << "] De @"
             << usuarios[m.getIdEmisor()].getNombreUsuario() << ": "
             << m.getTexto() << endl;
    }
}

// ============================================================
// CONSOLIDACIÓN DE MEMORIA DINÁMICA PARA MEDIDOR DE RECURSOS
// ============================================================

long RedSocial::bytesDinamicos() const {
    long total = usuarios.bytesDinamicos() +
                 mensajes.bytesDinamicos() +
                 relaciones.bytesDinamicos() +
                 tabla.bytesDinamicos() +
                 ultimosAmigosMostrados.bytesDinamicos() +
                 nuevasAmistadesA.bytesDinamicos() +
                 nuevasAmistadesB.bytesDinamicos();

    // Se recorre getCapacidad() porque new Usuario[capacidad] y new Mensaje[capacidad]
    // construyen por defecto TODOS los slots del bloque reservado en el heap
    for (int i = 0; i < usuarios.getCapacidad(); i++) {
        total += usuarios.getElementoCapacidad(i).bytesDinamicos();
    }
    for (int i = 0; i < mensajes.getCapacidad(); i++) {
        total += mensajes.getElementoCapacidad(i).bytesDinamicos();
    }
    return total;
}