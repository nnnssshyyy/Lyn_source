# Combate 3.1.39 — validación local

## Entrada primaria durante una captura

Se corrigió un borrado de M1 en la rama de jugador incapacitado o atrapado, antes de seleccionar el modo de aimbot. Con `pistol_rapid` activo, un arma no semiautomática producía `Next(false)` y ese resultado se aplicaba a M1, eliminando el ataque manual en cada comando. También ocurría si no se podía resolver el arma.

`combat_input::PausedPrimary` conserva ahora el comando completo para armas no semiautomáticas o desconocidas. El pulso solo se aplica si se resolvió positivamente un arma semiautomática. El arreglo se comparte entre los modos de aim. La pausa de asistencia y las restricciones de disparo impuestas por el motor durante una captura siguen siendo responsabilidad de sus estados actuales.

Este defecto dependía de `pistol_rapid`; no se ha confirmado que explique todos los casos que el usuario observó. Falta comprobarlo en una partida con Smoker.

## Dispersión

Se revisó la ruta angular de las seis familias hitscan: pistolas, subfusiles, rifles, escopetas, francotiradores y M60, correspondientes a 19 entradas del catálogo. Se mantiene el muestreo en el reloj del tick y su restauración.

- La corrección solo se entrega cuando ambos ejes son válidos y la semilla se restauró.
- Los valores finitos que exceden el rango solicitado al generador se rechazan.
- Los comandos sin número o semilla y las familias desconocidas se rechazan antes del muestreo.
- Los fallos no publican una corrección parcial.

No se añadieron familias nuevas. En escopetas se compensa el eje del disparo; no se elimina el patrón individual de los perdigones. Los proyectiles, armas cuerpo a cuerpo y objetos no utilizan esta compensación angular. Los perfiles personalizados mantienen su configuración de no spread.

## Verificación

La DLL se compiló en Release, MSVC x86. Pasaron las cinco suites seleccionadas: `aim_policy`, `aim_selection`, `runtime_api`, `shotgun_spread` y `combat_input`. Incluyen 125 comprobaciones de dispersión, 11 de entrada durante la pausa, 239 de selección/cámara y 188 de API. El log íntegro está en `build/combat-3.1.39/tests.log`.

Artefacto local: `build/combat-3.1.39/lyn.dll`. La entrega publicada permanece en el módulo 3.1.37; no se ha publicado esta DLL. No se ha realizado una prueba dentro del juego ni en Windows LTSC para esta revisión.
