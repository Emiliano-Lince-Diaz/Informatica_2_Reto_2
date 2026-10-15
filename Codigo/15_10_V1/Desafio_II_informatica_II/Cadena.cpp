#include "Cadena.h"
#include "MedidorRecursos.h"

namespace {
// Longitud de un texto terminado en '\0', calculada con un ciclo propio.
int medirTexto(const char* texto) {
    int n = 0;
    if (texto != nullptr) {
        while (texto[n] != '\0') {
            MedidorRecursos::contarIteracion();
            n++;
        }
    }
    return n;
}
}

Cadena::Cadena() : datos(new char[1]), longitud(0) {
    datos[0] = '\0';
}

Cadena::Cadena(const char* texto) : datos(nullptr), longitud(0) {
    longitud = medirTexto(texto);
    datos = new char[longitud + 1];
    for (int i = 0; i < longitud; i++) {
        MedidorRecursos::contarIteracion();
        datos[i] = texto[i];
    }
    datos[longitud] = '\0';
}

Cadena::Cadena(const Cadena& otra) : datos(nullptr), longitud(otra.longitud) {
    datos = new char[longitud + 1];
    for (int i = 0; i <= longitud; i++) {
        MedidorRecursos::contarIteracion();
        datos[i] = otra.datos[i];
    }
}

Cadena::~Cadena() {
    delete[] datos;
}

Cadena& Cadena::operator=(const Cadena& otra) {
    if (this != &otra) {
        // Se reserva primero: si algo fallara, el objeto queda intacto.
        char* nuevo = new char[otra.longitud + 1];
        for (int i = 0; i <= otra.longitud; i++) {
            MedidorRecursos::contarIteracion();
            nuevo[i] = otra.datos[i];
        }
        delete[] datos;
        datos = nuevo;
        longitud = otra.longitud;
    }
    return *this;
}

int Cadena::getLongitud() const {
    return longitud;
}

const char* Cadena::getDatos() const {
    return datos;
}

long Cadena::bytesDinamicos() const {
    return static_cast<long>(longitud + 1);
}

bool Cadena::operator==(const Cadena& otra) const {
    if (longitud != otra.longitud) {
        return false;
    }
    for (int i = 0; i < longitud; i++) {
        MedidorRecursos::contarIteracion();
        if (datos[i] != otra.datos[i]) {
            return false;
        }
    }
    return true;
}

bool Cadena::operator!=(const Cadena& otra) const {
    return !(*this == otra);
}

bool Cadena::operator<(const Cadena& otra) const {
    int minimo = (longitud < otra.longitud) ? longitud : otra.longitud;
    for (int i = 0; i < minimo; i++) {
        MedidorRecursos::contarIteracion();
        unsigned char a = static_cast<unsigned char>(datos[i]);
        unsigned char b = static_cast<unsigned char>(otra.datos[i]);
        if (a != b) {
            return a < b;
        }
    }
    return longitud < otra.longitud;
}

Cadena& Cadena::operator+=(const Cadena& otra) {
    // Valido tambien para c += c: se lee todo antes de liberar el buffer viejo.
    int nuevaLongitud = longitud + otra.longitud;
    char* nuevo = new char[nuevaLongitud + 1];
    for (int i = 0; i < longitud; i++) {
        MedidorRecursos::contarIteracion();
        nuevo[i] = datos[i];
    }
    for (int j = 0; j < otra.longitud; j++) {
        MedidorRecursos::contarIteracion();
        nuevo[longitud + j] = otra.datos[j];
    }
    nuevo[nuevaLongitud] = '\0';
    delete[] datos;
    datos = nuevo;
    longitud = nuevaLongitud;
    return *this;
}

Cadena Cadena::operator+(const Cadena& otra) const {
    Cadena resultado(*this);
    resultado += otra;
    return resultado;
}

char Cadena::operator[](int i) const {
    if (i < 0 || i >= longitud) {
        return '\0';
    }
    return datos[i];
}

