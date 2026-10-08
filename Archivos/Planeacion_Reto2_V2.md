# Plan de Proyecto v2 — Desafío II: *Amigos para siempre, con Ude@Book*

**Materia:** Informática II · 2026-2
**Inicio:** martes 6 de octubre · **Hito 1:** viernes 9 de octubre · **Entrega final:** viernes 16 de octubre (11 días)
**Tecnología:** C++ con POO, programa de **consola**, desarrollado en Qt Creator (proyecto C++ plano / CMake, sin módulos de Qt)
**Metodología:** Kanban con checkpoints diarios + dos hitos de entrega

---

## 1. Reglas duras (si se incumple alguna, la entrega puede ser inválida)

- [ ] Solo **C++** (nada de instrucciones de ANSI C: evitar `printf`, `malloc/free`, `FILE*`, `strcpy` y similares; usar `new/delete`, `iostream`, `fstream` y funciones propias para cadenas).
- [ ] **POO real:** mínimo 3 clases (el plan usa más).
- [ ] **Sin herencia.**
- [ ] **Sin contenedores STL** y **sin `struct`** (los nodos también son `class`). Solo se permite la librería para números aleatorios.
- [ ] Estructuras de datos **propias** y con **memoria dinámica** en el núcleo.
- [ ] **Sin copias innecesarias:** los parámetros de los métodos que reciben objetos o datos estructurados van por **referencia constante (`const Tipo&`) o por puntero**, nunca por valor sin necesidad.
- [ ] Programación **modular multi-archivo** (`.h` / `.cpp`).
- [ ] Sobrecargar **al menos 2 operadores**; usar constructor de copia, getters/setters, sobrecarga de métodos, funciones amigas y plantillas.
- [ ] **El almacenamiento permanente no se usa para procesar ni intercambiar datos:** al iniciar se carga **todo a RAM** (objetos instanciados); durante la ejecución solo se consulta la memoria; en el archivo solo se escriben las actualizaciones.
- [ ] Informe **escrito por ustedes** (no por IA) y video **no generado por IA**.
- [ ] Repositorio **público**, commits **al menos cada 2 días**, sin cambios después de la fecha de entrega.
- [ ] Video en YouTube entre **5 y 15 min**, sin acelerar, con **todos** los integrantes participando.
- [ ] En Ude@: **solo dos enlaces** (repositorio y video).

---

## 2. Hitos y distribución del esfuerzo

| Hito | Fecha | Contenido |
|---|---|---|
| **H1** | Vie 9 oct | Evidencia de análisis y diseño: análisis del problema, **diagrama de clases (obligatorio, sin él no hay sustentación)** en versión expandida y versión resumen, estructuras elegidas con su justificación de eficiencia, formato de archivos con una muestra real. |
| **H2** | Vie 16 oct | Código fuente, informe final, enlace al video, repositorio público. |

| Rúbrica | Peso | Dónde se trabaja |
|---|---|---|
| Diagrama de clases y diseño | 10 % | Días 1–4 |
| Verificación de eficiencia y selección de estructuras | 25 % | Días 2–4 (y se valida el día 9) |
| Medición del consumo de recursos | 10 % | Diseño día 2, implementación días 5–9 |
| Implementación del menú y funcionalidades | 55 % | Días 5–9 |

---

## 3. Roles sugeridos

| Rol | Responsabilidad |
|---|---|
| Responsable de estructuras de datos | Lista dinámica, tabla hash, matriz de adyacencia, cadena propia, fecha. |
| Responsable de funcionalidades | Menú y casos de uso II–VI. |
| Responsable de persistencia y medición | Archivos (I), medidor de recursos (VII), generador de datos. |
| Responsable de documentación y video | Informe, diagramas, guion y grabación. |

---

## 4. Arquitectura propuesta

### 4.1 Clases del dominio

