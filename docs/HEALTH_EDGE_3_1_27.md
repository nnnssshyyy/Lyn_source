# Vida alineada al relleno y sin pista negra: 3.1.27

El número de vida se coloca a partir de los límites visibles de sus glifos,
sin incluir el espacio vacío de la altura de línea de la fuente. Conserva
12 píxeles y se centra en el eje del relleno; su borde inferior queda a un
píxel nominal del extremo superior de la barra vertical. El relleno se
ajusta a píxeles enteros y el texto sigue exactamente sus mismos pasos.

Se elimina el rectángulo negro que recorría toda la altura o anchura de la
barra. Solo se dibuja la vida restante. Las marcas de segmentación, cuando
están activas, se limitan al relleno y no dejan líneas sobre la zona vacía.
El número conserva su sombra fina para que se pueda leer sobre el juego.

La vista previa del Hunter usa también los límites visibles de su fuente
y deja de dibujar la pista negra. La lectura de vida permanente, temporal
e incapacitación conserva su formato y sus cálculos.

## Validación

Release Win32 y las 21 suites existentes pasan. El renderer de comprobación
`build/health-edge-preview/` usa la misma fuente del overlay, los mismos
helpers de colocación y Direct3D9. Comprobó valores 100, 74, 50, 25 y 1:
en todos, el límite del glifo queda a 1,40 píxeles del relleno y el error
horizontal es menor de medio píxel. La captura es fuera de partida; falta
comprobar el resultado en el juego.

Respaldo de los fuentes anteriores: `build/health-edge-source-3.1.26/`.

## Entrega

El panel privado confirmó la publicación 3.1.27, firma y descarga real de
la DLL con SHA-256
`db51e762af779f22b6af220dca4b834c61577d15922927a74b311ff2c83766df`.
La entrega tiene 3 658 240 bytes. Pasaron las 35 pruebas de Accounts,
integridad UPX, carga nativa, reubicación, once recursos y conservación de
las secciones al descomprimir. El loader conserva su versión 3.1.9.

Accounts: commit `9a3b0fe35f1bab429faadb5facd8f6da7081bac3`.
Producción `7r6JGZJn5ZDBVUPtccj3rHKkyw5M` confirmada como Ready en Vercel.
