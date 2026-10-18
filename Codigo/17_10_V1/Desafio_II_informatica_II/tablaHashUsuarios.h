#ifndef TABLAHASHUSUARIOS_H
#define TABLAHASHUSUARIOS_H

#include "Cadena.h"

// Tabla hash con direccionamiento abierto (sondeo lineal).
// Mantiene la carga <= 50 %: si una insercion la superaria, la tabla crece sola
// (rehash), asi que insertar nunca descarta usuarios.
class TablaHashUsuarios {
private:
    int* indices;       // Posicion del usuario en la lista; -1 = casilla vacia
    Cadena** nombres;   // Llave de cada casilla ocupada (nullptr si esta vacia):
                        // solo se reserva memoria para las casillas realmente usadas
    int capacidad;      // Tamano total de la tabla
    int tamano;         // Cantidad de elementos actuales

    void reservar(int cap);
    void liberar();
    void intercambiar(TablaHashUsuarios& otra);
    int encontrarPosicion(const Cadena& nombre) const;  // casilla de la llave, o -1
    void colocar(const Cadena& nombre, int idx);        // requiere casilla libre y llave nueva
    void crecer();

public:
    TablaHashUsuarios(int cap = 2003);
    TablaHashUsuarios(const TablaHashUsuarios& otra);
    ~TablaHashUsuarios();
    TablaHashUsuarios& operator=(const TablaHashUsuarios& otra);

    // false solo si idx < 0 (el -1 se usa como marca de casilla vacia).
    // Si la llave ya existe, actualiza su indice.
    bool insertar(const Cadena& nombre, int idx);
    int buscar(const Cadena& nombre) const;   // -1 si no existe

    int getTamano() const;
    int getCapacidad() const;
    long bytesDinamicos() const;              // para MedidorRecursos
};

#endif // TABLAHASHUSUARIOS_H
