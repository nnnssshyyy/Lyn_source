# Raze V1/V2 integrados: módulo 3.1.30

La petición de implementar el resultado del análisis se concreta en las dos
rutas de lag ejecutable verificadas en Raze, conectadas al modo manual y a
la adaptación automática durante el descenso. El control anti-fall original
no tenía un consumidor de juego identificado. **Este cambio no demuestra
que el lag evite daño de caída; el modo automático sigue siendo experimental.**

## Comportamiento

En Varios / Otros, lag exploit tiene un selector Raze V1 / Raze V2 y conserva
su bind (Alt por defecto). Automatic no fall damage tiene un selector propio
fall method, sin bind requerido, y se activa durante el descenso. La selección
inicial de ambos es V1. Los factores V1 y V2 se guardan independientemente,
con rango 10–1000; el factor V1 conserva la clave de configuración anterior.
El factor elegido se comparte entre los modos manual y automático del mismo
método. Las demás funciones y el loader 3.1.10 conservan sus ajustes.

- **Raze V1:** obtiene el canal mediante VEngineClient013, slot 74, aumenta
  la secuencia de salida en +0x08 y solicita envío en el contexto del comando
  nativo actual. El flag se descarta al terminar el comando; no se deja una
  solicitud de lag pendiente para después de soltar o aterrizar.
- **Raze V2:** comprueba esa consulta y escribe el mismo campo a través de la
  vista validada del client state. Conserva la política de envío normal.
  Si la consulta devuelve nulo, conserva la solicitud de envío de esa rama
  de Raze, sin escribir una dirección nula.

Ambas rutas llegan al mismo canal en este engine. La diferencia de envío
es la diferencia material del selector; no se promete otra cadencia por
usar dos nombres. Se añadieron validaciones frente al original: jugador
vivo, rutas reconocidas, límites de factor/secuencia y punteros coincidentes.
No se reproducen sus escrituras sin comprobación de puntero nulo.

El modo automático conserva el inicio desde la primera velocidad vertical
negativa fuera del suelo y termina sobre suelo o al dejar de descender. El
manual mantenido tiene prioridad sobre el automático. Ambos comparten un
control de comandos: un replay, un cambio de tecla o de método no duplica
la escritura del mismo comando. Se eliminó la escritura manual por frame
renderizado. No se multiplica el lag por los frames de dibujo o por los
movimientos extra de rapid fire/speedhack.

## Prioridad de red

Una caída con el automático habilitado impide armar nuevas ráfagas de rapid
fire, aunque rapid esté habilitado sin bind. También pausa nuevos lotes de
speedhack. Una ráfaga ya pendiente termina su recuperación hasta recibir
ACK antes de que el lag modifique el canal. En consecuencia, si el jugador
cae inmediatamente después de una ráfaga, el lag puede esperar esos ACK.
Al aterrizar, los demás modos vuelven a su evaluación normal. El lag manual
mantenido bloquea igualmente nuevos lotes de ambos exploits.

Menú abierto, falta de foco, incapacidad, pin, fantasma, muerte, datos inválidos
y cambio de nivel liberan el modo automático. Airstuck conserva prioridad.
Si V1 no tiene contexto de envío validado, muestra unavailable y evita una
aplicación parcial. Ningún modo de lag crea un bucle de choke ni rebobina las
secuencias al terminar.

## Verificación del engine instalado

Engine SHA-256:
`8a6d468059394bf4cfce32f11dcba12ef45de88da1373084d5a2916d0bf7ba86`.

La creación de VEngineClient013 en VA 0x10339EB0 registra la fábrica
0x1005CB80, que devuelve el objeto 0x10425504. Su vtable 0x10359DEC contiene
en slot 74 el método 0x1005DDA0. Es `call 0x10082CA0; mov eax,[eax+10h]; ret`.
La firma de CL_Move resuelve exactamente el mismo getter 0x10082CA0.
Se usa esa relación al ejecutar, sin hardcodear las direcciones absolutas.
La vista devuelta es root+8; su canal en +0x10 equivale a root+0x18 de Raze.
Evidencia: `build/raze-lag-3.1.30/engine-v1-route.json`.

## Pruebas y entrega

Release Win32 y 21 suites: 0 fallos. Incluyen 172 comprobaciones de API
runtime con una ruta x86 nativa de slot 74: V1 pide envío, V2 no, se conserva
el fallback de canal nulo, se rechaza otra función en el slot y se verifica
que solo cambie la secuencia. La suite de movimiento tiene 722 comprobaciones;
persistencia 29; rapid fire 211. El menú real se renderizó fuera de partida
para V1 automático y V2 manual con objetos de producción y Direct3D9.
No hubo prueba en partida, medición de FPS ni confirmación de prevención de daño.

DLL: 3 662 848 bytes. SHA-256:
`a0a94cf17f0a195c4ccdd9f6834a3891bd24e2d93a790584ed8db9158f2ff6da`.
UPX, carga nativa, reubicación, once recursos y conservación de secciones al
descomprimir: correctos. Fuentes previos en `build/raze-lag-source-3.1.29/`;
binario previo en `build/raze-lag-3.1.30/previous-3.1.29/`.

El panel privado verificó versión, firma y descarga reales. Accounts: 35
pruebas, 0 fallos; commit `751b72c8354f76796a8d3ced798bc9e804558f95`. Producción
`5Hyf7nayQqr89evAHmDHYb1ervHr` confirmada Ready y Current en Vercel.
