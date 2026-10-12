#ifndef TABLAHASHUSUARIOS_H
#define TABLAHASHUSUARIOS_H

#include "Cadena.h"

using namespace std;

class TablaHashUsuarios {
private:
    int* indices;       // Arreglo dinámico para guardar las posiciones
    Cadena* nombres;    // Arreglo dinámico para guardar las llaves y resolver colisiones
    int capacidad;      // Tamaño total de la tabla
    int tamano;         // Cantidad de elementos actuales

    // Método privado para calcular el hash
    unsigned long calcularHash(const Cadena& str) const;

public:
    TablaHashUsuarios(int cap = 200000);
    ~TablaHashUsuarios(); // Destructor

    void insertar(const Cadena& nombre, int idx);
    int buscar(const Cadena& nombre) const;
};

#endif // TABLAHASHUSUARIOS_H
