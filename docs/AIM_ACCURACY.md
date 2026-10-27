# Precisión durante el movimiento

Los tres modos comparten selección y compensación. Esta entrega corrige errores
concretos de coherencia; no garantiza que todas las balas impacten en un servidor.

- Los huesos actuales conservan el `tick` generado por el motor. Antes se
  seleccionaba una pose interpolada y se enviaba el tiempo de una actualización
  de red más reciente, aunque el punto no procediera del historial.
- Solo las poses históricas válidas cambian el tick. Se comprueba también que el
  tick pueda usarse antes de seleccionar el registro. El límite está expresado
  en tiempo, en lugar de los anteriores 64 ticks para cualquier frecuencia.
- El historial se captura después de renderizar/interpolar. Su tiempo estimado
  es `clock.time - lerp`; no se etiqueta como si fuera una pose reconstruida en
  `m_flSimulationTime`. Si ese tiempo supera la última simulación disponible,
  no se guarda una muestra extrapolada. Las capturas se limitan aproximadamente
  a una por intervalo de comandos y reutilizan buffers al alcanzar 32 registros.
  No se reconstruyen animaciones remotas.
- Silent server también prueba centros antes de bordes. Normal y silent client
  conservan su retención de objetivo/punto para evitar oscilación.
- Normal sigue el cambio angular observado entre comandos consecutivos del
  mismo objetivo, arma y muestra. Suaviza la adquisición y el error restante,
  sin volver a retrasar todo el movimiento. Cambios grandes, comandos discontinuos
  y cambio de objetivo reinician esa continuidad. El ratón que ya sigue al
  objetivo no recibe dos veces la misma corrección.
- El disparo automático normal espera a estar dentro de una esfera conservadora
  de la hitbox/muestra seleccionada. Una cabeza lejana ya no comparte el mismo
  margen fijo de 0,5 grados que un torso cercano. El disparo manual no se cancela.
- Cuando `no spread` está habilitado, todas las familias hitscan usan el updater
  nativo con tickbase, intervalo y seed del disparo; antes solo las escopetas
  actualizaban ese valor en este contexto. Se restauran reloj, seed y spread.
  Los perfiles existentes siguen decidiendo si se activa recoil/no spread.
- La interpolación valida cvars registradas en client, engine, vstdlib y server;
  aplica límites de updaterate y ratio disponibles. Antes algunas cvars válidas
  de otros módulos caían al valor por defecto.

La decisión final de disparo se toma después de trigger y del filtro de pistolas,
antes de registrar el tiro o cambiar su tick. La cámara permanece separada del
ángulo compensado, también al disparar manualmente con no spread.

## Compensación en disparos del aimbot

El motor decide cuándo dispara con su propio reloj. Antes, silent client y silent
server solo escribían el ángulo cuando el cooldown local ya lo consideraba listo;
un comando que el servidor sí disparaba podía salir con la vista del ratón.

- Normal y silent client resuelven el objetivo y escriben el ángulo en cada
  comando con la tecla mantenida. Silent client restaura después la vista del
  ratón, así que la cámara no cambia; el comando que finalmente dispare ya
  lleva la solución. Silent server conserva la estimación de cooldown porque su
  cadencia de retención/envío necesita saber qué comando dispara.
- Los disparos del aimbot compensan siempre el punch (`m_vecPunchAngle`) y el
  spread determinista del comando, aunque los interruptores por arma de recoil
  y no spread estén apagados. Esos interruptores siguen gobernando los disparos
  manuales sin objetivo. La cámara no recibe esas correcciones.
- El tick histórico, el punch y el spread se aplican a todo comando apuntado con
  ataque mantenido, no solo al comando que la estimación local marca como
  disparo: si el servidor dispara en el comando siguiente, ya va corregido.
- Los perdigones de escopeta conservan su patrón; la compensación corrige el eje
  del disparo. El muestreo/restauración de spread no cambia de orden ni de
  contexto: sigue usando tickbase, intervalo y seed del comando.

La relación entre tick e interpolación está documentada en el
[código de lag compensation de Valve](https://github.com/ValveSoftware/source-sdk-2013/blob/master/src/game/server/player_lagcompensation.cpp#L348).
Es una referencia del mecanismo Source; no prueba la configuración exacta de un
servidor L4D2 o sus plugins.

Verificación de esta entrega (Release Win32): **526 comprobaciones en 13 suites,
0 fallos**, incluidas 28 de política, 66 de selección/cámara, 68 de spread, 49 de
rapid fire, 75 de API de runtime, 14 de persistencia y 19/106 de movimiento.
`build/Release` queda con `lyn.dll` y los ejecutables actualizados.

## Comprobación y límites

Release Win32: **489 comprobaciones / 12 suites**, incluidas 28 de política, 59
de selección/cámara/seguimiento, 69 de API y 68 de spread. Hay pruebas con la
selección real, cambio de pose, centros/bordes, historia con tick incompatible,
seguimiento angular, entrada del ratón, extracción real de geometría del modelo
simulado, cvars y muestreo/restauración de spread de las seis familias hitscan.

La DLL comprimida pasó integridad UPX, carga x86 con reubicación, comparación de
sus once recursos y comparación de secciones tras descomprimir. Original:
`build/aim-accuracy/lyn.dll`; entrega: `build/loader/lyn.dll`.

**Pendiente:** prueba real con jugador y objetivo moviéndose en tu servidor.
No se añadió una simulación nativa completa del próximo movimiento local.
Colisiones, animaciones, latencia, recoil/spread desactivados, dispersión propia
de perdigones y reglas del servidor aún pueden producir fallos. Las escopetas
conservan su patrón de perdigones; esta compensación corrige el eje del disparo.

Reinicia L4D2 antes de cargar esta versión. Prueba por separado normal, silent
client y silent server con la misma arma y configuración; compara quieto, en
strafe y saltando. El registro `Aim shot` permite comprobar pose histórica y
tick elegido, pero no confirma daño recibido por el servidor.
