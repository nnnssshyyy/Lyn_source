# Vista previa integrada y marca de agua: 3.1.26

Jugadores integra el Hunter 3D en la parte superior de la columna derecha.
El panel de colores y efectos se desplaza por separado debajo del modelo.
La vista conserva los clips de reposo, caminar y correr, pausa, giro con el
ratón, zoom con la rueda y colocación de salud por arrastre. Nombre, salud
y distancia tienen espacio por encima y debajo de la textura del modelo.
El selector de hitboxes se abre expresamente desde su botón.

La marca de agua muestra el icono actual del menú, VS 3 y FPS. Se dibuja en
la esquina superior derecha incluso con el menú cerrado, a partir del
contador de frames de ImGui. Respeta la transparencia del HUD y el color
del tema. No captura el juego ni añade desenfoque, texturas o ventanas.
Visuales → HUD / efectos permite ocultar la marca o solo el contador de
FPS. Ambas opciones se guardan con el perfil y empiezan activadas.

El código de rapid fire, red, predicción y lectura de vida no se modifica.
El respaldo anterior está en `build/menu-preview-source-3.1.25/`.

## Validación

Release Win32 compila y pasan las 21 suites existentes. La captura
`build/menu-preview-qa/preview.png` usa los objetos reales de la DLL y
Direct3D9, omitiendo el punto de entrada del módulo y sin abrir el juego.
El valor de FPS de esa captura es ilustrativo. La prueba en partida y la
medición del impacto de FPS quedan pendientes.

El arnés de captura también comprobó el arrastre real de salud de izquierda
a derecha, incluida la suelta, y el dibujo de la marca con el menú cerrado
y su ocultación al desactivarla. Accounts pasó sus 35 pruebas. La entrega
comprimida pasó integridad UPX, carga x86, reubicación, once recursos y
conservación de secciones tras descomprimir.

## Entrega

Módulo 3.1.26 publicado y comprobado mediante firma y descarga desde el
panel privado habitual. La DLL tiene 3 658 752 bytes y SHA-256
`8493fa8590052bd3b52ca5b2c855201194867ed8b3c9cccc433924ac6b7b949b`.
El loader conserva la versión 3.1.9 y el icono del usuario. Para actualizar,
reiniciar L4D2 y volver a cargar el módulo desde el loader.

Accounts: commit `bec12ce920cab4d0aaa1c30703fc7409eed81ebf`.
Producción `227mdsDLX3r3Wi2Fjq74n3c7g3SM` confirmada como Ready en Vercel.
