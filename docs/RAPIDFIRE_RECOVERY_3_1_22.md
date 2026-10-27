# Rapid fire original y recuperación de predicción: 3.1.22

El usuario probó 3.1.21 y confirmó que continuaba la pausa al soltar y que el
rapid fire había perdido el comportamiento anterior. Se retira la agrupación
de `stable network` y su condición de armado solo en el tick final. Los tres
métodos vuelven a declarar su estado de canal y solicitar envío en cada comando,
con los argumentos originales de CL_Move. Los perfiles antiguos no pueden
reactivar la ruta agrupada, porque su control ya no forma parte del esquema.
Se conserva `boost 2x` y la preferencia que el usuario haya guardado para él.
No se añade espera de ACK al armado ni un máximo de catorce extras.

## Evidencia de la prueba del usuario

El registro de 3.1.21 ya contiene pruebas con `stable_network=1`, factores
efectivos 334 y 167, e intervalo 0.03333. En las 19 liberaciones la cola
de envío era cero y el RTT registrado iba de 68.6 a 129.3 ms; las confirmaciones
terminaban de llegar en el mismo segundo o en el siguiente según la resolución
del registro. Esto no explica una pausa de varios segundos como una cola de
envío pendiente, ni mide el máximo de ping posterior a soltar. No permite
atribuirle una única causa a la animación congelada.

Se encontraron además **43 095** mensajes repetidos del diagnóstico de cámara. El caché
tenía seis posiciones, pero después de llenarse seguía registrando cada objeto
desconocido, abriendo y cerrando el archivo por cada mensaje. Ahora ese
diagnóstico registra como máximo sus primeras seis identidades. No cambia
la respuesta de ShouldDrawLocalPlayer ni la cámara.

## Recuperación al terminar una ráfaga

Se verificó estáticamente el `client.dll` instalado, SHA-256
`c0a8e1e88f7312db11bcfd4b0ec574d224a34b2d9a615d6088b28335eeb4af1f`:

- La inscripción de `VClientPrediction001` está en VA `0x1050DB20`, su factoría
  en `0x1022E040` y su tabla virtual en `0x105A41BC`.
- El slot 7 apunta a `0x1017A790`. Su cuerpo de 22 bytes invalida el fotograma
  anterior y limpia cuatro contadores del historial. Se cotejó con dumpbin.
- Corresponde a la operación nativa OnReceivedUncompressedPacket. El runtime
  exige que los 22 bytes coincidan y que pertenezcan a la imagen del cliente
  antes de llamar; si cambia la implementación, no escribe offsets supuestos.

Al soltar se marca el último comando realmente generado. Solo cuando está
confirmado, la cola nativa es cero y las correcciones anteriores están
retiradas se permite **una** llamada al reinicio nativo. La siguiente predicción
normal reconstruye el estado desde la actualización del servidor. No se llama
manualmente a Update en cada fotograma suelto. Mantener otra vez, cambiar de
jugador o limpiar el mapa cancela una recuperación anterior. El reset no es un
paquete de red ni una escritura de posición, reloj del jugador o cooldowns.

El código público de Valve conserva/restaura los relojes globales en Update y
reconstruye el estado original cuando cambia el fotograma de partida. Es una
referencia de flujo, no una prueba de equivalencia completa con L4D2:
[Valve Source SDK prediction.cpp](https://github.com/ValveSoftware/source-sdk-2013/blob/master/src/game/client/prediction.cpp).

La recuperación se registra con `native_history_reset`, último comando, ACK y
RTT. Si la firma no coincide se informa de su indisponibilidad y no se reintenta
cada fotograma. La desaparición de la pausa requiere otra prueba en partida;
no se presenta la hipótesis de historial retenido como una causa demostrada.

Respaldo de fuentes de 3.1.21: `build/rapidfire-recovery-source-3.1.21/`.
Mapa de la tabla y bytes del cliente: `build/prediction-native-vtable.json`.

## Pruebas

Release Win32 compiló y pasaron las **21 suites**, incluidas **202
comprobaciones de rapid fire**, **121 de API nativa**, **25 de persistencia** y
**44 de captura**. Se conserva la ejecución completa de factores altos y boost.
Las regresiones nuevas comprueban recuperación única después de ACK, cola
pendiente, nueva pulsación, cancelación del propietario y los factores 167/334
observados en el registro. La API ejecuta un cuerpo nativo equivalente y
comprueba que solo cambia sus campos de historial; rechaza un slot distinto o
ausente sin ejecutar Update ni escribir en otras partes del objeto.

No había un proceso de L4D2 activo durante esta revisión para reproducir la
escena congelada con la nueva DLL. El mapa de la operación nativa se obtuvo del
binario instalado, no de una sesión nueva en partida.

## Entrega

El 22 de octubre de 2026 se publicó el módulo **3.1.22** mediante el panel
privado de Vercel y la API de licencias vigente. El panel confirmó firma y
descarga real. DLL comprimida: **3 655 168 bytes**, SHA-256
`9c3e666cce4578cc8b42c3dd7b2ea3ad6643e32f292bddb676fb8e54631d7626`.
Carga x86, reubicación, once recursos y secciones tras descompresión pasaron la
validación. Paquete, evidencia filtrada y respaldo de 3.1.21:
`build/rapidfire-recovery-3.1.22/`.

El loader sigue en 3.1.8. Se reinicia L4D2 y se carga otra vez para recibir el
módulo nuevo. El menú ya no incluye stable network; los tres métodos usan su
envío original y mantienen el boost según la preferencia guardada.

Accounts actualizó ambas referencias de módulo desde `893000f`, con sus 35
pruebas correctas. El despliegue de producción
`JEBCkvwRZ39CyYes83fw7HWdtxck` terminó como Ready.
