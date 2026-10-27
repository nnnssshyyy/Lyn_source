# Menú 3.1.35: mejoras por fases

Estado: las tres fases están implementadas y compilan en Release Win32.
La interfaz pasó el arnés nativo y la revisión de siete capturas. La DLL sigue
siendo una **candidata local pendiente de comprobación en partida**.

## Fase 1: lectura, distribución y escala

- Texto secundario legible también con casillas desmarcadas, borde visible y marca
  de verificación; placeholders con más contraste y ventanas emergentes opacas.
- Barra lateral de 154 px y menú de 960 × 640 unidades, con más espacio entre filas.
- Fuente Inter de 16 px para el menú; las variantes de 20 y 24 px se rasterizan
  para 125 % y 150 %. El HUD y su fuente ESP conservan su tamaño propio.
- Escala 100/125/150 %, ajuste al área disponible y coordenadas del ratón y clips
  transformadas con el mismo factor. La posición se conserva al cambiar la escala.
- Al salir del dibujo del menú se restauran fuente, escala y coordenadas del ratón.

## Fase 2: organización e idioma

- Jugadores tiene General, Apariencia y Colores; el Hunter ocupa una columna fija
  en las tres páginas. Cada lista de opciones conserva su desplazamiento.
- Selector Español/English y catálogo central de traducciones para navegación,
  controles, selectores, mensajes de perfiles y controles del Hunter.
- Los nombres internos, IDs, categorías y claves guardadas conservan sus valores.
- Idioma y escala se guardan para el usuario de Windows en
  `HKCU\Software\lyn\interface`, separados de los perfiles.
- Los nombres de perfiles y los textos introducidos por el usuario se presentan
  como datos literales, incluidos Unicode y caracteres `##`.

## Fase 3: perfiles y búsqueda

- Pulsar una fila selecciona el perfil. El botón Cargar aplica sus datos.
- El pie del menú muestra el perfil activo y un asterisco al detectar cambios.
  Perfiles muestra también el estado y distingue perfil activo de seleccionado.
- Guardar/cargar actualiza la referencia; volver a los valores guardados elimina
  el indicador. El cálculo ignora la activación transitoria de los atajos.
- Cargar con cambios pendientes pide repetir la acción en cinco segundos;
  seleccionar otra fila anula esa confirmación. Se conservan las confirmaciones
  de borrar y restablecer.
- La búsqueda acepta nombres traducidos y omite diferencias de acentos. Abre la
  subpágina adecuada de Jugadores y desplaza la lista al control encontrado,
  usando su identidad para evitar confundir opciones con el mismo nombre.

## Comprobaciones

- Correcta la compilación final de `lyn` en `build-current/Release/lyn.dll`.
- Compila el arnés independiente `tools/menu-ui-preview`, que incorpora
  comprobaciones de idioma, IDs, conservación de perfiles, cambios pendientes,
  escalado, coordenadas, clips y desplazamiento hasta el resultado de búsqueda.
- El reintento del 23 de octubre ya se ejecutó: **36 comprobaciones de interfaz,
  cero fallos**. Se verificaron idioma, IDs, perfiles, escalado, coordenadas, clips
  y desplazamiento hasta el resultado de búsqueda.
- Se generaron y revisaron siete capturas: las tres páginas de Jugadores,
  Colores en inglés, Perfiles y General a 125 % y 150 %. Se corrigió el capturador
  para leer la superficie completa de Direct3D y exportar el área solicitada.
- CTest ejecutó las tres suites relacionadas (`visuals_render`, `hunter_render`
  y `settings_persistence`): **tres aprobadas, cero fallos**. La prueba de
  persistencia informa de 31 comprobaciones sobre 50 armas.
- La primera ejecución global de CTest quedó sin ejecutar por el bloqueo y por
  cinco ejecutables ausentes. Ese resultado histórico no cuenta como aprobado;
  las otras dieciocho suites no forman parte de esta revisión de interfaz.
- `lyn-compressed.dll` ocupa **3 674 624 bytes**, por debajo de 4 MiB. Pasó
  `upx -t`; tras descomprimir, las secciones `.text`, `.data`, `.fptable`, `.reloc`
  y los doce recursos coinciden con el original. No se probó la carga de la DLL
  ni su uso dentro del juego.
- Los registros actualizados están en `build/menu-ui-qa/retry-preview.log`,
  `retry-render-tests.log`, `retry-settings-tests.log` y `retry-compression-*.log`.
- Copia de fuentes anteriores: `build/menu-ui-source-3.1.34/`.
- Original: `build/menu-ui-3.1.35/lyn.dll`; candidata comprimida:
  `build/menu-ui-3.1.35/lyn-compressed.dll`. La entrega del loader y la versión
  publicada continúan en 3.1.34.

## Reproducir la revisión en un entorno de desarrollo autorizado

Desde la raíz del proyecto, con CMake disponible:

```powershell
cmake --build build-current --config Release --target lyn --parallel 4
cmake -S tools/menu-ui-preview -B build/menu-ui-qa/final-out -A Win32
cmake --build build/menu-ui-qa/final-out --config Release --parallel 4
.\build\menu-ui-qa\final-out\Release\preview.exe
```

El arnés no inicia el juego ni ejecuta el punto de entrada de la DLL. No guarda
preferencias de interfaz ni crea, carga o borra perfiles del usuario. Sus capturas
previstas se guardan en `build/menu-ui-qa/` e incluyen las tres páginas, ambos
idiomas, perfiles y las tres escalas. La compilación y los controles de lógica
no sustituyen la inspección de esas capturas y la comprobación posterior en juego.