| Clase | Responsabilidad |
|---|---|
| `Fecha` | Día/mes/año, comparación y formato. Sobrecarga de `operator<`, `operator==`, `operator<<`. |
| `Cadena` | Texto dinámico propio (en lugar de `std::string`), con constructor de copia y asignación. |
| `Foto` | Fecha de captura, tamaño, ruta, likes. Pertenece a un único álbum. |
| `Album` | Nombre, ruta, privacidad (público / privado / amigos), lista de fotos. |
| `Mensaje` | Emisor, receptor, texto (≤ 512 caracteres), fecha, leído/no leído. Método `marcarLeido()`. |
| `Usuario` | Nombre de usuario único, fecha de nacimiento, correo, foto de perfil (ruta), contraseña, ciudad, país, **fecha de inscripción**, álbumes, referencias a mensajes y solicitudes. |
| `RedSocial` (plataforma) | Coordina usuarios, relaciones, mensajes y las funcionalidades del menú. |
| `RelacionesAmistad` | Matriz de adyacencia (idealmente por bits) para amistad y conteo de amigos en común. |
| `MedidorRecursos` | Contador de iteraciones y cálculo de memoria en uso (ver 4.4). |
| `Menu` | Interacción por consola. |

### 4.2 Estructuras propias (todas con memoria dinámica)

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
- **Carrusel circular** de fotos (decidir el día 2): con arreglo dinámico, índice con módulo: siguiente `(i + 1) % n` y previa `(i - 1 + n) % n`. Si se prefiere una lista doblemente enlazada, debe ser **circular** (la cola apunta a la cabeza).

### 4.3 Conceptos que el enunciado pide demostrar

| Concepto | Dónde aplicarlo (sugerencia) |
|---|---|
| Constructor de copia | `Cadena`, `ListaDinamica<T>`, `Mensaje` |
| Sobrecarga de operadores (≥ 2) | `operator[]`, `operator==`, `operator<`, `operator<<`, `operator+=` |
| Función amiga | `operator<<` de `Fecha` / `Mensaje` (función amiga) y `friend class MedidorRecursos` en `ListaDinamica<T>`, `Usuario` y `RelacionesAmistad` para leer su capacidad interna |
| Plantillas | `ListaDinamica<T>` |
| Sobrecarga de métodos | Constructores y métodos de búsqueda |
| Getters/setters | Todas las clases del dominio |

> El estándar de UML simplificado no tiene notación para amistad (`friend`): no se dibuja en el diagrama.

### 4.4 Medición de recursos (decisión del día 2)

El enunciado pide la memoria total **en ese momento exacto**, incluyendo **variables locales y parámetros por valor**. Son dos partes distintas:

| Parte | Cómo medirla | Aviso |
|---|---|---|
| Memoria dinámica (heap) de estructuras y objetos | Cada clase con memoria dinámica reporta su peso (`sizeof(*this)` + capacidad reservada); el medidor suma. `MedidorRecursos` como clase amiga accede a la capacidad privada. Alternativa a evaluar: sobrecargar `new` y `delete` para llevar un contador de bytes. | Sobrecargar `new/delete` **globalmente** también cuenta lo que reservan `iostream` y `fstream`; hay que restarlo o limitarse a las clases propias. |
| Variables locales y parámetros por valor | La pila **no** se ve con `new/delete`. Registrar en cada funcionalidad el `sizeof` de sus locales y parámetros por valor (por ejemplo, una suma constante por funcionalidad) y añadirlo al reporte. | Sigue siendo manual: de ahí la regla de pasar por referencia, que además reduce este valor. |

Iteraciones: un contador estático que cada ciclo incrementa (también los de las estructuras propias, para contarlas "directa e indirectamente").

---

## 5. Cronograma día a día

> Regla general: **un commit por día como mínimo**, con mensaje descriptivo. Al cierre de cada día, 10 minutos de checkpoint: qué se terminó, qué bloquea, qué sigue.

### FASE 1 — Análisis y diseño (días 1–4) → entrega H1

#### Día 1 · Mar 6 oct — Análisis y arranque
- Lectura completa del enunciado por **todos**; extraer requisitos funcionales, no funcionales y restricciones (sección 1).
- Lista de **dudas para el profesor** (sección 11) y envío.
- Repositorio público creado, estructura `src/`, `include/`, `docs/`, `data/`, `tests/`; `.gitignore`; proyecto en Qt Creator (C++ plano, sin Qt).
- Tablero Kanban con todas las tareas de este plan.
- **Entregable:** repo con README inicial y primer commit; documento de requisitos.

