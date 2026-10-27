# Charger Teleport — 3.1.54

Se integró la ruta `bAutoTeleportCharger` de la DLL de Raze suministrada por el
usuario. La revisión se hizo con IDA Pro y Hex-Rays mediante el servidor MCP
local, sin ejecutar Raze. Los JSON originales y su manifiesto están en
`docs/re/charger-teleport/`.

## Evidencia de Raze

DLL: `C:\Users\User\Desktop\raze\RZL4D2_[unknowncheats.me]_.dll`.
SHA-256: `dfa8632e85c89c318577ef619f20bae2f4db5136042d680ee4dae4109234691a`.
La copia de entrada de la base de IDA tiene el mismo hash.

Con base preferida `0x10000000`:

- La opción usa el byte `0x103F53CB`.
- El hook `0x1016FF00` llama a `0x101472B0` desde `0x10170100`.
- El bloque que empieza en `0x101476A2` obtiene el jugador local, comprueba
  `m_zombieClass == 6` y resuelve `m_carryVictim` mediante el método de handles
  de la lista de entidades, índice 4.
- Cuando hay víctima, el indicador `0x103F68E4` todavía está apagado y el método
  de motor `+0x128` (índice 74, `GetNetChannelInfo`) devuelve un canal, la
  instrucción `0x10147770` escribe `0x7CF0BDC2` en `cmd + 0x14`.
- Esos bits representan aproximadamente `1e37f`, un valor finito, en el roll.
  El indicador impide repetir la escritura durante el transporte y se limpia
  cuando ya no se resuelve una víctima.

Esta ruta es independiente del parche de un byte de Charger Turn documentado
en `RAZE_CHARGER_TURN_ANALYSIS.md`.

## Integración

La casilla **Charger teleport**, en Movimiento, empieza desactivada y utiliza
el sistema existente de binds mediante clic derecho (incluido mantener).
La configuración se guarda con la clave `l4d2 movement / charger teleport`.
Debajo se muestran el estado y el número de comandos escritos durante el mapa.

`game::Api` resuelve `m_carryVictim` por nombre y conserva el handle completo,
incluido el serial. Una lectura fallida se distingue de una liberación real.
`charger_teleport::Pulse` permite un comando por transporte, conserva el mismo
resultado al reprocesar ese comando y consume la activación solamente después
de una escritura correcta. Abrir el menú, cambiar el bind o perder una lectura
no vuelve a disparar un transporte ya consumido. Un cambio de víctima, dueño
o mapa reinicia el estado correspondiente.

Se exige Charger vivo del equipo infectado, sin modo fantasma, juego con foco,
estado de transporte legible y canal de red disponible. Airstuck pausa el pulso.
La escritura se aplica después de normalizar los ángulos y corregir movimiento,
antes de que CInput copie y calcule el CRC del comando verificado. El valor
especial se limita al roll del comando; la actualización visible de la cámara
continúa usando sus ángulos normales.

## Validación y límite

Las pruebas específicas cubren el payload exacto, conservación del resto del
comando, filtros de activación, repeticiones, escritura fallida, handles con
serial distinto, lectura interrumpida, liberación, respawn y cambio de mapa.
La API tiene pruebas de resolución del netvar, handles y canal nativo.

El análisis estático y las pruebas verifican la implementación de la rutina.
**El resultado de teletransporte en una partida real, incluidos servidores de
Valve, no se ha verificado.** El contador confirma escrituras locales de
comandos; no confirma aceptación por el servidor ni desplazamiento del jugador.
