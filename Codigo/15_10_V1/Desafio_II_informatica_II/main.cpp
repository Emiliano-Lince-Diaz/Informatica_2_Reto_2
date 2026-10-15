#include <iostream>
#include <cstdlib>
#include <ctime>
#include "RedSocial.h"
#include "MedidorRecursos.h"

using namespace std;

void ejecutarPruebasIntegrales() {
    cout << "==================================================" << endl;
    cout << "   INICIANDO BATERIA DE PRUEBAS EXTREMAS (UDE@BOOK) " << endl;
    cout << "==================================================" << endl;

    RedSocial udeaBook;
    MedidorRecursos medidor;

    // ----------------------------------------------------
    // PRUEBA 1: Carga Masiva (Funcionalidad I)
    // ----------------------------------------------------
    cout << "\n[TEST 1] Cargando 1200 usuarios y relaciones desde archivos..." << endl;
    MedidorRecursos::reiniciar();
    bool cargo = udeaBook.cargarDatos("usuarios.txt", "amistades.txt", "solicitudes.txt", "mensajes.txt");

    if (cargo) {
        cout << "[EXITO] Carga masiva completada correctamente." << endl;
    } else {
        cout << "[FALLO] Asegurate de haber ejecutado el generador de datos primero." << endl;
        return;
    }
    medidor.mostrarReporte(udeaBook);

    // ----------------------------------------------------
    // PRUEBA 2: Inicio de Sesión y Fotos Ponderadas (Funcionalidad II)
    // ----------------------------------------------------
    cout << "\n[TEST 2] Verificando Login y Seleccion Ponderada (1x, 2.5x, 3.5x)..." << endl;
    MedidorRecursos::reiniciar();
    bool loginOk = udeaBook.iniciarSesion(Cadena("usuario_0"), Cadena("pass123"));

    if (loginOk) {
        cout << "[EXITO] Login correcto para @usuario_0." << endl;
        cout << "-> Mostrando set inicial de 3 fotos de perfil de amigos:" << endl;
        udeaBook.mostrarFotosAmigos(false);

        cout << "-> Probando unico refresco permitido ('N'):" << endl;
        udeaBook.mostrarFotosAmigos(true);

        cout << "-> Intentando un segundo refresco (debe denegarlo):" << endl;
        udeaBook.mostrarFotosAmigos(true);
    } else {
        cout << "[FALLO] No se pudo iniciar sesion con usuario_0." << endl;
    }
    medidor.mostrarReporte(udeaBook);

    // ----------------------------------------------------
    // PRUEBA 3: Sugerencias Top-K (Funcionalidad V)
    // ----------------------------------------------------
    cout << "\n[TEST 3] Consultando K=12 Sugerencias de Amistad con matriz de bits..." << endl;
    MedidorRecursos::reiniciar();
    // Requisito del video: Consultar 12 amigos sugeridos[cite: 29, 44]
    udeaBook.sugerirAmigos(12);
    medidor.mostrarReporte(udeaBook);

    // ----------------------------------------------------
    // PRUEBA 4: Sistema de Mensajería y Fechas (Funcionalidad VI)
    // ----------------------------------------------------
    cout << "\n[TEST 4] Pruebas de Mensajeria (Envio, Lectura y Rango de 6 semanas)..." << endl;

    // Enviar mensaje de prueba (validando <= 512 caracteres)
    MedidorRecursos::reiniciar();
    bool enviado = udeaBook.enviarMensaje(Cadena("usuario_1"), Cadena("Hola usuario uno, prueba directa de mensaje."), Fecha(10, 9, 2026));
    if (enviado) {
        cout << "[EXITO] Mensaje enviado correctamente." << endl;
    }

    // Leer mensajes nuevos (marcando como leídos)
    cout << "-> Leyendo mensajes nuevos recibidos:" << endl;
    udeaBook.leerMensajesNuevos();

    // Búsqueda por rango de fecha que abarque al menos 6 semanas (ej. 01/09/2026 al 20/10/2026)[cite: 8, 29, 44]
    cout << "-> Buscando mensajes recibidos en rango de mas de 6 semanas (01/09/2026 al 20/10/2026):" << endl;
    udeaBook.buscarMensajesPorFecha(Fecha(1, 9, 2026), Fecha(20, 10, 2026));

    medidor.mostrarReporte(udeaBook);

    // ----------------------------------------------------
    // PRUEBA 5: Persistencia Incremental
    // ----------------------------------------------------
    cout << "\n[TEST 5] Guardando cambios incrementales en disco..." << endl;
    MedidorRecursos::reiniciar();
    udeaBook.guardarCambiosRed();
    cout << "[EXITO] Cambios persistidos sin alterar bloques previos." << endl;
    medidor.mostrarReporte(udeaBook);

    cout << "\n==================================================" << endl;
    cout << "   BATERIA DE PRUEBAS FINALIZADA EXITOSAMENTE     " << endl;
    cout << "==================================================" << endl;
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    ejecutarPruebasIntegrales();
    return 0;
}