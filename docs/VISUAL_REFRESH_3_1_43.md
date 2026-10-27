# Transparencia, impactos y partículas · 3.1.43

El menú, Spectators, el panel de chat, el radar, el HUD de jefes y la marca de agua tienen fondos más transparentes. El texto y los controles conservan su contraste; el ajuste de opacidad del HUD continúa aplicándose al perfil. Se redujeron también las capas del fondo espacial y de las cabeceras, cuya superposición volvía opaca la ventana.

En **Visuales → HUD / efectos**, **Partículas al morir** añade destellos del color del tema en la posición comunicada por `player_death`, tanto para jugadores como para NPC. Cantidad: 8–32 partículas; duración: 0,4–2 segundos. Hay un máximo de 24 ráfagas simultáneas (768 partículas), con caducidad y limpieza al cambiar de mapa o desactivar la opción. Los controles se guardan en el perfil.

El hitmarker admite `player_hurt_concise`, resuelve correctamente el índice de su atacante y deja de exigir una entidad de render válida para conocer el userid local. Los eventos detallados y resumidos se emparejan una sola vez para evitar sumar el mismo daño dos veces. Las bajas confirmadas por `player_death` o `infected_death` también activan la marca central, sin generar una cantidad de daño ficticia.

## Límites

- Los archivos `resource/modevents.res` del juego instalado marcan `player_hurt` e `infected_hurt` como eventos locales no transmitidos por la red. Los impactos no letales de infectados comunes pueden no producir una marca en clientes remotos si no se recibe un evento de daño. Esta revisión no modifica esa política del servidor.
- Las partículas necesitan una posición válida del evento o una posición reciente de la víctima. No se usa `infected_id` como si fuera un índice de entidad. Muertes abortadas, posiciones inválidas y notificaciones duplicadas se descartan.
- La representación es un efecto 2D proyectado desde coordenadas del mundo, sin colisión física de las partículas.
- Las pruebas de eventos simulados y de dibujo no sustituyen una prueba dentro de una partida.

## Fuentes y validación

La definición de los eventos se comprobó contra los recursos de la instalación local de L4D2. Las interfaces se contrastaron con el [SDK de L4D2, `igameevents.h`](https://github.com/alliedmodders/hl2sdk/blob/l4d2/public/igameevents.h) y [`cdll_int.h`](https://github.com/alliedmodders/hl2sdk/blob/l4d2/public/cdll_int.h).

Las suites `visual_events`, `visuals_logic`, `visuals_runtime`, `visuals_render` y `settings_persistence` cubren decodificación, identidad local sin entidad visible, deduplicación, bajas de NPC, límites, caducidad, generación de geometría y persistencia. Los comprobantes de compilación, resultados y capturas se guardan junto al paquete de la entrega.

Descargas en Accounts utiliza ahora las variables de la paleta compartida con Inicio para tarjetas, selección de edición, iconos y progreso. El cambio se publicó mediante el repositorio existente de Vercel.
