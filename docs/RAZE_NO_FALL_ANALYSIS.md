# No fall damage de Raze: análisis estático

## Resultado

En la DLL disponible se identificaron el control **Anti fall damage**, su
bandera `bFallDamage` y el bind `iKeyFallDamage`. **No se encontró una rutina
de juego conectada a esas opciones.** Los consumidores comprobados pertenecen
a carga/guardado, reset y construcción del menú. No hay evidencia para afirmar
que esta copia active automáticamente el lag exploit al caer.

Sí se extrajo el algoritmo de **lag manual V1 y V2**, que cambia la secuencia
de salida del canal. Es un comportamiento distinto cuya función no consulta
la bandera anti-fall ni comprueba una caída. La adaptación automática que se
implementó en VS3 3.1.28/3.1.29 es propia; este análisis no la valida como una
reconstrucción del anti-fall de Raze ni demuestra que suprima el daño.

## Identidad y método

Binario: `C:/Users/User/Desktop/raze/RZL4D2_[unknowncheats.me]_.dll`.
Tamaño: 4 190 720 bytes. SHA-256:
`dfa8632e85c89c318577ef619f20bae2f4db5136042d680ee4dae4109234691a`.
ImageBase preferida `0x10000000`; las direcciones de este documento son VA.
RVA = VA menos ImageBase. Se verificó que el fichero original usado por IDA
coincide byte a byte con esta DLL, y que los 0x163 bytes de la función de lag
recuperados de la base IDA coinciden con el PE original.

Se usaron IDA/Hex-Rays, lectura del PE y un segundo desensamblador (Capstone
5.0.9 instalado únicamente bajo el directorio de análisis). El segundo barrido
lineal procesó 429 036 instrucciones y revisó accesos con anchura solapada y
reubicaciones. La DLL no se ejecutó ni se inyectó y no se modificó el juego.
Los fuentes de producción y la entrega 3.1.29 se conservaron.

## Trazado del control

| Elemento | Dirección | Evidencia |
| --- | --- | --- |
| Bandera anti-fall | `0x103F53A5` | Un byte, separado de addon bypass |
| Tecla anti-fall | `0x103F53A8` | Entero de configuración |
| Carga del bool | `0x10009746` | Escribe el resultado de leer `bFallDamage` |
| Carga de tecla | `0x1000978C` | Escribe el resultado de leer `iKeyFallDamage` |
| Menú | `0x100F4715`, `0x100F471C` | Captura punteros al bool y al bind |
| Guardado del bool | `0x1010C163` | Lee el byte para serializarlo |
| Guardado de tecla | `0x1010C1D4` | Lee el entero para serializarlo |
| Reset general | `0x100E8A18` | Escribe cero en la palabra que incluye el bool |

La constante de texto en `0x10196390`, completada por el constructor del menú,
produce `Anti fall damage`. El widget `0x100630B0` conserva los punteros en
un callback genérico de checkbox. Su vtable `0x101880B4` lleva al thunk
`0x10069850` y al cuerpo `0x10069910`, que reconstruye el widget. El callback
personalizado de esa llamada está a cero: no se encontró un efecto de juego
oculto en el callback del control.

Las tres reubicaciones que contienen exactamente la dirección `0x103F53A5`
son RVA `0x9747`, `0xF4718` y `0x10C166`: carga, menú y guardado. El barrido de
operandos añade la escritura de reset de dos bytes `0x103F53A4`; no añade una
lectura de gameplay, ni siquiera de anchura mayor que un byte.

## La bandera vecina no es no-fall

El byte `0x103F53A4` corresponde a **bAddonBypass**. IDA agrupa ambos bytes
como `word_103F53A4`, lo que puede inducir una atribución equivocada. La rutina
`0x1017CD70` lee el **byte bajo** (addon bypass), no el alto de anti-fall.
Su parche de engine no demuestra lógica anti-caída y no debe reutilizarse
como supuesto no fall damage.

## Algoritmo de lag comprobado

La función `0x10146F50` se llama desde CreateMove en `0x101701BF`.

- **V1:** con `bLagExploitV1 == 1` y su tecla mantenida, obtiene el jugador.
  Si existe y `m_lifeState == 0`, suma el factor a `channel + 0x08`, secuencia
  de salida (`0x10146FF1`). Con jugador existente solicita `ForceSend = 1`
  (`0x10146FF4`).
- **V2:** con `bLagExploitV2 == 1` y su tecla mantenida, comprueba que la
  consulta del canal devuelva un puntero. Si existe, suma el factor al campo
  `+0x08` del canal accesible desde `*(0x103EC9A8) + 0x18`
  (`0x1014703E`). Si no existe, solicita ForceSend (`0x10147043`).
- La política compartida solicita envío cuando ForceSend es 1 y la supresión
  de otro feature está a 0; la escritura de sendPacket está en `0x10170708`.
- El flujo de free camera contiene otras dos sumas de V1. Su activación pasa
  por `bFreeCamActive`, no por `bFallDamage`.

Ninguna de esas ramas lee `iKeyFallDamage` o `bFallDamage`, ni comprueba suelo,
velocidad vertical, proximidad de impacto o daño de caída. No se deduce una
prevención de daño a partir de alterar únicamente la secuencia de red.

## Entregables y límites

El pseudocódigo anotado está en [re/raze_no_fall.pseudo.cpp](re/raze_no_fall.pseudo.cpp).
Las salidas crudas de IDA, ensamblador, operandos, reubicaciones e identidad
están en `build/raze-no-fall-analysis/`.

La conclusión es estática y específica de este hash. No prueba una ausencia
absoluta de accesos calculados dinámicamente o de otra versión del programa;
sí documenta que no se identificó un algoritmo anti-fall enlazado al control
en la copia analizada. No se inventó una rutina para completar ese vacío.
