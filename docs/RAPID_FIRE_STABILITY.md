# Rapid fire sostenido: corrección de desincronización

**Estado: cambio revertido por petición del usuario.** Este documento describe la propuesta 3.1.8 y el rollback a 3.1.7. La entrega local posterior incorpora la [integración de Raze](RAZE_RAPIDFIRE_INTEGRATION.md), con los métodos Storm/choke/flush y activación sostenida. El ejecutable del loader no se modifica.

El síntoma comunicado es movimiento local aparente después de soltar rapid, seguido de una corrección de posición a los dos o tres segundos. No se reprodujo esta sesión dentro del juego.

La implementación anterior podía generar hasta 200 movimientos extra por tick. El registro `build/Release/lyn.log` contiene ráfagas de 200 extras a intervalos de 0,03333 s. Además escribía ACK -1 y choke 255 en el canal, utilizaba correcciones sin identificar el número completo del comando y borraba su historial al soltar la tecla. Estos son mecanismos compatibles con una divergencia de predicción; un ACK no demuestra daño ni movimiento aceptado por el servidor.

La corrección conserva la activación sostenida:

- Cada ráfaga espera el ACK de la anterior y una recuperación de al menos 250 ms. La recuperación aumenta con el número de extras y el intervalo del motor, hasta 1 s. Los ticks ordinarios siguen ejecutándose durante esa espera.
- Solo se generan extras cuando existe una orden de ataque. Cada lote respeta el máximo de 15 comandos nuevos y descuenta los comandos que ya estaban retenidos.
- No se escriben los campos de ACK ni de paquetes retenidos desde rapid fire. Los tres índices persistidos conservan sus velocidades como presets; los controles de Storm/choke retirados ya no representan una operación de red.
- Las correcciones se identifican por número completo de comando y se conservan al soltar o desactivar rapid hasta que el servidor las reconoce. El hook solo corrige al jugador propietario del historial.
- Un fallo de predicción interrumpe los extras. Si una ráfaga se corta, su corrección y rango pendiente reflejan los comandos efectivamente generados.
- El historial se restablece al cambiar de jugador o mapa. Con rapid desactivado y sin comandos pendientes se evita el sondeo adicional del transporte.

Se añadieron 38 comprobaciones de política: pulsación sostenida con ACK detenido, repetición espaciada durante tres segundos, soltar/reanudar, recuperación, ráfaga parcial, presupuesto del mensaje, historial al completar 150 comandos y reproducción de la predicción después de soltar.

La estabilidad y el daño efectivo en un servidor dedicado requieren una prueba en partida. La corrección del cliente no cambia las reglas de procesamiento ni los cooldowns del servidor.
