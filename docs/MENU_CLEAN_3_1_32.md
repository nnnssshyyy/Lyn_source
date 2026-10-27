# Menú simplificado: módulo 3.1.32

Se retiraron del menú los textos de ayuda y diagnóstico de las funciones:
notas de activación, estados de lag y no fall, contador de correcciones,
velocidad horizontal, aceleración aérea, estados de efectos y avisos de captura.
Los diagnósticos internos y registros del módulo siguen disponibles.

Los selectores muestran `V1` / `V2` para lag y `V1` / `V2` / `V3` para rapid
fire. Se acortaron las etiquetas de rapid y sus controles. Los nombres visibles
cambian; los índices, valores y claves de configuración permanecen iguales.

El usuario aclaró que deseaba quitar los atajos de ESP y anti OBS y conservar
sus interruptores. WindowProc ya no intercepta F6 para ESP ni F8 para captura.
Se retiraron las dos funciones de toggle y sus colas. ESP y anti OBS se cambian
con sus interruptores del menú. El único toggle de esa cola es ahora tercera
persona; su bind configurado y la apertura con Insert siguen funcionando.
Las etiquetas no anuncian F6/F8.

No se modificaron `features.cpp`, `game.cpp`, `game.h`, `storm_fall.h` ni
`movement_assists.h`: sus hashes coinciden con la entrega 3.1.31. La lógica de
caída automática y los métodos de red conservan esa implementación.

## Verificación

Release Win32 y las 21 suites nativas existentes terminaron sin fallos.
El menú de producción se renderizó con Direct3D9 fuera de partida para revisar
Movimiento, Otros, selector V2, ESP y anti OBS. Las capturas están en
`build/menu-clean-qa/` y en la entrega. No se añadieron tests para las etiquetas.

Fuentes anteriores: `build/menu-clean-source-3.1.31/`.
Entrega: `build/menu-clean-3.1.32/`.
La distribución continúa mediante Accounts/Vercel y la API firmada; el loader
conserva la versión 3.1.10.
