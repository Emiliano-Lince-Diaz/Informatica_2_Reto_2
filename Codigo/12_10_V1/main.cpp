
#include <iostream>
#include <cstdio>
#include <chrono>
#include <sstream>
#include <string>

#include "Cadena.h"
#include "Usuario.h"
#include "ListaDinamica.h"
#include "buscadorUsuarios.h"
#include "tablaHashUsuarios.h"
#include "MedidorRecursos.h"

using namespace std;
using namespace std::chrono;

// ============================================================
// MOCK TEMPORAL: simula RedSocial mientras se implementa.
// Cuando exista RedSocial real, elimina esta clase de prueba
// y utiliza la definicion real.
// ============================================================

class RedSocial {
private:
    int datosSimulados[1000];

public:
    RedSocial() {
        for (int i = 0; i < 1000; i++) {
            datosSimulados[i] = 0;
        }
    }
};

// ============================================================
// UTILIDADES DE PRUEBA
// ============================================================

int pruebasFallidas = 0;

void registrarResultado(bool correcto, const char* nombre) {
    if (correcto) {
        cout << "[EXITO] " << nombre << endl;
    } else {
        cout << "[FALLO] " << nombre << endl;
        pruebasFallidas++;
    }
}

Cadena generarNombre(int i) {
    char buffer[40];
    std::sprintf(buffer, "usuario_%06d", i);
    return Cadena(buffer);
}

unsigned long hashPrueba(const Cadena& nombre, int capacidad) {
    unsigned long hash = 5381;
    const char* ptr = nombre.obtenerTexto();

    while (*ptr) {
        hash = ((hash << 5) + hash) + *ptr;
        ++ptr;
    }

    return hash % capacidad;
}

// ============================================================
// PRUEBA 1: LISTA DINAMICA Y REDIMENSIONAMIENTO
// ============================================================

void pruebasListaDinamica() {
    cout << "\n--- PRUEBA 1: LISTA DINAMICA ---" << endl;

    ListaDinamica<int> listaStress;

    cout << "Insertando 500.000 elementos..." << endl;

    for (int i = 0; i < 500000; i++) {
        listaStress.agregar(i);
    }

    bool correcto = listaStress.getTamano() == 500000;

    for (int i = 0; i < listaStress.getTamano(); i++) {
        if (listaStress[i] != i) {
            correcto = false;
            cout << "Error en indice: " << i << endl;
            break;
        }
    }

    registrarResultado(
        correcto,
        "500.000 elementos conservados correctamente."
        );

    cout << "Probando constructor de copia y asignacion..." << endl;

    ListaDinamica<int> listaCopia = listaStress;
    ListaDinamica<int> listaAsignada;
    listaAsignada = listaStress;

    listaStress[0] = -1;

    registrarResultado(
        listaStress[0] == -1 &&
            listaCopia[0] == 0 &&
            listaAsignada[0] == 0,
        "Copias profundas independientes."
        );

    listaCopia = listaCopia;

    registrarResultado(
        listaCopia.getTamano() == 500000 &&
            listaCopia[0] == 0 &&
            listaCopia[499999] == 499999,
        "Autoasignacion segura."
        );

    ListaDinamica<int> vacia;
    listaAsignada = vacia;

    bool vaciaCorrecta = listaAsignada.getTamano() == 0;

    listaAsignada.agregar(123);

    vaciaCorrecta =
        vaciaCorrecta &&
        listaAsignada.getTamano() == 1 &&
        listaAsignada[0] == 123;

    registrarResultado(
        vaciaCorrecta,
        "Asignacion de lista vacia y reutilizacion."
        );
}

// ============================================================
// PRUEBA 2: BUSQUEDA BINARIA Y TABLA HASH
// ============================================================

