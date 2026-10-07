# Plan de Proyecto v2 — Desafío II: *Amigos para siempre, con Ude@Book*

**Materia:** Informática II · 2026-2
**Inicio:** martes 6 de octubre · **Hito 1:** viernes 9 de octubre · **Entrega final:** viernes 16 de octubre (11 días)
**Tecnología:** C++ con POO, programa de **consola**, desarrollado en Qt Creator (proyecto C++ plano / CMake, sin módulos de Qt)
**Metodología:** Kanban con checkpoints diarios + dos hitos de entrega


---

## 2. Reglas duras (si se incumple alguna, la entrega puede ser inválida)

- [ ] Solo **C++** (nada de instrucciones de ANSI C: evitar `printf`, `malloc/free`, `FILE*`, `strcpy` y similares; usar `new/delete`, `iostream`, `fstream` y funciones propias para cadenas).
- [ ] **POO real:** mínimo 3 clases (el plan usa más).
- [ ] **Sin herencia.**
- [ ] **Sin contenedores STL** y **sin `struct`** (los nodos también son `class`). Solo se permite la librería para números aleatorios.
- [ ] Estructuras de datos **propias** y con **memoria dinámica** en el núcleo.
- [ ] Programación **modular multi-archivo** (`.h` / `.cpp`).
- [ ] Sobrecargar **al menos 2 operadores**; usar constructor de copia, getters/setters, sobrecarga de métodos, funciones amigas y plantillas.
- [ ] **No usar archivos como área de procesamiento**: se cargan en memoria al inicio y solo se escriben las actualizaciones.
- [ ] Informe **escrito por ustedes** (no por IA) y video **no generado por IA**.
- [ ] Repositorio **público**, commits **al menos cada 2 días**, sin cambios después de la fecha de entrega.
- [ ] Video en YouTube entre **5 y 15 min**, sin acelerar, con **todos** los integrantes participando.
- [ ] En Ude@: **solo dos enlaces** (repositorio y video).

---

## 3. Hitos y distribución del esfuerzo

| Hito | Fecha | Contenido |
|---|---|---|
| **H1** | Vie 9 oct | Evidencia de análisis y diseño: análisis del problema, **diagrama de clases (obligatorio, sin él no hay sustentación)**, estructuras elegidas con su justificación de eficiencia, formato de archivos. |
| **H2** | Vie 16 oct | Código fuente, informe final, enlace al video, repositorio público. |

| Rúbrica | Peso | Dónde se trabaja |
|---|---|---|
| Diagrama de clases y diseño | 10 % | Días 1–4 |
| Verificación de eficiencia y selección de estructuras | 25 % | Días 2–4 (y se valida el día 9) |
| Medición del consumo de recursos | 10 % | Diseño día 2, implementación días 5–9 |
| Implementación del menú y funcionalidades | 55 % | Días 5–9 |


---

## 4. Roles sugeridos

| Rol | Responsabilidad |
|---|---|
| Responsable de estructuras de datos | Lista dinámica, tabla hash, matriz de adyacencia, cadena propia, fecha. |
| Responsable de funcionalidades | Menú y casos de uso II–VI. |
| Responsable de persistencia y medición | Archivos (I), medidor de recursos (VII), generador de datos. |
| Responsable de documentación y video | Informe, diagramas, guion y grabación. |

---

## 5. Arquitectura propuesta 

### 5.1 Clases del dominio

| Clase | Responsabilidad |
|---|---|
| `Fecha` | Día/mes/año, comparación y formato. Sobrecarga de `operator<`, `operator==`, `operator<<`. |
| `Cadena` | Texto dinámico propio (en lugar de `std::string`), con constructor de copia y asignación. |
| `Foto` | Fecha de captura, tamaño, ruta, likes. Pertenece a un único álbum. |
| `Album` | Nombre, ruta, privacidad (público / privado / amigos), lista de fotos. |
| `Mensaje` | Emisor, receptor, texto (≤ 512 caracteres), fecha, leído/no leído. |
| `Usuario` | Nombre de usuario único, fecha de nacimiento, correo, foto de perfil, contraseña, ciudad, país, fecha de inscripción, álbumes, referencias a mensajes y solicitudes. |
| `RedSocial` (plataforma) | Coordina usuarios, relaciones, mensajes y las funcionalidades del menú. |
| `RelacionesAmistad` | Matriz de adyacencia (idealmente por bits) para amistad y conteo de amigos en común. |
| `Persistencia` | Lectura y escritura de archivos. |
| `MedidorRecursos` | Contador de iteraciones y cálculo de memoria en uso. |
| `Menu` | Interacción por consola. |

