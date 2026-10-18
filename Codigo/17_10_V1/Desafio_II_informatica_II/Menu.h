// Menu.h
#ifndef MENU_H
#define MENU_H

#include "RedSocial.h"
#include "MedidorRecursos.h"
#include "Cadena.h"
#include "Fecha.h"

class Menu {
private:
    RedSocial* red;
    MedidorRecursos medidor;

    int leerEnteroSeguro(const char* prompt, int minVal, int maxVal);
    Cadena leerCadenaNoVacia(const char* prompt);
    Fecha leerFechaSegura(const char* prompt);

public:
    Menu(RedSocial* r);
    void mostrarLogin();
    void mostrarMenuPrincipal();
    void ejecutar();
};

#endif // MENU_H