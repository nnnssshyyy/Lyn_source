# Aimbot normal sin seguimiento en ZoneMod / Confogl

El síntoma comunicado es que la mira no sigue al objetivo. No se ha reproducido
en el servidor del usuario. El registro más reciente de `build/loader/lyn.log`
mostraba hooks instalados y resolución del movimiento, sin intentos de aim. Los
registros anteriores contienen intentos en distintos modos, pero no identifican
la configuración competitiva del servidor. No permiten atribuir el fallo a la
predicción ni a un bloqueo remoto.

Hay una condición comprobable en la configuración: un perfil con `custom combat
settings` puede sustituir el modo principal. Su modo predeterminado es `silent
client`. Seleccionar `normal` en el panel principal no elimina esa sustitución.
La nueva interfaz muestra el modo y auto shoot efectivos, el arma equipada y si
la configuración procede del perfil del arma.

El botón **apply main aim to this weapon** copia al arma equipada el interruptor
de aim, modo y auto shoot del panel principal. Mantiene los demás ajustes por
arma. La copia se realiza únicamente al pulsar el botón; no se cambian todos
los perfiles automáticamente.

Desde el módulo 3.1.13, por petición del usuario, **always / auto shoot** activa
el seguimiento y el disparo automático sin pulsar ataque. Tiene efecto en normal,
silent client y silent server, aunque el ajuste guardado de auto shoot del arma
esté apagado. El arma debe estar lista; normal también espera la alineación del
suavizado. Hold key y on attack conservan su preferencia guardada de auto shoot.
El combate automático se pausa en modo fantasma para evitar un spawn involuntario.

Para comprobar el seguimiento básico con la nueva DLL:

1. Reinicia el juego y carga `build/loader/lyn.dll` o la copia de Release.
2. En aimbot, activa **enabled**, elige **normal** y **activation: always / auto shoot**.
   Si hay un perfil de combate equipado, pulsa
   **apply main aim to this weapon**.
3. Comprueba en **Aim status** que aparezca **Effective: normal (enabled)** y
   **Effective auto shoot: on**. Desactiva temporalmente speedhack/rapidfire
   durante esta comprobación para aislar el seguimiento.
4. En los filtros, habilita los tipos que quieres seguir: special infected o
   Tank desde superviviente; enemy survivors desde infectado. Los compañeros
   de equipo y los infectados fantasma no son objetivos. Si un enemigo es amigo
   de Steam, el ajuste target Steam friends determina si puede seleccionarse.
5. Cierra el menú y prueba ante un enemigo visible y activo. Al abrirlo de
   nuevo, **Last aim attempt** conserva el resultado anterior. El aviso actual
   de menú abierto aparece por separado y no borra los contadores.

| Resultado | Qué revisar |
| --- | --- |
| Esperando tecla / ataque | La activación efectiva sigue en hold key / on attack. |
| Aim desactivado en el perfil | El interruptor de asistencia del arma equipada. |
| Sin entidades elegibles | Contadores friends/types/ghosts/FOV/range; ayudan a distinguir filtros de búsqueda de hitboxes. |
| Candidatos sin hitboxes | Resolución de StudioModel/SetupBones o estado de la entidad. |
| Hitboxes sin trazas | Máscara de zonas o FOV de los puntos concretos. |
| Hitboxes obstruidas | Visibilidad del objetivo seleccionado. |
| Ángulo aplicado, sin movimiento visible | Modo efectivo y setter de la cámara; un fallo de esa llamada ahora se registra explícitamente. |

Los contadores se actualizan en la selección real. Se leen fantasmas replicados
para contarlos y se descartan antes de hacer trazas. Los filtros conservan su
función. Los mensajes de activación se registran con límite de frecuencia y
también incluyen modo efectivo, perfil, activación y tecla.

En la entrega 3.1.13 pasaron **18 suites**, incluidas **76 comprobaciones de
selección y cámara**. Los casos ejercitan el selector de producción con fronteras
del motor simuladas: especiales en slots de jugador, amigos de Steam, categorías
desactivadas, fantasmas, supervivientes enemigos, rango/FOV y permiso de disparo
automático. Estas pruebas no certifican seguimiento ni impactos en un dedicado.

Esta entrega añade diagnóstico y controles para corregir una configuración
efectiva distinta de la esperada. No incorpora todavía predicción completa de
movimiento; se necesita observar el motivo real del fallo antes de atribuirle
el problema de seguimiento.
