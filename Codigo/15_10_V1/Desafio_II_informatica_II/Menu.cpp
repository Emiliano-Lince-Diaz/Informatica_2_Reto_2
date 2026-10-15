// Menu.cpp
#include "Menu.h"
#include <iostream>

using namespace std;

Menu::Menu(RedSocial* r) : red(r), medidor() {}

int Menu::leerEnteroSeguro(const char* prompt, int minVal, int maxVal) {
    int valor = 0;
    while (true) {
        cout << prompt;
        if (cin >> valor) {
            cin.ignore(10000, '\n');
            if (valor >= minVal && valor <= maxVal) {
                return valor;
            }
            cout << "[Error] Ingrese un numero entre " << minVal << " y " << maxVal << "." << endl;
        } else {
            cout << "[Error] Entrada invalida. Debe ingresar un numero entero." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
}

Cadena Menu::leerCadenaNoVacia(const char* prompt) {
    char buffer[1024];
    while (true) {
        cout << prompt;
        cin.getline(buffer, 1024);
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
        }
        Cadena texto(buffer);
        if (texto.getLongitud() > 0) {
            return texto;
        }
        cout << "[Error] El texto no puede estar vacio." << endl;
    }
}

Fecha Menu::leerFechaSegura(const char* prompt) {
    Fecha f;
    while (true) {
        Cadena txt = leerCadenaNoVacia(prompt);
        if (Fecha::desdeCadena(txt, f)) {
            return f;
        }
        cout << "[Error] Formato o fecha invalida. Use DD/MM/AAAA (ej. 15/10/2026)." << endl;
    }
}

void Menu::mostrarLogin() {
    cout << "\n==================================================" << endl;
    cout << "              BIENVENIDO A UDE@BOOK               " << endl;
    cout << "==================================================" << endl;

    Cadena usuario = leerCadenaNoVacia("Usuario: ");
    Cadena pass = leerCadenaNoVacia("Contrasena: ");

    MedidorRecursos::reiniciar();
    if (red->iniciarSesion(usuario, pass)) {
        red->mostrarFotosAmigos(false);
        medidor.mostrarReporte(*red);
    } else {
        cout << "\n[Error] Credenciales incorrectas." << endl;
        medidor.mostrarReporte(*red);
    }
}

void Menu::mostrarMenuPrincipal() {
    const Usuario& u = red->getUsuario(red->getUsuarioActual());
    cout << "\nBienvenido a UdeaBook " << u.getNombreUsuario() << "\n" << endl;
    cout << "Menu de opciones:\n" << endl;
    cout << "1.- Administrar amistades" << endl;
    cout << "2.- Visitar perfil de un amigo." << endl;
    cout << "3.- Ver sugerencias de amistad." << endl;
    cout << "4.- Administrar mensajes.\n" << endl;
    cout << ".- Ingrese 'N' o 'n' para alternar el grupo de fotos de perfil de amigos." << endl;
    cout << ".- Ingrese 'S' o 's' para cerrar su sesion.\n" << endl;
}

void Menu::ejecutar() {
    if (red == nullptr) return;

    MedidorRecursos::reiniciar();
    red->cargarDatos();
    medidor.mostrarReporte(*red);

    bool activo = true;
    while (activo) {
        if (!red->haySesionActiva()) {
            cout << "\n1. Iniciar sesion\n0. Guardar cambios y salir de Ude@Book\n";
            int op = leerEnteroSeguro("Seleccione: ", 0, 1);
            if (op == 0) {
                activo = false;
            } else {
                mostrarLogin();
            }
        } else {
            mostrarMenuPrincipal();
            Cadena entrada = leerCadenaNoVacia("Su opcion: ");
            char op = entrada[0];

            if (entrada.getLongitud() != 1) {
                op = '?';
            }

            switch (op) {
            case '1': {
                cout << "\n--- ADMINISTRAR AMISTADES ---" << endl;
                cout << "1. Enviar solicitud de amistad" << endl;
                cout << "2. Aceptar solicitudes de amistad recibidas" << endl;
                int sub = leerEnteroSeguro("Opcion: ", 1, 2);
                MedidorRecursos::reiniciar();
                if (sub == 1) {
                    Cadena dest = leerCadenaNoVacia("Nombre de usuario destino: ");
                    red->enviarSolicitud(dest);
                } else {
                    const ListaDinamica<int>& sols = red->getUsuario(red->getUsuarioActual()).getSolicitudesRecibidas();
                    if (sols.estaVacia()) {
                        cout << "No tienes solicitudes pendientes." << endl;
                    } else {
                        for (int i = 0; i < sols.getTamano(); i++) {
                            int idRem = sols[i];
                            cout << (i + 1) << ". @" << red->getUsuario(idRem).getNombreUsuario()
                                 << " (ID: " << idRem << ")" << endl;
                        }
                        int idxAceptar = leerEnteroSeguro("Seleccione numero a aceptar (0 para cancelar): ", 0, sols.getTamano());
                        if (idxAceptar > 0) {
                            red->aceptarSolicitud(sols[idxAceptar - 1]);
                        }
                    }
                }
                medidor.mostrarReporte(*red);
                break;
            }
            case '2': {
                Cadena amigo = leerCadenaNoVacia("Nombre del amigo a visitar: ");
                MedidorRecursos::reiniciar();
                red->visitarPerfilAmigo(amigo);
                medidor.mostrarReporte(*red);
                break;
            }
            case '3': {
                int k = leerEnteroSeguro("Ingrese la cantidad K de sugerencias (1..20): ", 1, 20);
                MedidorRecursos::reiniciar();
                red->sugerirAmigos(k);
                medidor.mostrarReporte(*red);
                break;
            }
            case '4': {
                cout << "\n--- ADMINISTRAR MENSAJES ---" << endl;
                cout << "1. Enviar mensaje a un amigo" << endl;
                cout << "2. Leer nuevos mensajes (no leidos)" << endl;
                cout << "3. Buscar mensajes por rango de fecha" << endl;
                int sub = leerEnteroSeguro("Opcion: ", 1, 3);

                if (sub == 1) {
                    Cadena receptor = leerCadenaNoVacia("Usuario amigo receptor: ");
                    Cadena texto = leerCadenaNoVacia("Mensaje (max 512 caracteres): ");
                    Fecha hoy = leerFechaSegura("Fecha de envio (DD/MM/AAAA): ");
                    MedidorRecursos::reiniciar();
                    red->enviarMensaje(receptor, texto, hoy);
                    medidor.mostrarReporte(*red);
                } else if (sub == 2) {
                    MedidorRecursos::reiniciar();
                    red->leerMensajesNuevos();
                    medidor.mostrarReporte(*red);
                } else {
                    Fecha ini = leerFechaSegura("Fecha de inicio (DD/MM/AAAA): ");
                    Fecha fin = leerFechaSegura("Fecha de fin    (DD/MM/AAAA): ");
                    MedidorRecursos::reiniciar();
                    red->buscarMensajesPorFecha(ini, fin);
                    medidor.mostrarReporte(*red);
                }
                break;
            }
            case 'N':
            case 'n': {
                MedidorRecursos::reiniciar();
                red->mostrarFotosAmigos(true);
                medidor.mostrarReporte(*red);
                break;
            }
            case 'S':
            case 's': {
                red->cerrarSesion();
                cout << "[OK] Sesion cerrada." << endl;
                break;
            }
            default:
                cout << "[Error] Opcion invalida. Ingrese 1..4, 'N' o 'S'." << endl;
                break;
            }
        }
    }

    MedidorRecursos::reiniciar();
    red->guardarCambiosRed();
    cout << "\n[OK] Cambios de red y mensajeria guardados en disco." << endl;
    medidor.mostrarReporte(*red);
}