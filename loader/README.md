# VS 3 / loader

Aplicación de escritorio C# / WPF, Win32, para cargar la DLL nativa del proyecto en `left4dead2.exe`.

## Uso

1. Ejecuta `Lyn.Loader.exe`. La versión 3.1.12 ofrece dos ediciones: **compacta**, de aproximadamente 0,5 MB, que requiere .NET Desktop Runtime 8 para Windows x86, y **completa**, de unos 40 MB, que incluye .NET en un paquete LZMA2. Ambas descargan el mismo módulo firmado tras validar licencia y equipo. El ejecutable se puede distribuir solo, incluso con otro nombre.
2. Registra tu usuario en el portal de cuentas y activa una clave generada desde el panel privado. Inicia sesión en el loader con ese usuario y contraseña.
3. Pulsa **Load**: si el juego está cerrado se abre desde Steam; espera al menú principal y carga el módulo en un solo paso.
4. Cuando el módulo confirma su plan firmado, el loader se cierra solo. El módulo queda autorizado durante toda la partida; ábrelo en el juego con **Insert**.


[Descargar loader](https://vs3-accounts.vercel.app/descarga) · [Registro y licencia](https://vs3-accounts.vercel.app) · [Panel privado](https://vs3-admin.vercel.app). El panel permite generar claves con duración y revocar acceso. La firma del servidor se verifica en el loader y en la DLL. Cada ticket firmado dura hasta 120 segundos; la primera autorización válida queda ligada al proceso del juego para toda la partida. **Reanudar sesión** recupera la autorización de un módulo ya presente en la misma ruta sin volver a cargarlo. El token de sesión no se guarda localmente. Detalles en `docs/VS3_LICENSES.md`.

**Recordar credenciales** es opcional y se desactiva de inicio. Solo después de
un login correcto guarda usuario y contraseña en el Administrador de credenciales
de Windows, para ese perfil y ese PC. La contraseña no aparece en `settings.json`
ni en los registros. Al abrir el loader rellena los campos sin iniciar sesión
automáticamente. Desmarcar la casilla borra la entrada guardada. Cualquier proceso
que controle el mismo usuario de Windows podría leer credenciales genéricas;
no se presenta como protección frente a un Windows comprometido.

Desde **Seguridad de la cuenta** en la web puedes cerrar todas las sesiones
confirmando tu contraseña. También se revocan los navegadores autorizados, pero
se conserva el PC vinculado y la licencia. La operación no cambia la contraseña
ni borra credenciales guardadas en otros equipos. Los procesos del juego que ya
recibieron autorización mantienen el comportamiento descrito anteriormente.

El loader se cierra solo cuando el módulo confirma su activación firmada. También
puedes cerrarlo manualmente. Desde 3.1.9, al salir se elimina su ejecutable descargado,
incluso si no se inició sesión o no se pulsó **Load**. El registro queda en
`%LOCALAPPDATA%\lyn\loader`; allí están `loader.log` y `settings.json`.

Desde 3.1.10, la X y Alt+F4 preparan la limpieza antes de cerrar la ventana.
Si hay un inicio de sesión, activación u otra operación en curso, el cierre
se completa cuando termina o se cancela esa operación. No requiere iniciar
sesión ni cargar la DLL para eliminar el ejecutable descargado.

## Hardware match (3.1.7)

El primer inicio de sesión con el loader actualizado vincula un PC a la cuenta.
Otro PC queda bloqueado y muestra un código de solicitud de diez minutos.
Para autorizarlo, abre **Gestionar equipo** en el loader del PC vinculado y entra
en tu cuenta de la web. El enlace autoriza ese navegador una sola vez y caduca
en cinco minutos. En **Hardware match**, compara el código, revisa la solicitud
y confirma tu contraseña. Se sustituye el PC anterior y se revocan sus sesiones
del loader. Compartir únicamente la contraseña no permite aprobar otro equipo.

Al vincular el primer PC se muestra una sola vez un código de recuperación.
Guárdalo fuera de ese equipo. Si lo pierdes, la web puede autorizar una solicitud
con tu contraseña y ese código; al usarlo se reemplaza por uno nuevo. Un navegador
autorizado también puede generar un código nuevo u olvidarse desde la web.

No se envían números de serie ni identificadores de Windows en claro. Se envía
una huella específica de VS 3 y una clave pública. Cada solicitud se firma con
una clave local CNG no exportable (TPM si está disponible, proveedor de Windows
como alternativa). No es una garantía absoluta frente a la manipulación del SO.
Cambiar Windows, su perfil o la clave protegida puede requerir autorización.
Los módulos ya autorizados para una partida conservan su autorización hasta
cerrar ese proceso; cambiar de PC no termina un juego abierto remotamente.

El módulo se descarga del servicio después de validar la licencia. Su manifiesto
firma versión, hash, tamaño y proceso. Se guarda en `modules/<SHA-256>/lyn.dll`
y se comprueba antes de reutilizarlo. Una descarga incompleta no sustituye el
archivo final. Las versiones usan carpetas distintas; reinicia el juego para
cambiar un módulo ya cargado. Preparar el archivo no inicia una carga.

El servidor genera y firma los parches de los hooks. Windows carga el PE y
MinHook conserva los trampolines locales. El trabajador verifica el plan antes
de activar los hooks, sin hacer HTTP en el hilo del juego. Detalles y pruebas
en `docs/VS3_SERVER_HOOKS.md`.

El panel lateral usa una malla original de 40 triángulos vectoriales, en negro
mate con relieve sutil. Una luz tenue pasa entre caras contiguas cada 8 segundos.

## Interfaz

Dos columnas oscuras con un panel lateral de figuras geométricas animado. El banner
de L4D2 mantiene su color original. Conserva
transiciones breves. El usuario puede recordar su nombre; las
configuraciones del menú del juego siguen siendo independientes.

Los campos de usuario y contraseña tienen esquinas de 5 px, texto seminegrita y
un borde de foco visible. Las casillas usan un marco oscuro redondeado.

## Carga y confirmación

Implementación propia mediante las API documentadas de Windows. Se consultó la estructura del loader de RijiN como referencia del flujo; no se copió su código de mapeo, hooks de ejecución, servidor de licencias o desactivación de VAC.

- Solo admite `left4dead2.exe` de 32 bits y una instancia del juego.
- Identifica la instancia por PID, tiempo de creación y ruta; vuelve a comprobarla usando el handle de la operación.
- Espera `client.dll`, `engine.dll` y `vguimatsurface.dll`, los mismos prerrequisitos que usa `src/dllmain.cpp`.
- Rechaza EXE, ensamblados .NET, DLL x64 y rutas de 260 caracteres o más.
- Evita una segunda carga si ya existe un módulo con el mismo nombre. Para cambiar una DLL cargada, reinicia el juego.
- Resuelve `LoadLibraryW` en el módulo de Windows que realmente contiene la función, contemplando exports reenviados y la base del módulo del proceso destino.
- Envía únicamente la ruta Unicode a una reserva de memoria de lectura/escritura e inicia la carga normal de Windows.
- Solo informa carga confirmada cuando el hilo termina y la lista del proceso contiene la misma ruta y dirección de módulo.
- Si pasan 30 segundos, informa **Carga pendiente** y bloquea el reintento para esa instancia, incluso tras reabrir el loader. La reserva temporal se libera cuando finaliza el hilo. Si se cierra el loader durante ese estado, Windows recupera esa pequeña reserva al cerrar el juego.
- Los handles y la reserva temporal se liberan al terminar; no se termina un hilo de carga ni se libera su argumento mientras lo usa.

`DllMain` del proyecto crea un trabajador aparte para instalar los hooks. **Módulo confirmado** verifica la carga de Windows; no demuestra que el menú o todas las funciones estén operativos. La comprobación final se hace en partida y con el registro propio de la DLL.

Ante acceso denegado, el loader muestra el error de Windows y no eleva privilegios automáticamente. `LoadLibraryW` puede devolver NULL por dependencias o inicialización; no se presenta un `GetLastError` local como si fuera el del proceso remoto.

## Compilar

El icono usa la imagen entregada por el usuario en `Assets/loader-icon.png`.
`create-icon.ps1` conserva sus proporciones y genera `Assets/vs3.ico` con tamaños
16, 20, 24, 32, 40, 48, 64, 128 y 256. La ventana WPF y las dos ediciones del
ejecutable usan ese recurso; el empaquetador también lo copia al ejecutable
externo de la edición completa. El script de compilación lo regenera cuando
cambia la imagen de origen.

Requiere Windows y .NET SDK 8. Desde la raíz del proyecto:

```powershell
.\loader\build-loader.ps1
```

Para generar ambas ediciones: `.\loader\build-releases.ps1`. La compacta queda en
`build\loader-compact\Lyn.Loader.exe`; la completa, en `build\loader\Lyn.Loader.exe`.
El runtime de la compacta se obtiene desde [Microsoft](https://dotnet.microsoft.com/en-us/download/dotnet/8.0), seleccionando **Desktop Runtime · Windows · x86**.

El script publica una aplicación autónoma x86 en `build\loader-payload` y la comprime con el SDK oficial LZMA 26.04 en `build\loader\Lyn.Loader.exe`. Verifica el SDK con SHA-256, prueba la integridad del paquete y comprueba que la extracción devuelve exactamente el ejecutable original. El resultado conserva su versión y manifiesto `asInvoker`: no instala .NET ni solicita elevación. El extractor espera al loader y elimina su carpeta temporal al cerrarse. El runtime sigue ocupando espacio sin comprimir mientras está abierto; esta optimización reduce el tamaño del archivo distribuido.

Desde 3.1.5 el paquete lleva el icono de VS 3 y prepara su contenido sin mostrar la ventana «Extracting». Desde 3.1.9 el cierre normal programa el borrado del ejecutable descargado, sin depender de la confirmación `VS3R` o de haber pulsado **Load**. La limpieza espera a que terminen el loader y el extractor y comprueba el hash antes de borrar ese único archivo. `--dev` conserva la descarga. Si otra aplicación bloquea el archivo, la limpieza reintenta durante un tiempo limitado. El cierre forzado del proceso o un apagado pueden impedir que se ejecute este paso.

Validación de limpieza: `dotnet run --project tests/loader_cleanup/LoaderCleanup.Tests.csproj --configuration Release`. La integración usa el proyecto desechable `tests/loader_cleanup_probe`, empaquetado con el mismo script, y `packaging/test-loader-cleanup.ps1`; no carga módulos ni abre el juego.

La primera compilación descarga el SDK oficial a `build\tools`. Las siguientes reutilizan las herramientas verificadas. `-NoPackageCompression` conserva como alternativa la publicación single-file con compresión de .NET. El script copia también la DLL de `build\Release\lyn.dll` y este documento. Puedes usar `-DllPath 'C:\ruta\lyn.dll'` para copiar otra compilación a la carpeta de entrega. La versión utilizada por el loader procede del servicio firmado. El paquete distribuido no contiene esa DLL.

Para compilar solamente el código C#:

```powershell
dotnet build .\loader\Lyn.Loader.csproj -c Release
```

## Modo dev

`Lyn.Loader.exe --dev` inyecta una compilación local en lugar del módulo firmado del
servicio, conservando inicio de sesión, licencia y autorización por tubería. La DLL se
elige con `--dev-dll "C:\ruta\lyn.dll"` o automáticamente: la última selección local válida, la
`lyn.dll` más reciente junto al loader o en `build\Release\lyn.dll`. Los bytes se copian
a `%LOCALAPPDATA%\lyn\loader\dev\<sha256>\lyn.dll` para no bloquear la carpeta de
compilación; el registro queda en `loader.log`. Cambiar de versión exige reiniciar el
juego. El modo dev es solo para pruebas: los usuarios reciben siempre el módulo firmado
del servicio.

## Archivos

| Archivo | Función |
|---|---|
| `MainWindow.xaml` / `.cs` | Interfaz, animación, estado, cierre automático y operación asíncrona |
| `Theme.cs` | Paleta de acento del loader |
| `LicenseClient.cs` | Autenticación, descarga y verificación de módulos y planes firmados |
| `LicenseLeaseHost.cs` | Renovación y protocolo dúplex con ACK por tubería del usuario |
| `GameSession.cs` | Detección y preparación del juego |
| `PeImage.cs` | Validación de la DLL sin ejecutarla |
| `LibraryLoader.cs` | Carga estándar, confirmación y liberación de recursos |
| `NativeMethods.cs` | Firmas Win32 y handles con cierre automático |
| `LoaderSettings.cs` | Preferencias, registro local y resolución de la DLL en modo dev |
| `DevModule.cs` | Copia con hash de la compilación local usada por `--dev` |

## Referencias

- [LoadLibraryW — Microsoft](https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-loadlibraryw)
- [CreateRemoteThread — Microsoft](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createremotethread)
- [GetModuleHandleExW — Microsoft](https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulehandleexw)

La compilación y la ejecución dentro de L4D2 son comprobaciones separadas. Esta entrega no incluye una prueba de carga en una partida.
