// RedSocial.cpp
#include "RedSocial.h"
#include "MedidorRecursos.h"
#include <fstream>
#include <cstdlib>

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
        }
    } else if (huboCambioEstadoLeido) {
        // Si se marcaron mensajes como leídos, se actualiza el archivo de mensajes con el nuevo estado
        ofstream archMsg(rutaMensajes, ios::trunc);
        if (archMsg.is_open()) {
            for (int i = 0; i < mensajes.getTamano(); i++) {
                MedidorRecursos::contarIteracion();
                const Mensaje& m = mensajes[i];
                archMsg << m.getIdEmisor() << ";"
                        << m.getIdReceptor() << ";"
                        << m.getFechaEnvio() << ";"
                        << (m.estaLeido() ? 1 : 0) << ";"
                        << m.getTexto() << "\n";
            }
            archMsg.close();
        }
    }

    // 2. Escritura incremental (deltas) de nuevas amistades aceptadas
    if (nuevasAmistadesA.getTamano() > 0) {
        ofstream archAmi(rutaAmistades, ios::app);
        if (archAmi.is_open()) {
            for (int i = 0; i < nuevasAmistadesA.getTamano(); i++) {
                MedidorRecursos::contarIteracion();
                archAmi << nuevasAmistadesA[i] << ";" << nuevasAmistadesB[i] << "\n";
            }
            archAmi.close();
        }
    }

    // 3. Persistir estado vigente de solicitudes pendientes
    ofstream archSol(rutaSolicitudes, ios::trunc);
    if (archSol.is_open()) {
        for (int u = 0; u < usuarios.getTamano(); u++) {
            const ListaDinamica<int>& sols = usuarios[u].getSolicitudesRecibidas();
            for (int s = 0; s < sols.getTamano(); s++) {
                MedidorRecursos::contarIteracion();
                archSol << sols[s] << ";" << u << "\n";
            }
        }
        archSol.close();
    }

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
// FUNCIONALIDADES III y IV (Conector base para Persona A)
// ============================================================

bool RedSocial::enviarSolicitud(const Cadena& destino) {
    if (!haySesionActiva()) return false;
    int idDestino = tabla.buscar(destino);
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

void RedSocial::visitarPerfilAmigo(const Cadena& amigo) {
    // Implementación completa del carrusel corresponde a Persona A (Día 8)
    int idAmigo = tabla.buscar(amigo);
    if (idAmigo < 0) {
        cout << "[Error] El usuario @" << amigo << " no existe." << endl;
        return;
    }
    cout << "\n--- PERFIL DE @" << usuarios[idAmigo].getNombreUsuario() << " ---" << endl;
    cout << "Ciudad: " << usuarios[idAmigo].getCiudad() << " | Pais: " << usuarios[idAmigo].getPais() << endl;
    cout << "(Modulo de Carrusel Circular administrado por Persona A)" << endl;
}

// ============================================================
// FUNCIONALIDAD V (PERSONA B): SUGERENCIAS DE AMISTAD TOP-K
// Complejidad: O(N * (bloques_64bits + K)) con K <= 20
// ============================================================

void RedSocial::sugerirAmigos(int k) {
    if (!haySesionActiva()) return;

    if (k < 1 || k > 20) {
        cout << "[Error] El parametro K debe estar entre 1 y 20." << endl;
        return;
    }

    int* topIds = new int[k];
    int* topComunes = new int[k];
    int encontrados = 0;

    int totalUsuarios = usuarios.getTamano();

    for (int u = 0; u < totalUsuarios; u++) {
        MedidorRecursos::contarIteracion();
        if (u == usuarioActual) continue;
        if (relaciones.sonAmigos(usuarioActual, u)) continue;
        // Ignorar si ya hay solicitud pendiente en cualquier dirección
        if (usuarios[u].tieneSolicitudDe(usuarioActual) ||
            usuarios[usuarioActual].tieneSolicitudDe(u)) {
            continue;
        }

        int enComun = relaciones.contarEnComun(usuarioActual, u);
        if (enComun <= 0) continue;

        // Inserción ordenada descendente en el Top-K (acotado a máximo 20 posiciones)
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

    cout << "\n¿Desea enviar solicitud a alguna sugerencia? (0 = No, o ingrese el numero 1.." << encontrados << "): ";
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
// FUNCIONALIDAD VI (PERSONA B): SISTEMA DE MENSAJERÍA
// ============================================================

bool RedSocial::enviarMensaje(const Cadena& amigo, const Cadena& texto, const Fecha& fechaActual) {
    if (!haySesionActiva()) return false;

    // 1. Validar longitud del texto (no vacío y <= 512 caracteres)
    int lon = texto.getLongitud();
    if (lon == 0) {
        cout << "[Error] El mensaje no puede estar vacio." << endl;
        return false;
    }
    if (lon > 512) {
        cout << "[Error] El mensaje excede el limite maximo de 512 caracteres (" << lon << ")." << endl;
        return false;
    }

    // 2. Validar existencia y relación de amistad con el receptor
    int idReceptor = tabla.buscar(amigo);
    if (idReceptor < 0) {
        cout << "[Error] El usuario @" << amigo << " no existe." << endl;
        return false;
    }
    if (!relaciones.sonAmigos(usuarioActual, idReceptor)) {
        cout << "[Error] Solo puedes enviar mensajes a usuarios que sean tus amigos." << endl;
        return false;
    }

    // 3. Crear y despachar mensaje
    Mensaje nuevo(usuarioActual, idReceptor, texto, fechaActual, false);
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

    if (noLeidos.estaVacia()) {
        cout << "\n[Bandeja al dia] No tienes mensajes nuevos sin leer." << endl;
        return;
    }

    // Ordenamiento por inserción según Fecha de envío (cronológico ascendente)
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

    // Según apartado VI.c, se listan todos los mensajes RECIBIDOS en ese rango (ambas inclusive)
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

    if (enRango.estaVacia()) {
        cout << "\nNo se encontraron mensajes recibidos entre " << ini
             << " y " << fin << "." << endl;
        return;
    }

    // Ordenar cronológicamente para presentación limpia
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

    for (int i = 0; i < usuarios.getTamano(); i++) {
        total += usuarios[i].bytesDinamicos();
    }
    for (int i = 0; i < mensajes.getTamano(); i++) {
        total += mensajes[i].bytesDinamicos();
    }
    return total;
}