### 5.2 Estructuras propias (todas con memoria dinámica)

| Estructura | Uso | Motivo de eficiencia |
|---|---|---|
| `ListaDinamica<T>` (plantilla, arreglo que crece) | Álbumes, fotos, listas de mensajes, solicitudes | Acceso por índice O(1), `operator[]` sobrecargado. |
| Tabla hash o arreglo ordenado con búsqueda binaria | Buscar usuario por nombre | O(1) promedio / O(log n). |
| Matriz de adyacencia (bits) | Amistades | Amigos en común con operaciones sobre filas (AND + conteo de bits). |
| Listas ordenadas por fecha | Mensajes | Búsqueda por rango de fechas con búsqueda binaria. |

**Ideas de eficiencia a evaluar y justificar en el informe:**
- Mensajes guardados **una sola vez** y referenciados desde emisor y receptor (evita copias).
- Paso de objetos por **referencia constante**, no por valor.
- Sugerencias de amistad: contar amigos en común solo para los **amigos de amigos** y quedarse con los K mejores con selección parcial O(n·K), sin ordenar todo.
- Selección aleatoria ponderada de 3 amigos: pesos 1 / 2.5 / 3.5 con **ruleta (pesos acumulados)**, sin repetir dentro del grupo y sin repetir en el único refresco permitido.

### 5.3 Conceptos que el enunciado pide demostrar

| Concepto | Dónde aplicarlo (sugerencia) |
|---|---|
| Constructor de copia | `Cadena`, `ListaDinamica<T>`, `Mensaje` |
| Sobrecarga de operadores (≥ 2) | `operator[]`, `operator==`, `operator<`, `operator<<`, `operator+=` |
| Función amiga | `operator<<` de `Fecha`/`Mensaje` o acceso del medidor |
| Plantillas | `ListaDinamica<T>` |
| Sobrecarga de métodos | Constructores y métodos de búsqueda |
| Getters/setters | Todas las clases del dominio |

---

## 6. Cronograma día a día

> Regla general: **un commit por día como mínimo**, con mensaje descriptivo. Al cierre de cada día, 10 minutos de checkpoint: qué se terminó, qué bloquea, qué sigue.

### FASE 1 — Análisis y diseño (días 1–4) → entrega H1

#### Día 1 · Mar 6 oct — Análisis y arranque
- Lectura completa del enunciado por **todos**; extraer requisitos funcionales, no funcionales y restricciones (sección 2).
- Lista de **dudas para el profesor** (sección 12) y envío.
- Repositorio público creado, estructura `src/`, `include/`, `docs/`, `data/`, `tests/`; `.gitignore`; proyecto en Qt Creator (C++ plano, sin Qt).
- Tablero Kanban con todas las tareas de este plan.
- **Entregable:** repo con README inicial y primer commit; documento de requisitos.

#### Día 2 · Mié 7 oct — Modelo y estructuras
- Borrador de diagrama de clases (notación UML simplificada vista en clase) con atributos, métodos y relaciones.
- Análisis de **estructuras de datos** candidatas y tabla comparativa de complejidad por funcionalidad.
- Diseño del **formato de archivos** (usuarios, amistades, solicitudes, álbumes, fotos, mensajes) con ejemplo de líneas.
- Diseño del **medidor de recursos** (cómo se cuentan iteraciones y cómo se calcula la memoria).
- **Entregable:** diagrama v1 y tabla de eficiencia en `docs/`.

#### Día 3 · Jue 8 oct — Eficiencia y pulido del diseño
- Verificar el cumplimiento del requisito de eficiencia **antes de implementar** (25 %): estimar iteraciones y memoria para 1000 usuarios en cada funcionalidad.
- Diagrama de clases final: relaciones correctas, constructores de copia, getters/setters, operadores sobrecargados.
- Redactar (con sus propias palabras) las secciones de análisis, consideraciones y estructuras del informe.
- Esqueletos de los `.h` de todas las clases (compilan sin errores).
- **Entregable:** documento de análisis/diseño listo y revisado por todos.

#### Día 4 · Vie 9 oct — **HITO 1**
- Mañana: revisión cruzada final, exportar diagrama a PNG/PDF, etiqueta de Git `hito1-diseno`.
- **Entrega del H1** (verificar que el enlace sea accesible).
- Tarde: empezar implementación de `Cadena`, `Fecha`, `ListaDinamica<T>`.
- **Entregable:** evidencia H1 entregada.

