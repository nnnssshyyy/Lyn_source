# Dependencias incluidas

Se publica el contenido de los checkouts locales como archivos normales, sin sus carpetas `.git`, para que **Download ZIP** permita compilar sin inicializar submódulos. Se conservan los avisos y licencias de cada dependencia.

| Carpeta | Revisión de origen | Cambios locales |
| --- | --- | --- |
| `vendor/freetype` | `42608f77f20749dd6ddc9e0536788eaad70ea4b5` | Sin cambios de código en el checkout de origen. |
| `vendor/imgui` | `f5befd2d29e66809cd1110a152e375a7f1981f06` | Configuración local en `imconfig.h`. |
| `vendor/minhook` | `c3fcafdc10146beb5919319d0683e44e3c30d537` | Cambios locales en `include/MinHook.h` y `src/hook.c`. |
| `vendor/velocity-resourcer` | `0bbb573c1ae9ba12b9fe976d6a518f64dabb2dc2` | Recursos originales del menú. |

Los recursos necesarios para ejecutar la DLL están integrados durante la compilación. La procedencia y adaptación de la interfaz se documentan en [src/velocity_ui/PORT.md](src/velocity_ui/PORT.md).
