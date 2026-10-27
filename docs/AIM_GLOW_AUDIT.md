# Auditoría de aim y native glow — 12 de octubre de 2026

## Fallos corregidos

### 1. Los tres modos de aim podían detenerse antes de seleccionar entidades

`Api::Resolve` usaba el offset de la propiedad de red `movetype` como si fuera
almacenamiento directo. En el `client.dll` instalado, su descriptor tiene offset
**0**: el proxy de recepción (RVA `0x45BB0`) escribe el byte en **entity+0x144**.
El código anterior leía el primer byte de la dirección de la vtable del jugador
y exigía que fuera 2 (`MOVETYPE_WALK`). Ese error afectaba al camino compartido
por normal, silent client y silent server.

Ahora se busca `m_MoveType` en el datamap, validando sus descriptores y rechazando
offsets cero. No se introduce `0x144` como constante del código de producción.
Los fallos anteriores a la selección también actualizan el último diagnóstico.

### 2. Glow escribía valores sin registrar el efecto

Las escrituras directas de tipo/color omitían los callbacks de recepción de
`CGlowProperty`. El proxy de tipo (RVA `0x109AB0`) registra o retira la propiedad
del gestor; el proxy de color actualiza el estado asociado. Escribir los campos
no reproduce esas operaciones.

Ahora se conservan el descriptor, el offset del contenedor y el del campo durante
el recorrido de RecvTables. Las aplicaciones y restauraciones pasan por los
proxies reales de la propiedad, con un `RecvProxyData` de 28 bytes. Se valida el
tipo, los offsets, el módulo del callback y el valor resultante. El contador ya
no cuenta como aplicado un intento fallido; hay contadores separados de errores
y entidades sin soporte.

### 3. Indicadores verdes insuficientes y errores no registrados

Antes, disponer de `ModelInfo`, `EngineTrace` y un hook instalado aparecía como
prueba de funcionamiento. Los indicadores identifican ahora qué interfaz está
disponible y muestran por separado comandos recibidos y último aim aplicado.
El contexto de silent server solo se marca disponible después de comprobar la
pila de una llamada real. Los mensajes de error se registran con límite de
frecuencia para no escribir al disco en cada frame.

## Comprobaciones y límites

- Se leyó la DLL anterior en el proceso de L4D2 sin escribir en su memoria:
  12 087 comandos procesados y contexto de silent server disponible. El estado
  instantáneo era menú abierto/sin foco; no permite confirmar un disparo ni un
  fallo de selección durante ese instante. Confirma que el callback sí recibía
  comandos en esa sesión.
- La inspección del binario instalado confirma el proxy de movimiento y las
  notificaciones del glow descritas arriba.
- Compilación **Release Win32** correcta: `build/Release/free_esp.dll`.
- **31 comprobaciones aprobadas**: 16 de política de aim y 15 de resolución de
  propiedades, datamap, invocación de proxies y restauración. Las 2 suites de
  CTest pasaron. Los callbacks de la prueba son simulados; no ejecutan el motor.
- El proceso del juego dejó de estar disponible durante la inspección. La DLL
  nueva aún requiere comprobar aciertos, glow visible y estabilidad en partida.
  Esta auditoría no certifica ausencia de otros fallos.

Registro de compilación: `build/logs/runtime-fix-build.log`.

Para comprobar la DLL nueva: reiniciar L4D2, cargarla, activar aim y usar la tecla
configurada con el menú cerrado. El panel conserva el último intento. Para glow,
activar `native glow` y revisar aplicados/errores/sin soporte en la misma página.
