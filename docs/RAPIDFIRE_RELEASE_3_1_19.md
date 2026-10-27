# Recuperación al soltar rapid fire: 3.1.19

**Histórico:** el usuario informó pérdida de efecto con esta versión. La ventana
de quince comandos y el máximo de catorce extras descritos aquí se retiraron en
[3.1.20](RAPIDFIRE_SPEED_3_1_20.md); las correcciones al soltar se conservan.

El usuario informó congelamiento temporal de la escena tras mantener rapid fire
dos segundos y soltar la tecla, con los tres métodos. No había una partida
ejecutándose para reproducir visualmente el síntoma. La revisión del camino
común encontró estado que sobrevivía a la ráfaga y predicción redundante.

## Correcciones

- Las correcciones guardan el número completo del comando. Un comando distinto
  con el mismo módulo 150 ya no hereda su desplazamiento. Un callback antiguo
  tampoco borra una entrada más reciente. Los ACK retiran las correcciones de
  inmediato; no se espera a reutilizar las 150 posiciones.
- RunCommand solo corrige el reloj del jugador que realmente está prediciendo
  si es el jugador local. Los comandos de otro jugador no cambian su tickbase.
- Al soltar no se añade otra llamada manual a Prediction::Update por el mero
  hecho de conservar historial pendiente. La predicción nativa sigue usando
  las correcciones de comandos todavía sin confirmar.
- Los tres métodos declaran ACK/choke solo dentro de cada CL_Move nativo. Al
  volver, ChannelPatch restaura únicamente los valores que sigan siendo sus
  propias escrituras. Conserva los ACK nuevos y el contador limpio de
  SendDatagram; no restaura una instantánea en un canal diferente.
- Cada ráfaga reserva el comando normal, utiliza como máximo 14 extras y se
  reduce según la ventana de 15 comandos pendientes de confirmar. Los factores
  guardados se conservan como solicitudes, sujetas a ese presupuesto. Se
  generan siempre los movimientos nativos normales aunque no haya margen.
- Se vuelve a consultar la tecla antes de cada extra. Soltar cancela la cola,
  solicita envío y devuelve inmediatamente los ticks prestados que no llegaron
  a utilizarse, si la predicción base ya había aplicado la corrección completa.

## Pruebas

Release Win32 compiló y pasaron **21 suites**, incluidas **92 comprobaciones de
rapid fire** y **112 de API nativa**. La compilación final de la DLL se fuerza
después de la última edición del historial para evitar reutilizar un objeto
que un compilador hubiera empezado antes de esa edición.

Las regresiones incluyen pulsaciones de dos segundos, factores 6 y 200, ACK
retrasado tres fotogramas y pérdidas intermitentes. Comprueban que continúan
los comandos normales, no se añaden predicciones manuales tras soltar y el
historial termina de retirarse al llegar el ACK, sin esperar otra vuelta del
anillo. Son simulaciones del flujo de comandos, no una medición de red real.

La API nativa se prueba con los tres métodos durante 120 llamadas por método,
incluidos envío/ausencia de envío, actualización del ACK por el motor y cambio
de canal. También se comprueban identidades de comandos y jugadores, recuentos
del historial y la devolución de la corrección en ráfagas interrumpidas.

El comportamiento del servidor y la desaparición del congelamiento en una
partida real aún requieren comprobación. No se cambia la velocidad del servidor
ni se garantiza daño adicional por generar comandos locales.

Referencia de la predicción nativa y su historial:
[Valve Source SDK](https://github.com/ValveSoftware/source-sdk-2013/blob/master/src/game/client/prediction.cpp).
La rama y el ABI concretos de L4D2 se conservan según las resoluciones del proyecto.

## Entrega

El 22 de octubre de 2026 se publicó **3.1.19** mediante el panel privado de
Vercel y la API de licencias vigente. El panel confirmó firma y descarga real.
DLL comprimida: **3 655 168 bytes**, SHA-256
`3edea79ebe904f6cf8670070d3531b7b0b07a01bfe776a769298cd89c2d3c816`.
Se verificaron carga x86, reubicación, los once recursos y las secciones tras
descompresión. El paquete y el respaldo de 3.1.18 están en
`build/rapidfire-release-3.1.19/`. El loader sigue en 3.1.8 y entrega el módulo
actual al cargar después de reiniciar el juego.
Accounts publicó la referencia de versión desde el commit `65a6780`;
el despliegue de producción `4ywvz4g9S72SeKTUfYfV7bT2y6DS` terminó como Ready.
