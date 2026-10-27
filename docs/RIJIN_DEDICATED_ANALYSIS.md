# RijiN en servidores dedicados: revisión de las rutas de L4D2

Revisión estática del 17 de octubre de 2026 sobre
`C:\Users\User\Downloads\RijiN-main\RijiN-main`.

**La diferencia más relevante es el estado usado para construir el disparo:**
RijiN ejecuta predicción nativa antes del aimbot y la compensación, selecciona
registros del objetivo teniendo en cuenta latencia/interpolación y coordina
cuándo envía los comandos. El código expresa esos mecanismos; no demuestra que
todas sus funciones produzcan el resultado esperado en un dedicado concreto.

La búsqueda de los nombres `zonemod` y `confogl` en el texto accesible a `rg` de
`src`, `configs` y `tests` no encontró coincidencias. La ruta estudiada utiliza
mecanismos generales de Source. No se compiló ni ejecutó RijiN y no se conectó
a ningún servidor. Se conservaron hashes de 25 fuentes y copias de seis fuentes
oficiales del entorno competitivo en el
[manifiesto](re/rijin-dedicated/manifest.json).

**Orden real del comando.** En
[CreateMove](<C:/Users/User/Downloads/RijiN-main/RijiN-main/src/cheats/left4dead2/hooks.cpp:3>)
se guardan entrada y estado local, se inicia el control de paquetes e
interpolación, se ejecutan movimiento/speedhack y después `start_prediction`.
Dentro del contexto resultante se capturan huesos, se ejecuta el aimbot y se
aplican recoil/spread. Al terminar se limitan ángulos, se conserva la dirección
del movimiento, se decide el tick del objetivo y se finaliza la predicción.

| Pieza | Qué hace la fuente | Alcance para un dedicado |
| --- | --- | --- |
| Predicción | `CL_RunPrediction`, MoveHelper y `CPrediction::RunCommand` antes de seleccionar y corregir el disparo. | Permite consultar posición, reloj y estado del arma tras simular el comando local. La concordancia con el servidor requiere medición. |
| Aimbot e historial | Usa poses históricas válidas y un tick derivado de simulation time más interpolación. | Compensa el desfase temporal del objetivo dentro de su ventana de validez. |
| Paquetes | Gestiona `bSendPacket`, límites locales de retención y envío del comando actual/siguiente. | Influye en cuándo llega la entrada; por sí solo no confirma que haya disparo o daño. |
| No spread | Lee spread después de la predicción y resta dos muestras aleatorias; para escopetas cambia la identidad del comando. | Depende de que el cálculo remoto siga las mismas reglas. El patrón de perdigones de ZoneMod es una dependencia distinta. |
| Speedhack | Genera comandos adicionales, coordina su envío y ajusta tickbase durante predicción local. | La cantidad solicitada localmente no prueba la cantidad que simula el servidor. |
| ESP y cámara | Utilizan entidades, huesos y renderizado locales. | Su presentación no valida la lógica de impactos ni crea información que el servidor no haya enviado. |

**Predicción nativa y restauración.** La implementación heredada de
[start_prediction](<C:/Users/User/Downloads/RijiN-main/RijiN-main/src/base/cheats/source/managers/engine_prediction.cpp:3>)
activa los indicadores de predicción, actualiza comandos pendientes, asigna
MoveHelper al jugador y ejecuta `RunCommand`. Después conserva el tickbase
predicho, repone el tickbase local anterior e instala la semilla del comando.
[end_prediction](<C:/Users/User/Downloads/RijiN-main/RijiN-main/src/base/cheats/source/managers/engine_prediction.h:113>)
restaura el reloj global guardado y la semilla anterior.
El hook de `PostThink` omite esa llamada cuando procede de su ejecución propia
de `RunCommand`. La restauración explícita observada no es una copia integral
de todas las variables del jugador y arma.

Nuestro
[muestreo actual](<C:/Users/User/Desktop/projectos c++/free/src/game.cpp:614>)
prepara tickbase, intervalo y semilla únicamente alrededor de `UpdateSpread`.
Todavía no ejecuta esa predicción completa antes de seleccionar objetivo y
calcular el disparo. Es una diferencia concreta para investigar con trazas de
posición de disparo, velocidad, flags, cooldown y spread antes/después de
predicción; no una garantía automática de mayor precisión al copiarla.

**Escopetas: qué cambia realmente RijiN.** En
[misc::nospread](<C:/Users/User/Downloads/RijiN-main/RijiN-main/src/cheats/left4dead2/misc.h:4>)
comprueba que el arma tenga spread y que éste sea positivo. Para una escopeta
asigna al número del comando el valor de 32 bits `0x807FF533` y recalcula su
semilla mediante `MD5_PseudoRandom` con máscara de 31 bits. Instala esa semilla
global y resta las muestras `HorizSpread` y `VertSpread` a pitch/yaw.
La función no contiene un bucle que corrija individualmente cada perdigón.

La compatibilidad de ese cambio debe analizarse junto al
[hook de GetUserCmd](<C:/Users/User/Downloads/RijiN-main/RijiN-main/src/cheats/left4dead2/hooks.cpp:352>),
que devuelve el elemento por secuencia del anillo de 150 comandos para el slot
principal. Además,
[fix_nospread_pred](<C:/Users/User/Downloads/RijiN-main/RijiN-main/src/cheats/left4dead2/misc.h:39>)
guarda/restaura spread cuando se repredice un comando. Ese respaldo es un único
float estático, sin clave de arma/comando; no conviene tratarlo como un sistema
completo de snapshots. La corrección se invoca desde el hook natural de
`RunCommand` cuando la ejecución no procede de su predicción propia.

