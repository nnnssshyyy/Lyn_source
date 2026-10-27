# Posiciones de etiquetas y nieve de fondo, 3.1.37

El nombre y la distancia se pueden arrastrar en la vista previa a cualquiera de
los cuatro lados. Los destinos disponibles tienen un halo dorado; el destino
seleccionado se ilumina más. Soltar confirma la posición. Escape, clic derecho
o soltar fuera de un destino conserva la posición anterior. La barra de vida
mantiene su arrastre independiente.

El número de vida queda centrado sobre la barra y 2 píxeles más abajo. El mismo
ajuste se aplica a la vista previa y al dibujo de las etiquetas.

También se pueden elegir las posiciones en Jugadores > Apariencia. Las
etiquetas que comparten un lado ocupan filas separadas y reservan espacio para
la vida y las etiquetas de estado. Las posiciones se guardan en el perfil;
los perfiles antiguos conservan el nombre arriba y la distancia abajo.

La nieve utiliza 72 partículas triangulares con caída y giro suaves. Se dibuja
en la capa de fondo de la pantalla y excluye el rectángulo completo del menú,
incluidos los halos: no aparece dentro de los paneles ni sobre los controles.
El tamaño y la velocidad de las partículas se mantienen al cambiar la escala
del menú. La opción Nieve de la barra lateral permite desactivarla y se guarda
para el usuario de Windows, independientemente del perfil.

## Validación

- DLL y arnés compilados en Release Win32.
- 79 comprobaciones de interfaz, cero fallos: arrastre, confirmación,
  cancelación, separación de etiquetas, destinos dorados, límites y exclusión
  de partículas, activación y restauración del estado gráfico.
- Tres suites aprobadas: renderizado (34 comprobaciones), vista previa DX9
  (29) y persistencia de ajustes (33).
- Nueve capturas en `build/slots-snow-qa/`, incluidos los destinos dorados y
  la nieve exterior en las escalas 100 %, 125 % y 150 %.
- Paquete comprobado con UPX y comparación de secciones y recursos tras
  descomprimir. Los detalles y hashes están en `build-info.json` y
  `package-validation.json` dentro de la carpeta de la candidata.
- No se ha probado la carga de esta DLL dentro del juego ni en Windows 10 LTSC.

DLL publicada: `build/slots-snow-3.1.37/lyn.dll`.
Original sin comprimir y símbolos: en esa misma carpeta.

Publicada como módulo **3.1.37** mediante el panel privado de Vercel. El panel
confirmó firma y descarga autenticada con SHA256
`146616eefc745378671094232a9deb1f0ce23549ca4f5786d9eb5ed8382fb93b`.
Accounts actualizó las variantes completa y compacta desde el commit
`f5d414b3ffaf42c21caa9bebcd972c2e17d7c685`; sus 37 pruebas aprobaron.
Vercel confirmó el despliegue `Nw2FwVagVD8uKonsQzUSZvVMVKRx` como Ready y
producción actual para `vs3-accounts.vercel.app`.
Las copias de Release, loader y compressed coinciden con esta entrega. Las
copias anteriores se conservan en `previous-3.1.36/` dentro de la carpeta de
publicación. Los comprobantes están en `published.jpg`, `accounts-production.jpg`
y `verification.json`.
