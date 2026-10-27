# Prioridad por infectado especial · módulo 3.1.33

En **Filtros → prioritize infected** se pueden marcar Smoker, Boomer, Hunter,
Spitter, Jockey, Charger y Tank de forma independiente. Los marcados forman el
primer grupo de selección en normal, silent client y silent server. Dentro del
grupo se conserva la política elegida: FOV, distancia o smart shot.

Si ningún marcado ofrece un punto válido, el selector continúa con los demás
objetivos habilitados. La prioridad no habilita categorías desactivadas ni
omite los filtros de equipo, amigos, fantasma, muerte, visibilidad, FOV o alcance.
Tank mantiene su interruptor general independiente. Witch no recibe prioridad
por estar entre Charger y Tank en el enum.

El seguimiento estable cambia inmediatamente de un objetivo no prioritario a
uno marcado válido. Conserva la estabilidad entre objetivos del mismo grupo;
el último ajuste de FOV no puede restaurar un objetivo de prioridad inferior.
Cambiar los tipos marcados invalida la selección retenida. El presupuesto
existente de 64 candidatos evaluados y 192 trazas no aumenta.

Las siete preferencias forman parte del registro de configs y del snapshot
del hilo del juego. Son globales para todas las armas, incluidas aquellas con
perfiles propios. En configs anteriores quedan desmarcadas, conservando el
orden previo. El buscador del menú dirige sus resultados al panel Filtros.

## Validación

- Release Win32 compilado correctamente.
- 28 escenarios de política, 239 comprobaciones del selector/cámara y 31 de
  persistencia entre 50 armas: tres suites sin fallos.
- El selector real se ejercitó en los tres modos y las tres políticas, con
  cambio de prioridad, retorno de visibilidad, fantasma, FOV, alcance y filtros
  generales. Se comprobaron las siete clases y varias selecciones simultáneas.
- Menú real renderizado con D3D9 fuera de partida, sin cargar en el juego:
  `build/special-priority-qa/filters.png`.
- Empaquetado: integridad UPX, carga x86 con reubicación, 11 recursos y secciones
  de código/datos/reubicación idénticas tras descomprimir.

Entrega local: `build/special-priority-3.1.33/lyn.dll`; copia para el loader en
`build/loader/lyn.dll`. Fuentes anteriores en
`build/special-priority-source-3.1.32/`; DLL anterior incluida en la entrega.
Esta revisión no se ha probado en partida.

## Publicación

El panel privado de Vercel publicó el módulo 3.1.33 y verificó su manifiesto
firmado y su descarga autenticada. Archivo de 3 663 872 bytes, SHA-256
`372a5ac440e2bbdc288f3cee97ce71f90a62ee01d7e44e9551a6749d5ca9d6d5`.
Confirmación en `build/special-priority-3.1.33/published.png` y
`verification.json`. El loader conserva la versión 3.1.11 en ambas ediciones;
recibe el módulo nuevo desde la API vigente. No se desplegó el portal histórico
de Sites ni se anunció una migración a una VPS.

Accounts y Admin completaron sus despliegues de Vercel desde el commit
`95508e565859f4d1953337ee6b6323801e727ee1`. Ambos manifiestos de las ediciones
compacta y completa muestran módulo 3.1.33 y conservan los hashes del loader.
