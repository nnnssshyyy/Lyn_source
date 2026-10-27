# Aumento de rapid fire y recuperación de ráfagas largas: 3.1.21

**Histórico:** el usuario confirmó que la pausa persistía y que el modo agrupado
había cambiado el rapid fire. [3.1.22](RAPIDFIRE_RECOVERY_3_1_22.md) retira esa
ruta y añade una recuperación nativa después de confirmarse la última ráfaga.

El usuario confirmó que quería aumentar la velocidad de los métodos actuales.
Se añade **boost 2x**, habilitado por defecto para perfiles nuevos y anteriores,
en la misma sección de rapid fire. Mantiene el factor de cada método y dobla
sus movimientos extra: por ejemplo, factor 6 produce 12 extras y factor 200
produce 400. Desactivarlo recupera el ritmo de 3.1.20. No activa rapid fire si
el perfil lo tenía apagado. Su estado se guarda por perfil.

## Continuidad al soltar

Al revisar las ráfagas largas se encontró que limpiar la posición módulo 150
de un extra también borraba una corrección de otro comando base anterior. La
limpieza se realiza antes de actualizar la predicción y de que el callback
nativo copie el comando nuevo, por lo que podía retirar el desplazamiento que
la predicción todavía necesitaba. Con un factor de 200 ya se podía alcanzar
este caso dentro de una sola ráfaga.

Ahora `Ring::Clear` limpia únicamente la identidad completa pedida. Los extras
sin corrección y los `Set` de cero no borran una base distinta pendiente. Un
armado nuevo puede sustituir una base anterior en su posición, manteniendo el
recuento correcto y evitando lecturas por alias. Los ACK siguen retirando las
correcciones confirmadas. El tamaño del historial nativo del motor no cambia.

Se conservan la cancelación al soltar, la devolución de ticks prestados no
utilizados, la corrección solo del jugador local, la ausencia de una llamada
manual a predicción estando inactivo y la restauración del canal después de
cada movimiento nativo. No se vuelve a imponer espera de ACK ni un máximo de
catorce extras de rapid fire. Se conservan los cambios de captura de 3.1.18.

## Ping al soltar y cola de red

El usuario precisó después que el ping subía al soltar y que el cuerpo del
personaje permanecía detenido mientras podía mover la vista y desplazarse sin
animación. No se dispone de mediciones de esa sesión para atribuirle una única
causa. En el código se identificaron dos caminos concretos de presión de red:
forzar un paquete por extra y declarar ACK/choke artificiales en cada uno.

Se añade **stable network**, activado por defecto también en perfiles
anteriores. Conserva todos los extras y los argumentos originales de CL_Move,
pero agrupa comandos reales en mensajes de hasta quince comandos. El último
extra siempre envía la cola, y si la tecla se suelta a mitad de la ráfaga, el
primer movimiento normal envía lo pendiente. No genera extras de recuperación
ni espera confirmaciones para armar otra ráfaga. Una ráfaga de 400 extras más
su base se serializa en 27 mensajes en la simulación, conservando sus 401
movimientos. El límite es por mensaje, no por fotograma ni por ventana de ACK.
La ráfaga se arma en el tick final de envío nativo; los ticks intermedios
ejecutan su movimiento normal sin añadir extras a un tick que no puede enviar.

Esta ruta deja las secuencias y los contadores del canal en manos del motor:
no escribe ACK -1 ni declara 255 paquetes retenidos. Al desactivar el control
se conserva la ruta de red histórica de los tres métodos. En esa ruta se
corrige además la restauración después de SetChoked: si el motor incrementó
la secuencia y el contador artificial en uno, se conserva ese paquete real
sin dejar el contador 255 convertido en 256.

Si no se puede resolver el contexto de envío, no se arma una ráfaga nueva.
Al soltar se registra localmente outgoing, ACK, comandos pendientes de envío,
tickbase y RTT; esos datos permiten distinguir una cola local de confirmaciones
retrasadas en una sesión posterior. No se envían a ningún servicio.

