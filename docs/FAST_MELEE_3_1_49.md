# 3.1.49 — Ciclo melee y bind propio

En **Filtros → Automatización**, activa **Cuerpo a cuerpo rápido / Fast melee** y asigna **Tecla de Fast melee (mantener)**. Por defecto es Mouse4. Equipa un melee, conserva una primaria en el inventario y mantén la tecla junto con ataque. El menú muestra el estado de la asistencia y las condiciones que faltan.

Se eliminó la espera fija de 120 ms: el ciclo observa `m_bInMeleeSwing` y espera a que termine el swing nativo antes de cambiar a la primaria. Si el ataque no llega a confirmarse, no fuerza el cambio. La propiedad se resuelve por nombre; la evidencia del binario instalado se conserva con el paquete.

Los cambios generados por la asistencia ya no se interpretan como selecciones manuales al repredicir un comando. Un historial de 150 comandos conserva el resultado por jugador, arma y semilla sin retroceder la fase de recuperación. El regreso al melee continúa aunque se suelte el bind y espera la confirmación del arma activa. Recargar, empujar, usar otro objeto o seleccionar manualmente otra arma cancela la asistencia.

La ayuda ya no depende de los campos de física del auto strafe. Requiere un superviviente vivo y móvil, melee y primaria reales del inventario y un estado de swing legible. No escribe cooldowns ni inventa ataques. Los perfiles antiguos reciben Mouse4 cuando todavía no tienen la tecla nueva.

Validación: Win32 Release; 770 comprobaciones de lógica de movimiento/ciclo melee, 189 de API y 61 de perfiles. Incluyen swings largos, repredicción, selecciones propias y manuales, retorno lento, ausencia de swing y persistencia del bind. La cadencia y los impactos reales siguen pendientes de una prueba en partida.