#### Día 2 · Mié 7 oct — Modelo, estructuras y medición
- Borrador de diagrama de clases (UML simplificado del curso, **sin `Persistencia`**) con atributos, métodos y relaciones.
- Análisis de **estructuras de datos** candidatas y tabla comparativa de complejidad por funcionalidad.
- **Decisión del carrusel circular** (módulo sobre arreglo o lista circular).
- **Diseño del medidor de recursos** (sección 4.4): cómo se cuenta heap, cómo se cuentan locales y parámetros por valor, si se sobrecarga `new/delete` y qué clases son amigas.
- Diseño del **formato de archivos** (usuarios, amistades, solicitudes, álbumes, fotos, mensajes) y **muestra escrita a mano** (~10 usuarios) con ese formato.
- **Entregable:** diagrama v1, tabla de eficiencia y muestra del formato en `docs/`.

#### Día 3 · Jue 8 oct — Eficiencia, pulido del diseño y generador
- Verificar el cumplimiento del requisito de eficiencia **antes de implementar** (25 %): estimar iteraciones y memoria para 1000 usuarios en cada funcionalidad.
- Diagrama de clases final en **versión expandida y versión resumen**: relaciones con cardinalidad, constructores de copia, getters/setters, operadores sobrecargados.
- **Generador de datos, primera versión** (con el formato ya definido) para producir un archivo de muestra real que se adjunta al informe y al H1. Si no alcanza, se entrega la muestra hecha a mano del día 2.
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
- `MedidorRecursos` básico integrado desde ya (según lo decidido el día 2).
- **Generador de datos, versión final:** ≥ 1000 usuarios, amistades, solicitudes, álbumes (al menos un usuario con ≥ 2 álbumes y ≥ 5 fotos en cada uno), mensajes que abarquen ≥ 6 semanas.
- **Definición de terminado:** estructuras sin fugas y con casos límite probados.

#### Día 6 · Dom 11 oct — Funcionalidad I y II
- **I. Carga/actualización de datos** (no aparece en el menú): lectura de todo a RAM, escritura de actualizaciones, validación de archivos malformados. Al iniciar, la consola imprime cuántos usuarios se cargaron en memoria.
- **II. Ingreso:** autenticación con credenciales; mostrar 3 fotos de perfil de amigos con **selección ponderada**; **un solo** refresco con un grupo distinto.
- Medición de recursos conectada a I y II.

#### Día 7 · Lun 12 oct (festivo en Colombia; confirmar en el calendario) — Funcionalidades III y V
- **III. Administrar amistades:** enviar solicitud (sin enviar a amigos ni a quien ya la envió) y aceptar solicitudes (actualiza la matriz en ambos sentidos).
- **V. Sugerencias de amistad:** K entre 1 y 20, ordenadas por amigos en común, con opción de enviar solicitud.
- Medición de recursos en III y V.

#### Día 8 · Mar 13 oct — Funcionalidades IV y VI
- **IV. Visitar perfil de un amigo:** listar álbumes públicos/amigos, **carrusel circular** (salir, siguiente, previa, dar/quitar like) con la solución elegida el día 2.
- **VI. Mensajes:** enviar (no vacío, ≤ 512 caracteres, solo a amigos), leer nuevos (ordenados por fecha; después se llama a `marcarLeido()` en cada uno), buscar por rango de fechas (extremos inclusivos).
- **Persistencia de cambios al cerrar el programa:** solicitudes aceptadas, mensajes nuevos, **estado leído** y likes; solo se escribe lo que cambió.
- Medición de recursos en IV y VI.