### FASE 2 — Implementación (días 5–9)

#### Día 5 · Sáb 10 oct — Cimientos
- Terminar `Cadena`, `Fecha`, `ListaDinamica<T>`, tabla hash / búsqueda, matriz de adyacencia; pruebas pequeñas por consola de cada una.
- `MedidorRecursos` básico integrado desde ya.
- **Generador de datos** (programa aparte en C++): ≥ 1000 usuarios, amistades, solicitudes, álbumes (al menos un usuario con ≥ 2 álbumes y ≥ 5 fotos en cada uno), mensajes que abarquen ≥ 6 semanas.
- **Definición de terminado:** estructuras sin fugas y con casos límite probados.

#### Día 6 · Dom 11 oct — Funcionalidad I y II
- **I. Carga/actualización de datos** (no aparece en el menú): lectura, escritura, validación de archivos malformados.
- **II. Ingreso:** autenticación con credenciales; mostrar 3 fotos de perfil de amigos con **selección ponderada**; **un solo** refresco con un grupo distinto.
- Medición de recursos conectada a I y II.

#### Día 7 · Lun 12 oct (festivo en Colombia; confirmar en el calendario) — Funcionalidades III y V
- **III. Administrar amistades:** enviar solicitud (sin enviar a amigos ni a quien ya la envió) y aceptar solicitudes (actualiza la matriz en ambos sentidos).
- **V. Sugerencias de amistad:** K entre 1 y 20, ordenadas por amigos en común, con opción de enviar solicitud.
- Medición de recursos en III y V.

#### Día 8 · Mar 13 oct — Funcionalidades IV y VI
- **IV. Visitar perfil de un amigo:** listar álbumes públicos/amigos, carrusel circular (salir, siguiente, previa, dar/quitar like).
- **VI. Mensajes:** enviar (no vacío, ≤ 512 caracteres, solo a amigos), leer nuevos (ordenados por fecha, luego marcar como leídos), buscar por rango de fechas (extremos inclusivos).
- Persistir cambios (solicitudes aceptadas, mensajes, likes, leídos).
- Medición de recursos en IV y VI.

#### Día 9 · Mié 14 oct — Integración y congelamiento
- Menú principal completo y probado de punta a punta con ≥ 1000 usuarios.
- Verificar la medición (VII) en **cada** funcionalidad; comprobar que la memoria reportada es coherente.
- Revisión de fugas de memoria (contador de `new`/`delete` propio y/o AddressSanitizer).
- Revisar restricciones implícitas y entradas inválidas.
- Revisión de código: referencias en vez de copias, comentarios de los algoritmos esenciales.
- **Congelamiento de funcionalidades al final del día**: de aquí en adelante solo se corrigen errores.

### FASE 3 — Documentación y entrega (días 10–11)

#### Día 10 · Jue 15 oct — Informe y video
- **Informe final** (redactado por ustedes): análisis y alternativa de solución; diagrama de clases final; algoritmos esenciales explicados en alto nivel (sin pegar código); problemas de desarrollo; evolución de la solución; formato de archivos explicado.
- **Video** (ver guion en la sección 8): ensayo y grabación.
- Ensayo de sustentación: cada integrante explica un módulo del código ajeno.

#### Día 11 · Vie 16 oct — **HITO 2: ENTREGA**
- Mañana: subir el video a YouTube (comprobar sonido, legibilidad y duración), repositorio público con todo (informe, código, anexos), etiqueta `v1.0-entrega`.
- Verificar con una **ventana de incógnito** que repositorio y video abren.
- Enviar en Ude@ **solo los dos enlaces**.
- **Después de entregar, no se toca el repositorio.**

---

## 7. Funcionalidades: criterios de aceptación

| Func. | Criterio de aceptación |
|---|---|
| **I** Carga/actualización | Carga ≥ 1000 usuarios sin errores; las actualizaciones se guardan; no está en el menú; los archivos no se usan para procesar. |
| **II** Ingreso | Credenciales validadas; 3 fotos distintas entre sí; un único refresco con 3 fotos nuevas; prioridad 1 / 2.5 / 3.5 según amigos en común. |
| **III** Amistades | No se envía solicitud a amigos ni a quien ya tiene una pendiente de esa persona; aceptar actualiza ambos lados. |
| **IV** Perfil de amigo | Solo álbumes "amigos" o "público"; carrusel circular; like dar/quitar. |
| **V** Sugerencias | K validado (1–20); orden descendente por amigos en común; opción de enviar solicitud. |
| **VI** Mensajes | No vacíos, ≤ 512 caracteres, solo a amigos; nuevos ordenados por fecha y marcados como leídos; rango de fechas con extremos inclusivos. |
| **VII** Recursos | Al terminar **cada** funcionalidad se muestran iteraciones (directas e indirectas, incluyendo componentes externos) y memoria total en ese instante (todas las estructuras, locales y parámetros por valor). |

