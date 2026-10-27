# Captura de visuales sin iniciar el juego

Este ejecutable Win32/DX9 dibuja visuals_window.cpp y overlay_paint.h sobre
un escenario de prueba. Sustituye únicamente el origen del snapshot y el
contexto del menú. Los muñecos y los datos son simulados; la captura lo indica.
No inicia ni inyecta el juego, no abre servidores y no utiliza credenciales.

Compilar con CMake/MSVC Win32 después del proyecto principal. Ejecutar
`preview <directorio de salida>` para guardar tres PNG con edades de efecto
de 40, 110 y 200 ms y comprobar que los vértices generados sean finitos.
La ventana DX9 permanece oculta.
