// ListaDinamica.h
#ifndef LISTADINAMICA_H
#define LISTADINAMICA_H

#include "MedidorRecursos.h"

using namespace std;

// Arreglo dinamico que crece (capacidad x2). Plantilla propia, sin STL.
// Requisito para T: constructor por defecto y operador de asignacion.
template <typename T>
class ListaDinamica {
private:
    T* datos;
    int tamano;
    int capacidad;

public:
    ListaDinamica() : datos(new T[10]), tamano(0), capacidad(10) {
    }

    ListaDinamica(const ListaDinamica<T>& otra)
        : datos(nullptr), tamano(otra.tamano), capacidad(otra.capacidad) {
        datos = new T[capacidad];
        for (int i = 0; i < tamano; i++) {
            MedidorRecursos::contarIteracion();
            datos[i] = otra.datos[i];
        }
    }

    ~ListaDinamica() {
        delete[] datos;
    }

    ListaDinamica<T>& operator=(const ListaDinamica<T>& otra) {
        if (this != &otra) {
            T* nuevo = new T[otra.capacidad];
            for (int i = 0; i < otra.tamano; i++) {
                MedidorRecursos::contarIteracion();
                nuevo[i] = otra.datos[i];
            }
            delete[] datos;
            datos = nuevo;
            tamano = otra.tamano;
            capacidad = otra.capacidad;
        }
        return *this;
    }

    // Seguro incluso si elem es una referencia a un elemento de esta misma lista:
    // el arreglo viejo se libera despues de copiar elem.
    void agregar(const T& elem) {
        if (tamano == capacidad) {
            int nuevaCapacidad = capacidad * 2;
            T* nuevo = new T[nuevaCapacidad];
            for (int i = 0; i < tamano; i++) {
                MedidorRecursos::contarIteracion();
                nuevo[i] = datos[i];
            }
            nuevo[tamano] = elem;
            delete[] datos;
            datos = nuevo;
            capacidad = nuevaCapacidad;
        } else {
            datos[tamano] = elem;
        }
        tamano++;
    }

    int getTamano() const { return tamano; }
    int getCapacidad() const { return capacidad; }
    bool estaVacia() const { return tamano == 0; }
    void vaciar() { tamano = 0; }
    long bytesDinamicos() const { return static_cast<long>(capacidad) * static_cast<long>(sizeof(T)); }

    // Permite a bytesDinamicos() inspeccionar los slots construidos por defecto (0 .. capacidad-1)
    const T& getElementoCapacidad(int i) const {
        return datos[i];
    }

    T& operator[](int i) {
        if (i < 0 || i >= tamano) {
            throw "ListaDinamica: indice fuera de rango";
        }
        return datos[i];
    }

    const T& operator[](int i) const {
        if (i < 0 || i >= tamano) {
            throw "ListaDinamica: indice fuera de rango";
        }
        return datos[i];
    }
};

#endif // LISTADINAMICA_H