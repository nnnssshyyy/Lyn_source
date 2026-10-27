# Documentación

Índice de la documentación del proyecto. Los documentos describen la entrega y su
historial; los datos de ingeniería inversa viven en `docs/re/`.

## Visión general

| Documento | Tema |
| --- | --- |
| [PERFORMANCE.md](PERFORMANCE.md) | Optimización conservando funciones y animaciones. |
| [AIM_ACCURACY.md](AIM_ACCURACY.md) | Precisión durante el movimiento (aimbot). |
| [AIM_STABILITY.md](AIM_STABILITY.md) | Estabilidad de normal y silent client. |
| [SPECIAL_PRIORITY_3_1_33.md](SPECIAL_PRIORITY_3_1_33.md) | Prioridad configurable por tipo de infectado especial en los tres modos de aim. |
| [ESP_FONT_3_1_34.md](ESP_FONT_3_1_34.md) | Tipografía compacta del ESP, tamaños rasterizados y alineación nítida. |
| [NORMAL_AIM_DIAGNOSTICS.md](NORMAL_AIM_DIAGNOSTICS.md) | Aimbot normal sin seguimiento en ZoneMod / Confogl. |
| [RAPID_FIRE_FIX.md](RAPID_FIRE_FIX.md) | Rapidfire: corrección del envío y la predicción. |
| [SHOTGUN_CLIENT_SPREAD.md](SHOTGUN_CLIENT_SPREAD.md) | Compensación de escopetas desde el cliente. |
| [AIR_MOVEMENT.md](AIR_MOVEMENT.md) | Bunny hop y auto strafe. |
| [HUNTER_PREVIEW.md](HUNTER_PREVIEW.md) | Hunter 3D en el menú. |
| [CHAT_SPY_IMPLEMENTATION.md](CHAT_SPY_IMPLEMENTATION.md) | Chat spy integrado en FREE. |

## Distribución y servicio

| Documento | Tema |
| --- | --- |
| [VS3_LICENSES.md](VS3_LICENSES.md) | Loader, licencias y modo dev. |
| [VS3_SERVER_HOOKS.md](VS3_SERVER_HOOKS.md) | Distribución y hooks por servidor. |
| [DLL_COMPRESSION.md](DLL_COMPRESSION.md) | Empaquetado y compresión de la DLL. |
| [LOADER_ICON_3_1_9.md](LOADER_ICON_3_1_9.md) | Imagen del usuario como icono del loader completo, compacto y su ventana. |
| [LOADER_CLOSE_3_1_10.md](LOADER_CLOSE_3_1_10.md) | Limpieza al cerrar con X/Alt+F4 y cierre pendiente durante operaciones. |
| [HARDWARE_CLOCK_3_1_11.md](HARDWARE_CLOCK_3_1_11.md) | Solicitudes de equipo con reloj de Windows desajustado y actualización automática en Accounts. |
| [HEALTH_LABEL_3_1_25.md](HEALTH_LABEL_3_1_25.md) | Texto de vida de 12 píxeles, centrado y siguiendo el relleno vertical. |
| [MENU_PREVIEW_3_1_26.md](MENU_PREVIEW_3_1_26.md) | Vista ESP integrada y marca de agua con logo, VS 3 y FPS. |
| [HEALTH_EDGE_3_1_27.md](HEALTH_EDGE_3_1_27.md) | Vida alineada al relleno mediante glifos visibles y sin pista negra. |
| [MOVEMENT_HUNTER_3_1_28.md](MOVEMENT_HUNTER_3_1_28.md) | Airstuck, binds separados de ceiling/Hunter, melee, visión y caída experimental. |
| [AUTO_FALL_3_1_29.md](AUTO_FALL_3_1_29.md) | Lag automático desde el inicio del descenso hasta aterrizar. |
| [RAZE_LAG_3_1_30.md](RAZE_LAG_3_1_30.md) | Rutas Raze V1/V2, envío nativo y prioridad de caída automática. |
| [STORM_FALL_3_1_31.md](STORM_FALL_3_1_31.md) | Caída automática Storm, historial por comando y corrección de predicción. |
| [MENU_CLEAN_3_1_32.md](MENU_CLEAN_3_1_32.md) | Menú sin ayudas, métodos V1/V2 y retirada de los atajos F6/F8. |