El cálculo por intervalo limita su conversión a entero antes de multiplicar,
de modo que un intervalo positivo extremadamente pequeño no desborda el
recuento de movimientos.

## Verificación

Las regresiones de dos segundos pasan por el ejecutor de producción con los
tres métodos, factores 6/40/200 y boost activado/desactivado. Exigen el recuento
completo en cada fotograma pese al retraso de ACK y pérdidas intermitentes;
además verifican que la base siga disponible durante todos los extras, que
se retire al confirmarse y que no haya actualización manual al soltar.

La prueba de API ejecuta el slot nativo simulado de Prediction::Update durante
400 extras y comprueba que conserva el desplazamiento incluso después de dos
vueltas al anillo. También se prueban cancelaciones al inicio, a los tres
extras, a mitad de la ráfaga y al completarla, junto con la persistencia de un
boost desactivado y el valor por defecto en perfiles anteriores.

Las regresiones de red comprueban el empaquetado completo, el envío de la cola
tras cancelaciones y el transporte nativo simulado sin secuencias ficticias.
Cubren también la rama SetChoked de la restauración histórica y el guardado de
una preferencia explícita por esa ruta.

Release Win32 compiló y pasaron **21 suites**, incluidas **225 comprobaciones
de rapid fire**, **121 de API nativa**, **26 de persistencia**, **21 de
presupuesto de movimiento** y **44 de captura**. La compilación final se hizo
después de la última modificación de fuentes.

No había un proceso de L4D2 disponible para medir la cadencia, el daño aceptado
por un servidor ni comprobar visualmente la desaparición de la pausa. El
refuerzo duplica los movimientos locales solicitados; no supone una garantía
de daño doble ni de la frecuencia que acepte un servidor concreto.

Respaldo de fuentes anterior: `build/rapidfire-boost-source-3.1.20/`.

## Entrega

El 22 de octubre de 2026 se publicó el módulo **3.1.21** mediante el panel
privado de Vercel y la API de licencias vigente, con firma y descarga real
verificadas. DLL comprimida: **3 655 680 bytes**, SHA-256
`28bff5947a95efd6a33f0e0a75387632893632885ec25c2e8678abd2bd0a5c0d`.
Se verificaron carga x86, reubicación, los once recursos y las secciones tras
descompresión. El paquete y el respaldo de la DLL 3.1.20 están en
`build/rapidfire-boost-3.1.21/`.

El loader continúa en 3.1.8. Para recibir el módulo actual hay que reiniciar
L4D2 y volver a cargarlo; no hace falta descargar otra vez el loader. En el menú
de rapid fire quedan activados por defecto **boost 2x** y **stable network**.
La comprobación visual del ping y de las animaciones sigue pendiente de partida.

Accounts publicó ambas referencias de módulo desde el commit `cd48746`, con
35 pruebas correctas. El despliegue de producción
`AtQVJ9HjTjsaByo2eetd7G25BjdW` terminó como Ready.

## Revisión posterior de los registros locales

Al continuar se encontró `lyn.log` junto al módulo 3.1.21 en la caché del
loader. Confirma la instalación de TickMove y RunCommand, pero no contiene
eventos de armado, ACK ni liberación de rapid fire. Por tanto, la carga del
módulo está observada; todavía no hay una prueba de ráfaga y recuperación de
esta versión que permita atribuir un resultado al cambio de red.

El registro de 3.1.20 sí contiene ráfagas de 200 extras en el método Storm,
intervalo 0.03333 y confirmaciones de comandos. Esas confirmaciones no prueban
daño ni sincronía de animaciones. El informe limitado a datos de rapid fire
queda en `build/rapidfire-log-audit-3.1.21.json`. No se publicaron cambios nuevos
basados únicamente en esos registros de una versión anterior.
