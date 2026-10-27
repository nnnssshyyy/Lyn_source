# Lyn — Left 4 Dead 2 x86

Código fuente de la DLL, el loader, las pruebas y las dependencias. Las dependencias de C++ están incluidas; no necesitas descargar submódulos.

## Descargar y compilar

Usa **Code → Download ZIP** o clona el repositorio. En Windows, instala Visual Studio 2022 o posterior con **Desarrollo para el escritorio con C++**, CMake 3.24 o posterior y el SDK de .NET 8. Desde PowerShell:

```powershell
git clone https://github.com/nnnssshyyy/Lyn_source.git
cd Lyn_source
.\build.ps1
.\loader\build-loader.ps1 -NoPackageCompression
```

Resultados: `build/Release/lyn.dll` y `build/loader/Lyn.Loader.exe`. Consulta **[BUILDING.md](BUILDING.md)** para las pruebas, la versión compacta y el modo de desarrollo.

También puedes descargar los binarios desde **[Releases](https://github.com/nnnssshyyy/Lyn_source/releases)** o desde los artefactos de **[Actions → Build Lyn](https://github.com/nnnssshyyy/Lyn_source/actions/workflows/build-lyn.yml)**.

La web y sus herramientas de publicación no forman parte de esta copia. El loader conserva la autenticación y la licencia del servicio. Las notas siguientes documentan el funcionamiento y el historial del módulo.

DLL interna para L4D2 de 32 bits, con ESP propio y la interfaz original de Velocity adaptada a DX9.

## Uso

**Loader C#:** `loader/build-loader.ps1` genera `build/loader/Lyn.Loader.exe` y copia `lyn.dll` a la carpeta de entrega. El ejecutable se puede distribuir solo: obtiene el módulo firmado tras validar la licencia. Incluye detección de L4D2 x86, apertura y carga explícita, y diseño VS 3. [Instrucciones del loader](loader/README.md).

**Licencias:** registro de usuario y activación por clave en [VS 3 / cuenta](https://vs3-accounts-n7q4.rockyknoll128.chatgpt.site). El servidor firma una autorización temporal y el plan de hooks; el loader se cierra cuando el módulo confirma la activación de la sesión. [Funcionamiento y validación](docs/VS3_LICENSES.md).

**Descarga:** [Accounts / VS 3](https://vs3-accounts.vercel.app/descarga). El loader se descarga desde Accounts con un nombre aleatorio de letras y números, con mayúsculas y minúsculas. El [panel de licencias](https://vs3-admin.vercel.app) conserva su acceso privado. El código de la web no se incluye en este repositorio. Las próximas publicaciones de módulos van a la VPS; no se vuelve a desplegar en el portal privado antiguo de Sites.

1. Ejecuta `build/loader/Lyn.Loader.exe`, inicia sesión y activa tu clave. Pulsa el botón para abrir L4D2 x86 si hace falta, esperar al menú principal y cargar el módulo; el loader se cierra después de la confirmación.
2. **Insert** abre/cierra el menú; **F6** alterna el ESP de personajes con el menú cerrado.
3. El menú muestra seis secciones con nombre: **Combate** (Aimbot, Armas), **Filtros**, **Jugadores**, **Visuales** (ESP, radar, espectadores, chat), **Varios** (movimiento, cámara, otros) y **Configs**. La búsqueda lleva a la sección correcta; Configs separa buscar, crear, guardar y cargar perfiles, con confirmaciones al borrar o restablecer.
4. Las funciones principales empiezan desactivadas. Para apuntar, activa `enabled`, elige `normal`, `silent client` o `silent server`, cierra el menú y mantén **Mouse 5**. `auto shoot` viene marcado en perfiles nuevos; con él desmarcado debes disparar manualmente en silent. También puedes elegir `on attack` o `always`. Trigger usa **Mouse 4**.
5. **Hitbox Selection** permite pulsar las zonas del Hunter: clic izquierdo avanza entre Ignore / First / Second / Third / Fourth; clic derecho retrocede. El clic activa **custom zones**. El aimbot busca las zonas habilitadas en ese orden, con desempate anatómico; si no ve una zona pasa a la siguiente. Los cambios se guardan con el perfil. Auto sigue eligiendo según arma y objetivo. El dibujo ilustra los grupos: las hitboxes usadas en partida son las del motor.
6. **ESP Preview** muestra las cajas normales/esquinas, grosor, nombre, barra de salud, distancia y esqueleto según las opciones. Usa un Hunter de ejemplo con 250 HP y 15 m; permanece visible aunque ESP o la categoría estén apagados. Glow (borde por silueta con el color elegido) y color de modelos también se reflejan en la vista previa.
7. En **Varios → Cámara**, `third person` utiliza `centered behind player` por defecto y **F5** (rebindeable en `third person key`) lo activa/desactiva con el menú cerrado. Desmarca `centered behind player` para ajustar hombro/altura. La cámara converge con el rayo del arma para que la mira central coincida con el impacto real.
8. Mantén tu tecla de salto para auto bunny hop y auto strafe. El strafe calcula el empuje de mayor ganancia aérea; con directional activado, WASD elige el rumbo, y apagado sigue la cámara. Las teclas ya no pausan la ayuda. Desde reposo puede añadir avance si solo mantienes salto. [Comportamiento y validación](docs/AIR_MOVEMENT.md).
9. En **Varios → Otros**: **speedhack** genera comandos nuevos de movimiento mientras mantienes la tecla (Shift por defecto), con multiplicador de 1–15x; 1x es la velocidad normal. Funciona independientemente del aimbot y pausa al abrir el menú, perder el foco, morir, quedar atrapado o entrar en modo fantasma. **speed key = none** (Esc) lo deja activo mientras el jugador pueda moverse. El menú muestra el estado y los ticks adicionales generados; los límites de procesamiento del servidor pueden reducir o anular la aceleración, tanto en una partida alojada como en un servidor ajeno. **airstuck** congela con `movetype` 0 y **lag exploit** empuja la secuencia del canal. Los colores del glow nativo se eligen en **Jugadores**.
10. Activa **ESP (F6)** y **skeleton** en **Jugadores** para dibujar huesos sobre personajes. **skeleton width** ajusta el grosor (0.5–4 px) en partida y en la vista previa. Funciona con las cajas desactivadas y respeta las categorías y el alcance del ESP. La casilla y el grosor se guardan en el perfil.
11. **Visuales → Radar / espectadores** reúne el radar y la lista de espectadores. El radar tiene alcance horizontal de 5–200 m, tamaño de 160–400 px, rotación con la vista o norte fijo, filtros propios y marcas de altura. Funciona con ESP apagado y con objetivos fuera de pantalla. Arrastra su título con el menú abierto; guarda la posición por perfil y se oculta fuera de partida con el menú cerrado.
12. **Combate → Armas** ofrece **50 perfiles** para armas de fuego, pistolas dobles, cuerpo a cuerpo, arrojables y objetos equipables. Las armas hitscan permiten personalizar aim, modo, FOV, alcance, smoothing, hitbox, recoil, spread, trigger y auto pistol; todos los elementos permiten offsets, ángulos y FOV de viewmodel. Cada override empieza apagado y hereda los ajustes globales; al equipar otra arma se aplica su perfil. El aimbot conserva su interruptor principal y tecla globales. Hay botones para seleccionar el arma equipada, copiar los ajustes globales y restablecer un arma. Las ocho imágenes distintas adjuntas se integran en la DLL; la Uzi repetida usa un único recurso.
13. **Visuales → Chat spy** muestra el chat recibido del otro equipo en un panel desplazable y, opcionalmente, en el HUD del juego. Activa **chat spy** y elige **show panel** / **show in game HUD**. Incluye filtro de duplicados recientes, prefijos de equipo, desplazamiento automático, ocultación del panel vacío y limpieza manual o periódica (0–240 minutos; 0 desactiva el temporizador). Conserva hasta 128 mensajes, se vacía al cambiar de mapa y guarda opciones/posición con el perfil. Funciona con ESP y aimbot apagados. Solo procesa mensajes que el servidor entregue al cliente; falta probarlo en partida. [Implementación y validación](docs/CHAT_SPY_IMPLEMENTATION.md).
14. **Varios → Otros → rapidfire** repite mientras se mantiene su tecla. Expone Storm, choke y flush, con velocidad 1–200. La secuencia integra el armado antes de `Prediction::Update`, las correcciones de tickbase por comando y los argumentos originales de `CL_Move`, confirmados con IDA/MCP. El contador **ACKed commands** muestra reconocimiento de comandos; el daño efectivo requiere comprobarse en partida. Mientras rapidfire está armado pausa speedhack. [Integración y límites](docs/RAZE_RAPIDFIRE_INTEGRATION.md).

La pestaña Status y los indicadores técnicos de CreateMove se retiraron del menú. El registro junto a la DLL conserva los fallos de resolución para diagnóstico. El Hunter está integrado en la DLL: no requiere copiar imágenes junto al juego.

**Precisión en movimiento:** los tres modos conservan el tick nativo para poses actuales; normal sigue el desplazamiento angular sin retrasarlo dos veces y el disparo automático usa el tamaño de la hitbox. Todas las familias hitscan actualizan su spread en el reloj del disparo cuando la opción está activa. [Cambios, pruebas y límites](docs/AIM_ACCURACY.md).

**Estabilidad del aimbot:** normal y silent client retienen objetivo/punto visible, prefieren la pose actual y separan la cámara del ángulo compensado del disparo. Normal conserva el suavizado configurado; silent client sigue la dirección del ratón. [Cambios y validación](docs/AIM_STABILITY.md).

**Rendimiento:** snapshots del ESP y perfiles se copian solo cuando corresponde; el Hunter reutiliza buffers de GPU conservando animación, texturas y efectos. Las pruebas DX9 comparan ambas rutas píxel por píxel. [Optimización, mediciones y límites](docs/PERFORMANCE.md).

**Hunter 3D:** la vista previa del ESP está integrada en **Jugadores**, encima de su panel de colores y efectos. Conserva animación, pausa, giro al arrastrar y zoom con la rueda. La barra de salud se puede arrastrar a cualquiera de los cuatro lados. Hitbox Selection conserva su ventana independiente, desplazable por el título, cerrable con `x` y accesible con `open hitbox selector`. Cajas y skeleton siguen la pose, y las prioridades rellenan la superficie del modelo. Detalles y procedencia en [Hunter preview](docs/HUNTER_PREVIEW.md). Validación visual DX9 fuera de partida; falta comprobarla dentro del juego.

**Marca de agua:** muestra el logo del menú, **VS 3** y los FPS en la esquina superior derecha, también con el menú cerrado. En **Visuales → HUD / efectos** se pueden ocultar la marca completa o solo los FPS; las opciones se guardan en el perfil. Usa el contador de frames del renderer y la transparencia del HUD, sin capturas adicionales ni desenfoque.

### Spectator list

Activada por defecto; se cambia en **Visuales → Radar / espectadores** y se guarda con el perfil. Muestra los nombres de quienes observan directamente a tu jugador en primera persona (`OBS_MODE_IN_EYE`) o cámara de seguimiento (`OBS_MODE_CHASE`). **include chase camera** permite limitarla a primera persona. La cabecera incluye el contador; **camera labels** muestra 1P / 3P y **hide empty list** la oculta cuando está vacía y el menú está cerrado. Funciona con el menú cerrado y el ESP apagado. Usa los colores y la transparencia del tema.

El área de nombres se despliega y se recoge suavemente cuando llegan o se van espectadores. Las filas se ordenan por nombre y se identifican por userid y slot, por lo que los nombres duplicados, renombrados o reordenados conservan su animación. Al dejar de observarte desaparecen de inmediato y se anima el cierre del espacio. Los nombres largos se recortan antes de las etiquetas de cámara. La cabecera permanece anclada, salvo cuando necesita ajustarse al borde de la pantalla.

Abre el menú con **Insert** y arrastra el título **Spectators** para moverla; la rueda recorre las listas largas. La posición se guarda con el perfil y se adapta a la resolución. La **X** desactiva la lista; **show / reset spectator window** la activa y devuelve a la esquina superior derecha. Mientras arrastras la ventana, el menú y los previews de Hunter no reciben esos clics.

Spectators, ESP Preview y Hitbox Selection siguen el tema activo del menú: fondos, títulos y controles se actualizan al cambiar la paleta. Los colores de salud, ESP y prioridades del selector conservan sus ajustes propios.

La lista se actualiza cada 100 ms desde `FRAME_NET_UPDATE_END`, resuelve el handle completo del objetivo y elimina los nombres al cambiar de objetivo, salir de los modos habilitados o desconectarse. No descarta espectadores por tener el cuerpo dormido o estar en modo idle. Se limpia al cambiar de mapa y descarta datos de más de 500 ms; el renderer solo recibe nombres copiados. Los filtros y handles se verificaron con entidades simuladas; falta comprobarlos con otro jugador dentro de una partida.

### Integrado en esta entrega

- Aimbot normal y **silent de cliente**: selección por FOV/distancia/prioridad de especiales, hitboxes reales, visibilidad, FOV y alcance ajustables, suavizado en modo normal, disparo automático opcional y compensación de recoil. Solo armas de fuego de impacto instantáneo. El historial/backtrack cubre players y bots (especiales y comunes de campaña), con pose renderizada y reescritura de `cmd.tick` solo para registros históricos válidos; los centros tienen prioridad sobre los bordes.
- Filtros para comunes, especiales, Tank, Witch y supervivientes enemigos; omisión de Witch calmada y comunes en llamas. Los aliados bloquean las trazas y no son objetivos.
- Trigger con tecla, retardo y filtro de cabeza. Autoempujón con alcance, FOV, visibilidad y cooldown; excluye Tank, Charger y Witch.
- ESP de personajes/comunes con cajas, esquinas, nombres, salud, barra de vida (segmentos/colores escalonados opcionales), distancia y esqueletos animados; categorías y alcance ajustables. Etiquetas de estado (GHOST, FRIEND, ON FIRE, INCAP, IDLE, RAGED/ANGER %) y paleta por categoría (11 colores) con override de amigos/fantasmas.
- ESP de objetos con clasificación por clase/modelo: armas, cuerpo a cuerpo, salud, munición, mejoras, arrojables, proyectiles/fuego y objetos transportables. Omite objetos en posesión de una entidad; colores por categoría de objeto.
- Glow mediante propiedades nativas y coloreado de modelos con pasada opcional a través de paredes. Conserva/restaura los valores anteriores; el coloreado requiere que el motor use `DrawModelExecute`.
- Bunnyhop y auto strafe normal/direccional, condicionados a resolver los campos de movimiento; auto pistol para armas semiautomáticas.
- Tercera persona mediante hooks directos, colisión, distancia, hombro/altura, FOV de cámara, eliminación visual del recoil, offsets/ángulos/FOV del arma.
- **Speedhack por ticks de movimiento**, **airstuck** (`m_MoveType`), **lag exploit** (`m_nOutSequenceNr`) y **rapidfire/double tap**, con teclas configurables. Rapidfire integra la [secuencia confirmada de Raze](docs/RAZE_RAPIDFIRE_INTEGRATION.md), con activación sostenida, velocidades de 1 a 200 y métodos Storm/choke/flush. La propuesta de espera entre ráfagas de 3.1.8 quedó [revertida](docs/RAPID_FIRE_STABILITY.md). Speedhack conserva su multiplicador fraccionario y sus ticks no fuerzan ataque. Ya no se modifica `host_timescale`.
- Perfiles persistentes para los controles nuevos y atajos procesados también con el menú cerrado. Los ajustes nuevos usan categorías `l4d2 ...` y no reinterpretan ajustes de combate antiguos.

**La migración de RijiN es parcial.** El inventario completo, las diferencias y los módulos pendientes están en [docs/RIJIN_PORT.md](docs/RIJIN_PORT.md). No hay controles operativos ficticios para los módulos pendientes.

La cámara y el aimbot no usan comandos de consola. Silent conserva la vista local; no implementa el modo de ocultación de paquetes del proyecto de referencia. La distancia usa 0.0254 metros por unidad, como RijiN.

## Limpieza de opciones de CS2

- Eliminados skins, cuchillos cosméticos, guantes, agentes, paint kits, StatTrak y su catálogo vacío.
- Eliminados Zeus, compra automática, auto revolver, clan tag, radar, killfeed y equipamiento en el marcador de CS2.
- Eliminados double tap y hide onshot heredados de su implementación de combate.
- Eliminados C4, dinero, armadura, kit, desactivación, flashbang, humo, HE y señuelo. El ESP de proyectiles conserva Molotov y área de fuego; las flags conservan zoom, ping y distancia.
- Eliminados los sonidos internos de CS2, la selección de sus materiales de cielo, colores de nubes/sol y parámetros de humedad/anisotropía de Source 2. Se conserva el campo de sonido personalizado.
- Eliminada la opción de piernas de primera persona. Se conserva la eliminación de nombres sobre los jugadores, que puede adaptarse a L4D2.

Los controles retirados ya no se registran como atajos, resultados de búsqueda ni campos de configuración. Los perfiles anteriores ignoran sus campos al cargar; no se borraron los perfiles guardados. Las flags usan una clave nueva para evitar reinterpretar los índices de las antiguas flags de CS2.

Para reemplazar una DLL ya cargada, reinicia el juego. Esta primera base mantiene los hooks hasta el cierre del proceso y no implementa descarga en caliente.

## Compilación

Requisitos: Windows, CMake y Visual Studio con C++/SDK de Windows.

```powershell
cmake -S . -B build -A Win32
cmake --build build --config Release --target lyn --parallel
```

También puedes ejecutar `./build.ps1`. Resultado: `build/Release/lyn.dll` y símbolos `lyn.pdb`.

La entrega comprimida está en `build/compressed/lyn.dll`, también preparada junto al
loader: **3,62 MB frente a 8,51 MB**. La compilación sin comprimir con la compensación
de escopetas, diagnóstico del aim y movimiento está en `build/aim-accuracy/lyn.dll`.
[Compresión y comprobaciones](docs/DLL_COMPRESSION.md).

## Estructura

### Carpetas

| Carpeta | Contenido |
| --- | --- |
| `src/` | Código C++ de la DLL: features, aimbot, ESP, menú y hooks. |
| `loader/` | Aplicación C# / WPF que valida la licencia y carga la DLL (incluye modo dev). |
| `tests/` | Suites C++ y arneses de prueba del loader. |
| `tools/` | Utilidades de desarrollo: preview del Hunter, QA e inspección. |
| `packaging/` | Scripts de empaquetado y entrega (comprimir, validar y publicar). |
| `docs/` | Documentación; artefactos de ingeniería inversa en `docs/re/`. Índice en `docs/README.md`. |
| `sites/` | Sitios web (panel privado y descargas) con su propio proyecto y despliegue. |
| `vendor/` | Dependencias de terceros: ImGui, MinHook, FreeType y otras. |
| `assets/` | Imágenes y recursos usados por la DLL y el preview del menú. |
| `build/` | Salida de CMake (`build/Release/lyn.dll`), registros recientes (`build/logs/`) y preparación de entrega. |
| `build-current/` | Árbol de compilación validado que usan los scripts de empaquetado. |
| `dist/` | Paquetes `.tar.gz` listos para publicar. |
| `archive/` | Historial conservado: capturas previas y artefactos antiguos. No participa en la compilación. |

### Archivos principales

| Archivo | Función |
| --- | --- |
| `src/memory.*` | Lecturas y llamadas protegidas ante fallos de acceso. |
| `src/source.*` | Interfaces x86, estructuras ABI y búsqueda acotada de netvars. |
| `src/esp.*` | Proyección y recorte de cajas reutilizados; el recolector anterior ya no se ejecuta. |
| `src/game.*` | Servicios de Source y ABI usados por las funciones nuevas. |
| `src/aimbot.*`, `src/aim_logic.h` | Selección FOV/distancia/smart shot, zonas, filtros e historial. |
| `src/features.*` | Comandos normal/silent, automatización, cámara y visuales. |
| `src/spectators.*` | Lista independiente de espectadores en primera persona; publica nombres y estado sin punteros de entidades. |
| `src/velocity_ui/spectators_window.*` | Ventana persistente de espectadores, arrastre y posición por perfil. |
| `src/velocity_ui/menu.features.cpp` | Páginas del menú conectadas al runtime nuevo. |
| `src/weapon_catalog.h`, `src/weapon_fields.inc`, `src/velocity_ui/weapons_page.cpp` | Catálogo, ajustes persistentes y página por arma. |
| `src/radar_logic.h`, `src/velocity_ui/radar_window.*` | Proyección del radar y ventana desplazable; recibe posiciones copiadas desde el runtime. |
| `src/movement_batch.h` | Presupuesto fraccionario de speedhack, límite compartido con rapidfire y ejecución de ticks adicionales. |
| `src/air_movement.h` | Aceleración aérea, dirección WASD/cámara e intención de salto conservada al reprocesar comandos. |
| `src/rapid_fire.h` | Crédito ganado en reposo, límites de ráfaga, espacio para comandos retenidos y política de envío único. |
| `src/chat_spy.*`, `src/chat_logic.h`, `src/velocity_ui/chat_window.*` | Listener de chat, captura del HUD, filtro temporal de duplicados y panel persistente. |
| `src/hooks.*` | Ciclo del juego, Present/Reset de DX9, mensajes de entrada y cursor. |
| `src/menu.*` | Integración entre Velocity UI, ImGui/DX9 y la cola de entrada. |
| `src/velocity_ui/` | Menú y XUI copiados del proyecto indicado, fuentes integradas y adaptador de dibujado DX9. |
| `src/log.*` | Registro junto a la DLL. |
| `src/dllmain.cpp` | Inicio del trabajador después de cargar la DLL. |

## Cómo funciona

- `src/game.*` concentra las interfaces, ABI, resolución por firmas únicas, tablas de red, datamaps, entidades, modelos, hitboxes y trazas.
- `src/features.*` implementa los hooks de comandos/cámara/modelos y la recopilación del ESP. El callback final de comandos usado por RijiN modifica ángulos, movimiento, botones y el tick de una posición histórica válida. Conserva número y semilla. El juego calcula después el CRC de su copia verificada. El layout de `CUserCmd` de L4D2 es de 88 bytes.
- Los índices de entidades se renuevan cada 250 ms; los datos visibles se actualizan desde `FrameStageNotify`. Los nombres tienen caché de un segundo. Los modelos usan una caché acotada que se vacía al cambiar de mapa.
- El selector del aimbot limita el trabajo a 64 candidatos y 192 trazas por comando. El historial almacena hasta 32 capturas por jugador, se purga al cambiar de handle/modelo/mapa o teleportarse y valida antigüedad, latencia e interpolación. Las prioridades personalizadas se aplican tanto a las capturas históricas como a las hitboxes actuales.
- Silent de servidor usa el contexto de `CL_Move` validado por firmas, retorno de llamada y límites de la pila del hilo. Retiene un comando de disparo y fuerza un comando siguiente con la vista original. Si ese contexto no se puede verificar, el modo queda inactivo y el motivo se registra en el log.
- La compensación de spread resuelve el campo y `SharedRandomFloat` desde el código instalado y restaura la semilla global temporal. En **guns**, las cuatro escopetas T1/T2 tienen **spread compensation (client)** independiente: muestrea `UpdateSpread` con reloj de tickbase/intervalo y semilla del disparo, restaura el contexto y compensa el eje. Aísla cámara/viewmodel también al disparar manualmente y muestra la última corrección. Conserva número y semilla del comando; el patrón de perdigones del servidor permanece. [Uso y límites](docs/SHOTGUN_CLIENT_SPREAD.md).
- DirectX recibe capturas POD y no invoca métodos de entidades. Se descartan capturas de más de 250 ms. Se reutiliza el recorte de cajas del ESP anterior para evitar que desaparezcan al cruzar el plano cercano.
- Skeleton recoge las matrices de `SetupBones` y conexiones padre/hijo marcadas `BONE_USED_BY_HITBOX`, con un máximo de 128 huesos por modelo y 8192 segmentos por captura. Las extensiones de pies solicitan además `BONE_USED_BY_BONE_MERGE` (máscara `0x500`); antes se dibujaban sin pedir sus matrices. Omite conexiones al hueso de escena y une los helpers con el antecesor animado. Solo solicita poses de entidades dentro del alcance y del frustum; no invalida la caché de huesos del motor. La topología y su máscara se guardan por modelo/cabecera/checksum y se limpian al cambiar de mapa. Las líneas se recortan antes de dividir por perspectiva. El Hunter 3D del menú usa los huesos de su propia animación; la imagen de respaldo conserva articulaciones ilustrativas.
- Las funciones de comandos se pausan con el menú abierto, al perder el foco, morir, estar incapacitado o bajo control de un infectado. Las ayudas de movimiento omiten escaleras, noclip y agua profunda.
- Los cambios de glow se guardan por handle/serial y se restauran al desactivar, cambiar de mapa o detener el renderer. Los overrides de material se limitan a una llamada de dibujo.
- La DLL mantiene sus hooks hasta cerrar el proceso. No implementa descarga en caliente.

## Interfaces y ABI

Se confirmaron las declaraciones contra el código local de referencia de L4D2 indicado por el usuario; el ESP se escribió de nuevo. El menú actual procede de Velocity.

| Interfaz | Métodos usados |
| --- | --- |
| `client.dll!VClient016` | GetAllClasses 7, LevelShutdown 6, FrameStageNotify 34. |
| `engine.dll!VEngineClient013` | GetPlayerInfo 8, GetLocalPlayer 12, GetMaxClients 21, IsInGame 26, WorldToScreenMatrix 37. |
| `client.dll!VClientEntityList003` | GetClientNetworkable 0, GetClientEntity 3, GetHighestEntityIndex 6, GetMaxEntities 8. |
| `vguimatsurface.dll!VGUI_Surface031` | UnlockCursor 58 y LockCursor 59, opcionales. |

Las estructuras tienen comprobaciones de tamaño y posición durante la compilación: RecvProp 0x3C, tabla hija 0x28, offset 0x2C, ClientClass 0x18 y nombre de PlayerInfo en 0x08.

Dependencias fijadas: [Dear ImGui v1.91.9b](https://github.com/ocornut/imgui/tree/v1.91.9b), [MinHook v1.3.4](https://github.com/TsudaKageyu/minhook/tree/v1.3.4) y [FreeType 2.13.3](https://github.com/freetype/freetype/tree/VER-2-13-3), con sus licencias en `vendor/`. Ver `src/velocity_ui/PORT.md` para la procedencia del menú y sus recursos.

## Estado de esta entrega

La DLL Release Win32 compiló correctamente. Pasaron **489 comprobaciones en 12 suites**: 28 de política de aim, 59 de selección/cámara, 12 de snapshots/configuración, 29 del Hunter DX9, 69 del runtime, 68 de compensación de spread, 14 de persistencia para los 50 perfiles y opciones de chat/ráfaga, 19 del presupuesto de movimiento, 100 de movimiento aéreo, 49 de rapidfire, 27 de la lógica de chat y 15 de su runtime con motor simulado y hook real de MinHook. Se corrigió además la clase del M16: el cliente instalado expone `CAssaultRifle`, no `CRifle`. La DLL actual todavía no se comprobó en partida: seguimiento del aim, bunnyhop/strafe, compensación e impactos de escopetas, daño de las ráfagas nuevas, aceleración aceptada por el servidor, aspecto de los skeletons, radar, cámara por arma, espectadores reales, recepción de chat, FPS y estabilidad siguen pendientes.

El panel **Aim status** muestra modo y auto shoot efectivos, perfil equipado y último intento. Puede aplicar enabled/modo/auto shoot principales al arma equipada y conserva los filtros de la última selección al abrir el menú. La activación `always` ya respeta auto shoot apagado. [Diagnóstico de seguimiento en Versus](docs/NORMAL_AIM_DIAGNOSTICS.md).

Para repetir las comprobaciones independientes del motor:

```powershell
cmake -S . -B build -A Win32 -DFREE_BUILD_AIM_TESTS=ON
cmake --build build --config Release --target aim_logic_tests aim_selection_tests performance_tests hunter_render_tests runtime_api_tests shotgun_spread_tests settings_persistence_tests movement_batch_tests air_movement_tests rapid_fire_tests chat_logic_tests chat_runtime_tests
ctest --test-dir build -C Release --output-on-failure
```
