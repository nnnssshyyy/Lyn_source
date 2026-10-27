# Rendimiento de privacidad: módulo 3.1.18

La ruta de ventana de 3.1.17 seguía presentando una segunda swap chain y
componiendo una ventana layered sobre el juego. Limitarla a 60–90 Hz no
eliminaba ese trabajo. Esta entrega añade un método explícito para captura de
juego y reduce trabajo adicional del método de ventana.

## Métodos

En World, bajo `OBS / Discord privacy (F8)`, `capture method` permite elegir:

- **display / window capture**: conserva la ventana con exclusión de captura de
  Windows. Sigue siendo el valor por defecto, también para perfiles anteriores.
- **OBS game capture only (fast)**: dibuja desde un EndScene procedente de
  `gameoverlayrenderer.dll`, dentro de su escena existente. No crea ventana,
  swap chain, Clear ni Present adicionales. Requiere Steam Overlay activo,
  una fuente **Captura de juego** de OBS y **Capture third-party overlays**
  desactivado. Este método no sirve para ocultar visuales en captura de pantalla
  o monitor; depende del punto de captura y debe comprobarse en la grabación.

El método ligero selecciona solo un retorno ejecutable dentro del módulo
esperado, con el backbuffer principal como destino. Otros dispositivos,
destinos offscreen, retornos y llamadas repetidas en una misma presentación se
descartan antes de construir la UI. La búsqueda del módulo se limita a una por
segundo. Reset o un cambio de módulo eliminan la selección anterior. Sin Steam
no dibuja en una ruta alternativa. F8 permite salir incluso sin menú visible.

Solo un Present correcto renueva la captura de entrada del menú. Se conservan
la liberación al perder foco y la expiración de la entrada si deja de dibujarse.
Cambiar de método durante un frame descarta sus dibujos, evitando enviarlos al
destino que correspondía al método anterior.

## Menos trabajo en ventana privada

- Los draw lists vacíos ocultan la ventana y omiten Clear, envío de vértices y
  Present. El presupuesto de refresco también se respeta en este caso.
- Una consulta GPU de evento permite omitir la siguiente actualización privada
  si sigue pendiente la anterior. Se consulta una vez, sin FLUSH ni esperas;
  las presentaciones normales del juego siguen enviando el trabajo.
- La privacidad desactiva la copia de escena y las siete pasadas de blur del
  menú. La última textura se libera al cambiar; no queda una imagen congelada.
- Se conserva el presupuesto de 60–90 Hz, la afinidad verificada, el aislamiento
  del buffer del juego y la presentación sin espera del método de ventana.

## Validación

Compilación Release Win32 y **21 suites correctas**, incluidas **44
comprobaciones de captura**. Se verifican exclusión de captura aceptada por
Windows, aislamiento real de buffers, restauración de viewport/destinos,
préstamo de una escena DX9 sin BeginScene/EndScene anidados, selección de
retorno, bordes del módulo, recarga/ausencia y una sola actualización ante cinco
llamadas por presentación. La persistencia comprueba la selección explícita y
que un perfil antiguo conserve el método de ventana.

El empaquetado conserva una copia de 3.1.17, comprueba UPX, carga x86 con
reubicación, los once recursos y la igualdad de las secciones ejecutables y de
datos tras descomprimir. Los resultados quedan en `build/privacy-3.1.18/`.

No había L4D2 ni OBS ejecutándose. No se midieron FPS de una partida ni se
comprobó una grabación real o el retorno de Steam en ejecución. Las pruebas
acreditan las rutas y las operaciones eliminadas; no un aumento concreto de FPS.

Fuentes: [escenas DX9](https://learn.microsoft.com/en-us/windows/win32/api/d3d9/nf-d3d9-idirect3ddevice9-endscene),
[consulta sin FLUSH](https://learn.microsoft.com/en-us/windows/win32/api/d3d9/nf-d3d9-idirect3dquery9-getdata),
[propiedades de Captura de juego de OBS](https://obsproject.com/kb/game-capture-source).

## Entrega

El 22 de octubre de 2026, el panel privado de Vercel publicó **3.1.18** y confirmó
la firma y la descarga autenticada. DLL comprimida: **3 654 144 bytes**, SHA-256
`9034333afd0c09a54f859d00efcc9214364e2a6860d823856633685545f0932d`.
Las copias de Release, loader y compressed coinciden. El ejecutable del loader
permanece en 3.1.8; obtiene este módulo por la API vigente al cargar de nuevo.
Accounts publicó la referencia 3.1.18 desde el commit `536aa9e`, con despliegue
de producción `5T2WMmtLpk6uAnY6Y9seHi4biKxD` confirmado como Ready.
