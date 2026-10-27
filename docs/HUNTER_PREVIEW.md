# Hunter 3D en el menú

Dos vistas disponibles mientras el menú está abierto:

- **Jugadores → Vista previa / Hunter**: integrada en la columna derecha; cajas, salud, nombre, distancia y skeleton según los ajustes actuales. Los colores y efectos tienen su propio panel desplazable debajo, para mantener visible el modelo.
- **Hitbox Selection / Hunter**: ventana independiente con relleno sobre cada zona del modelo; se abre desde Combate → Aimbot → open hitbox selector.

El selector de hitboxes se mueve arrastrando su título y se cierra con `x`. Su posición se conserva durante la sesión y se ajusta a la resolución. La vista ESP se mueve junto al menú y no necesita una segunda ventana.

## Controles

- Preview ESP: arrastrar el modelo lo gira.
- Selector: clic izquierdo avanza la prioridad; clic derecho retrocede. Shift + arrastrar gira el modelo.
- Rueda: acercar/alejar. El selector independiente también conserva el deslizador de zoom.
- Vista ESP: Animación permite Reposo, Caminar y Correr; Pausar/Reproducir controla el movimiento; Restablecer recupera giro, zoom y tiempo.
- Selector: Animation, speed, pause/play y reset view conservan sus controles completos.
- Arrastra la barra de salud a uno de los cuatro lados resaltados y suelta para guardar su posición; Esc cancela. Las etiquetas tienen espacio reservado por encima y debajo del modelo.
- Cambiar una prioridad activa el preset custom conservando las otras zonas del preset mostrado. Las prioridades se guardan mediante los mismos ajustes existentes del aimbot.

## Implementación

El Hunter es una malla 3D con 9.658 vértices, 11.701 triángulos y 53 huesos. Los recursos se prepararon desde los archivos de la instalación local de L4D2, sin modificarla:

- `models/infected/hunter.mdl`, `hunter.vvd`, `hunter.dx90.vtx`.
- `models/infected/anim_hunter.mdl`, `anim_hunter.ani`.
- `materials/models/infected/hunter/hunter_01.vtf`, mip de 512 píxeles.
- Ciclos: `@Idle_Standing_01` (104 frames), `a_Walk_Upper_KnifeN` (57), `a_RunN` (32). El visor reproduce estos clips y no combina capas de gameplay o IK del motor.

`hunter_model.cpp` interpola las poses locales mediante cuaterniones, calcula la jerarquía y deforma la malla usando sus pesos originales. Cada ventana tiene su propia textura de destino DX9 y buffer de profundidad. Las cajas se calculan sobre los vértices deformados y el skeleton usa las articulaciones de la misma pose. La selección busca el triángulo visible más cercano; las siete regiones visuales se asignan por pesos óseos y controlan los grupos de hitboxes existentes. No se muestran cubos de selección.

El trabajo ocurre en el hilo de render, solo con el menú abierto y la ventana correspondiente visible. No crea entidades ni llama métodos privados del juego para animar. Los estados de DirectX, destinos y viewport se restauran al finalizar, y las texturas del pool por defecto se liberan antes de Reset. Si falla el render 3D, permanece el selector/preview estático con una indicación de fallback.

Los recursos están integrados en `lyn.dll` (RCDATA 202/203), por lo que no requieren rutas externas al ejecutar. El script `tools/build_hunter_preview.py` permite regenerarlos desde una copia instalada de L4D2; requiere Python y Pillow:

```powershell
python tools/build_hunter_preview.py 'C:\Program Files (x86)\Steam\steamapps\common\Left 4 Dead 2'
cmake --build build --config Release --target lyn --parallel
```

Referencias de formato: [studio.h de Valve](https://github.com/ValveSoftware/source-sdk-2013/blob/master/src/public/studio.h), [optimize.h de Valve](https://github.com/ValveSoftware/source-sdk-2013/blob/master/src/public/optimize.h) y [lector de frame animations de SourceIO](https://github.com/REDxEYE/SourceIO/blob/master/library/models/mdl/structs/local_animation.py). El conversor y el renderizador son específicos de este proyecto; los modelos, texturas y animaciones proceden del juego de Valve.

Compilación Release Win32 realizada. La visualización y la interacción dentro de una partida quedan pendientes de comprobación; no se ejecutaron tests nuevos ni se cargó esta DLL en el juego durante esta entrega.
