# Optimización conservando funciones y animaciones

Se redujo trabajo repetido en publicación del ESP, perfiles, selección del aimbot y subida de la malla del Hunter. No se cambió la frecuencia de actualización de animaciones ni la resolución de sus render targets, texturas, efectos o controles.

## Cambios

- **Publicación por tipo de dato.** Cada comando publica estado y marcador de aim; ya no copia los 8192 huecos del pool de huesos y las 384 entradas de objetivos. La geometría se publica al terminar la captura visual del frame. El renderer solo la copia cuando cambia su revisión.
- **Solo elementos activos.** La copia conserva todos los objetivos, segmentos y puntos de radar utilizados, junto con sus contadores. Las colas sin utilizar no se copian ni se dibujan. Cambios de mapa y capturas vacías invalidan los datos anteriores inmediatamente.
- **Configuración por cambios.** El menú sigue procesando entradas y animaciones cada frame. Publica y sanea los 50 perfiles solo cuando cambian valores. El runtime reutiliza la configuración hasta que cambian su revisión o el arma equipada; sigue comprobando el arma sin una espera artificial.
- **Memoria del aimbot.** Se reutilizan los vectores temporales de candidatos/puntos. Las capturas nuevas pueden reutilizar la memoria de registros ya caducados. El historial desactivado se limpia una vez; no se recorre entero en cada actualización de red. Se conservan los filtros, prioridades, multipoints y límites de historial/trazas.
- **Hunter en DX9.** Cada vista sube sus vértices animados una vez por frame. Los siete grupos anatómicos reutilizan ese buffer, y sus índices permanecen en GPU hasta una invalidación. Los bloques de estado de DX9 también se reutilizan, capturando/restaurando el estado en cada render. Si no se puede crear un buffer, permanece disponible la ruta anterior de subida. Los recursos nuevos se liberan antes de Reset.
- **Picking.** Un puntero fuera de los límites del modelo no recorre todos sus triángulos. Los puntos dentro de esos límites conservan la misma prueba de selección.

## Evidencia

| Comprobación | Anterior | Optimizado |
| --- | --- | --- |
| Copia de una captura de ejemplo: 20 objetivos, 500 huesos y 20 puntos de radar | 257552 bytes | 15187 bytes |
| Tiempo medio de esa copia, 4000 iteraciones de la prueba local | 5,472 µs | 0,244 µs |
| Subidas de vértices por Hunter/frame, para siete grupos | 7 | 1 |
| Subidas de índices en frames consecutivos sin cambios de dispositivo | Cada grupo/frame | Reutilización del buffer estático |

El ejemplo copia un **94,1 % menos de bytes**. El tiempo es una medición aislada de CPU y depende del equipo/carga: no representa una mejora equivalente de FPS de todo el juego.

`hunter_render_tests` ejecutó ambas rutas sobre un dispositivo DX9 real y recursos originales, en render targets fuera de pantalla. Los píxeles coincidieron en reposo, caminar, correr y rellenos de prioridades. También comprobó avance de tiempo/velocidad, esqueleto, restauración de estado y bindings de buffers, recreación de recursos y Reset nativo de DX9. Las ventanas de prueba permanecen ocultas.

## Validación y uso

La DLL compiló en Release Win32. Pasaron **268 comprobaciones en diez suites**: 20 de política de aim, 28 de selección/cámara, 12 de snapshots/configuración, 29 del Hunter DX9, 57 de API, 12 de persistencia, 19 de movimiento, 49 de rapidfire, 27 de chat y 15 de su runtime. No hubo pruebas omitidas por falta de DX9 en esta ejecución.

La versión independiente está en `archive/build-history/optimized/lyn.dll`; `build/Release` y el loader reciben la misma compilación cuando sus archivos están disponibles. Reinicia L4D2 para cargarla. La versión anterior de estabilidad de aim permanece en `archive/build-history/aim-stable`.

Todavía falta medir FPS/tiempo de frame dentro de una partida, con la misma escena y opciones antes/después. No se asegura un incremento concreto de FPS a partir del benchmark de copias. El beneficio dependerá de si el límite está en CPU, GPU o el propio juego.