#### Día 9 · Mié 14 oct — Integración y congelamiento
- Menú principal completo y probado de punta a punta con ≥ 1000 usuarios.
- Verificar la medición (VII) en **cada** funcionalidad (heap + locales y parámetros por valor); comprobar que la memoria reportada es coherente.
- Revisión de fugas de memoria (contador de `new`/`delete` propio y/o AddressSanitizer).
- Revisar restricciones implícitas y entradas inválidas.
- Revisión de código: **parámetros por referencia constante o puntero**, comentarios de los algoritmos esenciales.
- **Congelamiento de funcionalidades al final del día**: de aquí en adelante solo se corrigen errores.

### FASE 3 — Documentación y entrega (días 10–11)

#### Día 10 · Jue 15 oct — Informe y video
- **Informe final** (redactado por ustedes): análisis y alternativa de solución; diagrama de clases final; algoritmos esenciales explicados en alto nivel (sin pegar código); problemas de desarrollo; evolución de la solución; formato de archivos explicado con la muestra.
- **Video** (ver guion en la sección 7): ensayo y grabación.
- Ensayo de sustentación: cada integrante explica un módulo del código ajeno.

#### Día 11 · Vie 16 oct — **HITO 2: ENTREGA**
- Mañana: subir el video a YouTube (comprobar sonido, legibilidad y duración), repositorio público con todo (informe, código, anexos), etiqueta `v1.0-entrega`.
- Verificar con una **ventana de incógnito** que repositorio y video abren.
- Enviar en Ude@ **solo los dos enlaces**.
- **Después de entregar, no se toca el repositorio.**

---

## 6. Funcionalidades: criterios de aceptación

| Func. | Criterio de aceptación |
|---|---|
| **I** Carga/actualización | Carga ≥ 1000 usuarios en RAM sin errores y lo informa al iniciar; las actualizaciones se guardan; no está en el menú; los archivos no se usan para procesar. |
| **II** Ingreso | Credenciales validadas; 3 fotos distintas entre sí; un único refresco con 3 fotos nuevas; prioridad 1 / 2.5 / 3.5 según amigos en común. |
| **III** Amistades | No se envía solicitud a amigos ni a quien ya tiene una pendiente de esa persona; aceptar actualiza ambos lados. |
| **IV** Perfil de amigo | Solo álbumes "amigos" o "público"; carrusel **circular** (del último pasa al primero y viceversa); like dar/quitar. |
| **V** Sugerencias | K validado (1–20); orden descendente por amigos en común; opción de enviar solicitud. |
| **VI** Mensajes | No vacíos, ≤ 512 caracteres, solo a amigos; nuevos ordenados por fecha y luego marcados como leídos, y ese estado se guarda al cerrar; rango de fechas con extremos inclusivos. |
| **VII** Recursos | Al terminar **cada** funcionalidad se muestran iteraciones (directas e indirectas, incluyendo componentes externos) y memoria total en ese instante (todas las estructuras, locales y parámetros por valor). |

---

## 7. Guion del video (5–15 min, sin acelerar, todos participan)

| Tiempo | Contenido |
|---|---|
| ≤ 3 min | Presentación de la solución, análisis y arquitectura. |
| ≤ 6 min | Demo con **≥ 1000 usuarios**. Al inicio, mostrar brevemente la consola cargando los datos (mensaje con el número de usuarios cargados en memoria) o el tamaño de los archivos. Demostrar: (a) carga, ingreso y fotos de amigos; (b) 12 sugerencias y solicitud al de mayor coincidencia; (c) búsqueda de mensajes en un rango de ≥ 6 semanas; (d) visita a un amigo con ≥ 2 álbumes y ≥ 5 fotos cada uno, recorriendo ambos. **Después de cada una, detener la grabación de la consola un momento y enfocar en pantalla la memoria y las iteraciones** antes de pasar a la siguiente. Al menos una demostración con su consumo es obligatoria para optar a la sustentación. |
| ≤ 5 min | Explicación del código: variables, estructuras de control y ventajas frente a otras opciones. |

Ensayar con cronómetro; verificar audio y que se lea la consola (fuente grande).

---

## 8. Estructura del informe (cada sección escrita por el equipo)

