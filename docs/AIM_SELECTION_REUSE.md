# Reutilización de la selección de objetivo — 20 de octubre de 2026

## Síntoma

Con el aimbot activo en cualquier modo y el rapid fire sosteniendo la ráfaga, el frame rate caía a 10-20 FPS. Con el aimbot apagado, la misma ráfaga no bajaba el frame rate.

## Causa

La ráfaga del rapid fire repite **un único frame de entrada** dentro del mismo tick del motor: el comando base más N comandos extra. `SelectAim` reutilizaba la selección de objetivo solo para los extras 1..32; a partir del 33 cada comando extra volvía a ejecutar `aimbot::Select`, que paga `SetupBones` por cada candidato y hasta 192 trazas de rayo. Con 200 extras eso son miles de llamadas al motor dentro del mismo tick: el trabajo de un frame se multiplicaba por el número de extras.

El comentario de la propia función ya describía la intención correcta ("the target selection is reused for every generated command, keeping the same point for every shot"); el límite de 32 la contradecía.

## Cambio

- `rapid_fire::ReuseSelection` fija la política: un extra reutiliza la selección del frame cuando la caché pertenece a la generación actual.
- `SelectAim` aplica esa política: no queda ventana de reutilización, todos los extras del frame comparten la selección del comando base.
- `TickMoveHook` inicializa la caché de selección al empezar cada tick (resuelta, sin objetivo). El comando base la sobrescribe con la selección real, de modo que un extra nunca puede provocar un barrido nuevo.

Resultado: **un barrido de `SetupBones` + trazas por frame** en lugar de uno por comando generado. La selección no cambia dentro del tick porque el mundo replicado no cambia; el mismo punto se mantiene para todos los disparos de la ráfaga.

## Verificación

- `rapid_fire_tests`: **74 comprobaciones** (69 + 5 de regresión de la política de reutilización: el comando base nunca reutiliza; los extras 1, 32 y 200 sí; una generación anterior no; una caché sin resolver no; fuera de la ráfaga no).
- `runtime_api_tests`: 85 comprobaciones. ctest: **15/15 suites**.
- UPX: `upx -t` correcto; carga nativa x86 PASS (11 recursos, 2.862.261 bytes); roundtrip `.text/.data/.fptable/.reloc` idénticos.
- Módulo **3.1.10**: `sha256 3480c6c5a8df85ff4c1e17a45074f5e24a4ba59793f1f73028315603da8c6b81`, 3.634.688 bytes comprimidos (8.561.664 bytes originales). Empaquetado en `build/aim-fps-3.1.10/`.

## Publicación

Módulo **3.1.10** publicado por el panel privado de Vercel contra la API de licencias del servicio actual: `sha256 3480c6c5a8df85ff4c1e17a45074f5e24a4ba59793f1f73028315603da8c6b81`, 3.634.688 bytes. Firma ECDSA del manifiesto y descarga autenticada verificadas por el propio panel. La página de Accounts muestra `Loader 3.1.4 · Módulo 3.1.10` (commit `2615c6b`).

El loader no se modifica: sigue hablando con la API actual y toma el módulo nuevo tras validar la licencia.

## Pendiente

- Prueba en partida: aimbot normal y silent con rapid fire, anotando el frame rate antes y después.

