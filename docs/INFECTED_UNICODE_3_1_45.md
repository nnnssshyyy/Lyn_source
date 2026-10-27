# 3.1.45 — Aim de infectado y nombres Unicode

En **Combate → Aimbot**, «Aim como infectado» habilita el seguimiento de supervivientes para Smoker, Boomer, Hunter, Spitter, Jockey, Charger y Tank. Requiere el interruptor principal de aim y respeta su tecla/modo de activación, FOV, distancia, suavizado y filtro de amigos. En activación por ataque acepta el botón primario y el secundario.

Esta modalidad utiliza la vista visible y poses actuales. La selección automática prefiere el torso y no hereda los filtros de clases de infectados destinados al superviviente. Las garras y habilidades se activan manualmente: no añade ataques ni calcula trayectorias balísticas. Se pausa en modo fantasma, al morir, al usar objetos y durante los movimientos especiales que ya bloquean la asistencia. La configuración de armas del superviviente permanece guardada.

Los nombres del ESP conservan caracteres UTF-8 completos y sustituyen bytes inválidos sin partir el siguiente carácter. Los controles no pueden introducir líneas adicionales. La fuente admite puntos de código de 32 bits: Inter y Noto Math integradas, más las fuentes de símbolos y kana disponibles en Windows. Esto cubre letras decorativas, corazones, estrellas, braille, griego, cirílico y emoji monocromos de las fuentes disponibles. No se promete cobertura de todo Unicode ni composición de secuencias emoji complejas. Se mantiene el límite de bytes que entrega el motor.

Validación: compilación Win32 Release; 348 comprobaciones de selección/cámara, 43 de perfiles y 102 de nombres/fuentes. El atlas resultante mide 4096 × 4096. Las capturas Direct3D de nombres y controles se guardan junto al paquete. No se ha probado dentro de una partida.

La opción «Precisión de cajas» introducida en 3.1.44 sigue desactivada por defecto.
