# Recuperación de velocidad de rapid fire: 3.1.20

**Actualización:** [3.1.21](RAPIDFIRE_BOOST_3_1_21.md) añade el refuerzo 2x
solicitado por el usuario y corrige la pérdida de historial durante extras que
dan la vuelta al anillo.

El usuario informó que 3.1.19 había perdido el efecto anterior. La corrección
del congelamiento había introducido dos restricciones independientes: esperar
confirmaciones dentro de una ventana de quince comandos y ejecutar como máximo
catorce llamadas extra a CL_Move. Esto recortaba factores altos y podía dejar
sin extras varios fotogramas seguidos incluso con el factor habitual.

## Cambio

- Se elimina la ventana de ACK como condición para volver a armar. El cálculo
  vuelve a ser `max(factor, int(0.06 / intervalo + 0.5))`, limitado a 200 extras.
- El ejecutor compartido mantiene el factor completo de rapid fire. Speedhack
  conserva su propio máximo de catorce extras; los factores no se suman.
- Cada extra de rapid fire conserva los argumentos nativos y su ruta de envío.
  El máximo de comandos de un mensaje no se utiliza como máximo de llamadas a
  CL_Move por fotograma.
- Se conservan las correcciones de 3.1.19 al soltar: identidad completa de
  comando, retiro del historial confirmado, corrección solo del jugador local,
  restauración del canal después del movimiento nativo, ausencia de predicción
  manual al estar inactivo y devolución de ticks que una cancelación no usó.
- Se conservan las optimizaciones de captura de 3.1.18.

## Verificación

Las regresiones pasan por el mismo `movement_batch::Combined` que usa el
ejecutor real. Simulan 120 fotogramas pulsados para los tres métodos y factores
6, 40 y 200, con confirmaciones retrasadas y pérdidas intermitentes. Exigen el
factor completo en **cada** fotograma: el test anterior solo exigía que hubiera
algún armado y no detectaba esta regresión. Después comprueban diez fotogramas
sueltos, el retiro de correcciones y una nueva pulsación sin espera añadida.
También se conserva la reparación de ráfagas canceladas antes de su final.

Estas pruebas validan el flujo local y las restauraciones. El daño aceptado por
el servidor y la desaparición visual de la pausa siguen pendientes de partida.
No había un proceso de L4D2 disponible durante esta revisión.

Release Win32 compiló y pasaron las **21 suites**, con **116 comprobaciones de
rapid fire**, **21 de presupuesto de movimiento**, **112 de API nativa** y **44
de captura**. No se modificaron fuentes después de esa compilación.

El respaldo de los fuentes de 3.1.19 queda en
`build/rapidfire-speed-recovery-source-3.1.19/`.

## Entrega

El 22 de octubre de 2026 se publicó el módulo **3.1.20** mediante el panel
privado de Vercel y la API vigente. El panel confirmó firma y descarga real.
DLL comprimida: **3 654 656 bytes**, SHA-256
`d62664edcb5e3522f6bcbc9bdbcb0e41476255a4c43cc1dac0d5ccae985e8c25`.
La carga x86, reubicación, los once recursos y las secciones descomprimidas
pasaron la validación. Paquete y respaldo de 3.1.19:
`build/rapidfire-speed-3.1.20/`.

Accounts actualizó ambas referencias de módulo en el commit `18072f3` y el
despliegue de producción `7zetPSvybTyTvL2Z4q9UhP4Sridy` terminó como Ready.
Pasaron también sus 35 pruebas. El loader sigue en 3.1.8; se reinicia L4D2 y se
vuelve a cargar para recibir la DLL actual, sin descargar de nuevo el loader.
