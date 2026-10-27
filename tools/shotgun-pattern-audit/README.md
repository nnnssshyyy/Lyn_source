# Auditoría offline de patrones

Compilar con MSVC Win32 y ejecutar `pattern_audit 0` para contrastar todos los
presets con CUniformRandomStream de vstdlib instalado. No carga client.dll,
no abre el juego, no inyecta y no conecta a servidores. El directorio de Steam
está indicado en el fuente. Un argumento positivo busca offline ese número
de comandos; el resultado no se instala.

Los presets se contrastan también en shotgun_pattern_tests: MD5 del número,
rango radial completo y correspondencias de serialización. Una coincidencia
offline no demuestra aceptación o impactos en Valve.
