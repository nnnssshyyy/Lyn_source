# No spread de Raze: revisión con IDA Pro y MCP

**La ruta identificada como `No Spread` excluye las cuatro escopetas T1/T2.**
Su compensación utiliza `UpdateSpread`, dos muestras de `SharedRandomFloat` y
resta del punch, dentro de un contexto de predicción de movimiento. No contiene
una rutina de agrupación total de perdigones para estas cuatro armas.

## Binario y evidencia

- Archivo analizado: `C:\Users\User\Desktop\raze\RZL4D2_[unknowncheats.me]_.dll`.
- SHA256: `dfa8632e85c89c318577ef619f20bae2f4db5136042d680ee4dae4109234691a`.
- PE32, base preferida `0x10000000`. Las direcciones de este informe son VA.
- Sesión MCP: `raze-nospread`, con Hex-Rays y desensamblado de IDA.
- Base anotada guardada:
  `C:\Users\User\.codex\tools\ida-analysis\raze\raze-nospread-annotated.i64`.
- Se exportaron completas seis funciones centrales, **856 instrucciones**.
  Las páginas de cada exportación están completas y su recuento coincide con
  `total_instructions` de MCP. También se conservaron cadenas, referencias,
  resolutores, tabla de saltos y bytes originales.
- Se trabajó sobre una copia de la base anterior de rapidfire. No se ejecutó
  Raze ni se modificó la DLL del proyecto durante esta revisión.

## Exclusión de escopetas, comprobada en ensamblador

En `0x10026C6B`, `NoSpread_ShouldRun` consulta el método de arma del slot 383
(`vtable + 0x5FC`). Decrementa el ID y despacha mediante la tabla
`0x10185140` en `0x10026C77`.

La tabla tiene 37 entradas. `0x10026C43` devuelve falso con `xor eax,eax`;
`0x10026C45` conserva el `AL=1` asignado antes del salto.

| Arma | Nivel | ID nativo | Destino de la tabla | Resultado |
| --- | --- | ---: | --- | --- |
| Pump shotgun | T1 | 3 | `0x10026C43` | Excluida |
| Chrome shotgun | T1 | 8 | `0x10026C43` | Excluida |
| Auto shotgun | T2 | 4 | `0x10026C43` | Excluida |
| SPAS-12 | T2 | 11 | `0x10026C43` | Excluida |

Los IDs se contrastaron adicionalmente con los cuatro vtables del `client.dll`
local: sus métodos retornan inmediatamente 3, 8, 4 y 11, respectivamente.
Se utilizó lectura PE y desensamblado complementarios para esta comprobación.
El hash del cliente está incluido en el manifiesto de evidencia.

La lista aceptada por Raze es **1, 2, 5, 6, 7, 9, 10, 26, 32–37**.
No se deduce esta exclusión de nombres de menú: se decodificaron los 148 bytes
de la tabla obtenidos mediante MCP. La prueba reproducible está en
[`re/nospread/raze-shotgun-exclusion-proof.json`](re/nospread/raze-shotgun-exclusion-proof.json).

## Recorrido de la función

| Dirección | Nombre anotado | Papel |
| --- | --- | --- |
| `0x10026BB0` | `NoSpread_ShouldRun` | Comprueba botones, estados colgado/lengua y lista de armas. |
| `0x10026D60` | `NoSpread_ApplyShotAngles` | Calcula y resta los offsets, resta punch, normaliza y escribe ángulos. |
| `0x10023180` | `Weapon_CanFireFromTickbase` | Comprueba cooldown de arma y jugador con tiempo de tickbase. |
| `0x1002D2B0` | `NoSpread_PredictionBegin` | Prepara semilla, reloj y movimiento predicho. |
| `0x1002D780` | `NoSpread_PredictionEnd` | Restaura el contexto temporal y limpia la semilla global. |
| `0x10027EA0` | `NoSpread_FixMovement` | Conserva la dirección del movimiento al cambiar los ángulos. |

1. `CreateMove` llama a `PredictionBegin` en `0x10170117`. La preparación usa
   `MD5_PseudoRandom(command_number) & 0x7FFFFFFF` como semilla global. El helper
   `0x1017C4A0` devuelve los cuatro bytes del digest MD5 situados en el offset 6;
   sus constantes iniciales se comprobaron también con MCP.
