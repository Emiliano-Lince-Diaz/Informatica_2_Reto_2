#ifndef CADENA_H
#define CADENA_H

#include <cstring>

class Cadena {
private:
    char* texto;
public:
    Cadena() {
        texto = new char[1];
        texto[0] = '\0';
    }
    Cadena(const char* txt) {
        texto = new char[strlen(txt) + 1];
        strcpy(texto, txt);
    }
    // Regla de los tres
    Cadena(const Cadena& otra) {
        texto = new char[strlen(otra.texto) + 1];
        strcpy(texto, otra.texto);
    }
    Cadena& operator=(const Cadena& otra) {
        if(this != &otra) {
            delete[] texto;
            texto = new char[strlen(otra.texto) + 1];
            strcpy(texto, otra.texto);
        }
        return *this;
    }
    ~Cadena() { delete[] texto; }

    const char* obtenerTexto() const { return texto; }

    bool operator==(const Cadena& otra) const { return strcmp(texto, otra.texto) == 0; }
    bool operator<(const Cadena& otra) const { return strcmp(texto, otra.texto) < 0; }
};

#endif