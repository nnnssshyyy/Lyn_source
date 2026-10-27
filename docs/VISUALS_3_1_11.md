# Visuales 3.1.12

En **Visuales → HUD / efectos** se activan hitmarker, daño flotante, flechas fuera de pantalla, HUD de Tank/Witch y trayectoria estimada de los tres arrojables. Los nuevos efectos empiezan apagados para respetar las configs existentes. En **Jugadores** se ajustan iconos, separación de vida temporal y distribución de etiquetas.

El editor del HUD permite arrastrar las cabeceras, cambiar el ancho con la esquina inferior derecha y ajustar tamaño, escala del HUD de jefes y opacidad. Radar, espectadores, chat y HUD de jefes conservan los ajustes dentro de cada perfil. El botón de restablecer recupera la distribución inicial. Las animaciones existentes del menú y de espectadores se mantienen.

## Datos y límites

- Daño: eventos recibidos `player_hurt` e `infected_hurt`, filtrados por el usuario local, con agregación breve de perdigones por víctima. Algunos eventos de infectados comunes no se transmiten a clientes remotos. Sin ese evento no se inventa un impacto ni un número de daño.
- Tank/Witch: salud replicada y máximo nativo, o máximo observado para la barra. `HP --` indica que el servidor no entrega salud. Las barras se interpolan y las cifras muestran el valor actual; no se simulan puntos de vida desconocidos.
- Arrojables: aproximación balística para molotov, pipe bomb y bilis, con hull traces, gravedad del servidor y rebotes limitados. La liberación real, scripts del servidor, latencia y objetos físicos pueden cambiar el vuelo. El extremo se identifica como estimado.
- ESP: verde para vida permanente, ámbar para temporal; barras de incapacitación conservan su escala propia. Iconos de incapacidad, captura, curación y reanimación. Los estados de acción caducan si se pierde su evento final.
- Coste: captura de amenazas a 20 Hz, trayectoria a 10 Hz con un máximo de 129 trazas por actualización, colas y buffers fijos. El hilo de renderizado usa snapshots y no consulta entidades.

## Validación

Release Win32 y 18 suites CTest: políticas existentes, persistencia, eventos x86, daño confirmado, física acotada, layout, dibujo ImGui y manejo de entrada. También se comprueban compresión, carga nativa reubicada, recursos y secciones al descomprimir. Esto no sustituye una prueba dentro de una partida: no se ejecutó L4D2 durante esta validación.

La revisión 3.1.12 valida la vida del jugador directamente desde sus netvars para habilitar la trayectoria, porque la captura visual puede dejar `Entity::health` sin rellenar. Tres pruebas cubren ese caso, muerte y falta de datos.

El loader 3.1.4 conserva su ejecutable comprimido. La entrega del módulo 3.1.12 continúa por Accounts y la API firmada vigente; no se realizó una migración a VPS.
