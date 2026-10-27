# Commons muertos como objetivos: módulo 3.1.14

El filtro de NPC aceptaba commons sin `m_iHealth` ni `m_lifeState` replicados
mientras conservaran colisión. Faltaba excluir la secuencia de muerte: durante
esa transición pueden seguir siendo entidades legibles y sólidas. La retención
de normal/silent client y el historial podían seguir usando sus hitboxes.

La comprobación común a normal, silent client y silent server ahora descarta
etiquetas de secuencia que contienen `Death`, sin distinguir mayúsculas. Usa
el nombre resuelto por el motor, sin números de animación fijos. También se
resuelve `m_bClientSideRagdoll` por nombre y se descarta un NPC cuando esa bandera
está activa. Se mantienen las comprobaciones existentes de vida y colisión.
La ausencia de salud replicada sigue siendo válida para un common vivo.

Al descartar un cadáver se eliminan sus poses históricas, incluso si aún no ha
llegado el siguiente frame de captura. El estado se asocia al handle completo,
por lo que un nuevo infectado en el mismo slot no hereda el estado de muerte.
Las etiquetas se consultan cuando cambia la secuencia; una consulta fallida
no queda guardada como definitiva y se vuelve a intentar.

La regresión reproduce entidades que siguen legibles y sólidas durante su
animación de muerte, con historial activado y desactivado en los tres modos.
Antes de la corrección fallaban 26 comprobaciones del selector. Las pruebas
también cubren sustitución por otro objetivo vivo, cadáveres sin trazas,
invalidación de poses, reutilización de slots y recuperación de etiquetas.

La implementación se contrastó con `is_alive()` del código local de RijiN
(`src/cheats/left4dead2/structs/base_entity.cpp`, líneas 257–271), que también
rechaza secuencias `Death`, y con su volcado de la propiedad
`DT_BaseAnimating_m_bClientSideRagdoll`. No se copian offsets de ese volcado.
El [volcado de netprops de L4D2 de AlliedModders](https://wiki.alliedmods.net/Left_4_Dead_2_Netprops_3#Infected:)
documenta las propiedades de animación y colisión del infected.

Las pruebas automatizadas no sustituyen una comprobación en partida real.

Validación de esta entrega: 18 suites nativas completas, 130 comprobaciones de
selección/cámara, 40 de política de aim y 97 del runtime. La DLL comprimida pasó
integridad UPX, carga x86 con reubicación y comparación de los once recursos.
El publicador privado verificó su firma y descargó los mismos 3.649.024 bytes.
SHA-256: `3657a88a633aa69b4138f431ad6eedd9493509cf78cf1c320b7a4a713e9a496a`.
