# Fuente compacta y nítida para ESP · módulo 3.1.34

El overlay usa Inter Bold de 12 píxeles, integrada en la DLL. Nombres, clase,
distancia, vida, flags y etiquetas de objetos se alinean a píxeles enteros y
conservan su sombra fina. La vista previa del Hunter usa la misma familia y
tamaño para nombres, distancia y vida.

La familia del HUD se rasteriza una sola vez en tamaños enteros de 10 a 18 px.
Las etiquetas de daño, amenazas fuera de pantalla y el HUD Tank/Witch eligen
la variante más cercana y dibujan a su tamaño nativo. La medida del texto usa
esa misma variante; no se amplía o reduce la textura de una fuente de 12 px.
Los glifos latinos extendidos y cirílicos se integran como fallback en el atlas.

La vida conserva 12 px y se coloca usando los límites visibles de los glifos.
Se conservan los dos pases existentes del texto del ESP. La carga y creación
de las fuentes ocurren durante la inicialización del renderer.

## Verificación

Release Win32 correcto y 34 comprobaciones de renderizado existentes sin
fallos. La vista DX9 usa la inicialización de fuentes de producción, comprueba
acentos/cirílico y verifica tamaños nativos. Para vida permanente, temporal e
incapacitación, el límite de texto queda a 1,00 px del relleno.

Vista: `build/esp-font-qa/preview.png`; registro de la comprobación en
`build/esp-font-qa/font-validation.log`. El render es fuera de partida.

Fuentes anteriores: `build/esp-font-source-3.1.33/`. La entrega local se prepara
en `build/esp-font-3.1.34/`; la copia para el loader está en
`build/loader/lyn.dll`. La revisión requiere todavía validación en partida.

La DLL 3.1.34 está publicada mediante el panel privado de Vercel, con firma y
descarga verificadas. Accounts muestra la misma versión tras el despliegue de
producción del commit `3425857`; pasaron sus 37 pruebas de descarga y seguridad.
El comprobante y los datos de publicación están en
`build/esp-font-3.1.34/published.jpg` y `verification.json`.
