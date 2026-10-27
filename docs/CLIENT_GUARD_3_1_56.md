# Client guard / Anti Anti Cheat — 3.1.56

Adaptación de los dos archivos `AntiAntiCheat` suministrados de l4d2Simple-s.
La implementación se encuentra en `src/client_guard.{h,cpp}` y la política
independiente del motor en `src/client_guard_logic.h`.

## Menú y configuración

Misc → Protection / Protección. Todos los interruptores empiezan desactivados,
con los binds independientes del menú existente (clic derecho). Las reglas se
guardan dentro del perfil y sus códigos compartidos:

- Consultas de ConVars: las 25 sustituciones del archivo original.
- ConVars del servidor: `sv_pure=0` y `sv_consistency=0`.
- Comandos del servidor: `bind`, `exec` y `cl_consistencycheck`.
- Mensajes de usuario: Shake, Fade y BlurFade, identificados por nombre.
- Opciones independientes: CrosshairAngle, CRC de archivos, sonidos nulos,
  latidos y registro de los eventos filtrados.

El editor admite añadir, modificar, eliminar y restablecer reglas. `*` y `?`
son comodines ASCII sin distinción entre mayúsculas y minúsculas. Un valor
vacío bloquea la consulta o el ajuste; una lista vacía filtra cero elementos.
Máximo 64 reglas por grupo y 255 caracteres por nombre/valor. El registro usa
el archivo de log, omite valores de ConVars y limita la frecuencia a 8 eventos/s.

## Cambios frente al código suministrado

Los mensajes de red CRC y CrosshairAngle tienen rutas propias; no se bloquean
los mensajes de usuario 14 y 20. El registro nativo se valida antes de habilitar
el filtro de mensajes de usuario. Los comandos encadenados se examinan completos,
respetando comillas, comentarios y separadores.

El bloqueo de red retorna éxito al consumidor local después de omitir el envío,
en vez de mantener el `blockNetMsg=false` del framework de referencia. La lista
filtrada de ConVars sí se entrega al consumidor. Los punteros de respuestas y
vectores se sustituyen temporalmente y se restauran, sin cambiar la propiedad
del asignador ni reutilizar un buffer estático entre llamadas.

Se suprimen los inicios de sonidos nulos y heartbeatloop, preservando mensajes
STOP. No se envían volumen/pitch negativos ni se modifica el canal de voz.
No se eliminan flags de ConVars ni se escribe un salto sin verificar. La cámara
del proyecto ya dispone de tercera persona independiente del modo de juego;
se conserva esa ruta en Misc → Camera, sin duplicar el parche antiguo.

Las firmas deben ser únicas dentro de las secciones ejecutables. Cada mensaje
se valida por tipo y nombre antes de acceder a su carga. Las rutas ausentes o
datos malformados conservan el comportamiento original. La desactivación de la
licencia o del módulo deja pasar las llamadas originales.

## Evidencia y límites

`docs/re/client-guard/native-audit.json` registra hashes y ubicaciones comprobadas
en IDA del motor y cliente instalados. Los tests nativos ejercitan las seis rutas,
el ABI de sonido, restauración y errores. El matcher se contrasta con un algoritmo
independiente en 12.000 casos. Los tests de perfiles comprueban reglas, listas
vacías, binds y compatibilidad con perfiles anteriores.

Esta validación no incluye una partida real ni demuestra que un servidor acepte
las respuestas filtradas. El módulo no ofrece garantía de indetectabilidad.
