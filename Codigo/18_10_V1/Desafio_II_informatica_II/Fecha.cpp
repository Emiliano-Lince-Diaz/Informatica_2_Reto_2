// Fecha.cpp
#include "Fecha.h"

using namespace std;

Fecha::Fecha() : dia(1), mes(1), anio(2000) {}

Fecha::Fecha(int d, int m, int a) : dia(d), mes(m), anio(a) {}

Fecha::Fecha(const Fecha& otra) : dia(otra.dia), mes(otra.mes), anio(otra.anio) {}

int Fecha::getDia() const { return dia; }
int Fecha::getMes() const { return mes; }
int Fecha::getAnio() const { return anio; }

void Fecha::setFecha(int d, int m, int a) {
    dia = d;
    mes = m;
    anio = a;
}

bool Fecha::esValida() const {
    if (anio < 1900 || anio > 2100 || mes < 1 || mes > 12 || dia < 1) {
        return false;
    }
    int diasMes[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool bisiesto = (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
    if (bisiesto) {
        diasMes[1] = 29;
    }
    return dia <= diasMes[mes - 1];
}

Cadena Fecha::aCadena() const {
    Cadena d = (dia < 10) ? (Cadena("0") + Cadena::desdeEntero(dia)) : Cadena::desdeEntero(dia);
    Cadena m = (mes < 10) ? (Cadena("0") + Cadena::desdeEntero(mes)) : Cadena::desdeEntero(mes);
    Cadena a = Cadena::desdeEntero(anio);
    return d + Cadena("/") + m + Cadena("/") + a;
}

bool Fecha::desdeCadena(const Cadena& texto, Fecha& salida) {
    char sep = '/';
    if (texto.buscar('/') == -1 && texto.buscar('-') != -1) {
        sep = '-';
    }
    if (texto.cantidadCampos(sep) != 3) {
        return false;
    }
    int p0 = 0, p1 = 0, p2 = 0;
    if (!texto.campo(0, sep).aEntero(p0) ||
        !texto.campo(1, sep).aEntero(p1) ||
        !texto.campo(2, sep).aEntero(p2)) {
        return false;
    }
    // Soporta DD/MM/AAAA o AAAA-MM-DD
    if (p0 > 31) {
        salida.setFecha(p2, p1, p0);
    } else {
        salida.setFecha(p0, p1, p2);
    }
    return salida.esValida();
}

bool Fecha::operator<(const Fecha& otra) const {
    if (anio != otra.anio) return anio < otra.anio;
    if (mes != otra.mes) return mes < otra.mes;
    return dia < otra.dia;
}

bool Fecha::operator==(const Fecha& otra) const {
    return dia == otra.dia && mes == otra.mes && anio == otra.anio;
}

bool Fecha::operator<=(const Fecha& otra) const {
    return (*this < otra) || (*this == otra);
}

ostream& operator<<(ostream& os, const Fecha& f) {
    os << f.aCadena();
    return os;
}