Cadena Cadena::subcadena(int inicio, int cantidad) const {
    if (inicio < 0) {
        inicio = 0;
    }
    if (inicio >= longitud || cantidad == 0) {
        return Cadena();
    }
    if (cantidad < 0 || inicio + cantidad > longitud) {
        cantidad = longitud - inicio;
    }
    Cadena resultado;
    delete[] resultado.datos;
    resultado.datos = new char[cantidad + 1];
    for (int i = 0; i < cantidad; i++) {
        MedidorRecursos::contarIteracion();
        resultado.datos[i] = datos[inicio + i];
    }
    resultado.datos[cantidad] = '\0';
    resultado.longitud = cantidad;
    return resultado;
}

int Cadena::buscar(char c, int desde) const {
    if (desde < 0) {
        desde = 0;
    }
    for (int i = desde; i < longitud; i++) {
        MedidorRecursos::contarIteracion();
        if (datos[i] == c) {
            return i;
        }
    }
    return -1;
}

Cadena Cadena::campo(int indice, char separador) const {
    if (indice < 0) {
        return Cadena();
    }
    int actual = 0;
    int inicio = 0;
    for (int i = 0; i <= longitud; i++) {
        MedidorRecursos::contarIteracion();
        if (i == longitud || datos[i] == separador) {
            if (actual == indice) {
                return subcadena(inicio, i - inicio);
            }
            actual++;
            inicio = i + 1;
        }
    }
    return Cadena();
}

Cadena Cadena::desdeCampo(int indice, char separador) const {
    if (indice < 0) {
        return Cadena();
    }
    if (indice == 0) {
        return *this;
    }
    int actual = 0;
    for (int i = 0; i < longitud; i++) {
        MedidorRecursos::contarIteracion();
        if (datos[i] == separador) {
            actual++;
            if (actual == indice) {
                return subcadena(i + 1, longitud - (i + 1));
            }
        }
    }
    return Cadena();
}

int Cadena::cantidadCampos(char separador) const {
    int campos = 1;
    for (int i = 0; i < longitud; i++) {
        MedidorRecursos::contarIteracion();
        if (datos[i] == separador) {
            campos++;
        }
    }
    return campos;
}

bool Cadena::aEntero(int& resultado) const {
    int i = 0;
    bool negativo = false;
    if (longitud > 0 && datos[0] == '-') {
        negativo = true;
        i = 1;
    }
    if (i >= longitud) {
        return false;                       // vacia o solo "-"
    }
    long acumulado = 0;
    for (; i < longitud; i++) {
        MedidorRecursos::contarIteracion();
        if (datos[i] < '0' || datos[i] > '9') {
            return false;
        }
        acumulado = acumulado * 10 + (datos[i] - '0');
        if (acumulado > 2000000000L) {
            return false;                   // evita desbordar int
        }
    }
    resultado = static_cast<int>(negativo ? -acumulado : acumulado);
    return true;
}

unsigned long Cadena::hash() const {
    unsigned long h = 5381;
    for (int i = 0; i < longitud; i++) {
        MedidorRecursos::contarIteracion();
        h = h * 33 + static_cast<unsigned char>(datos[i]);
    }
    return h;
}

Cadena Cadena::desdeEntero(long valor) {
    char buffer[24];
    int pos = 23;
    buffer[pos] = '\0';
    bool negativo = valor < 0;
    unsigned long magnitud = negativo ? 0UL - static_cast<unsigned long>(valor)
                                      : static_cast<unsigned long>(valor);
    if (magnitud == 0) {
        buffer[--pos] = '0';
    }
    while (magnitud > 0) {
        MedidorRecursos::contarIteracion();
        buffer[--pos] = static_cast<char>('0' + magnitud % 10);
        magnitud /= 10;
    }
    if (negativo) {
        buffer[--pos] = '-';
    }
    return Cadena(buffer + pos);
}

std::ostream& operator<<(std::ostream& salida, const Cadena& c) {
    salida << c.datos;
    return salida;
}

const char* Cadena::obtenerTexto() const {
    return datos;
}
