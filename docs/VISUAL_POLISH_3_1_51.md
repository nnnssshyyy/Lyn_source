# 3.1.51 — Claridad del ESP y HUD

Los nombres, distancias y estados del ESP usan fondos oscuros discretos y
sombras que respetan el alfa del texto. Los marcos se alinean al píxel y su
contorno tiene más contraste. No se cambia el tamaño del volumen proyectado
ni se activa box precision por defecto.

Las barras de vida muestran el carril vacío y un relleno con brillo ligero.
La paleta de vida pasa por rojo, ámbar y verde; conserva los colores por tramos
y la separación de vida temporal. Los estados a la derecha reservan espacio
para la barra y el valor de vida cuando se colocan en ese mismo lado.
La vista previa de Hunter comparte los colores y el tratamiento de etiquetas.

El hitmarker confirmado se contrae y desaparece gradualmente; los impactos
en cabeza usan un tono ámbar. El daño flotante se presenta en pequeñas
etiquetas, prioriza el más reciente y usa una rejilla acotada para evitar
superposiciones. El sufijo HS procede del evento de daño en cabeza existente.
No se generan confirmaciones a partir de pulsar ataque.

Las flechas fuera de pantalla usan la paleta del tipo de infectado; cada
sector conserva la amenaza más cercana y sus etiquetas evitan solaparse.
La trayectoria estimada utiliza una línea progresiva y un marcador de final
más legible, manteniendo la simulación y sus límites anteriores.

El panel de Tank/Witch separa nombre, vida exacta, distancia y estado. La
barra principal refleja la vida actual; una franja secundaria anima la pérdida
de vida. El editor muestra un ejemplo marcado DEMO cuando no hay jefes activos;
ese ejemplo no aparece al cerrar el editor. Se conservan arrastre, tamaño,
escala y opacidad del panel.

No se añaden opciones persistentes ni se sustituyen perfiles, binds o colores
por categoría. Estas modificaciones afectan al render y la distribución de
etiquetas; no añaden lecturas del juego ni alteran comandos de combate.

Validación: Win32 Release, 26 suites sin fallos; 43 comprobaciones de render
y 36 de lógica visual. Se inspeccionan capturas del menú y un ejecutable DX9
que utiliza el HUD y las primitivas de producción sobre datos simulados, con
eventos de 40, 110 y 200 ms. La escena de captura no es una partida real.
