# Reto 2: Sistema de Red Social (Estructuras de Datos Propias)

Este repositorio contiene la implementación en C++ puro de un sistema central de red social. El proyecto está diseñado bajo restricciones estrictas de rendimiento y gestión de memoria, sin poder usar la Standard Template Library (STL) de C++ (como `std::string`, `std::vector` o algoritmos predefinidos).

## Funcionalidades Principales

*   **Búsqueda optimizada de usuarios:** Localización de perfiles en $O(1)$ mediante una Tabla Hash con encadenamiento.
*   **Amigos en común y sugerencias (Top-K):** Cálculo ultrarrápido de intersecciones sociales utilizando una matriz de adyacencia implementada a nivel de bits y el algoritmo de Brian Kernighan.
*   **Mensajería privada por rangos de fecha:** Búsqueda binaria $O(\log M)$ sobre historiales de chat locales para consultas por fecha, con gestión eficiente de estados de lectura.
*   **Feed ponderado de perfiles:** Sistema probabilístico de aparición de fotos basado en pesos (2, 5 y 7) según la cantidad de amigos interconectados en común.
*   **Carrusel circular de fotos:** Navegación ininterrumpida de álbumes en $O(1)$ utilizando arreglos dinámicos y aritmética modular.

## Arquitectura y Estructuras de Datos

Todo el ecosistema de datos ha sido construido desde cero para garantizar un control absoluto sobre el *Heap* y el *Stack*:

*   `ListaDinamica<T>`: Arreglo genérico redimensionable que actúa como base para la mayoría de colecciones del sistema.
*   `Cadena`: Clase orientada al manejo seguro de caracteres y parseo de archivos de texto plano utilizando búferes estáticos.
*   `Fecha` y `FechaHora`: Gestión de marcas temporales y validación de rangos cronológicos.
*   `MedidorRecursos`: Módulo de auditoría técnica integrado mediante la sobrecarga global de los operadores `new` y `delete` para garantizar cero fugas de memoria y contabilizar iteraciones de los algoritmos.

## Estructura de Archivos de Datos

La persistencia se maneja a través de archivos de texto plano (`.txt`) delimitados por el carácter tubería (`|`). El sistema construye las relaciones en memoria al iniciar la ejecución a partir de los siguientes archivos generados masivamente (≥ 1000 usuarios):

*   `usuarios.txt`
*   `amistades.txt`
*   `solicitudes.txt`
*   `albumes.txt`
*   `fotos.txt`
*   `mensajes.txt`

## Compilación y Ejecución

Para compilar el proyecto utilizando GCC (g++):

```bash
# Compilar todos los archivos fuente
g++ -Wall -Wextra -std=c++11 src/*.cpp -o red_social

# Ejecutar el programa
./red_social
```
