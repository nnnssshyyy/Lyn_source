# Icono del loader, 3.1.12

El loader usa el triángulo azul proporcionado en `assets/icon.png`. El PNG de
1254 × 1254 píxeles se copia sin modificar a `loader/Assets/loader-icon.png`.
Su SHA-256 es
`33263f843dc9ea72e3b372ed83f6dbda824f92ef3a90ff9261b05de20da3bddf`.

La conversión conserva las proporciones y la transparencia y genera los tamaños
16, 20, 24, 32, 40, 48, 64, 128 y 256. El ICO se incluye en el ejecutable
compacto, en el ejecutable interno de la edición completa, en su paquete
exterior y en el recurso de la ventana WPF.

## Validación

- Compilación Release Win32 de las dos ediciones: correcta.
- Los nueve iconos Win32 coinciden byte a byte con los del ICO. Se verificaron
  también los dos idiomas de los recursos del ejecutable exterior.
- El paquete LZMA2 pasó la comprobación de integridad y la extracción recuperó
  exactamente el ejecutable interno.
- El comprobador visual existente renderizó las dos pantallas en cuatro
  momentos de la animación y aprobó sus dos comprobaciones de cierre.
- Accounts: 37 pruebas y compilación de producción correctas. Las pruebas
  reconstruyeron las dos ediciones a partir de sus fragmentos de descarga.

Completa: 41 641 620 bytes, SHA-256
`5bcab9925ac6601701e8a5c6d44e4557be2d1a055b63097a9d85cf22ae65362c`.
Compacta: 506 968 bytes, SHA-256
`beceb4a7db6848e61a6dde02b9dc9fd75029ddda71d141198a491f62b97946b8`.

Artefactos y comprobantes: `build/loader-icon-3.1.12/`. La copia anterior está
en `previous-3.1.11/`. El módulo distribuido sigue en 3.1.37; la versión del
loader y la de la DLL se registran por separado en el manifiesto de Accounts.

## Publicación

Publicado en Accounts/Vercel desde el commit
`a4074d4aa35deea74bfb354df39a23f52b133847`. El despliegue
`CGyBbgjXuGxVvw87mwLvi8RHAD8R` terminó como Ready en producción y tiene
asignado `vs3-accounts.vercel.app`. El comprobante está en `published.jpg`.
