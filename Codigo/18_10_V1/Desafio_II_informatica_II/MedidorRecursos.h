// MedidorRecursos.h
#ifndef MEDIDORRECURSOS_H
#define MEDIDORRECURSOS_H

#include <iostream>

using namespace std;

class RedSocial;

class MedidorRecursos {
private:
    static long iteraciones;
    static long bytesLocalesFuncionalidad;

public:
    static void contarIteracion();
    static void sumarIteraciones(long cantidad);
    static void registrarMemoriaLocal(long bytes);
    static long getMemoriaLocal();
    static void reiniciar();
    static long getIteraciones();

    long calcularMemoria(const RedSocial& red);
    void mostrarReporte(const RedSocial& red);
};

#endif // MEDIDORRECURSOS_H