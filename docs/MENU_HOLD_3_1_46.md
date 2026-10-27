# 3.1.46 — Menú organizado y hold propio para infectados

**Combate → Aim infectado → Tecla de aim infectado (mantener)** permite asignar una tecla exclusiva. El valor inicial es Mouse5. La asistencia funciona mientras se mantiene pulsada y se detiene al soltarla, incluso si el aim general usa «Siempre» o «Al disparar». Requiere el interruptor principal de aim y «Aim como infectado». La tecla general, su activación y la configuración de armas se conservan por separado. Los perfiles anteriores reciben Mouse5 cuando aún no incluyen este campo.

Distribución del menú:

- Combate: Aimbot, Aim infectado y Armas. Activación, seguimiento, zonas, compensación e historial tienen sus propios grupos.
- Filtros: Objetivos y Automatización. El disparo al apuntar, la asistencia de armas y el empujón automático están juntos.
- Jugadores: General, Apariencia y Colores, con la vista previa al lado.
- Visuales: Mundo, Radar/espectadores, Chat, Efectos y HUD. La visibilidad y privacidad están en Efectos; el tamaño de los paneles está en HUD.
- Varios: Movimiento, Cámara y Otros. Cámara, viewmodel y mundo aprovechan la altura disponible.
- Perfiles: conserva su gestión actual. La búsqueda lleva a las nuevas ubicaciones.

Se conservaron todos los campos que aparecían en el menú anterior. «Precisión de cajas» sigue desactivada por defecto. El aim de infectado conserva el seguimiento visible y los ataques manuales introducidos en 3.1.45.

Validación: compilación Win32 Release, pruebas de activación hold y persistencia, y revisión de capturas Direct3D del menú en español y en tres escalas. No se ha probado dentro de una partida.