**ZoneMod cambia la comparación.** La revisión se contrastó con el commit
`3fd5dcb9801d0c401a7b3f2fc7196b71a97b5044` de Competitive Rework, fechado el
15 de octubre de 2026. Su
[lista de plugins de ZoneMod](https://github.com/SirPlease/L4D2-Competitive-Rework/blob/3fd5dcb9801d0c401a7b3f2fc7196b71a97b5044/cfg/cfgogl/zonemod/shared_plugins.cfg#L107)
carga atributos de armas y dispersión estática. El
[plugin de dispersión](https://github.com/SirPlease/L4D2-Competitive-Rework/blob/3fd5dcb9801d0c401a7b3f2fc7196b71a97b5044/addons/sourcemod/scripting/l4d2_static_shotgun_spread.sp)
reemplaza el RNG de los perdigones por factores estáticos y configura anillos y
perdigón central. **Inferencia:** elegir una semilla especial en el cliente no
elimina un anillo generado por ese cálculo remoto independiente.

La
[configuración revisada](https://github.com/SirPlease/L4D2-Competitive-Rework/blob/3fd5dcb9801d0c401a7b3f2fc7196b71a97b5044/cfg/cfgogl/zonemod/shared_settings.cfg#L234)
también sustituye Auto por Pump y SPAS por Chrome en las reglas de armas;
ajusta pellets y dispersión de las T1; y fija límites de interpolación.
Por tanto hay que registrar el arma realmente equipada y la configuración
efectiva. Confogl admite distintas configuraciones y un servidor puede tener
plugins propios: esta copia oficial no identifica la instalación del usuario.

**Sincronización del objetivo.** El
[aimbot](<C:/Users/User/Downloads/RijiN-main/RijiN-main/src/cheats/left4dead2/features/raytrace_aimbot.cpp:282>)
selecciona el tick del registro y puede retener el paquete para su modo silent.
La
[validación temporal](<C:/Users/User/Downloads/RijiN-main/RijiN-main/src/cheats/left4dead2/features/lag_compensation.cpp:5>)
combina latencias de entrada/salida e interpolación, y compara con el tiempo
del tickbase predicho. El registro requiere una diferencia inferior a 200 ms
menos un tick. El gestor de interpolación limita el ratio según las cvars del
servidor y centraliza la escritura del tick del comando.
Nuestro aimbot ya calcula latencia/interpolación y filtra historial; no falta
toda esa capa. La diferencia a medir es usar un contexto temporal coherente
con el comando predicho en las distintas etapas.

**Speedhack y disparos visuales.**
[speedhack](<C:/Users/User/Downloads/RijiN-main/RijiN-main/src/cheats/left4dead2/misc.h:162>)
vacía primero un lote retenido, ejecuta predicción y solicita entre 2 y 21
repeticiones modificando el retorno del movimiento. El
[gestor de paquetes](<C:/Users/User/Downloads/RijiN-main/RijiN-main/src/cheats/left4dead2/features/packet_manager.cpp:5>)
tiene un límite local fijo de 14 para retención; no lee ahí un presupuesto
negociado con el dedicado. El
[hook de RunCommand](<C:/Users/User/Downloads/RijiN-main/RijiN-main/src/cheats/left4dead2/features/tickbase_correction.cpp:5>)
resta un tickbase al detectar otra ejecución en el mismo tick local.

Un detalle directamente relacionado con el problema de disparos sin daño:
[can_fire](<C:/Users/User/Downloads/RijiN-main/RijiN-main/src/cheats/left4dead2/structs/base_player.cpp:8>)
da permiso local durante speedhack aunque el cooldown consultado todavía no
haya vencido. Eso permite al código de aim considerar que disparará; no aporta
una confirmación remota de que el arma pudo disparar. No se encontró una ruta
separada de rapidfire activa que pruebe la cadencia real de todas las armas.

**Código presente que no está conectado en el loader suministrado.**
La inscripción de
[CL_SendMove](<C:/Users/User/Downloads/RijiN-main/RijiN-main/src/loader/loader_stub/load_procedures/c_procedure_l4d2.h:210>)
está comentada, aunque exista una función que construye hasta 15 comandos
nuevos y 7 de respaldo. También están comentadas las inscripciones de
[RunSimulation](<C:/Users/User/Downloads/RijiN-main/RijiN-main/src/loader/loader_stub/load_procedures/c_procedure_l4d2.h:158>)
y
[AdjustPlayerTimeBase](<C:/Users/User/Downloads/RijiN-main/RijiN-main/src/loader/loader_stub/load_procedures/c_procedure_l4d2.h:260>),
la llamada a `add_record` del speedhack y los bloques que aplicarían esos
registros en los hooks. Su existencia no demuestra que expliquen el
comportamiento de esta versión. Las referencias a `server.dll` corresponden al
proceso local, no a escritura de memoria en un dedicado remoto.

El orden útil para continuar en nuestro proyecto es medir primero predicción y
estado de disparo; después contrastar tick del objetivo e historial; y separar
los comandos generados/enviados/confirmados de los eventos reales de disparo y
daño. Para escopetas se necesita además conocer qué cálculo de perdigones usa
el servidor. Esa validación debe comparar un dedicado base y uno con la
configuración exacta, manteniendo constantes arma, posición y movimiento.

La entrega de esta revisión es documental. Los fuentes de runtime y la DLL
conservan los hashes registrados; no se incorporó código de RijiN. La DLL sigue
siendo la compilación anterior de compensación por tick, sin validación nueva
en partida.
