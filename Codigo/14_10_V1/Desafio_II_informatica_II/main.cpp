// Bateria de pruebas de las estructuras (Cadena, ListaDinamica, BuscadorUsuarios,
// TablaHashUsuarios y MedidorRecursos). Es un arnes de pruebas, no el programa final:
// el main real de Ude@Book debe vivir aparte.

#include <iostream>

#include "Cadena.h"
#include "Usuario.h"
#include "ListaDinamica.h"
#include "BuscadorUsuarios.h"
#include "TablaHashUsuarios.h"
#include "MedidorRecursos.h"

using std::cout;
using std::endl;

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

// prefijo + numero de 6 digitos con ceros a la izquierda (sin funciones de C).
Cadena generarNombre(int i, const char* prefijo = "usuario_") {
    Cadena numero = Cadena::desdeEntero(i);
    Cadena resultado(prefijo);
    for (int k = numero.getLongitud(); k < 6; k++) {
        resultado += Cadena("0");
    }
    resultado += numero;
    return resultado;
}

unsigned long hashPrueba(const Cadena& nombre, int capacidad) {
    return nombre.hash() % static_cast<unsigned long>(capacidad);
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

    // Agregar un elemento de la propia lista justo cuando se llena (antes: use-after-free).
    ListaDinamica<int> llena;
    for (int i = 0; i < 10; i++) {
        llena.agregar(i * 11);
    }
    llena.agregar(llena[0]);

    registrarResultado(
        llena.getTamano() == 11 && llena[10] == 0 && llena[9] == 99,
        "agregar(lista[0]) al crecer la lista no usa memoria liberada."
        );

    registrarResultado(
        ListaDinamica<int>().bytesDinamicos() ==
            static_cast<long>(10 * sizeof(int)),
        "bytesDinamicos de una lista nueva = capacidad * sizeof(T)."
        );
}

// ============================================================
// PRUEBA 2: BUSQUEDA BINARIA Y TABLA HASH
// ============================================================

void pruebasBuscadores() {
    cout << "\n--- PRUEBA 2: BUSCADORES ---" << endl;

    ListaDinamica<Usuario> usuarios;
    const int numUsuarios = 200000;

    TablaHashUsuarios tabla;            // capacidad por defecto: crece sola (rehash)
    BuscadorUsuarios buscador;

    cout << "Insertando " << numUsuarios << " usuarios..." << endl;

    bool todasInsertadas = true;
    for (int i = 0; i < numUsuarios; i++) {
        Cadena nombre = generarNombre(i);
        usuarios.agregar(Usuario(nombre));
        todasInsertadas = tabla.insertar(nombre, usuarios.getTamano() - 1) && todasInsertadas;
    }

    registrarResultado(
        usuarios.getTamano() == numUsuarios &&
            todasInsertadas && tabla.getTamano() == numUsuarios,
        "Insercion de 200.000 usuarios en la lista y en la tabla (con rehash)."
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
        "Busqueda binaria (lista ordenada): extremos, centro y nombre inexistente."
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

    // Ningun usuario se pierde: cada uno se encuentra en su indice.
    bool todosEncontrados = true;
    for (int i = 0; i < numUsuarios && todosEncontrados; i += 97) {
        todosEncontrados = tabla.buscar(generarNombre(i)) == i;
    }
    registrarResultado(todosEncontrados, "Tabla hash: usuarios muestreados en su indice correcto.");
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
        tabla.buscar(nombre) == 20 && tabla.getTamano() == 1,
        "Insertar una clave duplicada actualiza su indice sin ocupar otra casilla."
        );
}

// ============================================================
// PRUEBA 6: CRECIMIENTO (ya no se rechazan inserciones)
// ============================================================

