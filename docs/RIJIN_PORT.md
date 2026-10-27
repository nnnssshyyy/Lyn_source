# Adaptación de funciones de RijiN a FREE L4D2

Referencia proporcionada por el usuario:
`C:\Users\henry\OneDrive\Desktop\RijiN-main\src\cheats\left4dead2`.

Esta entrega reimplementa un primer grupo de funciones en el runtime MSVC/x86 del proyecto. No es una importación completa de RijiN. Su código depende de `src/base`, de paquetes de firmas suministrados por su cargador, de su sistema de predicción y de otras clases globales. El menú Velocity se conserva; las cinco páginas de juego compiladas ahora están en `menu.features.cpp`.

## Inventario

| Referencia | Estado en este proyecto | Diferencias / trabajo pendiente |
| --- | --- | --- |
| `raytrace_aimbot` | Normal, silent de cliente y silent de servidor; FOV, distancia, smoothing porcentual, auto shoot, zonas automáticas o personalizadas y visibilidad | Adaptado al callback final previo al CRC; conserva la secuencia de comandos. No añade penetración de paredes (la referencia de L4D2 tampoco la ejecuta). |
| `aimbot_filters` | FOV, distancia y smart shot; prioridad de amenazas, categorías, idle por secuencia, quemados, Witch calmada, amigos de Steam y selección de torso por arma | La clasificación one-shot conserva la heurística de RijiN. La consulta de amigos depende de los exports Steam disponibles; no importa la lista personalizada de prioridades de jugadores. |
| `auto_bash` | Empujón automático con trazas, cooldown, alcance y categorías | No interpreta secuencias de animación ni llama al `TestSwingCollision` privado. Usa un alcance conservador hasta 75 unidades. |
| `auto_strafe` | Strafe en aire normal/direccional | No evitación automática de paredes; depende de datamaps/campos válidos. |
| `player_esp` | Cajas, esquinas, nombres, salud, barra, distancia y filtros; incluye comunes | No flags de Steam/lista de jugadores ni colores individuales. |
| `world_esp` | Clases y modelos conocidos, cajas/nombre/distancia/filtros/rango, proyectiles/fuego | No detecta objetivos por eventos de misión; los transportables son una categoría general. Los objetos sin caja válida muestran texto en su origen. |
| `chams` | Color de modelo con pasada visible/oculta, preservación del material; glow nativo por netvars | No stencil/blur/fresnel, materiales personalizados ni huesos históricos. Las rutas rápidas del renderer que no llaman a DrawModelExecute no reciben este coloreado. Glow requiere propiedades disponibles en esa entidad. |
| `thirdperson` | Cámara directa en RenderView, modelo local, colisión, hombro, altura y distancia | Sin comandos de consola. Configuración única, sin animación de transición ni tecla separada de invertir hombro. |
| `viewmodel_offset` | Posición, ángulos y FOV del arma | Sin sway personalizado ni visualización del ángulo silent sobre el arma. |
| `misc::bunnyhop`, `auto_pistol` | Integrados | Se conservan los comandos originales fuera de las condiciones de activación. |
| `misc::nospread`, `fix_nospread_pred` | Compensación angular con `SharedRandomFloat`, semilla del comando y restauración de la semilla global | El offset se extrae de la lectura en `CTerrorGun::FireBullet`. No se cambia `command_number` a la constante de escopeta de RijiN; no se garantiza agrupación de perdigones ni la equivalencia de spread bajo su predicción completa. |
| `engine_prediction` | Pendiente | El disparo consulta el reloj del motor/tickbase y cooldowns; no se simula un tick adicional del jugador. |
| `lag_compensation`, `interpolation_manager` | Historial acotado, validación temporal con latencia/interpolación y ajuste de tick de disparo | Capturas de hitboxes del motor, sin forzar animaciones ni cambiar mapas de interpolación. Se invalidan al cambiar de handle/modelo/mapa o teleportarse. |
| `packet_manager`, `tickbase_correction` | Envío para silent de servidor integrado; corrección de tickbase pendiente | Contexto `CL_Move` y pila validados; máximo un comando retenido por la función, seguido de envío con vista original. No porta los exploits de tickbase. |
| `speedhack`, `break_tickbase`, `pill_exploit` | Pendientes | El original depende del retorno de pila de su hook y del canal de red; `pill_exploit` está bajo DEV/STAGING. Esas dependencias no están portadas. |
| `teleport` | No implementado en la referencia | El cuerpo funcional está comentado. No se presenta una casilla operativa. |
| `cheat_detection`, `player_list_handler` | Pendientes | No se importaron los servicios externos, intercambio de SteamID ni la infraestructura de cuentas del original. |

Los módulos pendientes quedan documentados aquí. La pestaña Status se retiró del menú; esos módulos no se activan mediante los perfiles antiguos.

## Comprobaciones realizadas

