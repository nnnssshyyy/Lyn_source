# DLL comprimida

La entrega comprimida es `build/compressed/lyn.dll`, también copiada a
`build/Release/lyn.dll` y `build/loader/lyn.dll`. La compilación actual sin
comprimir está en `build-current/Release/lyn.dll`.

| Archivo | Bytes | MB decimales |
| --- | ---: | ---: |
| Original | 8 534 528 | 8,53 |
| Comprimida | 3 627 520 | 3,63 |

Reducción: **57,50 %**. Se utilizó [UPX 5.2.1](https://github.com/upx/upx/releases/tag/v5.2.1)
con `--best --lzma --strip-relocs=0`. No se eliminaron imágenes, modelos ni animaciones.
El ZIP oficial de UPX para Windows x64 se verificó con el SHA256 publicado en su
release: `eabc6792a347d45e945be7748423e7868fd01b0d2bcaa2f4b1031fd71ff69bda`.

## Validación

- `upx -t`: integridad correcta.
- Carga real con `LoadLibraryW` en un proceso de prueba x86 independiente, reservando
  la dirección base preferida para comprobar también la reubicación.
- Los once recursos RCDATA cargados (Hunter, modelo, textura y ocho imágenes de armas)
  coinciden byte por byte con los assets originales: 2 862 261 bytes en total.
- Tras descomprimir para verificar, las secciones `.text`, `.data`, `.fptable` y
  `.reloc` coinciden con el original. UPX modifica metadatos de depuración,
  importaciones y el directorio de recursos; el archivo restaurado completo no
  tiene el mismo hash que la entrada. Los contenidos de los recursos se comprobaron
  mediante las API de Windows después de cargar la DLL comprimida.
- No se ha probado esta copia dentro de una partida. Reinicia el juego antes de
  cargarla si ya estaba usando la versión anterior.

SHA256 original: `758062d0ce4ca77d4c175b1934082547a5efc611d22a8d66c533667da95b4e29`.

SHA256 comprimida: `3e4db85a1ec8f62d02ccf6a10dd86397faeb96435b245a94a32a07d11142fc94`.

## Repetir después de compilar

Usa una salida nueva y como entrada la DLL recién compilada, sin comprimir:

```powershell
$upx = 'C:\Users\User\.codex\tools\upx-5.2.1\upx-5.2.1-win64\upx.exe'
& $upx --best --lzma --strip-relocs=0 -o build/compressed/lyn-new.dll build-current/Release/lyn.dll
& $upx -t build/compressed/lyn-new.dll
```

El verificador de carga utilizado está en `build/compressed/validation/`.
