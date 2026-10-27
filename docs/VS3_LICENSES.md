# VS 3: loader y licencias

## Acceso

- Panel y descarga privados: https://lyn-downloads-n7q4.rockyknoll128.chatgpt.site
- Registro y API pública: https://vs3-accounts-n7q4.rockyknoll128.chatgpt.site
- Loader autónomo x86: `build/loader/Lyn.Loader.exe`, versión 3.1.3.

El propietario entra al panel con su cuenta de ChatGPT. Genera una clave con
duración y cantidad; la clave completa solo se devuelve al crearla. La cuenta
del producto se registra en el portal público y se activa con esa clave. La
duración empieza en la primera activación, no al generar la clave. Otra cuenta
no puede reclamarla. Repetir la activación en la misma cuenta no amplía la fecha.

El usuario entra al loader con su usuario y contraseña; también puede activar
una clave desde allí. Un único botón abre L4D2 si está cerrado, espera al
menú principal y carga el módulo. Cuando el módulo confirma su plan firmado,
el loader se cierra solo. Si el módulo ya está presente y corresponde
a la misma ruta, **Reanudar sesión** renueva el acceso sin cargarlo otra vez y el loader también se cierra.
Para cambiar una DLL que ya está cargada, reinicia el juego.

## Qué hace el servidor

El servidor autentica la cuenta, comprueba licencia/vencimiento/revocación y
firma una autorización ECDSA P-256 con SHA-256. El servidor también entrega la DLL autenticada y genera un plan firmado con los
parches de los hooks. El cliente verifica ese plan y aplica los bytes localmente;
Windows carga el PE y MinHook construye los trampolines. Véase
`docs/VS3_SERVER_HOOKS.md` para el protocolo 2.

Cada autorización contiene versión, emisión, vencimiento, PID, tiempo de
creación del proceso Windows, nonce, producto y permisos. Dura hasta 120 segundos
y no supera la fecha de vencimiento de la licencia. El loader la renueva cada
30 segundos. Usa TLS normal, no sigue redirecciones y comprueba una clave pública
fijada en el código. No guarda contraseña ni token de sesión en las preferencias.

Una tubería de Windows limitada al usuario actual comunica la autorización a la
DLL: `VS3.<pid>.<creation FILETIME>`. La DLL vuelve a comprobar firma, proceso,
producto y fechas; la primera autorización válida deja autorizada la sesión
del proceso (PID y creación) hasta que el juego se cierre. La red y la
criptografía se ejecutan fuera de los comandos y del render. Cerrar el loader
no pausa las funciones ya autorizadas; sin la autorización inicial (y sin el
plan de hooks firmado) el módulo no arranca. No se promete protección absoluta
contra modificar un binario local.

## Almacenamiento y separación

La API usa D1: usuarios, hashes de contraseñas, licencias, hashes de sesiones y
contadores de intentos. Contraseñas con scrypt N=32768, r=8, p=3 y salt aleatorio
de 16 bytes. Se serializa ese trabajo dentro del isolate para limitar memoria.
Las claves de activación tienen 160 bits aleatorios y se almacenan por SHA-256.
Las sesiones aleatorias duran 8 horas; la web usa una cookie HttpOnly, Secure y
SameSite=Strict. Las rutas validan origen, tamaño de JSON y frecuencia de intentos.

El panel privado usa un proxy con rutas permitidas y acceso de propietario
impuesto por Sites. `LICENSE_ADMIN_KEY` y `LICENSE_API_ORIGIN` son variables de
producción del panel; la API usa `ADMIN_KEY` y `SIGNING_PRIVATE_JWK`. Las claves
privadas y administrativas no están en Git, EXE, DLL, HTML ni JavaScript cliente.
No conviertas el proyecto del panel en público. La API es otro proyecto.

No hay pagos, recuperación por correo ni renovación automática de suscripciones.
El panel muestra los 200 registros más recientes. Las cuentas técnicas de QA se
deshabilitan y sus licencias se revocan después de las comprobaciones.

## Validación de VS 3.1

- 551 comprobaciones C++ en 14 suites; 25 verifican firmas/planes, identidad,
  límites y vencimiento. Las pruebas del resto de funciones siguen pasando.
- Siete comprobaciones del transporte entre C# y el proceso nativo, incluyendo
  clientes que abandonan peticiones o respuestas.
- Tres suites de la API con SQLite y firmas reales: cuentas, activación,
  revocación, permisos, módulos y generación de saltos x86.
- Flujo contra producción con una cuenta temporal: módulo firmado, caché,
  autorización, plan remoto, verificación nativa, trampoline y restauración.
  La licencia temporal se revocó y la cuenta se deshabilitó al terminar.
- Seis pruebas de descarga, con reconstrucción exacta de las ocho partes del EXE.
- DLL comprimida validada con UPX, carga PE32, reubicaciones y 11 recursos.
- Ambas pantallas WPF renderizadas en cuatro posiciones del brillo, sin abrir una
  ventana ni cargar código en una partida. La precisión y movimiento en partida
  continúan pendientes de validación.

Los registros de paquete y entrega están en `build/server-hooks`.

Referencias: [OWASP Password Storage](https://cheatsheetseries.owasp.org/cheatsheets/Password_Storage_Cheat_Sheet.html)
y [Cloudflare Node Crypto](https://developers.cloudflare.com/workers/runtime-apis/nodejs/crypto/).
La comunicación entre los dos sitios usa la opción
`global_fetch_strictly_public`, documentada en
[Cloudflare Fetch](https://developers.cloudflare.com/workers/runtime-apis/fetch/).

## Interfaz del loader

Acceso y selección del juego siguen las referencias del usuario, con marca VS 3
y banner de L4D2. El fondo lateral usa una malla original de 40 triángulos en negro
mate, con iluminación tenue que pasa entre caras contiguas cada 8 segundos. Está
dibujado con vectores WPF. Los campos son claros y los botones principales verdes.
El banner conserva su color original.
Los campos de acceso tienen esquinas de 5 px y texto seminegrita, igual que las
etiquetas y las casillas redondeadas.
La esquina superior derecha conserva el botón de cerrar; el loader se cierra solo al confirmar la carga.

## Modo dev

`Lyn.Loader.exe --dev` (o `--dev-dll <ruta>`) permite inyectar una compilación local para
pruebas sin alterar el flujo normal: el inicio de sesión, la licencia y el contrato firmado
por tubería siguen siendo obligatorios, y el plan de hooks se verifica igual (proceso,
creación y nonce; no depende del hash del módulo). La DLL se elige con `--dev-dll` o
automáticamente entre las copias locales
(`build\Release\lyn.dll`); se copia a `%LOCALAPPDATA%\lyn\loader\dev\<sha256>\lyn.dll`
antes de cargarla, así la carpeta de compilación queda libre. Los usuarios normales
reciben siempre el módulo firmado del servicio.
