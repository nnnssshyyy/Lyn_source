# Rapid fire con boost 4x: 3.1.24

El usuario pidió aumentar la intensidad del rapid fire después de recibir
3.1.23. El boost pasa de 2x a 4x en los tres métodos. Con el mismo factor y
boost activado se genera el doble de extras que antes:

| Factor efectivo antes del boost | 3.1.23, boost 2x | 3.1.24, boost 4x |
| --- | --- | --- |
| 6 | 12 | 24 |
| 40 | 80 | 160 |
| 200 | 400 | 800 |

El factor sigue en 1..200 y el límite efectivo pasa de 400 a 800 extras.
El menú muestra `boost 4x`. Se conserva la clave interna histórica del
booleano para respetar la preferencia de los perfiles existentes: una
desactivación guardada no se pierde. Sin boost, la generación conserva su
factor original. No se habilita rapid fire automáticamente.

La generación adicional no demuestra por sí sola un aumento equivalente de
disparos o daño aceptados por el servidor. Esta entrega no contiene una
medición nueva de cadencia ni FPS en partida.

## Recuperación al soltar

`src/game.cpp`, `src/game.h` y `src/features.cpp` conservan exactamente sus
bytes de 3.1.23. Continúan la restauración del presupuesto de envío después
de los extras, la limpieza temporal de ACK/choke y la recuperación nativa de
predicción tras la confirmación de la ráfaga. La nueva cantidad pasa por ese
mismo flujo y por su consulta de la tecla entre extras.

Se amplían las regresiones de interrupción y devolución de ticks hasta 800
extras. La API nativa prueba la corrección base durante más de cinco vueltas
del anillo de 150 comandos. La simulación del presupuesto de envío cubre
ráfagas de dos segundos con 400 y 800 extras en cada uno de los tres métodos,
incluyendo el cargo del movimiento ordinario y la ausencia de espera sintética
al terminar. Se mantienen las regresiones de confirmación demorada y pérdida
de paquetes.

Respaldo de fuentes de 3.1.23: `build/rapidfire-power-source-3.1.23/`.

## Validación y entrega

Release Win32 compiló y pasaron las 21 suites: 211 comprobaciones de rapid
fire, 158 de API nativa, 25 de persistencia, 44 de captura y las restantes
regresiones. Accounts pasó sus 35 pruebas. La validación de carga x86,
reubicación, once recursos, integridad UPX y secciones tras descompresión
también pasó.

DLL comprimida: 3 656 704 bytes, SHA-256
`74b38d197a0852098696b4ec78003cf857ab9690aadd81ba016a9bd87c4a68e4`.
El paquete conserva una copia de la entrega 3.1.23 anterior.

No se ha medido la nueva cadencia o coste en una partida; las simulaciones
no se presentan como prueba de estabilidad en un servidor real.

El panel privado de Vercel y la API de licencias confirmaron la publicación
3.1.24, firma y descarga real con el hash indicado. Evidencia y paquete:
`build/rapidfire-power-3.1.24/`. El loader continúa en 3.1.8; reiniciar L4D2 y
cargar de nuevo entrega el módulo nuevo.

Accounts se desplegó desde commit
`6f782c2ffccaf2ad6d5e7bebb8b6c9ec36312425`. El despliegue de producción
`4CijVnfRcCsSuBZsu5VmN4pxCtsb` terminó como Ready en Vercel.
