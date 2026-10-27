# Rapidfire de Raze: análisis con IDA Pro 9.4 y MCP

El rapidfire de esta DLL combina **llamadas adicionales a `CL_Move`, compensación de `m_nTickBase` por comando y cambios en el canal de red**. Expone tres métodos. La revisión del 19 de octubre volvió a comprobar las cinco funciones mediante IDA/MCP e integró en el proyecto su orden de armado/predicción, los argumentos originales de movimiento y la conservación de correcciones al soltar. [Integración y límites](RAZE_RAPIDFIRE_INTEGRATION.md).

## Binario y alcance

- Original: `C:\Users\User\Desktop\raze\RZL4D2_[unknowncheats.me]_.dll`.
- SHA-256: `dfa8632e85c89c318577ef619f20bae2f4db5136042d680ee4dae4109234691a`.
- PE32, base preferida `0x10000000`. Todas las direcciones siguientes son VA de esa base; para obtener RVA se resta `0x10000000`.
- Análisis estático mediante el servidor local `ida-pro-mcp`, sesión `raze-rapid`, Hex-Rays y desensamblado. No se ejecutó ni se inyectó Raze.
- Base anotada: `C:\Users\User\.codex\tools\ida-analysis\raze\raze-rapidfire-annotated.i64`. Conserva las anotaciones anteriores de ChatSpy.

Se siguieron las cadenas de configuración hasta sus globales, las referencias a esos globales, los instaladores de hooks y los consumidores del contador y del historial de predicción. Se exportaron completas las cinco funciones principales: **1432 instrucciones**, comprobando cada página contra el total de IDA. Las ramas decisivas se cotejaron con ensamblador; no dependen únicamente del pseudocódigo de Hex-Rays.

## Mapa de funciones

| VA | Nombre asignado en IDA | Papel confirmado |
| --- | --- | --- |
| `0x100F4F40` | `RapidFire_DrawSettings` | Habilitación, tecla, selector de tres métodos y factores. |
| `0x101486E0` | `RapidFire_ArmExtraMoves` | Arma el contador compartido y la corrección de predicción; también contiene otros exploits. |
| `0x10148C40` | `RapidFire_ApplyNetChannelMode` | Escrituras específicas de cada método en el canal de red. |
| `0x1016CE80` | `RapidFire_Hook_CL_Move` | Repite la función original mientras quedan extras. |
| `0x1016CF10` | `RapidFire_Hook_RunCommand` | Resta la corrección por comando a `m_nTickBase` antes del original. |
| `0x1016FF00` | `RapidFire_Hook_CreateMove` | Limpia el slot, arma extras, actualiza predicción y aplica el método de red/envío. |

Los instaladores confirman los destinos: `0x1016CE60` engancha `CL_Move`, resuelto por la firma etiquetada en `0x10001370`; `0x1016CEF0` engancha el slot 18 de `VClientPrediction001`; `0x1016FEE0` engancha el slot 27 del objeto de ClientMode.

## Flujo común

1. El hook de `CL_Move` pone el contador compartido a `-1` antes de la llamada inicial. Llama al original con los mismos argumentos recibidos.
2. En `CreateMove`, Raze limpia la corrección del comando actual: `ring[command_number % 150] = 0`. Cada entrada ocupa ocho bytes; aquí se utiliza el primer entero.
3. Si existe jugador local y el contador sigue en `-1`, la función de armado comprueba habilitación, método y tecla. Cuando la tecla está pulsada calcula `N`, guarda `N` en el contador y en el slot del comando, y activa el indicador compartido de envío. El cálculo de rapidfire usa:

   ```cpp
   N = max(configured_speed, int(0.06f / interval_per_tick + 0.5f));
   N = max(N, 0);
   ```

   No exige `IN_ATTACK` en ese bloque. También incrementa y acota un entero en `local_player + 0x10`; cuando se suelta la tecla lo decrementa hasta cero. Ese campo no se identificó como un netvar y se conserva como campo opaco en el pseudocódigo.

