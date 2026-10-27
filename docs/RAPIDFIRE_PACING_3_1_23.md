# Recuperación del presupuesto de envío: 3.1.23

La prueba del usuario con 3.1.22 confirma que la pausa sigue presente. El
registro muestra ráfagas de 400 extras y recuperación nativa correcta, pero
tardía. En dos secuencias, la confirmación completa llegó en el mismo segundo
de la liberación y el reinicio solo se ejecutó cuatro segundos después:

- 08:10:44: último comando 226459; ACK completo 226460; recuperación 08:10:48.
- 08:18:49: último comando 478668; ACK completo 478669; recuperación 08:18:53.

La espera de ACK por sí sola no explica esa diferencia. La recuperación de
3.1.22 también exigía cola de comandos cero. Estos registros no miden el
campo de programación de envíos, por lo que no demuestran por sí solos que
ese campo sea la única causa de la animación detenida.

## Verificación del motor instalado

Se analizó engine.dll, SHA-256
`8a6d468059394bf4cfce32f11dcba12ef45de88da1373084d5a2916d0bf7ba86`.
La tabla CNetChan en VA preferida `0x10390EB4` contiene SendDatagram en
slot 47 (`0x101B9B30`) y CanPacket en slot 57 (`0x101B28B0`).

SendDatagram actualiza el double clearTime como
`max(clearTime, net_time) + bytes / rate`, con límite nativo posterior.
CanPacket usa el mismo double y reloj para decidir si se puede enviar.
En este motor son canal+0xB8 y net_time en VA preferida 0x106B2B48.
Restaurar ACK y choke no restaura ese presupuesto. Los envíos de las llamadas
extra pueden dejar un plazo futuro que impide el envío normal al soltar.

El runtime deriva los operandos de ambas funciones y exige coincidencia de
las cuatro referencias de clearTime, el desplazamiento de rate, el reloj,
los bytes de instrucciones y su pertenencia a la imagen de engine.dll.
Las secuencias deben ser únicas en cada ventana acotada de la función.
No usa una dirección absoluta ni escribe offsets de una versión desconocida.
Evidencia estática: build/rapidfire-native-pacing.json.

## Cambio

Después de ejecutar el CL_Move normal, se guarda su presupuesto de envío.
Se ejecuta el mismo lote completo de extras, con los mismos argumentos y
métodos. Al finalizar se retira únicamente el incremento de programación
producido durante ese lote. Se conserva el cargo del movimiento normal,
el tiempo real transcurrido y cualquier reducción que haya hecho el motor.

No se modifican secuencias, ACK, choke, rate, reloj global, buffers de recepción
o datos fiables mediante esta operación. Cambiar de canal, tabla, imagen,
retroceder el reloj o leer valores no finitos cancela la restauración.
La operación se aplica solo a extras de rapid fire; el movimiento normal y
speedhack conservan su presupuesto nativo. Se mantienen boost 2x, factor
completo y el reinicio de predicción tras confirmar la ráfaga.

El diagnóstico de liberación ahora registra pacing_wait_ms, pacing_removed_ms
y pacing_peak_ms. La firma no reconocida se registra una vez y no autoriza
escrituras. Esto permitirá separar una espera de envío de un problema restante
de predicción con una prueba real.

Respaldo de fuentes de 3.1.22: build/rapidfire-pacing-source-3.1.22/.

## Validación y entrega

Release Win32 compila y pasan las 21 suites: 155 comprobaciones de API
nativa, 202 de rapid fire, 44 de captura y las restantes regresiones del
proyecto. Las pruebas nuevas incluyen dos segundos con 400 extras en cada
método, cargo ordinario conservado, cambios de propietario, tiempo real,
relojes inválidos y rechazo de instrucciones ambiguas o incompatibles.

La corrección no se ha probado aún en una partida y no se afirma que la
pausa haya desaparecido en el equipo del usuario.

El módulo 3.1.23 se publicó mediante el panel privado de Vercel y la API de
licencias vigente, que confirmó firma y descarga real. DLL: 3 656 704 bytes,
SHA-256 `39c5caf14d40f8359d55a03e05b3caba9326831756eb2da35bc40f09cd905b4a`.
Carga x86, reubicación, once recursos y secciones tras descompresión correctos.
Paquete y respaldo anterior: build/rapidfire-pacing-3.1.23/.

Accounts pasó sus 35 pruebas. El loader continúa en 3.1.8: es necesario
reiniciar L4D2 y cargar de nuevo para recibir el módulo actualizado.

Accounts se desplegó desde commit `a971a388fa5d01dd11eb30c899f898c0d7979e10`.
Producción `CeEaMuYmkUzb1zuPc4BttDgvnwH3` confirmada como Ready en Vercel.
