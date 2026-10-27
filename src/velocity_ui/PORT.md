# Velocity UI para L4D2 / DX9 / x86

Origen local, proporcionado por el usuario:

`C:\Users\henry\OneDrive\Desktop\velocity-main\velocity-main\cs2\velocity-cs2\project\core\rendering\impl\menu`

Se partió de las páginas originales del menú, la biblioteca XUI y las primitivas geométricas de xdraw. El menú actual tiene seis pestañas tras retirar skins y las opciones específicas de CS2. Se mantienen las medidas (700 × 450), los SVG restantes, el avatar PNG, las fuentes, la distribución, las animaciones, los popups, el buscador y los cinco temas. La imagen decorativa de Konata se omitió por petición expresa del usuario; el GIF estaba comentado en el original.

La fuente principal Inter y Noto Math proceden de `external/xdraw/dependencies/fonts.hpp` del proyecto local. Inter Bold y Pixel7 proceden del repositorio de recursos enlazado por el propio proyecto: https://github.com/wubly/velocity-resourcer (commit registrado en su checkout dentro de `vendor/`). Los bytes necesarios están integrados en la DLL; no se descargan recursos al ejecutar el juego.

Adaptaciones:

- Dibujado original de vértices, índices, clipping y antialiasing convertido a listas de ImGui, que renderiza su backend DX9. Se conservó FreeType con las mismas opciones de rasterización y métricas del renderer original.
- Desenfoque con una cadena de cuatro niveles y shaders de downsample/upsample para DX9. Se guardan/restauran estados de GPU, viewport y destinos. Los recursos del pool por defecto se liberan antes de Reset.
- Hashes FNV de 32 bits en XUI y eliminación de helpers de grupos de armas dependientes de CS2.
- Entrada procesada en el hilo de render desde la cola existente; se limpian botones, foco y arrastres al cerrar el menú o perder el foco.
- Configuraciones de interfaz separadas en `HKCU\Software\FreeL4D2\VelocityUI\configs`. No se leen ni modifican configuraciones del proyecto original.
- Se eliminaron `menu.skins.cpp`, `catalog.hpp`, el estado del inventario cosmético y sus campos de configuración. También se retiraron las opciones de economía, C4, Zeus, clan tag, radar, granadas y recursos exclusivos de CS2; la lista detallada está en el README.
- La navegación, los iconos y el buscador usan las seis pestañas actuales. Los grupos de proyectiles se redujeron a Molotov y área de fuego, y las flags a zoom, ping y distancia.
- `menu.features.cpp` reemplaza en la compilación las cinco páginas de opciones de juego por controles conectados al runtime L4D2. Se conservan el marco, los iconos, las fuentes, los temas y Config. Las páginas antiguas quedan como referencia local y no se compilan. F6 y los nuevos controles usan el mismo ESP.
- `features_fields.inc` define los campos compartidos entre UI y el snapshot del hilo de juego. Los perfiles guardan estos campos bajo categorías nuevas `l4d2 ...`; los campos antiguos no controlan el runtime.
- El buscador muestra los ajustes de juego adaptados. Los módulos de RijiN todavía pendientes figuran como texto en Status, sin casillas que simulen una implementación.

`tools/port_velocity.py` documenta la extracción inicial de los archivos del checkout original. Ahora escribe exclusivamente en `build/velocity-original-import/velocity_ui` para conservar una referencia sin sobrescribir las adaptaciones actuales de L4D2. El código de adaptación está en `xdraw_dx9.cpp` y `src/menu.cpp`.

Compilación objetivo: MSVC, C++20, Release Win32. El resultado no se ha cargado en L4D2 ni se ha comparado visualmente dentro del juego durante esta entrega; no se afirma igualdad píxel a píxel del desenfoque entre DX11 y DX9.
