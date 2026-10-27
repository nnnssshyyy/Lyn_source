# Hardware match · loader 3.1.7

Un solo PC por cuenta. Primera vinculación atómica en la API; candidatos nuevos
obtienen 403 `hardware_pending`, código público de 12 hexadecimales y plazo de
10 minutos. No reciben token. La comparación incluye huella y clave pública.

## Prueba de posesión

P-256 / SHA-256, firma P1363 de 64 bytes, base64url. El payload UTF-8 es:

```text
VSHW|1
METHOD
pathname+search
SHA256(exact JSON body or empty)
SHA256(credential)
fingerprint
unixSeconds
nonce
```

Credential: `login:<normalized username>` al entrar, bearer en las demás
solicitudes. Ventana temporal -90/+15 segundos, nonce aleatorio de 32 bytes,
unicidad persistente en D1. Cubre sesión, endpoint y cuerpo; no basta con copiar
la huella ni el token. Los clientes anteriores sin este protocolo reciben 426.
Las sesiones migradas son web y no pueden usarse como bearer del loader.

Windows conserva `VS3.DeviceIdentity.v1` en CNG sin permitir exportar la clave
privada. Se prefiere TPM y se permite el proveedor software de Windows. La
huella es SHA-256 de un dominio VS 3, MachineGuid y UUID SMBIOS; esos valores
originales no salen del equipo. No se afirma atestación remota ni imposibilidad
de clonar un Windows manipulado. El perfil de Windows también protege la clave.

## Autorización en Accounts

El usuario aprueba en la web. Un navegador necesita sesión web y una concesión
`__Host-vs3_device` HttpOnly, Secure, SameSite Strict, válida durante 30 días.
La concede un enlace de un solo uso emitido por el loader vinculado; viaja en el
fragmento de Accounts, no en query, se retira al consumirlo y caduca en 5 minutos.
La concesión pertenece a la cuenta y no sirve en otra. La contraseña por sí sola
no concede permiso. Aprobar exige reintroducirla, comprobar código y propiedad
de solicitud, caducidad y revisión. CAS y lote D1 reemplazan la identidad,
aumentan revisión y revocan sesiones antiguas del loader.

Recuperación: contraseña y código aleatorio de 160 bits mostrado una sola vez.
Solo su hash se guarda. Autoriza una solicitud pendiente en la web, rota el
código y revoca las concesiones previas de navegadores. Un navegador autorizado
puede rotar el código o eliminar su propia concesión. Se aplican límites de
intentos tanto por IP como por cuenta. El BFF Accounts permite únicamente rutas
y campos explícitos; reconstruye cookies y no transmite headers arbitrarios.

El acceso firmado al módulo, leases y planes requiere prueba del PC vigente.
La DLL 3.1.17 no cambia. La autorización que ya recibió un proceso de juego
continúa hasta cerrarlo; no se anuncia revocación remota de partidas abiertas.

## Validación y entrega

- API: 16 pruebas; posesión, replay, cuerpo alterado, sesión copiada, carreras,
  autorización web, propiedad, caducidad, recuperación y bloqueo del password-only.
- Accounts: 31 pruebas; Admin: 15. Ambos builds y TypeScript correctos.
- Loader: 13 comprobaciones, ocho solicitudes verificadas independientemente,
  persistencia real de la identidad CNG y firma .NET aceptada por la API JS.
- QA local en navegador: segundo PC bloqueado, aprobación web correcta,
  sesión anterior 401 y nuevo equipo 200. Vista guardada en
  `build/hardware-match/web-hardware-approved.png`.
- Migraciones Drizzle 0001 y 0002 agregan tablas y columnas; no cambian cuentas,
  licencias o claves de firma. El sitio conserva D1, R2 y audiencia pública.
- Loader comprimido: 41,536,946 bytes, SHA-256
  `f62b67b43445d2d989375f27ae63663762b0e7fd5d0522c8a333fea8cd57228e`.
  El empaquetador verifica la extracción exacta del ejecutable original.

Repetir pruebas C#: `dotnet run --project tests/loader_hardware/LoaderHardware.Tests.csproj -c Release -- --windows-identity`.
La prueba de interoperabilidad debe ejecutarse dentro de 90 segundos de generar
`build/hardware-match/dotnet-proof.json`, por la ventana de firmas.
La validación privada de publicación prueba además el bloqueo de un segundo
equipo y el cambio desde la web con una cuenta QA deshabilitada al terminar.

Publicación verificada el 22 de octubre de 2026 (UTC): servicio versión 5,
commit `f3909b6194f226464e571709fe7cff5132203f3b`, despliegue
`appgdep_6ad95dbb2bcc819184c95b53fa1dbd3f` correcto. Accounts y Admin en Vercel
publicaron `36d52c820722b1b96f0fd83bf900c605994480e3` correctamente. El panel
privado completó la verificación real de firma, descarga, bloqueo de candidato,
rechazo de aprobación sin navegador autorizado, cambio web y revocación anterior.
Las rutas públicas de manifiesto y partes devolvieron 401 y no-store sin sesión.
La API permanece en su origen vigente; no se migró a una VPS ni se desplegó el
portal privado histórico de Sites.
