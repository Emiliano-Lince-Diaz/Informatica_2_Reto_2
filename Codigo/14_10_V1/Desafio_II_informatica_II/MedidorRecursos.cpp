#include "MedidorRecursos.h"
// #include "RedSocial.h" // Descomentar cuando la clase RedSocial este creada

// Inicializacion del atributo estatico
long MedidorRecursos::iteraciones = 0;

void MedidorRecursos::contarIteracion() {
    iteraciones++;
}

// Para sumar de una vez las iteraciones de un bloque conocido.
void MedidorRecursos::sumarIteraciones(long cantidad) {
    iteraciones += cantidad;
}

// Reinicia el contador
void MedidorRecursos::reiniciar() {
    iteraciones = 0;
}

long MedidorRecursos::getIteraciones() {
    return iteraciones;
}

// Metodo que calcula la memoria recorriendo la Red Social.
// Cuando exista RedSocial: sumar los bytesDinamicos() de cada estructura
// (ListaDinamica, RelacionesAmistad, TablaHashUsuarios, Cadena...) mas los
// sizeof de las variables locales y parametros por valor de la funcionalidad.
long MedidorRecursos::calcularMemoria(const RedSocial& /*red*/) {
    long memoriaTotal = 0;

    // Terminar cuando tengamos la clase RedSocial...

    return memoriaTotal; // Retorna 0 temporalmente hasta integrar RedSocial
}

void MedidorRecursos::mostrarReporte(const RedSocial& red) {
    // Se calcula la memoria sin alterar el contador de iteraciones que se va a mostrar.
    long iteracionesAlTerminar = getIteraciones();
    long memoriaUsada = calcularMemoria(red);

    std::cout << "\n==================================================" << std::endl;
    std::cout << "   [METRICAS DE CONSUMO - UDE@BOOK]   " << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << "-> Total de iteraciones : " << iteracionesAlTerminar << std::endl;
    std::cout << "-> Memoria en uso actual: " << memoriaUsada << " bytes" << std::endl;
    std::cout << "==================================================\n" << std::endl;
}