4. `CreateMove` llama a `Prediction::Update` con delta tick, último comando reconocido y outgoing + choked. Después aplica la variante de red del método seleccionado.
5. Al salir de cada llamada original de `CL_Move`, el hook decrementa el contador y vuelve a llamar al original si el valor anterior era positivo. Armar `N` produce **una llamada base y N llamadas extra**. Los argumentos `samples` y `final` se repiten; ese hook no reserva `final=true` únicamente para la última llamada.
6. El hook de `RunCommand` obtiene el desplazamiento por `command_number % 150`, resuelve `m_nTickBase` por netvars y ejecuta `local.tickbase -= shift` antes de llamar al original. La resta está confirmada en `0x1016CFD3`.
7. Al final de `CreateMove`, si el indicador compartido está activo y otro indicador de supresión no lo impide, escribe `sendPacket = true` (`0x10170708`). El indicador también lo usan otras funciones; no es exclusivo de rapidfire.

En estas funciones principales no hay un banco de crédito ganado omitiendo ticks en reposo. Tampoco hay una escritura directa a daño del arma ni un reinicio de `m_flNextPrimaryAttack`. La generación de comandos y el estado de predicción no bastan por sí solos para demostrar qué aceptará el servidor.

## Los tres métodos

Los índices internos son 0, 1 y 2; el menú los muestra como `Methot 1`, `Methot 2`, `Methot 3`.

| Menú / índice | Factor que arma N | Escrituras de red |
| --- | --- | --- |
| Methot 1 / 0, Storm | `iFactorSpeedRapidFireStorm`, UI 1–200 | Con tecla pulsada, `iFactorRapidFireStorm > 0` y corrección del comando anterior igual a cero: `channel + 0x10 = -1`, `channel + 0x1C = 255`. |
| Methot 2 / 1 | `iFactorSpeedMethod1RapidFire`, UI 1–200 | Con tecla pulsada y canal disponible: `channel + 0x1C = iFactorTickMethod1RapidFire`, UI 1–255. Activa envío. |
| Methot 3 / 2 | `iFactorSpeedMethod2RapidFire`, UI 1–200 | Con tecla pulsada: `channel + 0x10 = -1`, `channel + 0x1C = 255`. Activa envío. |

Storm expone además `iFactorRapidFireStorm` entre 1 y 100. No es correcto describirlo simplemente como el número de extras: el número de extras viene del otro factor. En la función de red aparece como condición positiva y dentro de una rama que compara el índice del comando actual con el del anterior.

### Particularidad de Storm

El ensamblador de `0x10148D49–0x10148DDA` calcula `current = command % 150` y `previous = (command - 1) % 150`. Luego compara ambos índices (`0x10148D7F`). Para comandos positivos ordinarios no son iguales. La rama de igualdad que limpia el slot y recalcula el contador con `max(factor, int(9.0f / interval + 9.0f))` no se alcanza en ese caso; se ejecuta el incremento del contador auxiliar `0x103F68E8`.

Se preserva esta condición tal como está en el binario. No se reinterpretó como recarga, comparación de tickbase ni mecanismo funcional de alternancia. La escritura de red posterior sí consulta la corrección del slot anterior. Por ello tampoco debe describirse Storm como una escritura incondicional de 255 en todos los extras.

## Qué representan los campos de red

Se cotejó con la copia del `engine.dll` instalado, SHA-256 `8a6d468059394bf4cfce32f11dcba12ef45de88da1373084d5a2916d0bf7ba86`:

- `CNetChan::SetChoked`, slot 46, `0x101B18F0`, incrementa `channel + 0x08` y `channel + 0x1C`.
- `CNetChan::SendDatagram`, `0x101B9B30`, escribe las secuencias de `+0x08` y `+0x0C`. Si `+0x1C > 0`, activa el bit `0x10` de flags y escribe sus ocho bits bajos en la cabecera (`0x101B9D5C–0x101B9D74`). Después limpia `+0x1C` e incrementa `+0x08`.
- Los accesores de secuencias `0x101B1350` y `0x101B1370` leen/escriben el trío `+0x08`, `+0x0C`, `+0x10`. El último se interpreta como la secuencia saliente reconocida (`out_sequence_ack`); esta denominación es una identificación del campo, no un símbolo recuperado del binario.

