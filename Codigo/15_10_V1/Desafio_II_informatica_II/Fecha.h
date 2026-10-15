// Fecha.h
#ifndef FECHA_H
#define FECHA_H

#include "Cadena.h"
#include <iostream>

using namespace std;

class Fecha {
private:
    int dia;
    int mes;
    int anio;

public:
    Fecha();
    Fecha(int d, int m, int a);
    Fecha(const Fecha& otra);

    int getDia() const;
    int getMes() const;
    int getAnio() const;
    void setFecha(int d, int m, int a);

    bool esValida() const;
    Cadena aCadena() const;
    static bool desdeCadena(const Cadena& texto, Fecha& salida);

    bool operator<(const Fecha& otra) const;
    bool operator==(const Fecha& otra) const;
    bool operator<=(const Fecha& otra) const;

    friend ostream& operator<<(ostream& os, const Fecha& f);
};

#endif // FECHA_H