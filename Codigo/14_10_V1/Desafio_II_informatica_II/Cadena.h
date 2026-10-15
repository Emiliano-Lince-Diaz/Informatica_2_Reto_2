#ifndef CADENA_H
#define CADENA_H

#include <iostream>

// Texto dinamico propio (reemplaza a la cadena de la libreria estandar).
// Invariante: datos siempre apunta a un arreglo de longitud+1 caracteres
// terminado en '\0' (nunca es nullptr).
class Cadena {
private:
    char* datos;
    int longitud;

public:
    // --- Forma canonica (constructor, copia, asignacion, destructor) ---
    Cadena();
    Cadena(const char* texto);
    Cadena(const Cadena& otra);
    ~Cadena();
    Cadena& operator=(const Cadena& otra);

    // --- Consulta ---
    int getLongitud() const;
    const char* getDatos() const;
    const char* obtenerTexto() const;   // alias de getDatos() (compatibilidad con el codigo del equipo)
    long bytesDinamicos() const;

    // --- Operadores ---
    bool operator==(const Cadena& otra) const;
    bool operator!=(const Cadena& otra) const;
    bool operator<(const Cadena& otra) const;
    Cadena operator+(const Cadena& otra) const;
    Cadena& operator+=(const Cadena& otra);
    char operator[](int i) const;   // fuera de rango devuelve '\0'

    // --- Utilidades de texto ---
    Cadena subcadena(int inicio, int cantidad) const;
    int buscar(char c, int desde = 0) const;          // -1 si no esta
    Cadena campo(int indice, char separador) const;   // campo N de "a;b;c"
    Cadena desdeCampo(int indice, char separador) const; // del campo N hasta el final
    int cantidadCampos(char separador) const;
    bool aEntero(int& resultado) const;               // false si no es entero valido
    unsigned long hash() const;                       // djb2, para la tabla hash

    static Cadena desdeEntero(long valor);

    friend std::ostream& operator<<(std::ostream& salida, const Cadena& c);
};

#endif // CADENA_H
