# Salud del ESP: Raze y L4D2

Análisis local con IDA Pro MCP del archivo `RZL4D2_[unknowncheats.me]_.dll`
(SHA-256 `dfa8632e85c89c318577ef619f20bae2f4db5136042d680ee4dae4109234691a`)
y del `client.dll` instalado de Left 4 Dead 2. Direcciones relativas a
imagen base `0x10000000`; se refieren a estas versiones concretas.

## Ruta de Raze

- `sub_1002EA20`, `0x100303BD`–`0x1003044B`: toma la salud con una llamada
  virtual en `entidad->vtable + 0x1D4` (índice 117) y pinta el número con
  `"%d"`. Los bloques equivalentes comienzan en `0x10030319`,
  `0x100304F9` y `0x1003047B` para otras posiciones/opciones de la barra.
- `sub_10035E70`, `0x10035EF0`–`0x10035F19`: resuelve
  `CTerrorPlayer.m_iMaxHealth` para el denominador de la barra.
- `sub_10167260`, `0x10167260`–`0x101672C9`: calcula un degradado a partir
  de salud actual / salud máxima. `sub_101671B0` elige colores por tramos
  (`<25`, `<40`, o superior). Estas funciones no alteran la cifra.
- `sub_101669E0`, `0x101670D2` y `0x10166E79`–`0x10166E81`: lee
  `m_isIncapacitated` y usa rojo para ese estado. Tampoco recalcula la salud.

La búsqueda de cadenas de este binario no encontró `m_healthBuffer` ni
`m_healthBufferTime` en la ruta del ESP. Raze delega la salud temporal al
método virtual del juego.

## Qué hace el método del juego

La vtable de `C_TerrorPlayer` en `client.dll` está en `0x105B6494`.
Su índice 117 apunta a `0x102522A0`, cuyo desensamblado muestra:

1. Leer `m_healthBuffer` de `entidad + 0x1FAC`.
2. Si es positivo, restar `(tiempo_del_juego - m_healthBufferTime) *
   pain_pills_decay_rate`, con mínimo cero. El tiempo se lee de globals;
   `m_healthBufferTime` está en `entidad + 0x1FB0`.
3. Llamar al índice 116 (`vtable + 0x1D0`), que en esta vtable apunta a
   `0x10069880` y lee `m_iHealth` de `entidad + 0xEC`.
4. Sumar salud base y temporal, y convertir a entero con `cvttss2si`, es
   decir, truncar hacia cero. No resta uno ni usa `ceil`.

La tasa queda vinculada a `pain_pills_decay_rate`: el inicializador en
`0x10518EE0` usa el nombre de esa variable, valor por defecto `0.27`, y el
objeto de convar `0x107CEC70`, cuya referencia usa `GetHealth`.

Por tanto, la cifra que Raze muestra puede modelarse como
`trunc(m_iHealth + max(0, m_healthBuffer - elapsed * pain_pills_decay_rate))`.
El servidor ya actualiza `m_iHealth` durante una incapacidad; aplicar además
`survivor_incap_decay_rate` a la reserva temporal descontaría dos veces.

## Integración

El ESP llama al mismo índice virtual 117 para supervivientes, con lectura
segura del puntero y validación del resultado. Calcula la parte temporal para
la etiqueta como `GetHealth() - m_iHealth`, muestra `total (base+temporal)`
cuando corresponda y `actual/máxima` al estar incapacitado. Si el método no
está disponible, una ruta de respaldo usa los campos de red con el mismo
truncamiento. El valor de Raze es el resultado del método del cliente: no
constituye una lectura directa de un estado privado del servidor.
