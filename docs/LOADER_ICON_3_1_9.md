# Icono del loader: 3.1.9

El icono se sustituyó por la imagen adjunta del usuario. La copia original está
en `loader/Assets/loader-icon.png`, SHA-256
`3bbfea82db8a917ded752bc7668a07c173b65c8ff24552e19be938d629ccd1ea`.
`loader/create-icon.ps1` ajusta el retrato completo a un lienzo cuadrado
transparente y genera nueve tamaños: 16, 20, 24, 32, 40, 48, 64, 128 y 256.

`Assets/vs3.ico` alimenta el icono Win32 del ejecutable, la ventana WPF mediante
una URI del ensamblado y el ejecutable externo del paquete completo. La edición
compacta contiene la misma imagen. El script de compilación regenera el ICO
cuando cambia el PNG de origen.

La versión del proyecto del loader ya era 3.1.9 antes de este cambio. El módulo
entregado mediante la API continúa en 3.1.24, con los mismos bytes. Esta tarea
cambia el icono y su integración como recurso.

## Comprobaciones

- Ambas ediciones compilaron; el paquete completo pasó su prueba de integridad
  y la extracción recuperó exactamente el ejecutable interno.
- Las nueve imágenes de los recursos Win32 coinciden byte por byte con el ICO
  en el paquete completo, su ejecutable interno y la edición compacta. También
  se verificaron los dos idiomas del recurso exterior.
- La comprobación visual existente abrió los recursos y renderizó las dos
  pantallas en cuatro momentos de la animación sin abrir una ventana ni iniciar
  sesión. Se corrigió la URI del icono para que también funcione al referenciar
  el ensamblado desde ese comprobador.
- Las 35 pruebas de Accounts verificaron, entre otras cosas, la reconstrucción
  exacta de ambas ediciones desde los nuevos fragmentos de descarga.

Edición completa: 42 139 524 bytes, SHA-256
`65521c87516fd50cc8b482f58517a9c75edcd6266155adcf5f64ca98c70cc256`.
Compacta: 998 486 bytes, SHA-256
`4ad7ea60f34d81e473a332b9f86bbcb146734a37f0b224dd862a27409b6800e2`.

Artefactos, copia anterior, registros y vista previa del icono:
`build/loader-icon-3.1.9/`.

## Publicación

Publicado en Accounts/Vercel desde commit
`2e18c5bcd6ddd68e34383f81bb984e399976a206`. El despliegue de producción
`ATTvbBduziig3KRuwnwebXu12u4K` terminó como Ready y Current. La captura
de confirmación está en el paquete local.