Por tanto, **255 en `channel + 0x1C` es el contador de paquetes retenidos declarado en la cabecera**, no 255 comandos nuevos, 255 disparos ni daño multiplicado. Es distinto de `clientstate.choked_commands`. El serializador de movimiento instalado sigue limitando cada mensaje a 15 comandos nuevos.

Raze usa en una ruta `*(engine_base + 0x4268EC) + 0x18` para encontrar el canal. La ruta por firma usa el estado raíz y su `+0x18`. Nuestro accessor validado devuelve `raíz + 8`, por lo que el mismo canal está en `+0x10` desde esa vista. La diferencia de ocho bytes también explica los offsets de outgoing/choked/ACK del pseudocódigo de Raze frente a los del proyecto. No hay que sumar ambos ajustes.

## Configuración recuperada

| VA | Clave persistida |
| --- | --- |
| `0x103F5404` | `bEnableRapidFire` |
| `0x103F5408` | `iKeyRapidFire` |
| `0x103F540C` | `iMethodRapidFire` |
| `0x103F5410` | `iFactorSpeedRapidFireStorm` |
| `0x103F5414` | `iFactorRapidFireStorm` |
| `0x103F5418` | `iFactorSpeedMethod1RapidFire` |
| `0x103F541C` | `iFactorTickMethod1RapidFire` |
| `0x103F5420` | `iFactorSpeedMethod2RapidFire` |

Estado compartido: contador de extras `0x103F6DBC`, correcciones por comando `0x103F690C`, indicador de envío `0x103F6908`. No son offsets transportables a otra DLL.

## Implicación para nuestro rapidfire

La versión 3.1.7 repetía el movimiento mientras se mantenía la tecla, corregía la predicción por comando y alteraba el estado declarado del canal. La revisión nueva encontró diferencias concretas: el runtime sustituía los argumentos de las llamadas extra, ejecutaba `CL_RunPrediction` antes de armar y borraba las correcciones al soltar. Esas tres diferencias se corrigieron con la secuencia confirmada de Raze.

El análisis identifica piezas que faltaban en la comparación anterior: especialmente el hook de `RunCommand` y la diferencia entre comandos retenidos y paquetes retenidos. No demuestra que Raze ni la implementación actual produzcan daño adicional en los servidores donde se informó el fallo. Eso requiere ejecución con seguimiento de comandos y de la salud del objetivo; un ACK de comandos o una animación de disparo no equivale a daño confirmado.

Las cinco funciones originales y sus 1432 instrucciones están exportadas nuevamente en `re/rapidfire/recheck-20261019/`. El hook final de comandos del proyecto sigue adaptando el ABI propio del cliente instalado. El acceso opaco a `local + 0x10` de Raze conserva una identificación pendiente y queda fuera del port. El daño y la estabilidad en partida siguen sin verificarse.

## Archivos de evidencia

- `re/rapidfire/RAZE_RAPIDFIRE_PSEUDOCODE.cpp`: reconstrucción legible y comentada, no código integrado en el runtime.
- `re/rapidfire/RAZE_RAPIDFIRE_DISASSEMBLY.asm`: cinco funciones completas con VA.
- `re/rapidfire/raze-evidence-manifest.json`: identidad y conteos de instrucciones.
- `re/rapidfire/raze-strings.json`, `raze-string-xrefs.json`, `raze-config-block.json`, `raze-global-xrefs.json`: conexión configuración → globals → funciones.
- `re/rapidfire/raze-critical-asm.json`, `raze-prediction-asm.json`: ramas y escrituras cotejadas.
- `re/rapidfire/raze-*-install.json`, `raze-clmove-signature.json`, `raze-prediction-interface.json`: destinos de hooks.
- `re/rapidfire/engine-senddatagram.json`, `engine-net-slot-46.json`, `engine-net-sequences-*.json`: cotejo del canal en el motor instalado.
- `re/rapidfire/raze-renames.json`, `raze-comments.json`, `raze-save.json`: anotaciones y guardado de la base.
