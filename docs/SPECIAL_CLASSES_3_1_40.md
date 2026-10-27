# Priorizar clases — revisión local 3.1.40

El panel derecho de **Filtros** incorpora el checkbox **Priorizar clases**. Al activarlo, solo se admiten las clases marcadas: Smoker, Boomer, Hunter, Spitter, Jockey, Charger y Tank. No hay sustitución por otra clase sin marcar cuando el objetivo elegido queda oculto, sale del FOV o está fuera de alcance. Los comunes, Witch y supervivientes tampoco forman parte de esta lista exclusiva.

Si no se marca ninguna clase, no se elige objetivo y el menú muestra «Marca al menos una clase». Al desactivar el checkbox se recuperan los filtros normales, conservando las marcas guardadas para una activación posterior. Los perfiles anteriores dejan este nuevo modo apagado.

La selección se aplica a los tres modos de aimbot y al filtro compartido de elegibilidad del trigger. Conserva los controles existentes de equipo, amigos, fantasmas, visibilidad, distancia, FOV y categorías habilitadas. No modifica el empujón automático. El historial de poses y la retención del objetivo no pueden reincorporar una clase excluida.

## Validación

- Compilación Release de MSVC, x86.
- Seis suites nativas aprobadas: selección, política de aim, persistencia, API, dispersión y entrada primaria.
- 327 comprobaciones de selección/cámara y 36 de persistencia, incluyendo las siete clases, los tres modos y las tres políticas de ordenación.
- 81 comprobaciones de comportamiento de UI; capturas en español e inglés y escalas de 100 %, 125 % y 150 %.

Los archivos y logs están en `build/special-classes-3.1.40`. `lyn.dll` es una compilación local sin comprimir; no se publicó mediante la API de licencias ni se verificó dentro de una partida. La entrega publicada del módulo permanece en 3.1.37.
