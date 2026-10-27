# Estabilidad de normal y silent client

Se corrigieron varias fuentes de saltos en el seguimiento y de movimiento visible procedente del comando de disparo.

- **Objetivo estable:** pequeñas diferencias de FOV/distancia no cambian de enemigo en cada comando. Una mejora clara o una amenaza de mayor prioridad sí permite cambiar. La retención termina al morir, perder visibilidad, salir de filtros/FOV, soltar activación, cambiar de arma/modo o cambiar el handle del jugador.
- **Punto estable:** las muestras llevan identidad de hueso, hitbox y punto. Normal y silent client conservan una muestra visible de la zona prioritaria; al adquirir prefieren el centro antes de probar sus bordes. Los multipoints siguen disponibles junto a obstáculos y se vuelve a comprobar visibilidad en cada selección.
- **Pose actual primero:** normal y silent client buscan primero los huesos actuales. Si no hay punto visible, todavía pueden usar historial válido. El orden de búsqueda de silent server se conserva.
- **Tiempo de animación:** el reloj de huesos/historial ya no se sustituye por el tickbase predicho usado para los cooldowns del arma.
- **Suavizado:** normal usa la dirección actual del ratón y una respuesta equivalente entre frecuencias de comandos. El valor existente del perfil se conserva; 0 % sigue siendo apuntado directo.
- **Vista separada:** el comando conserva el ángulo de disparo. Después del callback nativo, normal publica su ángulo suavizado y silent client restaura la dirección del ratón. Silent client escribe el ángulo en todo comando apuntado con la tecla mantenida (no solo cuando el cooldown local ya está listo), de modo que el comando que finalmente dispare ya lleva la solución; la restauración de la vista del ratón no cambia. Silent server conserva su estimación de cooldown por la cadencia de retención/envío. Durante el seguimiento/disparo, RenderView y el viewmodel consultan la dirección de entrada actual, evitando usar las correcciones de spread/recoil como dirección de cámara. Se conserva el recoil visual salvo cuando su opción de ocultación está activa.

La protección visual expira a los 250 ms, comprueba el handle completo del jugador y se pausa al perder foco/abrir el menú. La cámara no conserva una dirección congelada: vuelve a leer el ratón en cada render. Las vistas especiales alejadas del ojo del jugador no reciben el ajuste de primera persona.

## Archivos

- `src/aimbot.cpp`, `src/aim_logic.h`: retención, prioridad, puntos y suavizado.
- `src/game.h`, `src/game.cpp`: identidad de muestras de hitbox.
- `src/aim_camera.h`, `src/features.cpp`: separación entre disparo, dirección visible, recoil y reloj.
- `tests/aim_selection_tests.cpp`: selector real con entidades, huesos y trazas simulados; también comprueba las funciones de cámara usadas por el runtime.

## Validación

Release Win32 compiló correctamente. Pasaron **20 comprobaciones de política/suavizado** y **28 de selección/cámara**: pequeñas variaciones de FOV/distancia, cambio claro de objetivo, oclusión, centro/bordes, cambio de preset, latencia tras disparar, muerte/handle reutilizado, historial de respaldo, prioridades de amenazas, límite de trazas, ratón en movimiento y recoil visual.

También pasaron las otras seis suites del proyecto: API (57), persistencia (12), movimiento (19), rapidfire (49), lógica de chat (27) y runtime de chat (15). Total: **227 comprobaciones en ocho suites**.

No se verificó visualmente esta DLL en una partida. Falta comprobar el resultado con los FPS, armas, recoil y opciones de cámara usados por el usuario. Reinicia L4D2 y carga la DLL nueva; el proyecto no admite descarga de hooks en caliente.
