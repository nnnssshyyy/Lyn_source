# Movimiento y Hunter: módulo 3.1.28

Airstuck pasa de escribir el tipo de movimiento local a editar el tick del
comando, como la ruta comprobada en Raze (`0x101472B0`, tick `0xFFFFFF`).
Se aplica con el bind mantenido, jugador vivo y móvil, fuera del menú. Al
soltar o desactivar se conserva el tick nativo de cada comando, incluso si
predicción vuelve a visitar uno modificado. No se cambia el número, la
semilla ni el tipo de movimiento de la entidad. Se solicita envío normal.
La aceptación del exploit depende del servidor; falta comprobarlo en partida.

## Controles separados

- **Auto ceiling:** bind inicial Mouse4. Mientras se mantiene, fija exactamente
  pitch +12 grados y conserva el giro horizontal del mouse. No genera ataques.
- **Hunter auto click:** bind inicial Mouse5. Genera pulsos nativos de ataque
  cada 100 ms, con una liberación entre pulsos, solo con Hunter vivo, fuera de
  fantasma, menú, incapacidad, natación o escalera. El usuario mantiene agacharse
  para cargar pounce. No utiliza SendInput ni cambia el estado del mouse de Windows.
- Los binds son editables y se guardan por separado. Mientras cualquiera de
  estas ayudas está actuando, la selección automática del aim no sustituye
  el ángulo solicitado ni las liberaciones de ataque.

## Más movimiento y melee

Fast bunnyhop activa también el solver de aceleración aérea cuando bunnyhop
está habilitado, aunque auto strafe esté apagado. Conserva la dirección WASD,
la aceleración del servidor, la fricción y el límite nativo de proyección de
30 u/s; no añade CL_Move ni aumenta por sí solo el ritmo de envío. Si auto
strafe ya estaba encendido, el solver era el mismo y no hay una segunda
aceleración acumulada. El modo rápido está preparado por defecto; bunnyhop
conserva su estado previo.

Fast melee, al mantener ataque con melee y primaria en inventario, deja
120 ms de swing y cambia a la primaria y de regreso usando las entidades
reales del inventario. Espera los cambios observados del arma activa y limita
la espera a 400 ms por etapa. Un cambio manual, recarga, uso de objeto,
incapacidad, muerte o cambio de entidad cancela la ayuda. La selección usa
los campos nativos `weaponselect/weaponsubtype` antes del CRC. No se escriben
cooldowns del arma. El resultado de golpes y la mejora de cadencia requieren
prueba con las armas y el servidor de destino.

## Visión azul

Se localizaron los envíos de pesos de color de infectado y fantasma en el
cliente instalado. Se elimina solo el peso de esas dos llamadas; los pesos
correspondientes a tercer strike, mapa e introducción conservan su recorrido.

Cliente SHA-256: `c0a8e1e88f7312db11bcfd4b0ec574d224a34b2d9a615d6088b28335eeb4af1f`.
Firma única en RVA `0x27153a`; método de pesos RVA `0xdba50`; retornos
infectado `0x271552` y fantasma `0x271582`. Se comprueba que las llamadas
resuelvan al mismo método y se valida su prólogo. El método usa thiscall con
tres argumentos de pila (12 bytes). La opción restaura el comportamiento
original inmediatamente al apagarse. Una firma ausente se muestra como
no disponible. Evidencia: `build/movement-3.1.28/native-vision-validation.json`.

## No fall damage automático: experimental

Según la aclaración del usuario, se implementa como activación automática del
lag exploit. En survivor móvil, durante caída por debajo de -550 u/s, se
traza una caja en los pies dos ticks hacia delante. Un suelo cercano y
transitable solicita un único push de secuencia usando el valor del slider
sequence push (10–1000, valor inicial 100). No necesita mantener lag key ni
habilitar lag exploit manual.

Se rearma después de aterrizar, con 500 ms de separación mínima. No se repite
por FPS ni por reprocesar el mismo comando. Se pausa con rapid fire, incluida
su recuperación pendiente, speedhack mantenido y lag manual. No retiene
paquetes en un bucle ni rebobina la secuencia del canal al terminar. El menú
muestra un contador de pulsos y el log registra velocidad y comando.

**El envío de un pulso está implementado y probado; la supresión del daño
no está validada en partida.** Un salto de secuencia no demuestra por sí solo
que el servidor vaya a omitir daño. No se promete inmunidad. La bandera de
no-fall de la referencia analizada no mostró un consumidor runtime útil;
no se copió un ajuste que solo existía en la configuración.

## Comprobaciones y entrega

Release Win32 y 21 suites nativas: 0 fallos. Incluyen 612 comprobaciones de
movimiento y ayudas, 30 de rutas visuales con ABI nativa, 162 de API runtime,
27 de persistencia y 211 de rapid fire. Accounts: 35 pruebas, 0 fallos.
El menú real se renderizó fuera de partida con Direct3D9 y objetos de
producción, sin la entrada de inyección: `build/movement-3.1.28/movement.png`
y `exploits.png`. No hubo prueba en partida ni medición de FPS.

Integridad UPX, carga nativa, reubicación, once recursos y las secciones al
descomprimir: correctas. DLL: 3 661 824 bytes; SHA-256
`6e1d113de61d297be5e1f02c5e6b7ba9151f415ff44e1ce5058b97b0348b821a`.
El panel privado confirmó versión, firma y descarga real. Loader 3.1.10,
su icono y su limpieza al cerrar se conservan.

Respaldo de fuentes: `build/movement-source-3.1.27/`; módulo anterior:
`build/movement-3.1.28/previous-3.1.27/`. Análisis estático de referencia y
cliente en `build/movement-research-3.1.28/`, sin ejecutar la DLL de terceros
ni modificar los binarios del juego.

Accounts: commit `06060e303dfbc5d8653baf1e1754b2696fd97890`.
Producción `8NajFiVz7fYM8eUDD15PsHq2ZcrX` confirmada Ready y Current en Vercel.
