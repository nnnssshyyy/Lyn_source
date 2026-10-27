# Storm automático integrado: módulo 3.1.31

El modo «automatic no fall damage» pasa a usar la lógica de caída de Storm.
En Varios / Otros basta habilitar ese control; no tiene bind ni factor manual.
Raze V1/V2 quedan en el lag manual. La clave anterior `fall_lag_method` se conserva
al leer/guardar configuraciones antiguas, pero ya no selecciona el automático.

## Fuente y comportamiento

Referencia suministrada: `C:/Users/User/Downloads/Storm-L4D2-Updated-main/Storm-L4D2-Updated-main/Storm-master`.
Los hashes están en `build/storm-fall-research-3.1.31/reference-sources.json`.
No se ejecutó ni modificó ese proyecto.

- `Copy_Command.hpp:300`: solicita corrección al alcanzar
  `m_flFallVelocity + 800 * interval_per_tick >= 560`. Se usa la velocidad
  de caída del jugador, no el signo de `m_vecVelocity.z`.
- `Copy_Command.hpp:199`: calcula `((tickbase + 149) / 150) * 150 + 300`,
  equivalente a `Sequence_Shift(2)` sin comandos extra del lote. El modo
  automático no se ejecuta dentro de extras de rapid fire ni speedhack.
- El salto modifica exclusivamente `CNetChan + 8`. Se registra el mismo
  valor con identidad de jugador, número de comando y seed en 150 posiciones.
- `Run_Command.hpp:35,41,45`: se resta la corrección antes del RunCommand
  nativo; al volver se añade el mismo valor y se restablece el tiempo global
  a `(tickbase_restaurado - 1) * interval`. Se conserva el avance del motor.
- `Process_Movement.hpp:5`: durante esa simulación con tickbase negativo,
  se limpia la máscara de botones 1564, se conserva el estado real de duck
  y se ponen forward/side a cero antes de llamar al movimiento original.
- `Update.hpp:7`: mientras hay correcciones pendientes, la ventana de
  predicción queda en `incoming + (outgoing - incoming) % 150`.

La adaptación utiliza Prediction::Update y el ciclo RunCommand / MoveHelper
normal del motor. No copia la segunda simulación manual de Copy_Command,
la transferencia personalizada CPredictionCopy de Post_Network_Data_Received,
ni los reemplazos globales ReadPackets/SendMove de Storm. Estos últimos también
sirven a otras funciones de ese proyecto y no se instalaron como requisito
supuesto de anti-fall. Esta adaptación todavía necesita validación en partida.

## Identidad, prioridades y fin de caída

Un replay no vuelve a aumentar la secuencia. Los registros permanecen tras
aterrizar, desmarcar el control o perder foco hasta que llegue su ACK. Un
comando sin ACK no puede ser sobrescrito por otra vuelta del anillo. Jugador
nuevo, cambio de entidad y cambio de mapa limpian la identidad.

Rapid fire y speedhack esperan mientras Storm solicita el canal o conserva
correcciones pendientes. Una ráfaga anterior termina su recuperación primero.
El lag manual tiene prioridad para nuevas solicitudes y también espera las
correcciones pendientes de Storm. El automático solo se solicita para un
superviviente vivo y móvil, fuera del suelo, con física normal; se mantienen
las pausas de menú, falta de foco, incapacidad, pin y airstuck.

La predicción solo se corrige para el jugador local y el retorno validado
C_BasePlayer::PhysicsSimulate. Fallos de reloj bloquean nuevos saltos hasta
reiniciar/cambiar mapa, conservando el historial pendiente. Se rechazan
secuencias desbordadas, relojes no finitos, un reloj que ya es negativo,
campos ausentes, canal nulo y firmas desconocidas.

## Rutas del cliente instalado

Client SHA-256: `c0a8e1e88f7312db11bcfd4b0ec574d224a34b2d9a615d6088b28335eeb4af1f`.

Tres firmas únicas verificadas estáticamente:
- llamada a RunCommand en `0x100674AD`, retorno `0x100674C3`;
- CPrediction::Update `0x1017B780`, slot 3;
- ProcessMovement `0x10278870`.

Al ejecutar se confirma también que el objeto de predicción usado por el
retorno es la interfaz resuelta. Las direcciones absolutas no se codifican.
Fall velocity y duck se buscan por nombre en las RecvTables anidadas.
Evidencia: `build/storm-fall-research-3.1.31/native-routes.json`.

## Verificación

Release Win32 y 21 suites nativas: correctas. Pruebas específicas de umbral
según tickrate, alineación de tickbase, replay, seed y propietario, colisión
del anillo sin ACK, retiro por ACK, restauración del reloj y buffers de
movimiento. Se comprueba el reloj durante 100 predicciones consecutivas.
El menú de producción se renderizó con Direct3D9 fuera de partida.

No hubo prueba dentro de L4D2, medición de FPS ni comprobación de inmunidad al
daño. Los resultados de las pruebas nativas no demuestran que el servidor
acepte el exploit. El control continúa marcado como experimental.

Fuentes anteriores: `build/storm-fall-source-3.1.30/`.
Entrega y evidencias: `build/storm-fall-3.1.31/`.
