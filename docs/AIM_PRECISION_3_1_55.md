# Precisión de aim y limpieza del menú — 3.1.55

Se corrigieron desajustes de selección, disparo y transporte compartidos por
normal, Silent Client y Silent Server. Se eliminaron los párrafos de ayuda de
aim infectado, fast melee, Charger Teleport y Accuracy de escopetas. Los
controles, binds, encabezados y validaciones de selección siguen disponibles.

## Selección y disparo

Los tres modos prefieren ahora los huesos actuales de un objetivo visible.
Silent Server anteriormente buscaba primero el historial, por lo que podía
apuntar a una posición diferente mientras el infectado seguía expuesto.
El historial se conserva como alternativa válida cuando la posición actual
está cubierta. Un objetivo vivo no se abandona solamente porque se estimó que
el disparo anterior debía matarlo.

Una solución actual exige que el trazado del motor alcance la entidad elegida.
Llegar a un punto vacío o quedar bloqueado muy cerca de él ya no cuenta como
impacto confirmado. Los puntos históricos requieren un trazado completamente
libre o un impacto sobre la entidad seleccionada. Cuando reaparece el centro
de una hitbox, se recupera su margen interior en lugar de seguir apuntando a
una muestra estrecha del borde.

En normal, la pulsación manual de ataque también espera a que el suavizado
entre en la tolerancia angular calculada con el tamaño y distancia de la
hitbox. Antes se protegía únicamente el disparo automático. Sin objetivo
seleccionado se conserva el disparo manual. Un punto histórico reutilizado en
una ráfaga no puede dispararse si ya no permite calcular un tick válido.

## Silent Server

La retención del disparo y el envío de recuperación se deciden una sola vez al
final del comando. Antes se escribía `send=false`, se marcaba el envío del
siguiente comando y esa misma marca ocasionaba inmediatamente `send=true` en
el comando actual. La retención quedaba anulada.

Se preparan los comandos con ataque mantenido incluso durante el cooldown
estimado, porque el reloj nativo decide qué comando dispara realmente. Esos
comandos reciben el ángulo elegido y se retienen. El comando posterior envía
la recuperación de vista y suprime un ataque primario adicional sin alinear.
Los comandos sin ataque conservan su dirección visible. Las órdenes de envío
de recuperación y el transporte de ráfagas mantienen prioridad sobre la
retención de Silent Server.

## Validación

Las pruebas ejecutan el selector real con entidades, huesos y trazados
deterministas. Cubren cambios de visibilidad, paredes junto al punto, huesos
sin impacto, recuperación del centro, objetivos vivos tras un disparo,
historial como alternativa y los tres modos. También ejercitan el suavizado
real con varias intensidades y comprueban que el primer disparo manual entre
en la tolerancia, además de las decisiones de retención y envío.

El registro de compilación, CTest y las capturas nativas están en
`build/aim-precision-3.1.55/`. No se han medido impactos en una partida real.
Estas correcciones no garantizan todos los impactos: la aceptación del
servidor, latencia, obstrucciones y dispersión residual siguen interviniendo.
