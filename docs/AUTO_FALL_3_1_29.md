# Activación durante toda la caída: 3.1.29

La 3.1.28 solo solicitaba un pulso con velocidad vertical inferior a -550 u/s
y suelo cercano. Esa condición no correspondía al comportamiento aclarado
por el usuario: activar lag automáticamente mientras el jugador cae.

La 3.1.29 usa el estado de suelo y la velocidad vertical de cada comando:
fuera del suelo y con velocidad Z negativa, solicita lag. Se activa desde
el primer descenso, incluso en una caída corta o lejos del suelo. Continúa
durante toda la fase descendente y deja de pedir pushes de secuencia en el
primer comando sobre suelo o con velocidad ascendente/cero. No hay espera
por distancia al suelo, velocidad mínima de -550 ni cooldown entre caídas.

No hace falta mantener lag key ni activar lag exploit manual. Se usa el
valor de sequence push (10–1000). Un comando nativo puede pedir una sola
actualización; los replays no multiplican las escrituras. No se crea una
cola de paquetes retenidos ni se escribe un tick futuro. Al terminar cesan
las escrituras, sin rebobinar la secuencia propia del motor.

La ayuda está disponible para jugadores vivos de equipos 2 y 3, móviles,
fuera de fantasma, natación, escalera o menú. Incapacidad, cambio de nivel,
pérdida de foco, datos inválidos y desactivación cancelan su estado. Rapid
fire y su recuperación pendiente, speedhack mantenido y lag manual activo
conservan prioridad para evitar dos propietarios del canal a la vez.

El menú describe la activación durante el descenso y la liberación al
aterrizar. El contador pasa a llamarse Fall updates. El log registra el
inicio y el final de la fase, en lugar de escribir una línea cada comando.

## Verificación

Release Win32 y las 21 suites existentes: 0 fallos. La suite de movimiento
incluye 716 comprobaciones: descenso inmediato, duración de caída prolongada,
aterrizaje, rebote, activación a mitad de caída, owner nuevo, datos inválidos
y replays. Las pruebas de 30, 60 y 100 comandos impiden que ocho replays por
comando multipliquen los pushes. Las 211 comprobaciones de rapid fire y las
162 de API runtime conservan su resultado. No hubo prueba en partida.

**Se comprobó la lógica de activación y liberación; la eliminación del daño
sigue siendo experimental y depende del comportamiento del servidor.**

La DLL tiene 3 662 336 bytes, con SHA-256
`1a22253d4b31b100f6b185b47aef3e66e06f799ea5e9d79cb934f159caa7d8cf`.
UPX, carga nativa, reubicación, once recursos y conservación de secciones al
descomprimir: correctos. Loader 3.1.10 se conserva.

Respaldo de fuentes: `build/fall-source-3.1.28/`; binario anterior:
`build/fall-3.1.29/previous-3.1.28/`.

El panel privado verificó firma y descarga reales de la DLL 3.1.29.
Accounts: 35 pruebas, 0 fallos; commit `f24c9bf3b72dee4813b82678f5fc2b74e19f7256`.
Producción `Hego9eqjCwcgend6jiRzEsvRnhHy` confirmada Ready y Current en Vercel.