---

## 8. Guion del video (5–15 min, sin acelerar, todos participan)

| Tiempo | Contenido |
|---|---|
| ≤ 3 min | Presentación de la solución, análisis y arquitectura. |
| ≤ 6 min | Demo con **≥ 1000 usuarios**: (I) carga, ingreso y fotos de amigos; (II) 12 sugerencias y solicitud al de mayor coincidencia; (III) búsqueda de mensajes en un rango de ≥ 6 semanas; (IV) visita a un amigo con ≥ 2 álbumes y ≥ 5 fotos cada uno, recorriendo ambos. Mostrar **memoria e iteraciones** en cada una. |
| ≤ 5 min | Explicación del código: variables, estructuras de control y ventajas frente a otras opciones. |

Ensayar con cronómetro; verificar audio y que se lea la consola (fuente grande).

---

## 9. Estructura del informe (cada sección escrita por el equipo)

1. Análisis del problema y consideraciones de la solución.
2. Diagrama de clases.
3. Algoritmos esenciales intradocumentados (lógica en alto nivel, sin pegar código).
4. Formato de los archivos de datos (con ejemplos).
5. Análisis de eficiencia y estructuras elegidas.
6. Problemas de desarrollo.
7. Evolución de la solución.

---

## 10. Reglas de Git

- Ramas: `main` (estable) y una por funcionalidad (`func-II-ingreso`, etc.).
- **Mínimo 1 commit por día** (el requisito es cada 2 días; así se tiene margen).
- Mensajes claros: `feat: matriz de adyacencia con conteo de amigos en comun`.
- No subir archivos binarios temporales ni carpetas de build.
- Etiquetas: `hito1-diseno`, `v1.0-entrega`.
- Después de la fecha final, **ningún** commit.

---

## 11. Riesgos específicos

| Riesgo | Impacto | Mitigación |
|---|---|---|
| Usar sin querer STL, `struct`, herencia o funciones de C | **Muy alto** (invalida) | Revisión en cada merge con la checklist de la sección 2; buscar `#include <vector>`, `struct`, `printf`, `strcpy`. |
| Falta del diagrama de clases el día 9 | **Muy alto** (sin sustentación) | Diagrama v1 el día 2 y revisión el día 3. |
| Medición de memoria incorrecta | Alto | Diseñarla el día 2; cada clase expone cuánta memoria usa; validar con casos simples. |
| Rendimiento con 1000+ usuarios | Alto | Dataset desde el día 5; medir temprano; matriz de bits y selección parcial. |
| Fugas de memoria / punteros colgantes | Alto | Constructor de copia y destructor en cada clase con memoria dinámica; contador de `new`/`delete`; AddressSanitizer. |
| Integrante sin dominar el código | Alto (sustentación) | Revisión cruzada y ensayo de preguntas el día 10. |
| Video con mal audio o fuera de duración | Medio | Grabar el día 10, dejar el día 11 de colchón. |
| Retraso en funcionalidades | Medio | Orden de prioridad: I, II, III, V, IV, VI; VII se hace en paralelo. Congelamiento el día 9. |
| Enlaces inaccesibles | Alto (invalida) | Verificar en ventana de incógnito. |

---

## 13. Checklist final (día 11)

- [ ] Código multi-archivo, sin STL, sin `struct`, sin herencia, sin ANSI C.
- [ ] ≥ 3 clases, ≥ 2 operadores sobrecargados, constructor de copia, función amiga y plantilla.
- [ ] Las siete funcionalidades funcionan con ≥ 1000 usuarios.
- [ ] Iteraciones y memoria se muestran al terminar **cada** funcionalidad.
- [ ] Sin fugas de memoria.
- [ ] Informe completo, con diagrama y formato de archivos, escrito por el equipo.
- [ ] Video 5–15 min, con todos los integrantes, sin acelerar y con buen audio.
- [ ] Repositorio público con commits regulares.
- [ ] Dos enlaces en Ude@ funcionando.
- [ ] Sin cambios en el repositorio después de entregar.
