# Bunny hop y auto strafe

El módulo 3.1.13 permite bunny hop al infectado fantasma que mantiene física
de caminar, incluso si conserva campos de incapacidad o pin de su vida anterior.
Se mantienen las exclusiones de noclip, escaleras, cámara de espectador y natación.
Mientras es fantasma se procesa movimiento y se pausa el combate automático.
Pasaron las 18 suites nativas, con 121 comprobaciones de movimiento aéreo y una
secuencia de salto, liberación en el aire y nuevo salto al aterrizar como fantasma.
Falta comprobar esta corrección en una partida real.

En **Movement**, activa **auto bunny hop**, **auto strafe** y, para dirigir el
recorrido con WASD, **directional strafe**. Mantén tu tecla de salto habitual.
Con directional apagado, el rumbo lo marca la cámara; las teclas de movimiento
ya no desactivan la ayuda aérea. La cámara no se gira para hacer strafe.

El salto original se conserva por comando antes de corregirlo. Se libera en el
aire y se vuelve a solicitar en el primer comando con suelo y salto mantenido.
Reprocesar un comando ya corregido no pierde esa intención, ni convierte el
movimiento generado en una nueva orden manual. Al aterrizar en una repetición
se recupera también el movimiento original de suelo. Soltar salto en el comando
siguiente termina la ayuda. Los controles no cambian la tecla asignada en el juego.

El strafe anterior apuntaba el empuje casi en la dirección de la velocidad. La
rutina nativa limita cuánto puede acelerarse sobre ese eje: a velocidades altas
ese empuje no añadía velocidad. También abandonaba la ayuda con muchos giros
manuales. La nueva lógica calcula el empuje que maximiza la velocidad horizontal
en el modelo de aceleración aérea y elige entre sus dos lados para acercar el
rumbo a la dirección solicitada. Desde reposo, mantener salto sin WASD puede
añadir avance para iniciar la salida.

Se utiliza el intervalo del tick, se lee `sv_airaccelerate` cada 500 ms y se
consulta `m_flMaxspeed` y la fricción de superficie del jugador. La velocidad
máxima procede de la netvar; la fricción se resuelve por datamap validado.
Si faltan esas lecturas, se usan valores de cálculo de 220 u/s, fricción 1 y
air acceleration 10. La interfaz identifica la aceleración por defecto cuando
no pudo leer la cvar. No se escriben esas cvars, velocidad, posición ni tickbase.

**Evidencia física.** En el `client.dll` instalado, la tabla de
`CTerrorGameMovement` en `0x105BDB44`, slot 24, apunta a `AirAccelerate` en
`0x100FDD00`. La rutina limita la proyección del empuje a 30 u/s y calcula la
aceleración con wish speed, frame time y fricción. Se conserva el
[desensamblado](re/air-movement/client-air-accelerate.asm) y el
[hash del binario y constantes](re/air-movement/native.json). Se leyó el PE
estáticamente; no se cargó el juego para esta comprobación.

La fórmula coincide con la estructura pública de
[AirAccelerate de Valve](https://github.com/ValveSoftware/source-sdk-2013/blob/master/src/game/shared/gamemovement.cpp#L1584).
El cálculo elige una proyección `max(0, cap - aceleración)`, acotada por la
velocidad actual. Si la aceleración disponible alcanza el cap, el óptimo de
este modelo es perpendicular. No hay un multiplicador arbitrario de velocidad.

Por ejemplo, en el modelo con velocidad 220 u/s, aceleración 10, fricción 1 y
30 ticks, el antiguo empuje tenía una proyección superior al cap y no añadía
velocidad. El nuevo puede añadir 30 u/s en perpendicular, dejando una magnitud
de aproximadamente 222,04 u/s tras ese tick. Es un resultado del modelo de
aceleración, no una medición en un servidor con colisiones y aterrizajes.

**Validación.** Pasaron **489 comprobaciones en 12 suites**, incluidas **100 de
movimiento aéreo** y **61 del runtime**. El solver se contrasta con un modelo
directo independiente del cálculo inverso y una búsqueda de 720 direcciones:
72 combinaciones de velocidad, aceleración y tickrates 30/60/100/128. Se prueban
WASD, modo de cámara, salida desde cero, saltos sucesivos, repetición de comandos,
reutilización de slots, respawn, agua, escaleras, pausa y datos inválidos.

No se hacen trazas ni simulaciones adicionales de movimiento por tick y el
controlador usa almacenamiento fijo para 150 entradas. Se mantienen las claves
existentes de configuración, recursos y animaciones.

La DLL Release comprimida pasó integridad UPX, carga x86 con reubicación y
comparación de los once recursos. Falta probar el comportamiento y los FPS en
partida. Las reglas de salto, límites y plugins del servidor siguen determinando
la velocidad aceptada; esta mejora no certifica una velocidad concreta en
ZoneMod o Confogl.