- Compilación Release Win32 con MSVC y comprobaciones estáticas de layouts usados: prefijo de CUserCmd, Ray, Trace, View, Hitbox y ModelInfo.
- Inspección estática del `client.dll` instalado, Steam build **23990068**, sin cargar la DLL ni modificar archivos del juego. Las firmas de ClientMode, ambos caminos de globals, RenderView, ShouldDrawLocalPlayer, CamIsThirdPerson y CalcViewModelView tienen una coincidencia única.
- La descripción de `m_MoveType` en ese archivo utiliza un descriptor de **60 bytes**. El lector recorre descripciones válidas y sus bases; no usa `0x144` como offset fijo.
- Referencia pública adicional para los datamaps: [Valve, datamap.h](https://github.com/ValveSoftware/source-sdk-2013/blob/master/src/public/datamap.h). La disposición de esa rama es distinta y se admite únicamente si valida la tabla completa.
- No se ejecutaron pruebas dentro de L4D2. Quedan por comprobar los callbacks en ejecución, aciertos, cámara, rutas de modelos, FPS y cambios de mapa. La compilación y las coincidencias estáticas no confirman estos resultados.

### Actualización del aimbot — 12 de octubre de 2026

- El antiguo callback de ClientMode se sustituyó por el callback final de RijiN (`SetLastUserCommand`), restringido a su punto de llamada de CInput. El original sigue ejecutándose y el juego genera posteriormente la copia y CRC del comando.
- Firmas con una coincidencia en la instalación local: callback RVA `0x1346BD`, lectura de spread `0x30C61B`, llamada a SharedRandomFloat `0x30C640`, CL_Move de engine RVA `0x7D120` y llamada de cliente `0x7D2C9`. La variable de envío se obtiene del operando de la función, no de una dirección de pila sin validar.
- La disponibilidad del disparo sigue el criterio de RijiN de munición/cooldown, permitiendo interrumpir la recarga de una escopeta cargada. El menú muestra cuándo faltan arma, activación, hitboxes o visibilidad.
- Se corrigió también el argumento de índice de `CAM_IsThirdPerson`: el binario instalado termina con `ret 4`. El hook y su llamada original conservan ahora ese argumento.
- Pasaron 16 escenarios independientes del motor (máscaras, visibilidad de puntos históricos, sólidos, ventanas temporales y suavizado), ejecutados con CTest. La DLL y las pruebas compilan en Release Win32. Persisten advertencias anteriores del código de interfaz; los archivos del aimbot no generaron advertencias en esa compilación.

## Diagnóstico y límites

- El registro detalla firmas ausentes y errores de MinHook. Los indicadores técnicos y contadores ya no se muestran en el menú.
- Se copian configuraciones y datos entre hilos con bloqueos cortos. Los punteros a entidades no llegan al dibujado DX9 del ESP.
- Las escrituras de glow usan handles con serial; se restauran los valores anteriores y se respetan actualizaciones que no coincidan con lo último escrito por el módulo.
- El aimbot evita ghost, muertos, dormidos, aliados y armas sin soporte. Las trazas bloquean disparos obstruidos. No se implementa autowall.
- Para comunes/Witch sin salud replicada se usa la solidez para descartar cadáveres. No se inventan HP ni se dibuja barra de salud para esos casos; no se interpretan todavía sus secuencias de muerte.
- Los hooks permanecen instalados hasta cerrar el juego. Para usar una nueva compilación, reinicia L4D2 y carga la DLL nueva.

### Posición de tercera persona — 13 de octubre de 2026

- Se añadió la exclusión de `CAM_IsThirdPerson` para el retorno de
  `ClientMode::OverrideView`, como en RijiN. La firma tiene una coincidencia en
  el cliente instalado (RVA `0xC3F18`), precedida por `push -1; call eax`.
  Así el motor no usa sus propios ángulos/desplazamiento antes de aplicar la
  cámara del menú. Las consultas de modelo/animación siguen disponibles.
- La vista está centrada por defecto, incluso con perfiles anteriores que
  guardaban hombro/altura. Desmarcar `centered behind player` habilita esos
  valores sin borrarlos. Los desplazamientos opcionales usan los ejes completos
  de la cámara, incluyendo pitch y roll.
- La colisión conserva exactamente el punto devuelto por la traza. Se retiró
  el segundo desplazamiento lateral que salía del recorrido comprobado. Cuando
  quedan 15 unidades o menos, se conserva la vista original de primera persona.
- Compilación Release Win32 correcta. No se ejecutaron pruebas ni una partida
  para esta revisión de cámara; la colocación visual requiere confirmación en
  el juego. Registro: `build/logs/thirdperson-position-build.log`.

### Dirección fija de tercera persona — 13 de octubre de 2026

La exclusión anterior encontraba el retorno correcto, pero su ABI de retorno
era incorrecta. El cliente comprueba `test eax,eax` después de llamar a
`CAM_IsThirdPerson`. La versión `bool` de nuestro hook compilaba el falso como
`xor al,al`, conservando en los otros bytes la dirección de retorno cargada
para la comparación. Por tanto, OverrideView seguía tomando el camino nativo
y sus ángulos de cámara guardados.

Se cambió tanto el tipo del original como el hook a retorno `int`. La DLL
generada contiene ahora `xor eax,eax` para falso y `mov eax,1` para verdadero,
manteniendo `ret 4` y el argumento de índice. Además, cada frame obtiene la
dirección actual mediante `GetViewAngles` (slot 19), y usa esos mismos ángulos
para orientar la vista y situar la cámara detrás. No usa los ángulos de red del
modelo. El estado muestra pitch/yaw para observar si siguen al mouse.

Compilación Release Win32 correcta e inspección del ensamblado realizada. No se
ejecutaron pruebas ni se comprobó una partida para esta revisión. Registro:
`build/logs/thirdperson-direction-build.log`.

Revisión posterior: [errores de movimiento, registro del glow y diagnóstico](AIM_GLOW_AUDIT.md).

Resultado de compilación: `build/Release/free_esp.dll`. Registro más reciente: `build/logs/thirdperson-direction-build.log`.
