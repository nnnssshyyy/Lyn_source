# Charger turn de Raze — revisión del 21 de octubre de 2026

La integración posterior está documentada en
[Charger y correcciones de render, 3.1.17](CHARGER_AND_RENDER_FIXES.md).
Este informe conserva el resultado de la revisión estática previa.

**Se identificó y contrastó la rutina real.** Raze permite conservar los ángulos
nuevos de la mira cuando el cliente normalmente los sustituye por los anteriores
durante el estado de vista congelada. Su opción `Charger turn` no contiene un
cálculo de trayectoria curva, selección de objetivo ni modificación de velocidad.
Es un parche reversible de un byte en el código del cliente.

El análisis estático prueba ese cambio de flujo en el cliente instalado. No
prueba que cualquier servidor acepte el giro ni mide una curva o un impacto en
partida. No se integró esta función ni se publicó otra DLL durante esta revisión.

## Binarios y método

Se volvió a abrir Raze con IDA Pro y el MCP local de `mrexodia/ida-pro-mcp`.
Hex-Rays, referencias cruzadas y ensamblador se consultaron de nuevo en las
sesiones `raze-charger-turn` y `l4d2-charger-check`.

| Archivo | SHA-256 |
| --- | --- |
| `C:\Users\User\Desktop\raze\RZL4D2_[unknowncheats.me]_.dll` | `dfa8632e85c89c318577ef619f20bae2f4db5136042d680ee4dae4109234691a` |
| `C:\Program Files (x86)\Steam\steamapps\common\Left 4 Dead 2\left4dead2\bin\client.dll` | `c0a8e1e88f7312db11bcfd4b0ec574d224a34b2d9a615d6088b28335eeb4af1f` |

Los hashes de los ficheros coinciden con los metadatos de ambas bases de IDA.
Las direcciones siguientes son VA sobre base preferida `0x10000000`; el RVA se
indica explícitamente para el punto de parcheo. Se trabajó en copias de las bases
de análisis. No se ejecutó Raze ni se parchearon bytes del cliente o del proyecto.

## Del menú al código que se ejecuta

| Punto | Evidencia |
| --- | --- |
| `0x101744C2` y `0x101744C7` | `Giro charger` y `Charger turn` son traducciones de una misma etiqueta. |
| `0x10009A50` | La clave `bChargerTurn` se carga en el byte `0x103F53CD`. |
| `0x1010C635`–`0x1010C658` | La configuración guarda ese mismo byte con la clave `bChargerTurn`. |
| `0x100F49D6`–`0x100F49FE` | El control del menú recibe la dirección de ese byte. |
| `0x1016C5F0` | El instalador intercepta el método del cliente en `vtable + 0x88`, índice 34. |
| `0x1016C610` / `0x1016C6D8` | El callback de etapas llama primero al original y, en el caso 5, aplica la opción de giro. |
| `0x1017CCA0` | `ChargerTurn_ApplySetting`: encuentra el destino y aplica o retira el parche al cambiar la opción. |

El índice y el despacho son coherentes con `FrameStageNotify` y su etapa de
inicio de render. Esos nombres semánticos son anotaciones del análisis, no
símbolos exportados por Raze.

## Parche y restauración

La instrucción de Raze en `0x1017CCDF` calcula:

```text
destino = GetModuleHandleA("client.dll") + 0x65DB4
```

En `0x1017CD04` llama al gestor `0x1017C8E0` con valor `0xEB` y longitud 1.
El gestor copia primero los bytes originales a un registro de respaldo, cambia
la protección temporalmente, escribe y repone la protección anterior.

Raze compara la opción `0x103F53CD` con el último estado aplicado `0x103F7FD4`.
Así evita repetir la escritura en cada fotograma si la opción no cambió. Al
desactivarla busca el primer respaldo de ese destino con longitud 1 y restaura
el byte original. Después actualiza el último estado aplicado.

Reconstrucción legible de esa secuencia, **no código de integración**:

```cpp
if (!patchSite) {
    auto client = findLoadedClient();
    if (!client) return;
    patchSite = client + 0x65DB4;
}
if (enabled == previouslyApplied) return;
if (enabled)
    registry.backupAndFill(patchSite, 0xEB, 1);
else
    registry.restoreFirstMatching(patchSite, 1);
previouslyApplied = enabled;
```

Hex-Rays muestra `HMODULE += 104301` por el tipo de puntero inferido. El operando
del ensamblador es inequívocamente **`add eax, 65DB4h`**, que es el RVA en bytes.

## Qué cambia en el cliente instalado

El punto cae dentro de la función `0x10065CD0`, identificada estructuralmente como
`C_BasePlayer::CreateMove`. El tramo original comprobado es:

