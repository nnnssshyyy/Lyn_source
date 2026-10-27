# Texto de vida centrado y móvil: 3.1.25

El valor de vida pasa de 15 a 12 píxeles y queda centrado en el eje de la barra.
En las barras verticales su posición sigue el extremo superior del relleno:
al disminuir la vida, bajan tanto el borde verde como el número. El número
queda dos píxeles por encima del relleno y no se desplaza hacia un lateral.
Las barras horizontales conservan el texto centrado sobre su longitud.

La medida del texto y su dibujo usan la misma fuente y tamaño. El diseño
compartido también se usa en la vista previa arrastrable del menú, que emplea
la fuente pequeña de 12 píxeles ya disponible. La fila del nombre se reserva
en la parte superior y permanece estable mientras cambia la vida.

El formato de vida permanente, temporal e incapacitación se conserva. El
cambio afecta a la colocación y tamaño, no a los valores leídos del juego.

## Validación

La vista `build/health-label-preview/preview.png` se renderizó fuera de partida
con Direct3D9, el diseño compartido real y la fuente de ImGui. Muestra distintos
niveles de vida, barras a izquierda y derecha y el formato de vida temporal.
No es una captura de una partida.

Respaldo inicial: `build/health-label-source-3.1.24/`. El primer borrador, con
texto fijo arriba, se guardó antes de aplicar la aclaración del usuario en
`build/health-label-fixed-position-draft-3.1.25/`; no se publicó.

Release Win32 compila y pasan las 21 suites, incluidas 211 comprobaciones
de rapid fire, 158 de API nativa, 25 de persistencia y 44 de captura. Accounts
pasó sus 35 pruebas. También pasaron carga nativa x86, reubicación, once
recursos, integridad UPX y conservación de secciones tras descompresión.

Publicado el módulo 3.1.25 mediante el panel privado de Vercel y la API de
licencias vigente. El panel confirmó firma y descarga real. La DLL tiene
3 656 704 bytes y SHA-256
`b00e19fa4d955396fc06fd313b9648cbcd42eaf52171cbb2ea84b0adbc89c83d`.
El loader permanece en 3.1.9 con el icono del usuario. Reiniciar L4D2 y cargar
de nuevo obtiene el módulo actualizado.

Accounts se desplegó desde commit
`632c7bb2e977c60cddfe20bf7fedccd16c35a323`. Producción
`NVRGLn2dNo8Wjm5YCCuQC4SHnQjX` confirmada como Ready en Vercel.