## Análisis e ingeniería inversa

| Documento | Tema |
| --- | --- |
| [RAZE_RAPIDFIRE_ANALYSIS.md](RAZE_RAPIDFIRE_ANALYSIS.md) | Rapidfire de Raze (IDA Pro 9.4 y MCP). |
| [RAZE_NO_FALL_ANALYSIS.md](RAZE_NO_FALL_ANALYSIS.md) | Trazado de anti-fall y extracción de lag V1/V2; límites de esta DLL. |
| [RAZE_NOSPREAD_ANALYSIS.md](RAZE_NOSPREAD_ANALYSIS.md) | No spread de Raze (IDA Pro y MCP). |
| [RAZE_CHARGER_TURN_ANALYSIS.md](RAZE_CHARGER_TURN_ANALYSIS.md) | Giro del Charger: parche reversible de vista, cotejado con el cliente instalado. |
| [RAZE_ANTI_OBS_ANALYSIS.md](RAZE_ANTI_OBS_ANALYSIS.md) | Stream proof de Raze: selección de EndScene del overlay y comparación del coste con la ventana privada. |
| [CAPTURE_PERFORMANCE_3_1_18.md](CAPTURE_PERFORMANCE_3_1_18.md) | Captura de juego mediante Steam sin presentación adicional, menos trabajo en ventana privada y límites de validación. |
| [RAPIDFIRE_RELEASE_3_1_19.md](RAPIDFIRE_RELEASE_3_1_19.md) | Recuperación al soltar en los tres métodos: historial confirmado, estado de red temporal y presupuesto de comandos. |
| [RAPIDFIRE_SPEED_3_1_20.md](RAPIDFIRE_SPEED_3_1_20.md) | Recuperación del factor completo sin espera de ACK, conservando la limpieza al soltar. |
| [RAPIDFIRE_BOOST_3_1_21.md](RAPIDFIRE_BOOST_3_1_21.md) | Boost 2x en los tres métodos y conservación de correcciones durante ráfagas que reutilizan posiciones del anillo. |
| [RAPIDFIRE_RECOVERY_3_1_22.md](RAPIDFIRE_RECOVERY_3_1_22.md) | Restauración del envío original, reinicio nativo tras confirmarse la ráfaga y límite del diagnóstico de cámara. |
| [RAPIDFIRE_PACING_3_1_23.md](RAPIDFIRE_PACING_3_1_23.md) | Recuperación del presupuesto de envío tras extras, validado con SendDatagram y CanPacket. |
| [RAPIDFIRE_POWER_3_1_24.md](RAPIDFIRE_POWER_3_1_24.md) | Boost 4x, hasta 800 extras, conservando la recuperación de envío y predicción de 3.1.23. |
| [CHARGER_AND_RENDER_FIXES.md](CHARGER_AND_RENDER_FIXES.md) | Integración del giro, rutas nativas de fog/vomit y presupuesto de render privado, 3.1.17. |
| [RIJIN_PORT.md](RIJIN_PORT.md) | Adaptación de funciones de RijiN a FREE L4D2. |
| [RIJIN_DEDICATED_ANALYSIS.md](RIJIN_DEDICATED_ANALYSIS.md) | RijiN en servidores dedicados. |
| [RIJIN_LOADER_SERVER_ANALYSIS.md](RIJIN_LOADER_SERVER_ANALYSIS.md) | RijiN: qué prepara el servidor y qué instala el cliente. |
| [CHAT_SPY_ANALYSIS.md](CHAT_SPY_ANALYSIS.md) | Análisis del chat de Raze. |
| [AIM_GLOW_AUDIT.md](AIM_GLOW_AUDIT.md) | Auditoría de aim y native glow (12 de octubre de 2026). |

## Artefactos (`docs/re/`)

Firmas, desensamblados y notas por tema: `air-movement`, `nospread`, `rapidfire`,
`raze`, `anti-obs`, `rijin-dedicated`, `rijin-loader-server` y `shotgun-client`.
