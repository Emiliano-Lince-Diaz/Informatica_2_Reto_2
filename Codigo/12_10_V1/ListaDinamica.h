#ifndef LISTADINAMICA_H
#define LISTADINAMICA_H

template <typename T>
class ListaDinamica {
private:
    T* datos;
    int tamano;
    int capacidad;

public:
    // Constructor por defecto
    ListaDinamica() {
        capacidad = 10; // Capacidad inicial
        tamano = 0;
        datos = new T[capacidad];
    }

    // Constructor de copia
    ListaDinamica(const ListaDinamica<T>& otra) {
        capacidad = otra.capacidad;
        tamano = otra.tamano;
        datos = new T[capacidad];

        // Copiar los elementos uno por uno
        for (int i = 0; i < tamano; i++) {
            datos[i] = otra.datos[i];
        }
    }

    // Sobrecarga del Operador de Asignación
    ListaDinamica<T>& operator=(const ListaDinamica<T>& otra) {
        if (this != &otra) { // Evitar auto-asignación
            delete[] datos;  // Liberar la memoria vieja

            capacidad = otra.capacidad;
            tamano = otra.tamano;
            datos = new T[capacidad];

            for (int i = 0; i < tamano; i++) {
                datos[i] = otra.datos[i];
            }
        }
        return *this;
    }

    // Destructor
    ~ListaDinamica() {
        delete[] datos;
    }

    // Método para agregar elementos dinámicamente
    void agregar(const T& elem) {
        // Si el arreglo se llenó, necesita crecer
        if (tamano == capacidad) {
            capacidad = capacidad * 2; // Duplicar capacidad
            T* nuevosDatos = new T[capacidad]; // Reservar nueva memoria

            // Pasar los datos viejos al nuevo arreglo
            for (int i = 0; i < tamano; i++) {
                nuevosDatos[i] = datos[i];
            }

            delete[] datos; // Liberar el arreglo viejo
            datos = nuevosDatos; // Apuntar al nuevo arreglo
        }

        datos[tamano] = elem; // Insertar el elemento
        tamano++; // Aumentar contador
    }

    // Getter
    int getTamano() const {
        return tamano;
    }

    // Sobrecarga del operador [] para acceder como un arreglo normal
    T& operator[](int i) {
        return datos[i];
    }

    // Sobrecarga del operador [] para pasos por referencia constante
    const T& operator[](int i) const {
        return datos[i];
    }
};

#endif // LISTADINAMICA_H