void pruebasBuscadores() {
    cout << "\n--- PRUEBA 2: BUSCADORES ---" << endl;

    ListaDinamica<Usuario> usuarios;
    const int numUsuarios = 200000;

    TablaHashUsuarios tabla(400000);
    BuscadorUsuarios buscador;

    cout << "Insertando " << numUsuarios << " usuarios..." << endl;

    for (int i = 0; i < numUsuarios; i++) {
        Cadena nombre = generarNombre(i);
        usuarios.agregar(Usuario(nombre));
        tabla.insertar(nombre, usuarios.getTamano() - 1);
    }

    registrarResultado(
        usuarios.getTamano() == numUsuarios,
        "Insercion de 200.000 usuarios en la lista."
        );

    Cadena primero = generarNombre(0);
    Cadena medio = generarNombre(numUsuarios / 2);
    Cadena ultimo = generarNombre(numUsuarios - 1);
    Cadena inexistente("usuario_inexistente_999");

    int b1 = buscador.buscarPorNombre(usuarios, primero);
    int b2 = buscador.buscarPorNombre(usuarios, medio);
    int b3 = buscador.buscarPorNombre(usuarios, ultimo);
    int b4 = buscador.buscarPorNombre(usuarios, inexistente);

    registrarResultado(
        b1 == 0 &&
            b2 == numUsuarios / 2 &&
            b3 == numUsuarios - 1 &&
            b4 == -1,
        "Busqueda binaria: extremos, centro y nombre inexistente."
        );

    int h1 = tabla.buscar(primero);
    int h2 = tabla.buscar(medio);
    int h3 = tabla.buscar(ultimo);
    int h4 = tabla.buscar(inexistente);

    registrarResultado(
        h1 == 0 &&
            h2 == numUsuarios / 2 &&
            h3 == numUsuarios - 1 &&
            h4 == -1,
        "Tabla hash: extremos, centro y nombre inexistente."
        );
}

// ============================================================
// PRUEBA 3: BUSQUEDA BINARIA EN LISTA VACIA
// ============================================================

void pruebaBuscadorVacio() {
    cout << "\n--- PRUEBA 3: LISTA VACIA ---" << endl;

    ListaDinamica<Usuario> usuarios;
    BuscadorUsuarios buscador;
    Cadena nombre("usuario_000001");

    registrarResultado(
        buscador.buscarPorNombre(usuarios, nombre) == -1,
        "La busqueda binaria en lista vacia devuelve -1."
        );
}

// ============================================================
// PRUEBA 4: COLISIONES HASH
// ============================================================

void pruebaColisionesHash() {
    cout << "\n--- PRUEBA 4: COLISIONES HASH ---" << endl;

    const int capacidad = 101;
    TablaHashUsuarios tabla(capacidad);

    Cadena nombre1;
    Cadena nombre2;
    bool encontrada = false;

    for (int i = 0; i < 500 && !encontrada; i++) {
        Cadena candidato1 = generarNombre(i);

        for (int j = i + 1; j < 500; j++) {
            Cadena candidato2 = generarNombre(j);

            if (hashPrueba(candidato1, capacidad) ==
                hashPrueba(candidato2, capacidad)) {
                nombre1 = candidato1;
                nombre2 = candidato2;
                encontrada = true;
                break;
            }
        }
    }

    if (!encontrada) {
        registrarResultado(
            false,
            "Encontrar dos claves con la misma posicion hash."
            );
        return;
    }

    tabla.insertar(nombre1, 111);
    tabla.insertar(nombre2, 222);

    registrarResultado(
        !(nombre1 == nombre2) &&
            tabla.buscar(nombre1) == 111 &&
            tabla.buscar(nombre2) == 222,
        "Dos claves con colision se recuperan correctamente."
        );
}

// ============================================================
// PRUEBA 5: CLAVES DUPLICADAS
// ============================================================

void pruebaClavesDuplicadas() {
    cout << "\n--- PRUEBA 5: CLAVES DUPLICADAS ---" << endl;

    TablaHashUsuarios tabla(101);
    Cadena nombre("usuario_duplicado");

    tabla.insertar(nombre, 10);
    tabla.insertar(nombre, 20);

    registrarResultado(
        tabla.buscar(nombre) == 20,
        "Insertar una clave duplicada actualiza su indice."
        );
}

// ============================================================
// PRUEBA 6: LIMITE DE CAPACIDAD
// ============================================================

