# VS 3.1: distribución y hooks por servidor

El servidor de licencias entrega la DLL y genera los bytes de activación de los
hooks. Windows sigue cargando un PE normal; MinHook construye los trampolines
localmente para conservar la inicialización de MSVC, ImGui y los recursos.
Es una adaptación del reparto de responsabilidades de RijiN, no un port de su
imagen plana, rebasing remoto ni sus procedimientos de evasión.

## Flujo

1. El loader autentica al usuario y comprueba su licencia activa.
2. `/api/loader/manifest` firma SHA-256, tamaño, versión, PID, creación del
   proceso y nonce. La firma P-256 caduca en 120 segundos.
3. El loader descarga `/api/loader/module/<sha256>` con su sesión, verifica firma,
   tamaño, hash y PE x86 y guarda `modules/<sha256>/lyn.dll` en su caché local.
   Una descarga incompleta nunca sustituye el archivo final. El EXE ya no lleva
   la DLL incorporada.
4. Tras la carga de Windows, la DLL verifica su autorización temporal y prepara
   los trampolines. Las llamadas habituales de habilitación solo encolan hooks.
5. El trabajador de la DLL envía direcciones y prólogos al loader. Este solicita
   `/api/loader/prepare-hooks`; el servidor calcula E9 rel32 y, cuando corresponde,
   el salto corto del área de hotpatch.
6. La DLL verifica la firma, el proceso, el nonce, la vigencia de 30 segundos y
   la coincidencia exacta de toda la lista. MinHook vuelve a validar destino,
   bytes originales y parche antes de suspender hilos y aplicar esos bytes.
   Un cambio concurrente requiere solicitar otro plan.

El servidor no escribe en el proceso ni modifica un servidor dedicado de L4D2.
La renovación, HTTP y criptografía corren fuera de los hilos de juego/render.
La primera autorización válida queda ligada al proceso del juego para toda la
partida: el loader puede cerrarse tras la confirmación. Los hooks creados más
tarde construyen su parche localmente. Deshabilitar un hook y restaurar su prólogo no requiere Internet. El modo local original de MinHook se conserva para otros consumidores y pruebas.

## Transporte y publicación

La tubería `VS3.<pid>.<FILETIME>` es dúplex, del usuario actual, con longitud
acotada a 32 KiB, cancelación y ACK de recepción. Una conexión abandonada se
descarta sin detener el servidor ni la renovación independiente.

El administrador publica una DLL mediante `POST /api/admin/module`, protegida
por el panel privado. El API conserva versiones por hash en R2 privado y un
puntero de versión actual. Las descargas requieren sesión y licencia activa.
La clave privada de firma y la clave administrativa permanecen en el servidor.
Los clientes llevan únicamente la clave pública fijada.

## Correcciones de compatibilidad

- El arma equipada se resuelve por su handle completo y metadatos de inventario.
  No exige que su modelo en el mundo esté activo. Los objetivos conservan los
  filtros de dormancia, vida, geometría y visibilidad.
- En hitbox **automatic**, la selección intenta la zona preferida y después las
  otras zonas visibles. Los presets explícitos y las máscaras siguen respetándose.
- Bunnyhop usa el intervalo del comando. Si el salto sigue rechazado en el suelo,
  libera un comando antes de reintentar; mantiene la intención original durante
  repredicción y vuelve a armarse al aterrizar.

## Validación

- CTest: 551 comprobaciones en 14 suites, incluyendo los siete especiales,
  inventario dormido, salto/reintentos, firmas, snapshot de hooks, trampolines,
  cancelación de hooks pendientes, rechazo de parches y restauración.
- Transporte C# / C++: siete comprobaciones, con clientes que abandonan peticiones
  y respuestas. No se abre el juego.
- API: tres suites con SQLite real, firmas P-256, descarga de módulo, permisos,
  revocación y destinos reales de los saltos x86.
- `tests/loader_live` y `tests/live_hook_probe.cpp` recorren el servicio publicado,
  cliente de producción, tubería, verificación nativa y restauración en un proceso
  desechable, usando una cuenta temporal que se deshabilita al terminar.

La precisión, daño y comportamiento en ZoneMod/Confogl siguen requiriendo una
prueba en partida; estas verificaciones no los dan por confirmados.
