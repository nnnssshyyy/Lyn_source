# Borrado del loader al cerrar su ventana: 3.1.10

La X y Alt+F4 preparan el auxiliar de limpieza antes de cerrar la ventana,
también sin iniciar sesión ni pulsar Load. El cierre vuelve al dispatcher
antes de llamar Close de nuevo, para evitar un cierre anidado desde Closing.
App.OnExit conserva la preparación síncrona como respaldo.

El cierre durante una operación marcaba closeRequested, pero los finally
del login, la activación y la gestión del equipo no atendían esa petición.
Ahora las operaciones comparten FinishOperation: liberan busy y completan
el cierre pendiente. La carga sigue esperando su cancelación o finalización
antes de cerrar. Se conserva la animación de salida.

El auxiliar existente espera a que terminen el loader y su extractor,
comprueba sus identidades y el hash, y elimina únicamente el ejecutable
descargado. El extractor mantiene la limpieza de su propio temporal.
Se conservan registros, ajustes, credenciales, archivos vecinos y --dev.
No se añade vigilancia al iniciar: finalizar el proceso a la fuerza o apagar
Windows puede impedir la limpieza del cierre normal.

## Validación

- El arnés WPF existente renderizó ambas pantallas en cuatro momentos de
  animación. Se añadieron comprobaciones del evento real Closing antes del
  login y del cierre pendiente al terminar una operación; ambas pasan sin
  mostrar ventanas ni contactar con el servicio.
- Pasaron las 14 comprobaciones de limpieza y 7 de transporte/tubería.
- Los probes desechables completo y compacto verificaron el camino síncrono
  de salida, bloqueo de duplicados, elección del ejecutable descargado y su
  eliminación real sin cargar el módulo ni abrir el juego. El completo
  también verificó la eliminación del payload y su temporal.
- El paquete pasó integridad y extracción idéntica al ejecutable interno.
- Accounts pasó sus 35 pruebas, incluida la reconstrucción exacta de ambas
  ediciones desde los nuevos fragmentos.

La prueba que requiere un ticket firmado fresco por stdin no se ejecutó:
la invocación inicial sin fixture quedó registrada en license-tests.log.
El cliente de licencias y su verificación no cambian en esta entrega.

## Artefactos

`build/loader-close-3.1.10/` conserva fuentes anteriores, ambas ediciones,
la entrega 3.1.9 anterior, metadatos y registros.

Completo: 42 137 625 bytes, SHA-256
`a6476dabe9b2d83aa27a426645a12c9a90d01e5361ce78e8f62bfa1d070d1231`.
Compacto: 999 000 bytes, SHA-256
`3e54ebacdf365685f94724e89a719f7a6a2568b7d4a24721ca36a0033902ca89`.

La distribución conserva el módulo 3.1.27 con el mismo SHA-256
`db51e762af779f22b6af220dca4b834c61577d15922927a74b311ff2c83766df`.

## Publicación

Accounts/Vercel publicado desde commit
`332dfa95533e8fc42316dde408a7003f2b5173a6`. Producción
`BDQoLKZTtdCApW4QoCnLUoEoeATx` confirmada como Ready.
Los usuarios deben descargar el loader 3.1.10 desde Accounts.
