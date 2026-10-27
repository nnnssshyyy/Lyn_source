# Perfil de Steam en el menú, 3.1.36

El pie de la barra lateral muestra el nombre público de Steam en la primera
línea y el avatar de la cuenta conectada. Debajo conserva el perfil de ajustes
activo y el asterisco de cambios pendientes. Los nombres se dibujan como texto
literal UTF-8; los que no caben se acortan sin partir caracteres.

Se consulta la sesión que el juego ya abrió mediante los exports públicos de
`steam_api.dll`: `GetPersonaName`, `GetSteamID`, `GetLargeFriendAvatar`,
`GetImageSize` y `GetImageRGBA`. No se inicializa ni se cierra Steam desde el menú.
Las firmas corresponden a la [API plana de Valve](https://github.com/ValveSoftware/source-sdk-2013/blob/master/src/public/steam/steam_api_flat.h).
Los accessors usados (`SteamFriends_v017`, `SteamUser_v021`, `SteamUtils_v010`)
están exportados por la copia instalada de Left 4 Dead 2.

Las consultas se limitan a una cada dos segundos con el menú visible. El avatar
se decodifica una sola vez por identificador de imagen y se conserva en caché.
Un resultado pendiente se reintenta; cambiar de cuenta descarta la foto anterior.
Se validan las dimensiones antes de reservar memoria y se reconstruye la textura
tras invalidar el dispositivo gráfico. Sin datos disponibles se muestran «Steam»
y la imagen de reserva.

Fuentes: [nombre y avatar](https://partner.steamgames.com/doc/api/ISteamFriends),
[tamaño y píxeles RGBA](https://partner.steamgames.com/doc/api/ISteamUtils).

## Validación

- Compilación Release Win32 de la DLL y del arnés: correcta.
- Arnés: 49 comprobaciones, cero fallos. Incluye trece comprobaciones nuevas de
  nombres Unicode/`##`, espera de avatar, caché, renombrado, cambio de cuenta,
  dimensiones inválidas, error de decodificación, recuperación y sesión ausente.
- Tres suites de renderizado y persistencia: aprobadas.
- Siete capturas generadas en `build/steam-profile-qa/`; el arnés independiente
  no tiene la sesión de Steam del juego, por lo que muestra la reserva. Se revisó
  la distribución del pie en las tres escalas.
- La DLL comprimida pesa 3 676 160 bytes y pasó `upx -t`. El informe de paquete
  conserva los hashes y la comparación de recursos y secciones tras descomprimir.
- **Pendiente:** comprobar el nombre y la foto reales dentro del juego. No se
  afirma haber probado la conexión con la cuenta ni la carga de la DLL en partida.

Candidata comprimida: `build/steam-profile-3.1.36/lyn.dll`.
Original sin comprimir: `build/steam-profile-3.1.36/lyn-original.dll`.
Publicada como módulo **3.1.36** mediante el panel privado de Vercel. El panel
confirmó firma y descarga autenticada con SHA256
`95add9b1f31a39754c0896d63c7112343e4abf5e2493f5e1306deb559a7f1da2`.
Las copias de Release, loader y compressed coinciden con esa entrega; la versión
anterior se conservó en `previous-3.1.34/` dentro de la carpeta de publicación.

Accounts actualizó ambas variantes del módulo desde el commit
`7c28801cb7c196448e87ca932f966a9f9545fbf2`, con 37 pruebas aprobadas. Vercel
confirmó el despliegue `G8sioDdiCFjGtfzomtitRRXgVH6o` como Ready y producción
actual para `vs3-accounts.vercel.app`. Los comprobantes están en `published.jpg`,
`accounts-production.jpg` y `verification.json` dentro de la carpeta de entrega.

Para repetir el arnés desde la raíz del proyecto:

```powershell
cmake --build build-current --config Release --target lyn --parallel 4
cmake -S tools/menu-ui-preview -B build/steam-profile-qa/out -A Win32
cmake --build build/steam-profile-qa/out --config Release --parallel 4
.\build\steam-profile-qa\out\Release\preview.exe build/steam-profile-qa
```
