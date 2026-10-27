# Chat spy integrado en FREE

La opción está en **World → chat spy** y empieza apagada. El interruptor principal controla la recepción; **show panel** y **show in game HUD** eligen las salidas. Funciona con aimbot y ESP apagados.

El panel permite arrastrar el título con el menú abierto, recorrer mensajes con la rueda, vaciar el historial con **Clear** y cerrar solo el panel con la X. Tiene contador, nombres con color de equipo, texto multilínea ajustado al ancho, desplazamiento automático y ocultación cuando está vacío. El tema del menú controla el fondo y los textos. Las opciones y la posición pertenecen al perfil actual.

## Runtime

- `src/chat_spy.cpp` obtiene `GAMEEVENTSMANAGER002` y registra un listener cliente para `player_say` desde el hilo del juego. Lo retira al apagar la función, desconectarse o cambiar de nivel. El listener copia userid/texto a una cola acotada; no guarda punteros del evento.
- El procesamiento resuelve userid a slot, verifica de nuevo la identidad y compara equipos. Acepta emisores de los equipos 2/3 que sean distintos del equipo local. No exige que estén vivos.
- El hook de `CBaseHudChat::ChatPrintf` conserva los argumentos normales mediante formateo a un búfer de 4.096 bytes, igual al límite observado en el cliente instalado. Envía el texto al original como `%s`, por lo que porcentajes del mensaje no se interpretan como nuevos argumentos.
- El receptor espera 150 ms antes de emitir el evento, para admitir que el HUD normal llegue después. El filtro compara el texto normalizado y el userid cuando está disponible; en mensajes de HUD sin índice usa el nombre normalizado. La ventana de coincidencia es de 1.500 ms. Cada mensaje normal puede suprimir un solo evento.
- El historial y la cola de eventos tienen un máximo de 128 entradas; la cola y caché del HUD tienen un máximo de 250. Se preservan los bytes UTF-8 y se evita cortar caracteres al truncar. Las cadenas publicadas para dibujar son copias, sin acceso a entidades desde DirectX.
- La salida propia al HUD llama al trampolín original. No se vuelve a capturar a sí misma, y no elimina las entradas del panel.
- La limpieza periódica funciona desde el runtime aunque el panel esté oculto o se utilice solo la salida al HUD. El intervalo es 0–240 minutos. El cambio de mapa siempre vacía historial, caché y colas.

El código se escribió para el runtime del proyecto a partir de la [ruta analizada en Raze](CHAT_SPY_ANALYSIS.md). Se cambiaron las reglas de duplicados y los límites para permitir repeticiones legítimas y mantener acotada la memoria.

## ABI verificada con IDA Pro 9.4 / MCP

Se inspeccionó el `client.dll` instalado, SHA-256 `c0a8e1e88f7312db11bcfd4b0ec574d224a34b2d9a615d6088b28335eeb4af1f`.

| Función | VA en el cliente analizado | Resultado |
| --- | --- | --- |
| ChatPrintf | `0x10116260` | Firma única; cuatro argumentos fijos en la pila, incluido `this`, seguidos de varargs. Búfer nativo de `0x1000` bytes. |
| GetHudManager | `0x1010CDB0` | Firma única; recibe un slot. `-1` consulta el slot activo de pantalla dividida. |
| FindHudElement | `0x1010DBD0` | Firma única; `thiscall`, busca el nombre del elemento. |

Las direcciones se documentan para verificar el análisis. El runtime resuelve firmas únicas; no usa esas VA como direcciones fijas. Los índices virtuales de los eventos y del administrador proceden de las llamadas observadas en Raze: nombre 1, entero 6, texto 9, alta 3 y retirada 5. Userid a índice usa el método 9 del motor.

Los resultados de MCP están en `docs/re/raze/client-chat-signatures.json` y `client-0x*.json`.

## Validación

La DLL Release Win32 compiló. Las siete suites actuales pasaron: **182 comprobaciones**, incluidas 27 de lógica de chat, 15 de su runtime y 12 de persistencia. Las otras 128 cubren aim, API del runtime, presupuesto de movimiento y rapidfire.

`chat_logic_tests` comprueba normalización, conservación de Unicode, truncado, formato del HUD, orden de llegada de eventos, consumo de duplicados uno a uno, userids con nombres iguales, caducidad, limpieza y límites bajo ráfagas.

`chat_runtime_tests` ejecuta el módulo de producción con un motor simulado y un hook real de MinHook dentro del proceso de pruebas. Comprueba ABI del listener, slots de los getters de evento, registro cliente, recepción, equipos, paso de varargs y colores del HUD, salida por trampolín, porcentajes literales, limpieza y reconexión. Solo se sustituyen las llamadas al motor y la búsqueda de firmas.

Las pruebas de persistencia comprueban la recarga de las opciones nuevas y los valores predeterminados al cargar perfiles antiguos.

La recepción de chat del otro equipo, el aspecto del panel y la convivencia con otros plugins del juego todavía requieren una prueba en partida. La función depende de que el servidor entregue el evento correspondiente al cliente.