void pruebaLimiteCapacidad() {
    cout << "\n--- PRUEBA 6: LIMITE DE CAPACIDAD ---" << endl;

    TablaHashUsuarios tabla(4);

    Cadena n1("ana");
    Cadena n2("bruno");
    Cadena n3("carlos");

    tabla.insertar(n1, 1);
    tabla.insertar(n2, 2);
    tabla.insertar(n3, 3);

    registrarResultado(
        tabla.buscar(n1) == 1 &&
            tabla.buscar(n2) == 2 &&
            tabla.buscar(n3) == -1,
        "Se respeta el limite de insercion actual."
        );

    cout << "Nota: la tercera insercion se rechaza silenciosamente."
         << endl;
}

// ============================================================
// PRUEBA 7: TABLA HASH VACIA
// ============================================================

void pruebaTablaVacia() {
    cout << "\n--- PRUEBA 7: TABLA HASH VACIA ---" << endl;

    TablaHashUsuarios tabla(101);
    Cadena nombre("no_existe");

    registrarResultado(
        tabla.buscar(nombre) == -1,
        "La busqueda en una tabla vacia devuelve -1."
        );
}

// ============================================================
// PRUEBA 8: COMPARACION DE RENDIMIENTO
// ============================================================

void compararRendimiento() {
    cout << "\n--- PRUEBA 8: COMPARACION DE RENDIMIENTO ---"
         << endl;

    ListaDinamica<Usuario> usuarios;
    const int numUsuarios = 200000;
    const int numConsultas = 5000;

    TablaHashUsuarios tabla(400000);
    BuscadorUsuarios buscador;

    cout << "Preparando datos..." << endl;

    for (int i = 0; i < numUsuarios; i++) {
        Cadena nombre = generarNombre(i);
        usuarios.agregar(Usuario(nombre));
        tabla.insertar(nombre, i);
    }

    long long sumaBinaria = 0;

    auto inicioBinaria = steady_clock::now();

    for (int i = 0; i < numConsultas; i++) {
        int indiceBuscado = (i * 37) % numUsuarios;
        Cadena nombre = generarNombre(indiceBuscado);
        sumaBinaria += buscador.buscarPorNombre(usuarios, nombre);
    }

    auto finBinaria = steady_clock::now();

    double tiempoBinario =
        duration<double, milli>(finBinaria - inicioBinaria).count();

    long long sumaHash = 0;

    auto inicioHash = steady_clock::now();

    for (int i = 0; i < numConsultas; i++) {
        int indiceBuscado = (i * 37) % numUsuarios;
        Cadena nombre = generarNombre(indiceBuscado);
        sumaHash += tabla.buscar(nombre);
    }

    auto finHash = steady_clock::now();

    double tiempoHash =
        duration<double, milli>(finHash - inicioHash).count();

    registrarResultado(
        sumaBinaria == sumaHash,
        "Ambos buscadores devuelven indices coincidentes."
        );

    cout << "\nResultados de " << numConsultas
         << " busquedas existentes:" << endl;
    cout << "Busqueda binaria: " << tiempoBinario << " ms" << endl;
    cout << "Tabla hash:       " << tiempoHash << " ms" << endl;

    long long fallosBinarios = 0;
    long long fallosHash = 0;

    auto inicioBinarioMiss = steady_clock::now();

    for (int i = 0; i < numConsultas; i++) {
        char buffer[40];
        std::sprintf(buffer, "inexistente_%06d", i);

        Cadena nombre(buffer);
        fallosBinarios +=
            (buscador.buscarPorNombre(usuarios, nombre) == -1);
    }

    auto finBinarioMiss = steady_clock::now();

    double tiempoBinarioMiss =
        duration<double, milli>(
            finBinarioMiss - inicioBinarioMiss
            ).count();

    auto inicioHashMiss = steady_clock::now();

    for (int i = 0; i < numConsultas; i++) {
        char buffer[40];
        std::sprintf(buffer, "inexistente_%06d", i);

        Cadena nombre(buffer);
        fallosHash += (tabla.buscar(nombre) == -1);
    }

    auto finHashMiss = steady_clock::now();

    double tiempoHashMiss =
        duration<double, milli>(
            finHashMiss - inicioHashMiss
            ).count();

    registrarResultado(
        fallosBinarios == numConsultas &&
            fallosHash == numConsultas,
        "Ambos buscadores rechazan todos los nombres inexistentes."
        );

    cout << "\nResultados de " << numConsultas
         << " busquedas inexistentes:" << endl;
    cout << "Busqueda binaria: " << tiempoBinarioMiss << " ms" << endl;
    cout << "Tabla hash:       " << tiempoHashMiss << " ms" << endl;

    cout << "\nNota: los tiempos incluyen la generacion de cada nombre."
         << endl;
    cout << "No incluyen la construccion inicial de las estructuras."
         << endl;
}

