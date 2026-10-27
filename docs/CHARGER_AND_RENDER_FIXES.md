# Charger turn y correcciones de render — 3.1.17

`Movement` incorpora `charger turn`. Se adapta el bloque identificado en
[Raze](RAZE_CHARGER_TURN_ANALYSIS.md), con una firma única del cliente y validación
de los operandos, relación entre campos y destino del salto. No usa un RVA fijo.
Solo se aplica al Charger local vivo, del equipo infectado y fuera de fantasma.
Se restaura al desactivar, cambiar de clase/equipo, morir, salir del mapa o
revocar la licencia. Las escrituras comprueban protección y restauración,
actualizan la caché de instrucciones y preservan cambios de otro componente.

El parche conserva los ángulos nuevos del comando. No añade una curva automática
ni confirma que cualquier servidor dedicado acepte el giro.

## No fog y no vomit

La implementación de temporizadores/convars alrededor de `RenderView` se
sustituyó por las rutas que efectivamente deciden y dibujan los efectos.

- **Fog:** entrada 18 de `TerrorClientMode::ShouldDrawFog`. El vtable del cliente
  instalado está en `0x105BD4FC` y esa entrada apunta a `0x100C2ED0`, un cuerpo
  `mov al,1; ret`. Se sustituye solo la entrada del vtable; no se intercepta el
  pequeño cuerpo compartido por otros métodos. No se escriben convars de niebla.
- **Vomit:** la firma resuelve el método de dibujo de partículas de pantalla
  (`0x102547E0` en el cliente cotejado). Se valida también su consulta booleana
  vecina (`0x102547C0`) y que ambos accedan al mismo campo de partículas. Esa
  consulta alimenta `$vomitEnable` en el posprocesado (`0x101C4956`); el método
  de dibujo se llama desde `RenderVomitScreenSpaceParticles` (`0x101C75D0`).
  Se filtran los dos pasos. No se cambian los temporizadores de la entidad ni
  se desactiva todo el posprocesado para esconder un solo efecto.

Las opciones vuelven a llamar a los métodos originales al desactivarse o perder
validez el runtime/licencia. Si falta cualquiera de las dos rutas de vomit, no
se instala una solución parcial. El estado del menú indica disponibilidad de
las rutas resueltas, no una medición visual de la escena.

La evidencia MCP, las firmas y el cotejo de vtables están en
`docs/re/visual-removals/`. Cliente cotejado: SHA-256
`c0a8e1e88f7312db11bcfd4b0ec574d224a34b2d9a615d6088b28335eeb4af1f`.

## Coste de la privacidad

La copia anterior reconstruía y presentaba la ventana privada en cada Present
del juego. Ahora:

- El presupuesto privado sigue la frecuencia de la pantalla con límite de
  **60–90 Hz**; entre actualizaciones se conserva el fotograma ya presentado.
  Se omiten también la generación del menú/ESP y la presentación adicional
  en esos fotogramas. El juego continúa con su cadencia original.
- Un retraso no genera una ráfaga de trabajo acumulado. F8 se procesa entre
  actualizaciones; abrir/cerrar el menú puede forzar una actualización inmediata.
- Los parámetros y capacidades de DX9 se consultan al configurar/resetear el
  dispositivo. La geometría de ventana se consulta como máximo cada 100 ms.
- El desenfoque de fondo se refresca como máximo a 25 Hz y durante 200 ms tras
  cerrar el menú. Sus animaciones siguen usando el tiempo real de cada frame.
- La presentación adicional mantiene `D3DPRESENT_DONOTWAIT` y `DISCARD`.
  [Microsoft documenta](https://learn.microsoft.com/en-us/windows/win32/api/d3d9/nf-d3d9-idirect3dswapchain9-present)
  que la bandera permite devolver `WASSTILLDRAWING` cuando el hardware está
  ocupado. El presupuesto también limita los reintentos en ese caso.

La ventana sigue deshabilitada para entrada, sin activación y excluida de las
capturas compatibles. No se vuelve a dibujar el ESP en el buffer del juego
como alternativa al fallar la privacidad. La recuperación de controles de
3.1.16 se conserva.

## Verificación y límites

- **21 suites nativas**, todas correctas.
- **20 comprobaciones de efectos:** llamadas nativas con los detours reales,
  `this`, retornos, partículas, independencia, licencia inactiva, entrada de
  vtable compartida, restauración y fallo parcial.
- **19 comprobaciones del Charger:** estados de jugador, bloque incompleto,
  coherencia de campos, cambio/restauración del byte, protección de página y
  conflictos externos sobre código nativo aislado.
- **27 comprobaciones de privacidad:** presupuesto de cadencia, ausencia de
  ráfagas tras retrasos, foco/entrada, afinidad de captura y aislamiento real de
  los backbuffers DX9. A 1000 frames de entrada simulados por segundo se generan
  aproximadamente 90 actualizaciones privadas, o 60 para una pantalla de 60 Hz.
  Es una comprobación de trabajo programado, no una medición de FPS de L4D2.
- Persistencia, aim, movimiento, salud, rapidfire y las demás regresiones se
  conservaron y pasaron. El nuevo interruptor del Charger también se guarda.

No había una partida de L4D2 activa para comprobar visualmente niebla, bilis o
giro ni para medir FPS. Tampoco se verificó una grabación real de OBS/Discord.
La recuperación efectiva de FPS depende de la carga de la escena, el controlador
y la frecuencia de pantalla. La entrega utiliza el loader 3.1.6 existente.

## Entrega

La API vigente publicó y verificó la firma y la descarga autenticada del módulo
**3.1.17** el 21 de octubre de 2026 mediante el administrador privado de Vercel.
La DLL comprimida tiene **3 652 608 bytes** y SHA-256
`075843de55ea8f7011efa2eebe000509d660d2e8075de7d7dde5d2d28c578df8`.
Las copias de Release, loader y compressed coinciden con esa publicación.

Accounts incorpora una nueva interfaz de descarga con figuras SVG animadas,
estados de transferencia y verificación, cancelación y reintento. Conserva la
comprobación de licencia por parte, SHA-256 y la política de movimiento reducido.
La compilación de producción y sus 24 pruebas de descarga/seguridad pasaron;
también se probó la descarga completa, cancelación y reintento en un servidor
local con una cuenta de prueba aislada. Esa cuenta y ese backend no se publican.
Commit de la web: `e89739ab73bd91dbc36dce634d882ba46dc8a382`.
