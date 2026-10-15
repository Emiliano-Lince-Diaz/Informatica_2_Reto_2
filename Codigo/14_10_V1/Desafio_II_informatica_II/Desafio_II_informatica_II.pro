TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        BuscadorUsuarios.cpp \
        Cadena.cpp \
        MedidorRecursos.cpp \
        main.cpp \
        tablaHashUsuarios.cpp

HEADERS += \
    Cadena.h \
    ListaDinamica.h \
    MedidorRecursos.h \
    Usuario.h \
    buscadorUsuarios.h \
    tablaHashUsuarios.h
