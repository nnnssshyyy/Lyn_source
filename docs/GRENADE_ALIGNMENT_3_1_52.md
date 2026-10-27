# 3.1.52 — Alineación de la trayectoria de granadas

La aproximación anterior desplazaba el origen con pitch −10°, añadía 120
unidades/s hacia arriba y multiplicaba la velocidad del jugador por 1,25.
Además reutilizaba la curva hasta 100 ms incluso cuando cambiaba la vista,
la posición o el arma. Eso separaba el inicio de la mira y dejaba una curva
correspondiente a una orientación anterior.

La rama de lanzamiento normal de CBaseCSGrenade del server.dll instalado
utiliza el vector frontal de los ojos, un hull de liberación de 16 unidades
y velocidad frontal × 750 más la velocidad del jugador. No contiene esos
dos sesgos adicionales. La comprobación estática y el hash del binario están
en [re/grenade-alignment/native-launch.json](re/grenade-alignment/native-launch.json).
No se cargó ni modificó ese binario para obtener la evidencia.

La predicción de Molotov, pipe bomb y bile jar usa esa base. La simulación
parte del endpoint del hull de liberación, también ante un impacto parcial
con una pared cercana. Fallos de lectura, hull sólido, arma no compatible,
desactivación o cambio de mapa borran inmediatamente la curva anterior.

La caché sólo conserva una curva si jugador/arma con serial, ojo, ángulos,
velocidad, gravedad, horizonte y modo de impacto permanecen idénticos.
Una vista o posición nueva se recalcula en ese frame. Los estados estáticos
se refrescan cada 100 ms. El límite es un hull de liberación y hasta 128
trazas de vuelo, siempre en el hilo de captura; el render no añade trazas.

Validación: 98 comprobaciones de lanzamiento, proyección, liberación y caché.
Incluye 75 combinaciones de pitch, yaw, FOV y relación de aspecto: el error
máximo del origen respecto al centro fue 0,002075 píxeles en la proyección
de prueba. Es una prueba de geometría, no una captura de una partida.

La física del vuelo continúa siendo estimada. Props móviles, plataformas,
scripts del servidor, el instante real de liberación y VPhysics pueden
alterar el impacto o los rebotes. No se ha validado el impacto en una partida
real durante esta revisión. No se alteran los ángulos ni comandos del jugador.