1. Análisis del problema y consideraciones de la solución.
2. Diagrama de clases (versión expandida y versión resumen).
3. Algoritmos esenciales intradocumentados (lógica en alto nivel, sin pegar código).
4. Formato de los archivos de datos (con ejemplos y muestra real).
5. Análisis de eficiencia y estructuras elegidas.
6. Problemas de desarrollo.
7. Evolución de la solución.

---

## 9. Reglas de Git

- Ramas: `main` (estable) y una por funcionalidad (`func-II-ingreso`, etc.).
- **Mínimo 1 commit por día** (el requisito es cada 2 días; así se tiene margen).
- Mensajes claros: `feat: matriz de adyacencia con conteo de amigos en comun`.
- No subir archivos binarios temporales ni carpetas de build.
- Etiquetas: `hito1-diseno`, `v1.0-entrega`.
- Después de la fecha final, **ningún** commit.

---

## 10. Riesgos específicos

| Riesgo | Impacto | Mitigación |
|---|---|---|
| Usar sin querer STL, `struct`, herencia o funciones de C | **Muy alto** (invalida) | Revisión en cada merge con la checklist de la sección 1; buscar `#include <vector>`, `struct`, `printf`, `strcpy`. |
| **Usar los archivos como área de procesamiento o intercambio de datos** | **Muy alto** (invalida) | Cargar todo a RAM (objetos instanciados) al iniciar; ninguna consulta ni búsqueda lee del archivo; escribir solo actualizaciones. Revisar a mano las funciones de persistencia. |
| Falta del diagrama de clases el día 9 | **Muy alto** (sin sustentación) | Diagrama v1 el día 2 y revisión el día 3. |
| Medición de memoria incorrecta (olvidar locales y parámetros por valor) | Alto | Diseñarla el día 2 según la sección 4.4; validar con casos simples. |
| Copias innecesarias de datos estructurados | Alto | Regla de referencias constantes en la revisión de código del día 9. |
| Rendimiento con 1000+ usuarios | Alto | Dataset desde el día 3; medir temprano; matriz de bits y selección parcial. |
| Fugas de memoria / punteros colgantes | Alto | Constructor de copia y destructor en cada clase con memoria dinámica; contador de `new`/`delete`; AddressSanitizer. |
| Integrante sin dominar el código | Alto (sustentación) | Revisión cruzada y ensayo de preguntas el día 10. |
| Video sin las métricas por funcionalidad | Alto (puede no optar a sustentación) | Guion de la sección 7 y ensayo previo. |
| Video con mal audio o fuera de duración | Medio | Grabar el día 10, dejar el día 11 de colchón. |
| Formato de archivos sin probar con datos reales | Medio | Muestra el día 2 y generador el día 3. |
| Retraso en funcionalidades | Medio | Orden de prioridad: I, II, III, V, IV, VI; VII se hace en paralelo. Congelamiento el día 9. |
| Enlaces inaccesibles | Alto (invalida) | Verificar en ventana de incógnito. |

---

## 12. Checklist final (día 11)

- [ ] Código multi-archivo, sin STL, sin `struct`, sin herencia, sin ANSI C.
- [ ] ≥ 3 clases, ≥ 2 operadores sobrecargados, constructor de copia, función amiga y plantilla.
- [ ] Parámetros por referencia constante o puntero, sin copias innecesarias.
- [ ] Todo se carga a RAM al iniciar; los archivos no se usan para procesar.
- [ ] Las siete funcionalidades funcionan con ≥ 1000 usuarios.
- [ ] Iteraciones y memoria (heap, locales y parámetros por valor) se muestran al terminar **cada** funcionalidad.
- [ ] Mensajes leídos se guardan al cerrar.
- [ ] Sin fugas de memoria.
- [ ] Diagrama de clases en versión expandida y resumen, sin `Persistencia`.
- [ ] Informe completo, con diagrama y formato de archivos con muestra real, escrito por el equipo.
- [ ] Video 5–15 min, con todos los integrantes, sin acelerar, con buen audio y con las métricas de cada funcionalidad.
- [ ] Repositorio público con commits regulares.
- [ ] Dos enlaces en Ude@ funcionando.
- [ ] Sin cambios en el repositorio después de entregar.
