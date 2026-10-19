// MedidorRecursos.cpp
#include "MedidorRecursos.h"
#include "RedSocial.h"
#include "Menu.h"

using namespace std;

// Inicializacion de los atributos estaticos
long MedidorRecursos::iteraciones = 0;
long MedidorRecursos::bytesLocalesFuncionalidad = 0;

void MedidorRecursos::contarIteracion() {
    iteraciones++;
}

void MedidorRecursos::sumarIteraciones(long cantidad) {
    iteraciones += cantidad;
}

void MedidorRecursos::registrarMemoriaLocal(long bytes) {
    bytesLocalesFuncionalidad += bytes;
}

long MedidorRecursos::getMemoriaLocal() {
    return bytesLocalesFuncionalidad;
}

void MedidorRecursos::reiniciar() {
    iteraciones = 0;
    bytesLocalesFuncionalidad = 0;
}

long MedidorRecursos::getIteraciones() {
    return iteraciones;
}

// Calcula la memoria total en uso:
// 1. Objetos base en Stack (RedSocial + Menu + MedidorRecursos)
// 2. Toda la memoria dinamica del sistema en el Heap (incluyendo slots de capacidad en ListaDinamica)
// 3. Variables locales, parametros por valor y estructuras temporales de la funcionalidad ejecutada
long MedidorRecursos::calcularMemoria(const RedSocial& red) {
    long memoriaBaseStack = static_cast<long>(sizeof(RedSocial) + sizeof(Menu) + sizeof(MedidorRecursos));
    long memoriaTotal = memoriaBaseStack + red.bytesDinamicos() + bytesLocalesFuncionalidad;
    return memoriaTotal;
}

void MedidorRecursos::mostrarReporte(const RedSocial& red) {
    long iteracionesAlTerminar = getIteraciones();
    long memoriaUsada = calcularMemoria(red);

    cout << "\n==================================================" << endl;
    cout << "   [METRICAS DE CONSUMO - UDE@BOOK]   " << endl;
    cout << "==================================================" << endl;
    cout << "-> Total de iteraciones : " << iteracionesAlTerminar << endl;
    cout << "-> Memoria en uso actual: " << memoriaUsada << " bytes" << endl;
    cout << "   (Heap global + Stack base + " << bytesLocalesFuncionalidad << " bytes locales/temporales)" << endl;
    cout << "==================================================\n" << endl;
}