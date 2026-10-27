# Anti OBS de Raze: extracción y coste frente a VS 3

La adaptación opt-in y su validación están en
[CAPTURE_PERFORMANCE_3_1_18.md](CAPTURE_PERFORMANCE_3_1_18.md).
La comparación siguiente corresponde a la ruta de VS 3 3.1.17 analizada.

Revisión del 21 de octubre de 2026, hora de Lima, mediante IDA Pro 9.4 y MCP.
Se cotejó el SHA-256 del archivo original con el registrado en IDA:
`dfa8632e85c89c318577ef619f20bae2f4db5136042d680ee4dae4109234691a`.
Base de imagen: `0x10000000`. Las direcciones siguientes son VA de esa muestra.

**Hallazgo:** Raze implementa `bStreamProof` filtrando la llamada a `EndScene`
por su dirección de retorno dentro de un módulo cuyo nombre contiene
`gameoverlay`. Desde esa llamada dibuja sus elementos con el dispositivo y
destino de render existentes. La ruta reconstruida no utiliza una ventana
privada ni una presentación adicional como VS 3 3.1.17.

## Flujo reconstruido

| Dirección | Evidencia |
| --- | --- |
| `0x103F5396` | Booleano persistido como `bStreamProof`; 28 referencias directas. |
| `0x100095E0` / `0x1010BF31` | Carga / guardado del booleano de configuración. |
| `0x100E5851` | El selector `Streamer mode` escribe ese mismo booleano. |
| `0x1016E6B0` | Instala el hook en la entrada 42 del dispositivo DX9 (`+168`). |
| `0x1016E6D0` | Callback `EndScene`; un argumento `stdcall`, validado por `retn 4`. |
| `0x1016E71F` / `0x1016E734` | `VirtualQuery` del retorno y `GetModuleFileNameA` de su AllocationBase. |
| `0x1016E74A` / `0x1016E757` | Construcción de `gameoverlay` en la pila y búsqueda con `strstr`. |
| `0x103F73B0` | Guarda una dirección de retorno concreta, no una franja de direcciones. |
| `0x1016E780` → `0x1016E963` | Con privacidad activa, un retorno diferente salta directamente al original. |
| `0x1016E8AC`…`0x1016E930` | Trece helpers de dibujo alternativo para la ruta DX9 privada. |
| `0x1016E95B` | Envía los draw lists al backend DX9 `0x1006CC30`. |
| `0x1016E964` | Llama al `EndScene` original; no hay un segundo `Present` en esta ruta/backend. |

El filtro sucede **antes** de inicializar/procesar el frame de interfaz. Tras
encontrar el retorno de overlay, rechazar otra llamada cuesta principalmente
comparar el booleano y el retorno, y llamar al original. Con privacidad
desactivada, el callback deja pasar también otros retornos; no debe describirse
como una protección contra dibujo repetido para todos los modos.

El byte extra `4B` de `0x10261E00` queda antes del puntero buscado. Los bytes
realmente usados componen `gameove` + `rlay\0`, es decir, `gameoverlay`.
La comparación es sensible a mayúsculas. La muestra busca esa subcadena, no
verifica un nombre completo, firma del archivo ni identidad criptográfica del
módulo. Su relación con Steam Overlay se infiere del nombre y la llamada DX9.

## Dibujo, estado y efectos omitidos

El backend `0x1006CC30` utiliza buffers dinámicos de vértices/índices, conserva
el estado con `CreateStateBlock(D3DSBT_ALL)` y `Capture`, prepara el render 2D,
envía las primitivas recortadas y restaura matrices y estado mediante `Apply`.
Conserva el destino de render actual. No contiene la creación de una swapchain
adicional, el cambio a su backbuffer ni una segunda presentación.

Hex-Rays devuelve una representación incompleta de ese backend después de sus
comparaciones AVX iniciales. Por eso se exportó y revisó el ensamblador completo
de **349 instrucciones**; esa versión es la evidencia para este apartado.
El callback principal completo tiene **188 instrucciones**.

`Paint`, en `0x1016CC80`, sigue llamando al método original. Si `mode & 1` está
activo, actualiza información de pantalla. Con privacidad apagada ejecuta la
ruta ISurface habitual entre sus funciones de inicio y fin. Con privacidad
encendida omite esa lista y llama siete helpers de preparación alternativos;
las representaciones DX9 se procesan desde el callback seleccionado de
`EndScene`. Esto evita enviar las representaciones habituales por ambas rutas.

Además, los caminos de materiales/efectos de mundo comprobados en
`0x1014D5B7` y `0x1014E630` retornan sin dibujar sus efectos cuando se activa
`bStreamProof`. Parte del menor trabajo procede de omitir esos efectos.
No equivale a conservar automáticamente cada efecto y animación de VS 3.

