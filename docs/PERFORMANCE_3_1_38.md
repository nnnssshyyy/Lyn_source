# Optimización del renderizador, 3.1.38

El envío del menú a ImGui conserva los vértices compartidos en vez de duplicar
uno por cada índice de triángulo. Cada comando registra el comienzo de sus
vértices; los comandos grandes conservan una ruta por bloques compatible con
índices de 16 bits y `VtxOffset`.

La vista 3D reutiliza su textura al pausar la animación y entre actualizaciones
de pose a 60 Hz. El reloj sigue avanzando al ritmo de la interfaz. Girar,
cambiar el zoom, buscar otra posición de la animación o editar los colores
actualiza inmediatamente la imagen. La selección de zonas conserva el resultado
para un puntero y una pose sin cambios. La invalidación del dispositivo limpia
estas cachés. El indicador de cambios del perfil se consulta con el menú abierto.

## Medición y validación

- Menú de prueba: 20 316 → 6 144 vértices, una reducción del 69,8 %.
- Envío de geometría del mismo menú: 0,0691 → 0,0418 ms de CPU por cuadro.
- Prueba de 8 000 rectángulos: 48 000 → 32 000 vértices y
  137,890 → 71,621 microsegundos por envío.
- 240 llamadas por segundo a la vista animada producen 60 poses nuevas y
  reutilizan 180 imágenes; el tiempo de animación mantiene su velocidad.
- Una vista pausada reutiliza los píxeles sin recalcular la pose, subir vértices
  o dibujar de nuevo.
- 79 comprobaciones de interfaz, 37 de renderizado DX9 y 7 de geometría:
  cero fallos. Las nueve capturas son idénticas byte a byte a las de 3.1.37.
- Compilación Release Win32 y paquete UPX verificados.

Los tiempos proceden de un arnés independiente a 1280 × 800 con procesamiento
de vértices por software, sobre Ryzen 7 7730U y Radeon integrada. Miden trabajo
de CPU y envío al controlador; no equivalen a FPS dentro del juego. No incluyen
la captura del desenfoque de la escena ni el tiempo del motor.

Resultados reproducibles: `build/performance-qa/before.json`, `after.json`,
`component-tests.log` y `preview.log`. El arnés no inicia el juego ni ejecuta el
punto de entrada de la DLL. Para repetir:

```powershell
cmake --build build-current --config Release --target lyn ui_mesh_tests hunter_render_tests --parallel 4
ctest --test-dir build-current -C Release -R '^(ui_mesh|hunter_render)$' --output-on-failure
cmake -S tools/menu-ui-preview -B build/performance-qa/out -A Win32
cmake --build build/performance-qa/out --config Release --target ui_performance preview --parallel 4
.\build\performance-qa\out\Release\ui_performance.exe build/performance-qa/after.json
```

Candidata local: `build/performance-3.1.38/lyn.dll`.
Pendiente de medición dentro del juego y en Windows 10 LTSC. Esta candidata
no está publicada: la entrega vigente sigue siendo 3.1.37.
