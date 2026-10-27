# Visuales y privacidad: módulo 3.1.15

**Documento histórico.** La entrega 3.1.17 sustituye las rutas de fog/vomit y
limita el coste de la privacidad. Véase [CHARGER_AND_RENDER_FIXES.md](CHARGER_AND_RENDER_FIXES.md).

En World, junto a las opciones de objetos, están `no fog`, `no vomit` y
`OBS / Discord privacy (F8)`. Se guardan por perfil y empiezan desactivadas.
Se eliminó `avoid label overlap`, incluido el descarte de etiquetas, y se
retiraron los bloques de instrucciones y diagnóstico del menú. Los errores
de configuración y los estados breves de disponibilidad siguen visibles.

No fog guarda ambos valores numéricos de `fog_override`, `fog_enable` y
`fog_enableskybox`, aplica la anulación mientras se ejecuta RenderView y los
restaura al salir. No vomit hace lo mismo con las dos marcas de tiempo de la
superposición de bilis del jugador local, resueltas por nombre de propiedad.
Una actualización ajena durante ese intervalo no se sustituye al restaurar.
No elimina efectos de bilis o daño que determina el servidor.

El modo de privacidad dibuja menú, ESP y paneles en una ventana separada y una
swap chain adicional. Los render targets, profundidad y viewport del juego se
restauran antes de presentar. No se altera el buffer de captura de OBS/Discord
ni se instalan hooks en esos programas. La nueva ventana usa
[WDA_EXCLUDEFROMCAPTURE de Windows](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowdisplayaffinity).
Se verifica que Windows haya aceptado ese valor; si falla la ventana privada
o el destino de render, esos dibujos no vuelven al fotograma principal.

Requiere ventana o ventana sin bordes y una versión de Windows compatible con
la exclusión de captura. F8 permite desactivarlo incluso si el overlay no se
puede mostrar. Al perder foco, minimizar o revocar la licencia se oculta.
Glows, colores de modelos y la salida del chat spy al HUD del motor se pausan
en este modo porque forman parte de la imagen del juego; las preferencias se
conservan y vuelven al salir. No se ofrece garantía ante todos los métodos de
captura: Microsoft limita expresamente el alcance de esta API.

La prueba nativa crea dos ventanas de prueba, confirma la afinidad de captura
y dibuja un color distinto en cada swap chain. Comprueba que el buffer principal
mantiene sus píxeles originales y que el destino y viewport se restauran. No
equivale a una prueba real grabando una partida con OBS o Discord. Las pruebas
de efectos cubren fallos de metadatos, aplicación parcial, restauración,
actualizaciones externas y render anidado.

El loader 3.1.6 añade transiciones con easing al abrir, cerrar e iniciar sesión,
interpolación del ancho lateral, hover suave e iluminación a 60 Hz. La animación
del fondo se pausa al minimizar y respeta la preferencia de animaciones de
Windows. Mantiene autenticación, carga firmada y limpieza de la versión 3.1.5.
# Actualización 3.1.17

Las rutas de fog/vomit y el coste de la privacidad descritos originalmente abajo
se sustituyeron según [CHARGER_AND_RENDER_FIXES.md](CHARGER_AND_RENDER_FIXES.md).
La escritura temporal de temporizadores/convars ya no se utiliza.

