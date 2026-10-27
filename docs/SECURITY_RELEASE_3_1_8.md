# Seguridad y distribución · loader 3.1.8

Cambios del 21 de octubre de 2026 (Lima). Módulo nativo conservado en 3.1.17.

## Credenciales

Casilla opcional **Recordar credenciales**, inicialmente desmarcada. Se guardan
solamente tras autenticar correctamente, mediante CredWriteW genérico en el
Administrador de credenciales de Windows, con persistencia local para el mismo
usuario. El buffer nativo temporal se borra al liberarlo. No se escriben contraseñas
en JSON, logs o archivos de distribución. Desmarcar elimina la entrada con
CredDeleteW. No inicia sesión automáticamente. La protección depende del usuario
de Windows: otro proceso bajo ese usuario puede acceder a credenciales genéricas.

## Web y API

- Panel Admin con CSP por nonce y render dinámico, sin unsafe-inline/unsafe-eval
  en producción. Se verificó que los nueve scripts del HTML llevan el nonce
  emitido. Se añaden COOP/CORP same-origin, nosniff, anti-frame y otras cabeceras
  tanto en Admin como en Accounts; la protección privada de Vercel se conserva.
- Cierre global desde Accounts con sesión válida, origen y contraseña repetida.
  Revoca sesiones web y loader, enlaces de autorización y concesiones de
  navegadores; no borra licencia o identidad vinculada. No cambia contraseñas,
  no elimina credenciales de otros PC ni termina módulos ya autorizados.
- Máximo cinco sesiones web y dos del loader por cuenta. El límite se aplica
  después de un login correcto mediante lote D1 y orden estable.
- Se rechazan cookies duplicadas, mezcla cookie/bearer, mutaciones con cookie
  sin Origin correcto, content-types parecidos a JSON y campos desconocidos en
  registro, login y operaciones de hardware.
- Entrega nativa limitada a ocho descargas por minuto por cuenta. Los metadatos
  R2 se validan antes de firmarlos y el tamaño almacenado debe coincidir.
- Descarga nativa con plazo de 90 segundos y error HTTP limitado a 32 KiB.
  Se mantienen firma del manifiesto, hash, licencia y prueba del equipo.

## Dos ediciones

| Edición | Bytes | SHA-256 |
|---|---:|---|
| Compacta, requiere .NET Desktop Runtime 8 Windows x86 | 409686 | `7b5cf4322a3323132a31bbca1b65588cfabc92bde1b116501610e8802d57af11` |
| Completa, incluye .NET | 41538800 | `344ca97ff26741df6ad4af60941c34daa9ba0098fb6815fd939e1894402192e6` |

La compacta reduce un 99 % el archivo distribuido al usar un runtime instalado.
No se elimina ninguna función. La edición completa conserva el paquete LZMA2
verificado; no se aplicó trimming inseguro de WPF. El requisito de la compacta
aparece antes de descargar y enlaza a Microsoft. Ambas requieren la misma licencia
y cada parte de descarga pasa por autorización; ninguna se publica como estático.

## Validación

- API: 22 pruebas correctas; TypeScript y build del Worker correctos.
- Accounts: 35 pruebas, incluidas las dos ediciones y cierre global; build correcto.
- Admin: 16 pruebas y build correcto; los scripts del HTML de producción usan nonce.
- Windows Credential Manager: seis comprobaciones reales en x64 y Win32, con
  credenciales QA independientes que se eliminan al finalizar.
- Protocolo hardware: firmas C# verificadas de forma independiente y aceptadas
  por la API JavaScript. Se mantiene compatibilidad con loader 3.1.7.
- WPF: render de login y dashboard sin ventana. Casilla nueva visible.
- Navegador local: selector compacto/completo muestra 0,4/39,6 MB y el cierre
  global vuelve al formulario de acceso con confirmación.

El alcance cubre las pruebas y controles descritos; no constituye una auditoría
externa ni una garantía absoluta ante un servidor o un Windows comprometidos.

## Publicación

Servicio versión 6, commit `ee3bd3b1b41766535c0012af0589b99a6952b4c7`, despliegue
`appgdep_6ad98277cfc48191a9fbb3c80b67ff62` correcto. Accounts y Admin publicaron
`ccd66d3f450a0f804f3e55024e85079d49262938` correctamente en Vercel.
La descarga real del compacto desde Accounts devolvió 409686 bytes y el SHA-256
esperado; la captura está en `build/security-release/download-published.png`.
Sin sesión, su manifiesto y su parte binaria devuelven 401 con no-store.
La comprobación del panel privado terminó correctamente en producción: firma y
descarga del módulo vigente, cambio de hardware y cierre global de sesiones.
Su cuenta QA quedó deshabilitada y su licencia revocada al finalizar.
La API sigue en su destino vigente; no se hizo una migración a VPS ni se publicó
en el portal privado histórico de Sites.
