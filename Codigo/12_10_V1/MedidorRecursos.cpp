#include "MedidorRecursos.h"
// #include "RedSocial.h" // Descomentar cuando la clase RedSocial este creada

using namespace std;

// Inicializacion del atributo estatico
long MedidorRecursos::iteraciones = 0;

void MedidorRecursos::contarIteracion() {
    iteraciones++;
}

// Reinicia el contador
void MedidorRecursos::reiniciar() {
    iteraciones = 0;
}

long MedidorRecursos::getIteraciones() {
    return iteraciones;
}

// Metodo que calcula la memoria recorriendo la Red Social
long MedidorRecursos::calcularMemoria(const RedSocial& red) {
    long memoriaTotal = 0;

    // Terminar cuando tengamos la clase RedSocial...

    return memoriaTotal; // Retorna 0 temporalmente hasta integrar RedSocial
}

void MedidorRecursos::mostrarReporte(const RedSocial& red) {
    long memoriaUsada = calcularMemoria(red);

    cout << "\n==================================================" << endl;
    cout << "   [METRICAS DE CONSUMO - UDE@BOOK]   " << endl;
    cout << "==================================================" << endl;
    cout << "-> Total de iteraciones : " << getIteraciones() << endl;
    cout << "-> Memoria en uso actual: " << memoriaUsada << " bytes" << endl;
    cout << "==================================================\n" << endl;
}