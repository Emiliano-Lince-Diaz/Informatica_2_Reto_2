#ifndef MEDIDORRECURSOS_H
#define MEDIDORRECURSOS_H

#include <iostream>

class RedSocial;

class MedidorRecursos {
private:
    static long iteraciones;

public:
    static void contarIteracion();
    static void sumarIteraciones(long cantidad);
    static void reiniciar();
    static long getIteraciones();

    long calcularMemoria(const RedSocial& red);
    void mostrarReporte(const RedSocial& red);
};

#endif // MEDIDORRECURSOS_H
