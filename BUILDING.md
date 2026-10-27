# Compilar y descargar Lyn

## Requisitos

- Windows con Visual Studio 2022 o posterior, la carga **Desarrollo para el escritorio con C++**, las herramientas MSVC para x86/x64 y un Windows SDK.
- CMake 3.24 o posterior. Puede ser el que incluye Visual Studio; `build.ps1` lo busca automáticamente.
- SDK de .NET 8 para compilar el loader de C# / WPF.
- Git solo si prefieres clonar en lugar de descargar el ZIP.

El objetivo es **Win32 / x86**, incluso en Windows de 64 bits. No uses `-A x64`. ImGui, FreeType, MinHook, las fuentes y los recursos gráficos se incluyen en el árbol de código, conservando las modificaciones locales y sus avisos originales. No se requieren submódulos ni un gestor de dependencias de C++.

## Descargar el código

En GitHub usa **Code → Download ZIP**, extrae el archivo y abre PowerShell dentro de la carpeta del proyecto. También puedes clonar:

```powershell
git clone https://github.com/nnnssshyyy/Lyn_source.git
cd Lyn_source
```

Los comandos de esta guía se ejecutan desde la raíz del repositorio.

## DLL y loader

```powershell
.\build.ps1 -Configuration Release
.\loader\build-loader.ps1 -Configuration Release -NoPackageCompression
```

La DLL se genera en `build/Release/lyn.dll`. La carpeta `build/loader/` contiene `Lyn.Loader.exe`, una copia de `lyn.dll` y el documento de uso `LEEME.md`. El ejecutable es autónomo: incluye el runtime de .NET para x86. `-NoPackageCompression` evita el paso adicional de empaquetado con el SDK LZMA; el single-file de .NET mantiene su compresión interna.

Alternativa manual para la DLL, con CMake disponible en el PATH:

```powershell
cmake -S . -B build -A Win32
cmake --build build --config Release --target lyn --parallel
```

Para el loader compacto, que requiere .NET Desktop Runtime 8 x86 en el equipo que lo ejecute:

```powershell
dotnet publish loader/Lyn.Loader.csproj -c Release -r win-x86 --self-contained false -p:EnableCompressionInSingleFile=false -p:DebugType=None -p:DebugSymbols=false -o build/loader-compact
```

## Pruebas

```powershell
cmake -S . -B build -A Win32 -DFREE_BUILD_AIM_TESTS=ON
cmake --build build --config Release --parallel
ctest --test-dir build -C Release --output-on-failure
dotnet run --project tests/loader_hardware/LoaderHardware.Tests.csproj -c Release
dotnet run --project tests/loader_credentials/LoaderCredentials.Tests.csproj -c Release
```

Las pruebas de renderizado pueden omitirse si el equipo no dispone de Direct3D 9. Las pruebas de hardware usan un servidor simulado en memoria. `tests/loader_license` es una prueba de integración aparte que necesita un ticket firmado vigente por la entrada estándar; no se ejecuta sin esa preparación. Estas comprobaciones no sustituyen una prueba del módulo dentro del juego.

Las herramientas opcionales de `tools/inspection` usan Python, `pefile` y `capstone`. Sus paquetes descargados no se incluyen; puedes prepararlos con `python -m pip install --target tools/inspection/inspection-deps pefile capstone`. No son necesarios para compilar la DLL o el loader.

## Descargar una compilación de GitHub

Abre **[Actions → Build Lyn](https://github.com/nnnssshyyy/Lyn_source/actions/workflows/build-lyn.yml)**, selecciona una ejecución correcta de `main` y descarga **lyn-windows-x86** en la sección **Artifacts**. El ZIP contiene el loader autónomo, la DLL, la guía de uso y las sumas SHA-256. GitHub puede pedir que inicies sesión para descargar artefactos; se conservan durante 90 días.

El flujo compila desde el código publicado y ejecuta las pruebas antes de generar el paquete. El propietario puede repetirlo desde **Run workflow**. También puedes consultar **[Releases](https://github.com/nnnssshyyy/Lyn_source/releases)** para las entregas publicadas con descarga directa.

## Servicio y modo de desarrollo

Esta publicación incluye el cliente; la web, el servidor de licencias, sus claves privadas y los datos de usuarios se excluyen. El loader conserva el inicio de sesión, las licencias y la autorización del módulo. Una compilación propia no genera credenciales ni elimina esos requisitos.

Para probar tu DLL con una cuenta autorizada:

```powershell
.\build\loader\Lyn.Loader.exe --dev --dev-dll "$PWD\build\Release\lyn.dll"
```

El modo de desarrollo selecciona la DLL local y conserva la autenticación. Consulta [loader/README.md](loader/README.md) y [docs/VS3_LICENSES.md](docs/VS3_LICENSES.md) para el flujo completo.
