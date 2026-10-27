# Compresión del loader

## Loader 3.1.6

Conserva el empaquetado silencioso y añade transiciones con easing de entrada,
cierre, login y botones. El fondo interpola su iluminación a 60 Hz, se pausa al
minimizar y respeta la preferencia de animaciones de Windows. No cambia el flujo
de cuentas, entrega firmada ni limpieza tras la confirmación del módulo.
La publicación incluye el módulo 3.1.15; véase `docs/PRIVACY_AND_REMOVALS.md`.

## Loader 3.1.5

Se mantiene el paquete LZMA2 autónomo, con `Progress="no"` para abrir directamente la interfaz y recursos de icono VS 3 en el ejecutable exterior y el interior. Windows puede ocultar `.exe` en el Explorador; el archivo distribuido sigue siendo una aplicación ejecutable.

Tras la confirmación del plan del módulo, un proceso breve de limpieza espera a que terminen el loader y el extractor, verifica el SHA-256 y elimina exclusivamente el ejecutable que inició esa sesión. No se programa el borrado ante errores, cancelación o modo de desarrollo. El extractor elimina su propia carpeta temporal. Los registros y preferencias conservan sus rutas habituales.

Validado con 13 pruebas de limpieza, 7 de transporte y petición nativa, renderizado de las dos pantallas y un paquete desechable real que comprueba la identificación del ejecutable exterior, su eliminación y la eliminación de la extracción temporal. El resultado se guarda en `build/loader-3.1.5/validation.json`. La DLL continúa en 3.1.12.

## Loader 3.1.4

Se conserva el loader WPF x86 autónomo y se distribuye mediante el extractor oficial de LZMA SDK 26.04. No se recortan ensamblados de WPF ni se añade un requisito de instalar .NET. La interfaz, las animaciones, la autenticación y la entrega firmada del módulo mantienen su implementación.

| Archivo distribuido | Bytes | MiB |
| --- | ---: | ---: |
| Loader 3.1.3, single-file .NET comprimido | 66.176.195 | 63,11 |
| Loader 3.1.4, paquete LZMA2 | 41.520.976 | 39,60 |

Reducción del archivo distribuido: 37,26 %. Al abrirlo, el extractor prepara el ejecutable autónomo de 151.325.708 bytes en una carpeta temporal, lo ejecuta, espera a que termine y elimina esa carpeta. No modifica el registro ni instala un runtime. Los registros, preferencias y módulos del loader conservan sus rutas existentes en `%LOCALAPPDATA%\lyn\loader`.

`loader/build-loader.ps1` publica el ejecutable sin compresión interna y llama a `loader/pack-loader.ps1`. El empaquetado verifica la herramienta oficial mediante hashes fijados, comprueba la integridad del archivo 7z y compara el SHA-256 del ejecutable extraído con el original antes de sustituir la entrega. También conserva la versión 3.1.4 y el manifiesto `asInvoker` en el ejecutable exterior.

SDK: [LZMA SDK oficial](https://www.7-zip.org/sdk.html), versión 26.04. SHA-256 del archivo descargado: `bd98058d0e12c5da970dd4e5e1d8fa24f2d704914d96e69c28f92ed45421a439`.

Comprobaciones realizadas:

- Compilación Release autónoma, x86, sin errores.
- Prueba del archivo final y extracción exacta del ejecutable original.
- Rechazo de una copia con un byte alterado en el contenido comprimido.
- Manifiesto final `asInvoker` y versión 3.1.4 comprobados con las herramientas de Windows.
- Arranque del paquete final y observación de la interfaz con licencia validada; no se automatizaron credenciales ni la carga en el juego.
- Las seis pruebas de la descarga web pasan con los cinco fragmentos nuevos, incluyendo corrupción, respuestas incompletas y cancelación.
- La DLL 3.1.9 conserva su SHA-256 `93ad2f1464eb755d87eb439a4b721220d8827555dffd876034b279e46381f960`.

SHA-256 del paquete: `390a5f1054cb3093d5a07e8a32157d60fb0b01f7901499232e3b9074dc8bfa55`.

El resultado y los hashes se registran en `build/loader/loader-build-info.json`. `-NoPackageCompression` mantiene disponible la publicación single-file comprimida de .NET como alternativa para diagnóstico.
