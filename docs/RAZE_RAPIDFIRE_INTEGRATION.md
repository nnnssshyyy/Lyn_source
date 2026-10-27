# Integración del rapid fire de Raze — 19 de octubre de 2026

**Histórico:** la recuperación al soltar se describe en
[3.1.19](RAPIDFIRE_RELEASE_3_1_19.md), y la recuperación del factor completo en
[3.1.20](RAPIDFIRE_SPEED_3_1_20.md).

Se volvió a abrir la base anotada en IDA Pro 9.4 mediante `ida-pro-mcp`, se comprobó que su binario de entrada coincide con el original del usuario y se exportaron otra vez las cinco funciones principales. SHA-256 de Raze: `dfa8632e85c89c318577ef619f20bae2f4db5136042d680ee4dae4109234691a`.

## Cambios integrados

| Evidencia en Raze | Integración en el proyecto |
| --- | --- |
| `0x1016FF8E`, `0x1016FFC8`, `0x10170015`: limpiar el slot actual, armar la corrección y después llamar a Prediction slot 3. | `PrepareRapidCommand` ejecuta esa secuencia antes de procesar aim/movimiento. `Api::UpdatePrediction` llama a `VClientPrediction001::Update` con delta, `delta > 0`, ACK y outgoing + choked. |
| `0x1016CE90–0x1016CE9F`: todas las llamadas a `CL_Move` repiten los argumentos originales. | Los extras de rapid fire conservan `samples` y `final`. La política anterior solo ponía `final` en la última llamada. |
| `0x1016FF8E` y `0x1016CFA6–0x1016CFD3`: el slot se limpia al crear su comando y se consulta al repetir su predicción. | Soltar/desactivar rapid deja las correcciones del mismo jugador disponibles para RunCommand; cambiar de entidad o mapa reinicia el historial. |
| `0x10148C40`: tres métodos de estado de red, con la condición del comando anterior para Storm. | Se conservan Storm, choke y flush, sus sliders, escrituras y envío por comando. Cada aplicación vuelve a consultar la tecla, como Raze, para detectar una liberación durante la ráfaga. |
| `0x101489E1–0x10148A11`: `max(speed, int(0.06 / interval + 0.5))`. | Se conserva activación sostenida y velocidad de 1 a 200, sin espera de ACK para volver a armar. |

También se cotejó el `engine.dll` instalado mediante IDA/MCP, SHA-256 `8a6d468059394bf4cfce32f11dcba12ef45de88da1373084d5a2916d0bf7ba86`. El ensamblador de `CL_RunPrediction`, `0x10080E56–0x10080E8B`, confirma los argumentos y el delta en el estado raíz + 8. La resolución obtiene el desplazamiento de delta del operando de esa función; evita trasladar el offset `0xD4` de Raze sin ajustar la vista del estado.

## Adaptaciones y límites

El proyecto utiliza su hook final de comandos previo al CRC, con su ABI validado. Raze utiliza ClientMode slot 27. Los campos y direcciones globales de la otra DLL no son direcciones utilizables en este proyecto.

El entero que Raze escribe en `local + 0x10` sigue sin identificarse como campo seguro del cliente instalado. No se escribió ese offset opaco en la entidad. Por eso se integra la secuencia confirmada y se documenta esta diferencia; no se declara una copia literal completa del binario.

Un fallo del acceso o de la actualización de predicción cancela nuevos extras y conserva el estado normal del motor. Una interrupción parcial ajusta el historial al número de comandos generados para las reproducciones posteriores. La política de paquetes observada en Raze sigue sujeta a las reglas del servidor; una animación o un ACK no demuestra daño.

## Verificación

- `rapid_fire_tests`: 69 comprobaciones, incluidos armado, liberación, reutilización de slots y argumentos de cada extra.
- `runtime_api_tests`: 85 comprobaciones; el mock nativo valida la llamada real al slot 3, sus cuatro argumentos y la disponibilidad de la corrección antes de predecir.
- También pasan persistencia de configuración y presupuesto de movimiento.
- La compilación y el empaquetado se registran en `build/raze-rapid-integration/`.
- Daño efectivo y comportamiento en servidores dedicados: pendientes de prueba en partida.

## Evidencia

`docs/re/rapidfire/recheck-20261019/` contiene pseudocódigo, desensamblado, referencias a los globales, operandos y cotejo del motor. La función de CreateMove se exportó en ocho páginas para comprobar sus 749 instrucciones; el total de las cinco funciones es 1432.

Base con las nuevas anotaciones: `C:\Users\User\.codex\tools\ida-analysis\raze\raze-rapidfire-integrated-20261019.i64`.

La DLL anterior queda en `build/raze-rapid-integration/previous-3.1.7/`. La entrega se publicó como módulo **3.1.9** mediante el panel privado de Vercel, con firma y descarga autenticada verificadas. El módulo **3.1.10** corrige el desplome de frame rate al combinar aimbot y rapid fire: la selección de objetivo se resuelve una vez por tick y la reutilizan todos los comandos extra. Se publicó por el panel privado de Vercel con firma y descarga autenticada verificadas. Detalle en `docs/AIM_SELECTION_REUSE.md`. El loader **3.1.3**, disponible en Accounts, obtiene automáticamente el módulo nuevo tras validar la licencia. La API de licencias sigue en el servicio actual; no se desplegó el portal privado antiguo de Sites. El registro de publicación queda en `build/raze-rapid-integration/publication.json`.
