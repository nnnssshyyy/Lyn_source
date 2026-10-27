# Rapidfire: rollback a la política anterior (módulo 3.1.7)

Documento histórico del rollback. La implementación local actual incorpora la [revisión e integración de Raze](RAZE_RAPIDFIRE_INTEGRATION.md).

**Estado: módulo 3.1.7 publicado y verificado; pendiente de validación en partida.**

El cambio de 3.1.6 (ráfagas acotadas, una por pulsación y re-armado con ACK) fue revertido a
petición del usuario. Se restauró el comportamiento de 3.1.4/3.1.5 y se publicó el binario
previo exacto como 3.1.7.

## Comportamiento actual

- **Mantener pulsado repite.** Mientras el rapidfire está activo, `RapidReady` arma extras
  para el comando base y `rapid_frame_ready` los publica al render; al soltar se deja de armar.
- **Cantidad por frame.** `N = max(configured_speed, int(0.06 / interval_per_tick + 0.5))`,
  con el slider de velocidad (1–200). Los extras repiten el tick nativo con `Prediction::Update`.
- **Tres métodos** (el menú los llama `Methot 1`, `Methot 2`, `Methot 3`):
  - `Methot 1` / Storm: velocidad + factor Storm (1–100); el canal declara `-1`/`255`.
  - `Methot 2` / Choked: velocidad + paquetes retenidos (1–255); activa el envío.
  - `Methot 3` / Flush: velocidad; el canal declara `-1`/`255`.
- **Correcciones por comando.** El anillo de 150 posiciones guarda el desplazamiento de
  `m_nTickBase`; `RunCommand` lo resta y limpia la posición del comando. No espera el ACK
  del servidor.
- **Pistol rapid** alterna los comandos de ataque como antes.

## Riesgo conocido

Esta es la implementación anterior a 3.1.6. El fallo reportado —congelamiento breve al
soltar y retroceso a la posición anterior 2–3 segundos después— puede reaparecer, porque las
correcciones se aplican y se descartan sin esperar el ACK del servidor y el canal declara
paquetes retenidos. El diseño de 3.1.6 que lo corregía está archivado en
`build/rapidfire-stability/source-3.1.6/` (fuentes y binario original) y sigue disponible en
el historial de publicación como módulo 3.1.6. Si más adelante se quiere estabilidad sin
limitar a una ráfaga, la base correcta es 3.1.6: conservar el anillo hasta el ACK y eliminar
solo el bloqueo de re-armado, sin volver a las escrituras ficticias del canal.

## Verificación de la entrega

- Build Release y suite completa: **15/15 suites**, 0 fallos; `rapid_fire_tests` con
  **49 checks** (política anterior).
- Original: 8.559.616 B, `sha256 1C25F9C4…`.
- UPX 5.2.1 (`--best --lzma --strip-relocs=0`): 3.633.152 B,
  `sha256 16F7636F…`; `upx -t` OK.
- `check_load`: carga x86 nativa, reubicación y **11 recursos** (2.862.261 B) idénticos.
- Roundtrip UPX: `.text`, `.data`, `.fptable` y `.reloc` idénticos.
- Publicación 3.1.7 por `POST /api/admin/module`; E2E con licencia QA: firma ECDSA del
  manifiesto verificada con OpenSSL, descarga autenticada de 3.633.152 B y hash correcto.
  Licencia revocada y usuario QA deshabilitado.
- `in_game_tested: false`.

## Artefactos

- `build/rapidfire-stability/source-before/`: fuentes restauradas de 3.1.4/3.1.5.
- `build/rapidfire-stability/source-3.1.6/`: fuentes, manifiesto y binario original de 3.1.6.
- `build/compressed/build-info.json`: manifiesto vigente 3.1.7.
- `build/rapidfire-stability/source-3.1.6/build-info-3.1.6.json`: manifiesto anterior.