// ============================================================
// PRUEBA 9: MEDIDOR DE RECURSOS
// ============================================================

void pruebaMedidorRecursos() {
    cout << "\n--- PRUEBA 9: MEDIDOR DE RECURSOS ---" << endl;

    MedidorRecursos medidor1;
    MedidorRecursos medidor2;
    RedSocial redPrueba;

    // 1. Estado inicial y reinicio.
    MedidorRecursos::reiniciar();

    registrarResultado(
        MedidorRecursos::getIteraciones() == 0,
        "El contador comienza en cero despues del reinicio."
        );

    // 2. Conteo de iteraciones.
    for (int i = 0; i < 75; i++) {
        MedidorRecursos::contarIteracion();
    }

    registrarResultado(
        MedidorRecursos::getIteraciones() == 75,
        "El contador registra exactamente 75 iteraciones."
        );

    // 3. El contador es estatico: se comparte entre instancias.
    registrarResultado(
        medidor1.getIteraciones() == 75 &&
            medidor2.getIteraciones() == 75,
        "Las instancias comparten el mismo contador estatico."
        );

    // 4. Reinicio y conteo posterior.
    MedidorRecursos::reiniciar();
    MedidorRecursos::contarIteracion();
    MedidorRecursos::contarIteracion();
    MedidorRecursos::contarIteracion();

    registrarResultado(
        MedidorRecursos::getIteraciones() == 3,
        "El contador permite contar correctamente despues del reinicio."
        );

    // 5. Ejecucion de calcularMemoria.
    long memoria = medidor1.calcularMemoria(redPrueba);

    // La implementacion actual retorna cero temporalmente.
    // Esta prueba debe cambiar cuando se implemente la medicion real.
    registrarResultado(
        memoria == 0,
        "calcularMemoria retorna cero segun la implementacion provisional."
        );

    // 6. Capturar la salida para comprobar el formato del reporte.
    ostringstream salidaCapturada;
    streambuf* salidaOriginal = cout.rdbuf();

    cout.rdbuf(salidaCapturada.rdbuf());
    medidor1.mostrarReporte(redPrueba);
    cout.rdbuf(salidaOriginal);

    string reporte = salidaCapturada.str();

    bool reporteCorrecto =
        reporte.find("METRICAS DE CONSUMO") != string::npos &&
        reporte.find("Total de iteraciones") != string::npos &&
        reporte.find("Memoria en uso actual") != string::npos &&
        reporte.find("3") != string::npos &&
        reporte.find("0 bytes") != string::npos;

    registrarResultado(
        reporteCorrecto,
        "El reporte presenta las metricas provisionales esperadas."
        );

    cout << "\nReporte generado:" << endl;
    cout << reporte;
}

// ============================================================
// MAIN
// ============================================================

int main() {
    cout << "INICIANDO BATERIA DE PRUEBAS..." << endl;

    pruebasListaDinamica();
    pruebasBuscadores();
    pruebaBuscadorVacio();
    pruebaColisionesHash();
    pruebaClavesDuplicadas();
    pruebaLimiteCapacidad();
    pruebaTablaVacia();
    compararRendimiento();
    pruebaMedidorRecursos();

    cout << "\n----------------------------------------" << endl;

    if (pruebasFallidas == 0) {
        cout << "TODAS LAS PRUEBAS APROBADAS." << endl;
    } else {
        cout << "PRUEBAS FALLIDAS: " << pruebasFallidas << endl;
    }

    cout << "BATERIA FINALIZADA." << endl;

    return pruebasFallidas == 0 ? 0 : 1;
}