2. La preparación guarda el reloj, establece tiempo y frame time para un tick
   predicho y ejecuta `SetupMove`, `ProcessMovement` y `FinishMove`. Conserva ese
   contexto hasta la llamada de finalización.
3. En `0x101701CB`, `CreateMove` llama a `NoSpread_ApplyShotAngles`.
   Esta función requiere jugador y arma, menú cerrado, los filtros de
   `ShouldRun` y cooldown listo. El filtro de botones exige ataque primario y
   ausencia de uso: `(buttons & 0x21) == 1`.
4. La opción `bNoSpread` corresponde a `NoSpread_Enabled`, `0x103F4BF2`.
   Con ella activa, guarda el float `weapon + 0xD0C`, llama al `UpdateSpread`
   nativo mediante `0x103EC680` y lee el valor actualizado.
5. Llama a `SharedRandomFloat` mediante el puntero cacheado `0x103EEF30`,
   procedente de `0x103EC694`. Las llamadas están en `0x10026E5E` y
   `0x10026E8F`:

   ```cpp
   pitch -= SharedRandomFloat("CTerrorGun::FireBullet HorizSpread", -spread, spread, 0);
   yaw   -= SharedRandomFloat("CTerrorGun::FireBullet VertSpread",  -spread, spread, 0);
   ```

6. Restaura el spread original en `0x10026EC5`. Consulta el punch del jugador
   mediante `vtable + 0x4DC` en `0x10026ED1` y lo resta en el mismo bloque
   habilitado por no spread. Normaliza yaw y limita pitch a ±89°, corrige el
   movimiento y escribe los ángulos en el comando.
7. La función actualiza indicadores que consume `CreateMove` para gestionar
   los ángulos visibles y el envío según sus modos. Esos indicadores no añaden
   una compensación por perdigón.
8. `CreateMove` llama a `PredictionEnd` en `0x101701D6`. Restaura el reloj,
   limpia el puntero de comando actual y escribe `-1` en la semilla global.

La función central, su filtro y el ajuste de movimiento no reescriben el número
ni la semilla del comando. Tampoco se encontró en la búsqueda de inmediatos
la constante de escopeta de la referencia RijiN, `0x807FF533`; esta búsqueda por
sí sola no descarta constantes calculadas u ofuscadas en otras partes del binario.

## Diferencia relevante con el proyecto

La revisión encontró que el proyecto ya usaba el mismo tipo de muestras y
restauraba temporalmente el campo de spread. Después se adaptó una parte del
contexto: reloj basado en tickbase/intervalo y semilla del comando antes del
updater, con restauración al salir. La función nativa de esta build usa frame
time para interpolar el spread; las pruebas comprueban que la muestra preparada
no varía al cambiar la duración del fotograma.

**No se reproduce la simulación completa de movimiento de Raze.** Sigue
pendiente medir la concordancia con el valor consumido por el disparo durante
predicción en partida. La revisión estática y las pruebas simuladas no garantizan
concordancia con un servidor remoto. El manifiesto de evidencia conserva los
hashes y el estado de los archivos en el momento de la revisión, anterior a esta
adaptación. [Implementación y validación actual](SHOTGUN_CLIENT_SPREAD.md).

Portar esta rutina tal cual no proporciona no spread total para las escopetas:
su propio filtro las rechaza. No se ha verificado aquí otra ruta del binario que
agrupe sus perdigones. El informe describe la ruta vinculada a `bNoSpread` y sus
dependencias, no una prueba de todos los comportamientos posibles de Raze.

## Archivos para revisar

- [`Pseudocódigo normalizado`](re/nospread/RAZE_NOSPREAD_LOGIC.cpp): reconstrucción
  explicativa, separada de las fuentes compiladas del proyecto.
- [`Desensamblado completo`](re/nospread/RAZE_NOSPREAD_DISASSEMBLY.asm).
- [`Pseudocódigo de Hex-Rays`](re/nospread/raze-nospread-core.cpp).
- [`Tabla decodificada y comprobación de las cuatro armas`](re/nospread/raze-shotgun-exclusion-proof.json).
- [`Manifiesto y hashes`](re/nospread/evidence-manifest.json).

Los prototipos y algunas variables inferidos por Hex-Rays alrededor del código
AVX son imprecisos; la tabla, las escrituras, el orden de llamadas y los offsets
descritos se cotejaron con instrucciones, no únicamente con pseudocódigo.
