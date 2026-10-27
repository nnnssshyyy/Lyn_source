# Empaquetado

Scripts de la última fase de entrega, movidos desde `build/` para que sobrevivan a la
limpieza de esa carpeta. Se ejecutan desde la raíz del proyecto y siguen usando las rutas
de preparación en `build/` (`compressed`, `vs3-license`, `server-hooks`, `before-*`) y
`build-current/` como árbol de compilación validado.

| Script | Función |
| --- | --- |
| `package-aim-accuracy.py` | Empaqueta la entrega de precisión de aim (`build/aim-accuracy`). |
| `package-vs3-license.py` | Empaqueta la entrega de licencias VS 3 (`build/vs3-license`). |
| `package-server-hooks.py` | Empaqueta la entrega de hooks por servidor (`build/server-hooks`). |
| `package-rapid-raze.py` | Empaqueta la integración de rapid fire desde el build local `build/Release`, con cuatro suites y verificación de carga/reubicación/recursos. |

Los tres scripts históricos comprimen `lyn.dll`, verifican los resultados de pruebas de `build-current/Testing`
y copian la DLL final a `build/Release` y `build/loader`. Son revisiones históricas: los
números de comprobaciones esperados corresponden a la fecha de cada entrega.

La entrega de rapid fire usa `build/Release` y `build/Testing`, conserva el binario original
en `build/raze-rapid-integration/` y valida la DLL comprimida antes de copiarla a las rutas
de entrega. No modifica el ejecutable del loader ni publica en un servidor.