void pruebaCrecimientoTabla() {
    cout << "\n--- PRUEBA 6: CRECIMIENTO DE LA TABLA ---" << endl;

    TablaHashUsuarios tabla(4);

    Cadena n1("ana");
    Cadena n2("bruno");
    Cadena n3("carlos");

    bool ok1 = tabla.insertar(n1, 1);
    bool ok2 = tabla.insertar(n2, 2);
    bool ok3 = tabla.insertar(n3, 3);   // antes se rechazaba en silencio

    registrarResultado(
        ok1 && ok2 && ok3 &&
            tabla.buscar(n1) == 1 &&
            tabla.buscar(n2) == 2 &&
            tabla.buscar(n3) == 3,
        "La tercera insercion ya no se pierde: la tabla crece."
        );

    TablaHashUsuarios pequena(4);
    bool todos = true;
    for (int i = 0; i < 1200; i++) {
        todos = pequena.insertar(generarNombre(i), i) && todos;
    }
    for (int i = 0; i < 1200 && todos; i++) {
        todos = pequena.buscar(generarNombre(i)) == i;
    }
    registrarResultado(
        todos && pequena.getTamano() == 1200 && pequena.getCapacidad() >= 2400,
        "1200 usuarios en una tabla de capacidad 4: todos se encuentran, carga <= 50 %."
        );

    registrarResultado(
        !pequena.insertar(Cadena("negativo"), -1) && pequena.buscar(Cadena("negativo")) == -1,
        "Un indice negativo se rechaza explicitamente (devuelve false)."
        );
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

    registrarResultado(
        TablaHashUsuarios(0).getCapacidad() >= 4 &&
            TablaHashUsuarios(-5).buscar(nombre) == -1,
        "Capacidades invalidas (0, negativas) se corrigen y no dividen por cero."
        );
}

// ============================================================
// PRUEBA 8: COPIA DE LA TABLA Y REFERENCIA EN USUARIO
// ============================================================

void pruebaCopiaYReferencias() {
    cout << "\n--- PRUEBA 8: COPIA PROFUNDA Y SIN COPIAS ---" << endl;

    TablaHashUsuarios a(101);
    a.insertar(Cadena("x"), 1);

    TablaHashUsuarios b = a;            // antes: double free al destruir
    b.insertar(Cadena("y"), 2);

    TablaHashUsuarios c(7);
    c = a;
    c = c;
    c.insertar(Cadena("z"), 3);

    registrarResultado(
        a.buscar(Cadena("y")) == -1 && a.buscar(Cadena("z")) == -1 &&
            b.buscar(Cadena("x")) == 1 && b.buscar(Cadena("y")) == 2 &&
            c.buscar(Cadena("x")) == 1 && c.buscar(Cadena("z")) == 3,
        "Constructor de copia y asignacion de la tabla son independientes."
        );

    Usuario u(Cadena("ana"));
    registrarResultado(
        &u.getNombreUsuario() == &u.getNombreUsuario(),
        "getNombreUsuario devuelve una referencia (no copia el nombre)."
        );

    registrarResultado(
        a.bytesDinamicos() > 0 && b.bytesDinamicos() > a.bytesDinamicos(),
        "bytesDinamicos crece al guardar mas llaves."
        );
}

// ============================================================
// PRUEBA 9: COMPARACION DE ITERACIONES (binaria vs hash)
// Se compara con el MedidorRecursos, que es la metrica del enunciado.
// Los nombres se generan antes de medir, asi el conteo es solo de la busqueda.
// ============================================================

