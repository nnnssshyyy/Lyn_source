# 3.1.50 — Agrupación experimental de escopetas vanilla

Accuracy selecciona semillas de patrón y compensa el eje con la misma semilla.
Conserva perfiles, slider 0–100 y binds. No altera las otras familias de armas,
los cooldowns ni el ritmo de envío. Sigue apagado por defecto; los perfiles
que ya lo tenían activado conservan su elección.

El menú indica que la verificación en una partida de Valve está pendiente.
No hay confirmación de impactos o daño remoto.

| Arma | Perdigones vanilla | Radio máximo del candidato a 100 % |
|---|---:|---:|
| Pump | 10 | 0,159848 del límite radial nativo |
| Chrome | 8 | 0,109097 |
| Auto | 11 | 0,216262 |
| SPAS | 9 | 0,128493 |

Son límites del patrón calculado offline con el RNG nativo y los scripts
instalados, relativos al eje corregido. No son medidas de impactos en un
servidor. Se conserva el patrón original cuando ya cumple el objetivo.
La separación residual no se presenta como todos los perdigones en un punto.

La sincronización traduce copias temporales en el serializador nativo.
El número usado por la secuencia local y su anillo permanece intacto.
El historial incluye ambas bases del delta, persiste al liberar el bind y
se invalida ante reutilización del comando o buffer. Los números de destino
son positivos y su semilla se verifica con MD5.

Validación: Win32 Release; 604 comprobaciones de patrones/sincronización,
163 de eje, 61 de perfiles y 328 comparaciones con vstdlib. Las firmas nuevas
tienen una única coincidencia ejecutable en el cliente instalado. El mapa
Tumtara, los scripts y los binarios del juego no se modifican.
