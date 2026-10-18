// MedidorRecursos.cpp
#include "MedidorRecursos.h"
#include "RedSocial.h"

using namespace std;

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

// Calcula la memoria recorriendo la Red Social (estructura base + memoria dinamica)
long MedidorRecursos::calcularMemoria(const RedSocial& red) {
    long memoriaTotal = static_cast<long>(sizeof(RedSocial)) + red.bytesDinamicos();
    return memoriaTotal;
}

void MedidorRecursos::mostrarReporte(const RedSocial& red) {
    // Se guarda primero el conteo de iteraciones para no alterarlo durante calcularMemoria
    long iteracionesAlTerminar = getIteraciones();
    long memoriaUsada = calcularMemoria(red);

    cout << "\n==================================================" << endl;
    cout << "   [METRICAS DE CONSUMO - UDE@BOOK]   " << endl;
    cout << "==================================================" << endl;
    cout << "-> Total de iteraciones : " << iteracionesAlTerminar << endl;
    cout << "-> Memoria en uso actual: " << memoriaUsada << " bytes" << endl;
    cout << "==================================================\n" << endl;
}