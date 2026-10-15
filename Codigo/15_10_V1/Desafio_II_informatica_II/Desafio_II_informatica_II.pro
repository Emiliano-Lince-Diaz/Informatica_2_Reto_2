TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        BuscadorUsuarios.cpp \
        Cadena.cpp \
        Fecha.cpp \
        MedidorRecursos.cpp \
        Mensaje.cpp \
        Menu.cpp \
        RedSocial.cpp \
        RelacionesAmistad.cpp \
        Usuario.cpp \
        generador_datos.cpp \
        main.cpp \
        tablaHashUsuarios.cpp

HEADERS += \
    Album.h \
    Cadena.h \
    Fecha.h \
    Foto.h \
    ListaDinamica.h \
    MedidorRecursos.h \
    Mensaje.h \
    Menu.h \
    RedSocial.h \
    RelacionesAmistad.h \
    Usuario.h \
    buscadorUsuarios.h \
    tablaHashUsuarios.h