El hook de Reset se instala en la entrada 16 (`+64`). Invalida los recursos
DX9, llama al Reset original y solo reconstruye los recursos si tiene éxito.
La inicialización de la UI usa `InitOnceBeginInitialize` / `InitOnceComplete`.

## Diferencia con el coste actual de VS 3

| Trabajo | Raze, ruta revisada | VS 3 3.1.17 |
| --- | --- | --- |
| Selección del punto de dibujo | Retorno concreto de `gameoverlay` en EndScene | Hook de Present del juego |
| Destino de dibujo | Destino actual del dispositivo | Backbuffer de una swapchain adicional |
| Ventana de privacidad | No aparece en la ruta reconstruida | Ventana layered con afinidad `0x11` |
| Presentación adicional | No aparece en la ruta/backend revisados | `IDirect3DSwapChain9::Present` |
| Inicio/fin de escena adicional | El callback dibuja antes de terminar la escena existente | `BeginScene` / `EndScene` propios |
| Control de cadencia privado | Sin presupuesto ni número de frame en el callback | 60–90 Hz; reutilización entre actualizaciones |
| Estado de DX9 | State block, matrices, buffers y draw lists | Backend ImGui más cambio/restauración de RT/depth/viewport y Clear |

En nuestro código, `Surface::Prepare` crea la segunda swapchain;
`capture_overlay::Target` guarda/cambia/restaura destinos y limpia el backbuffer;
`Surface::Present` presenta esa superficie adicional. Todo sucede en el hilo y
dispositivo de render del juego, aunque esté limitado a 60–90 Hz. El límite
reduce frecuencia; no elimina el coste de ese trabajo y de la composición de
la ventana. Tampoco hay una salida por draw list vacío antes de Clear/Present.

Esta es una diferencia de coste **observada en el código**, y una candidata
razonable para explicar la caída restante. La cantidad de FPS atribuible a
cada paso requiere medir la ruta de ejecución y la GPU; no se obtuvo de IDA.
[Microsoft recomienda](https://learn.microsoft.com/en-us/windows/win32/api/d3d9/nf-d3d9-idirect3ddevice9-endscene)
como máximo una pareja BeginScene/EndScene entre presentaciones sucesivas.

La adaptación más directa para reducir ese trabajo sería un modo que seleccione
el EndScene de overlay y presente mediante la escena existente. Debe conservar
la restauración de estados, la recuperación de controles y el manejo de Reset;
comprobar el módulo y fallos de las APIs; evitar búsquedas repetidas cuando no
hay overlay; y detectar cambios de dispositivo/módulo. Añadir mediciones de
tiempo para construir UI, cambiar destinos, dibujar y presentar permitiría
comparar ambos caminos bajo la misma escena y resolución.

## Límites del método extraído

- **No es exclusión universal de capturas.** Depende del punto desde el que la
  herramienta captura. OBS ofrece expresamente una opción para capturar
  overlays de terceros; su [documentación oficial](https://obsproject.com/kb/game-capture-source)
  describe ese comportamiento. El código de Raze y su propio texto de menú
  también condicionan el resultado al método de captura.
- Una captura de escritorio/monitor puede incluir lo que se dibuja finalmente
  en el juego. No se comprobó OBS, Discord ni screenshots reales en esta revisión.
- Sin encontrar el retorno esperado, el modo activo omite su dibujo privado y
  vuelve a hacer la consulta del módulo en llamadas posteriores. No hay un
  presupuesto de reintento dentro de ese fragmento.
- No hay un número de frame ni una garantía de una sola ejecución por frame:
  acepta cada llamada que vuelve al mismo sitio seleccionado.
- El Reset revisado no limpia el retorno almacenado. La lista de referencias
  directas del global solo muestra su lectura/escritura en el callback EndScene.
- No había un proceso `left4dead2` activo al comprobarlo. No se hizo una
  comparación de FPS, ni se ejecutó Raze ni se modificó/publicó una DLL de VS 3.

## Artefactos entregados

- [Pseudocódigo de la lógica](re/anti-obs/RAZE_ANTI_OBS_LOGIC.cpp), separado de
  los fuentes que se compilan en el proyecto.
- [Evidencia MCP](re/anti-obs/recheck-20261021/manifest.json): ocho funciones con
  ensamblador completo, **903 instrucciones**, las 28 referencias al flag,
  configuración/UI, llamadas y comprobación del SHA-256 de la muestra.
- Base anotada guardada en
  `C:/Users/User/.codex/tools/ida-analysis/raze/raze-antiobs-annotated-20261021.i64`.
- Exportación reproducible mediante `tools/inspection/extract_antiobs.py`,
  utilizando una sesión abierta de IDA MCP con el mismo SHA-256.
