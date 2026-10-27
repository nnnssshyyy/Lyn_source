# Solicitudes de hardware entre instalaciones de Windows · loader 3.1.11

Los registros de producción mostraron intentos de `/api/loader/login` con
aproximadamente 25 204 segundos de retraso entre la marca de tiempo firmada y la
hora del servidor. La API rechazaba esas pruebas antes de `bindLogin`, por lo
que no se insertaba ninguna solicitud pendiente. La web, además, solo consultaba
las solicitudes al montar el panel o pulsar Actualizar.

## Corrección

- `/api/loader/time` devuelve hora Unix y el nonce aleatorio de la consulta,
  sin credenciales, con `Cache-Control: no-store`.
- El loader obtiene esa hora del origen HTTPS fijo antes de firmar. Valida el
  nonce y usa tiempo transcurrido monotónico; renueva la referencia cada cinco
  minutos. No cambia la hora de Windows ni amplía la ventana de aceptación de
  firmas (-90/+15 segundos).
- Las comprobaciones temporales del cliente para licencia, enlaces y respuestas
  firmadas usan la misma referencia. Las claves CNG y las comprobaciones de
  contraseña, firma, nonce, equipo y navegador autorizado se conservan.
- Accounts consulta cada diez segundos con la pestaña visible y al recuperar
  el foco. Evita peticiones simultáneas, muestra el número pendiente y abre el
  panel cuando llega una solicitud nueva. La aprobación sigue siendo explícita.

## Validación

- API: 23 pruebas correctas. Una reproduce un reloj atrasado siete horas:
  rechazo sin solicitud con la hora antigua; `hardware_pending` y solicitud
  visible desde la sesión web usando la hora recibida del servidor.
- Loader: 16 comprobaciones correctas con nueve firmas verificadas de forma
  independiente, reloj local incorrecto, renovación de hora, rechazo de nonce
  temporal ajeno y persistencia real de identidad CNG. Interoperabilidad .NET/JS
  correcta.
- Accounts: 37 pruebas y build/TypeScript correctos. Incluye consulta diferida,
  pestaña oculta, recuperación de foco, exclusión de solapamientos y reintento.
- Navegador local: página ya abierta sin solicitudes; un segundo equipo produce
  403 `hardware_pending`; la solicitud aparece sin recargar. Tras aprobación web,
  la sesión anterior devuelve 401 y el nuevo equipo obtiene 200.
  Captura: `build/hardware-time-release/web-request-arrived.png`.
- Empaquetado LZMA2: extracción y SHA-256 del payload verificados.

## Entrega

Loader compacto: 1 001 560 bytes, SHA-256
`5076fc0cdbd7d051c6c5b346a089153e0276f4dc59845faa636c9c3b443bc0b3`.

Loader completo: 42 140 488 bytes, SHA-256
`ef460cd55338986c9f0c1348dcd863e35aeca47f5258c1ad52a1e017db7e2e62`.

La DLL 3.1.32 conserva su versión y contenido. Se necesita descargar el loader
3.1.11 para que un Windows con el reloj desajustado envíe la solicitud.

API de licencias: versión 7, commit
`3964f605ee9c654580e0467f1d78b5afa79b3ee6`, despliegue
`appgdep_6ada9c71a3088191a0bc0705383b46c0` correcto. El endpoint publicado
de hora devolvió 200, nonce coincidente y no-store. Se conserva su origen
actual; no se migró a una VPS ni se publicó el portal histórico lyn-downloads.

Accounts/Vercel: commit `df8ea1ed3937cfb69c9319f19af0799505211c80`.
Los dos contextos de despliegue, Accounts y Admin, finalizaron correctamente.
Ambos manifiestos de descarga siguen devolviendo 401 y no-store sin sesión.