```asm
10065DA4  test byte ptr [esi+0F0h], 20h
10065DAB  jz   10065DEC
10065DAD  cmp  byte ptr [esi+14C8h], 0
10065DB4  jz   10065DE3                    ; bytes 74 2D
10065DB6  fld  dword ptr [esi+14BCh]
          ; copia los tres ángulos anteriores a cmd+0C/10/14
10065DDF  call eax                        ; engine vtable +50h
10065DE1  jmp  10065DF3
10065DE3  mov  byte ptr [esi+14C8h], 1
10065DEA  jmp  10065DF3
10065DEC  mov  byte ptr [esi+14C8h], 0
10065DF3  ; guarda los ángulos actuales como anteriores
```

Raze cambia **`74 2D` a `EB 2D`**. Conserva el desplazamiento del salto y hace
que siempre se salte el bloque que repone los ángulos anteriores cuando se
entra en esa rama de vista congelada. Los ángulos nuevos del comando sobreviven
y se guardan para el siguiente comando.

| Acceso observado | Interpretación por correspondencia estructural |
| --- | --- |
| `player + 0xF0`, máscara `0x20` | Estado que activa el bloqueo de vista en esta compilación. |
| `player + 0x14BC`, `+0x14C0`, `+0x14C4` | Tres componentes de los ángulos anteriores. |
| `player + 0x14C8` | Indicador de vista previamente congelada. |
| `cmd + 0x0C`, `+0x10`, `+0x14` | Pitch, yaw y roll del comando. |
| Llamada de motor por `vtable + 0x50` | Actualización de los ángulos visibles, identificada como `SetViewAngles`. |

La estructura coincide con el tratamiento de vista congelada del
[código público de Valve, `C_BasePlayer::CreateMove`](https://github.com/ValveSoftware/source-sdk-2013/blob/master/src/game/client/c_baseplayer.cpp#L1098).
Los nombres de campos se infieren de esa correspondencia y de los accesos
observados; no se trasladan los valores de flags de otra rama del SDK a L4D2.

La función del cliente no estaba definida en la copia inicial de la base. Se
delimitó desde su prólogo hasta `retn 8` y se exportó completa. El pseudocódigo
automático todavía contiene artefactos de seguimiento de pila en llamadas
virtuales; para los argumentos y offsets la evidencia decisiva es el ensamblador.

## Diferencia con `bAutoTeleportCharger`

La clave `bAutoTeleportCharger` usa otro byte, `0x103F53CB`. Su ruta está en
`0x101472B0`, llamada desde el hook de comandos. El bloque `0x101476A2`–
`0x10147777` comprueba clase de zombi 6 y el handle `m_carryVictim`. Cuando aparece
una víctima transportada y pasa otra condición de motor, escribe los bits
`0x7CF0BDC2`, aproximadamente `1e37f`, en el roll del comando (`cmd + 0x14`). Un
indicador evita repetir la escritura para la misma transición de transporte.

Es una ruta distinta del parche de vista. El nombre de configuración no prueba
por sí solo un teletransporte real, y no se comprobó ese efecto en una partida.

## Alcance y precauciones para una futura adaptación

- El RVA y los bytes originales sí coinciden con el cliente instalado analizado.
  Esto no demuestra compatibilidad con otras versiones del juego.
- La rutina de giro de Raze no comprueba clase de infectado ni estado de carga;
  el parche afecta a ese bloque compartido de vista congelada mientras esté activo.
- Raze usa un destino fijo y no valida una firma del código antes de escribir.
  Sus retornos de protección y restauración tampoco se comprueban en esa ruta.
  Actualiza su indicador incluso si no encontró un respaldo al desactivar.
- Esta rutina no modifica el código del servidor, la velocidad ni los paquetes.
  La aceptación de los ángulos y la trayectoria real requieren una prueba de
  servidor; ZoneMod y Confogl no se validaron en esta revisión.
- Una integración necesitaría resolver y validar el bloque de la compilación
  activa, limitar su aplicación a los estados previstos y restaurar con resultados
  comprobados. El informe no agrega esa función al runtime del proyecto.

## Evidencia guardada

`docs/re/charger-turn/` contiene cadenas, referencias de las opciones, bytes,
pseudocódigo y exportaciones MCP. Las cinco exportaciones completas suman
**572 instrucciones**: 72 del giro, 199 del respaldo/escritura, 181 del despacho,
7 de instalación del hook y 113 de `CreateMove` del cliente.

- [Desensamblado reunido](re/charger-turn/CHARGER_TURN_DISASSEMBLY.asm).
- [Pseudocódigo de la rutina de Raze](re/charger-turn/charger-turn-annotated.cpp).
- [Pseudocódigo del bloque del cliente](re/charger-turn/client-createmove-annotated.cpp).
- [Manifiesto de hashes y comprobaciones](re/charger-turn/evidence-manifest.json).

Bases anotadas guardadas fuera del proyecto:

```text
C:\Users\User\.codex\tools\ida-analysis\raze\raze-charger-turn-annotated-20261021.i64
C:\Users\User\.codex\tools\ida-analysis\l4d2\client-charger-annotated-20261021.i64
```
