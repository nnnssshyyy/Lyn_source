# Accuracy de escopetas

En **Combate → Armas**, selecciona Pump, Chrome, Auto o SPAS-12. Activa **Accuracy (escopeta)** y ajusta el slider entre 0 y 100 %. Clic derecho permite asignar un bind de alternar, mantener activado o mantener desactivado. Cada escopeta conserva su estado, porcentaje y bind, también sin custom combat settings. Recoil sigue teniendo su ajuste independiente.

Desde la implementación experimental 3.1.50, Accuracy selecciona un patrón compacto además de compensar el eje. Antes de esta revisión, sólo restaba el desvío angular. A 0 % conserva el disparo nativo; a 100 % selecciona el patrón más compacto de los candidatos auditados. **100 % no significa dispersión cero.**

**No se han probado impactos ni daño en una partida de Valve.** La coincidencia del RNG local y la inspección del protocolo no constituyen esa validación. La ruta está preparada para escopetas vanilla; no garantiza compatibilidad con servidores que sustituyan el patrón, el RNG o el número de perdigones. No instala componentes en servidores ni cambia scripts del juego.

## Patrón y sincronización

El FireBullet instalado centra el primer perdigón y utiliza SharedRandomFloat con la etiqueta CTerrorPlayer::FireBullet Spread y semillas adicionales 2…N para los demás. Pump tiene 10, Chrome 8, Auto 11 y SPAS 9 perdigones en los scripts montados desde update/pak01_dir.vpk.

Se buscaron offline 10 millones de números positivos y se conservaron 34 candidatos. Cada par número/semilla se comprueba con MD5_PseudoRandom. No se busca durante la partida. El selector considera todos los perdigones, respeta un patrón nativo que ya sea más compacto y aplica objetivos progresivos según el slider. La constante de RijiN deja perdigones exteriores en Pump y Auto.

El número local de comando se conserva para predicción, anillo y validación del CRC. La semilla elegida se escribe antes de crear la copia verificada. El adaptador del serializador nativo traduce números en copias temporales de **to y from**, para que el receptor derive la misma semilla. Las correspondencias persisten al soltar el bind y en retransmisiones; se acotan a 150 comandos, comprueban dirección, número y semilla y se borran al cambiar de mapa. No se altera la frecuencia ni se generan comandos extra.

El [ReadUsercmd de Valve](https://github.com/ValveSoftware/source-sdk-2013/blob/master/src/game/shared/usercmd.cpp) sirve como referencia del protocolo; el SDK 2013 no es el código exacto de L4D2. Las firmas y ABI de L4D2 se contrastaron separadamente con el binario instalado. Si las firmas del serializador o bucle de perdigones no validan de forma única, no se activa la agrupación.

## Eje del disparo

Se mantiene CTerrorGun::UpdateSpread con reloj acotado al tickbase local, restaurando el campo del arma, el reloj y la semilla global después de muestrear. Se resta la intensidad elegida de HorizSpread y VertSpread con la misma semilla del patrón. Un fallo de muestreo revierte la semilla candidata. Las muestras se acotan a 150 identidades con handles, tickbase e intervalo. La cámara y el viewmodel no reciben la corrección por disparo.

## Validación

604 comprobaciones de patrones/semillas/historial, 163 de compensación angular y 61 de perfiles; 328 comparaciones contra vstdlib nativo. Incluye últimos perdigones, intensidades, comandos posteriores, retransmisión, liberación del bind, reutilización de slots y copias de otros buffers. El auditor carga sólo vstdlib en su proceso; no inicia ni inyecta el juego.

[Detalles y límites de la 3.1.50](SHOTGUN_GROUPING_3_1_50.md).