void compararRendimiento() {
    cout << "\n--- PRUEBA 9: COMPARACION DE ITERACIONES ---" << endl;

    ListaDinamica<Usuario> usuarios;
    const int numUsuarios = 200000;
    const int numConsultas = 5000;

    TablaHashUsuarios tabla;
    BuscadorUsuarios buscador;

    cout << "Preparando datos..." << endl;

    for (int i = 0; i < numUsuarios; i++) {
        Cadena nombre = generarNombre(i);
        usuarios.agregar(Usuario(nombre));
        tabla.insertar(nombre, i);
    }

    ListaDinamica<Cadena> existentes;
    ListaDinamica<Cadena> inexistentes;
    for (int i = 0; i < numConsultas; i++) {
        existentes.agregar(generarNombre((i * 37) % numUsuarios));
        inexistentes.agregar(generarNombre(i, "inexistente_"));
    }

    long long sumaBinaria = 0;
    MedidorRecursos::reiniciar();
    for (int i = 0; i < numConsultas; i++) {
        sumaBinaria += buscador.buscarPorNombre(usuarios, existentes[i]);
    }
    long iterBinariaExiste = MedidorRecursos::getIteraciones();

    long long sumaHash = 0;
    MedidorRecursos::reiniciar();
    for (int i = 0; i < numConsultas; i++) {
        sumaHash += tabla.buscar(existentes[i]);
    }
    long iterHashExiste = MedidorRecursos::getIteraciones();

    registrarResultado(
        sumaBinaria == sumaHash,
        "Ambos buscadores devuelven indices coincidentes."
        );

    cout << "\nIteraciones en " << numConsultas << " busquedas existentes:" << endl;
    cout << "Busqueda binaria: " << iterBinariaExiste << endl;
    cout << "Tabla hash:       " << iterHashExiste << endl;

    long long fallosBinarios = 0;
    long long fallosHash = 0;

    MedidorRecursos::reiniciar();
    for (int i = 0; i < numConsultas; i++) {
        fallosBinarios += (buscador.buscarPorNombre(usuarios, inexistentes[i]) == -1);
    }
    long iterBinariaFalla = MedidorRecursos::getIteraciones();

    MedidorRecursos::reiniciar();
    for (int i = 0; i < numConsultas; i++) {
        fallosHash += (tabla.buscar(inexistentes[i]) == -1);
    }
    long iterHashFalla = MedidorRecursos::getIteraciones();

    registrarResultado(
        fallosBinarios == numConsultas &&
            fallosHash == numConsultas,
        "Ambos buscadores rechazan todos los nombres inexistentes."
        );

    cout << "\nIteraciones en " << numConsultas << " busquedas inexistentes:" << endl;
    cout << "Busqueda binaria: " << iterBinariaFalla << endl;
    cout << "Tabla hash:       " << iterHashFalla << endl;

    cout << "\nNota: cuentan las iteraciones propias y las de Cadena (comparaciones, hash)." << endl;
    cout << "No incluyen la construccion inicial ni la generacion de los nombres." << endl;
}

// ============================================================
// PRUEBA 10: MEDIDOR DE RECURSOS
// ============================================================

void pruebaMedidorRecursos() {
    cout << "\n--- PRUEBA 10: MEDIDOR DE RECURSOS ---" << endl;

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

    MedidorRecursos::sumarIteraciones(7);
    registrarResultado(
        MedidorRecursos::getIteraciones() == 10,
        "sumarIteraciones agrega un bloque de iteraciones de una vez."
        );
    MedidorRecursos::sumarIteraciones(-7);

    // 5. Ejecucion de calcularMemoria.
    long memoria = medidor1.calcularMemoria(redPrueba);

    // La implementacion actual retorna cero temporalmente.
    // Esta prueba debe cambiar cuando se implemente la medicion real.
    registrarResultado(
        memoria == 0,
        "calcularMemoria retorna cero segun la implementacion provisional."
        );

    // 6. Mostrar el reporte (se imprime abajo) no debe alterar el contador.
    cout << "\nReporte generado:" << endl;
    medidor1.mostrarReporte(redPrueba);

    registrarResultado(
        MedidorRecursos::getIteraciones() == 3,
        "Mostrar el reporte no modifica el contador de iteraciones."
        );
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
    pruebaCrecimientoTabla();
    pruebaTablaVacia();
    pruebaCopiaYReferencias();
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
