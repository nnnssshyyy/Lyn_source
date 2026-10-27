# 3.1.53 — Cajas de objetos según el modelo renderizado

El ESP de objetos sumaba los límites de colisión al origen y proyectaba esa
caja como si el modelo no estuviese rotado. En modelos asimétricos o colocados
de lado, como las armas de la pared de Tumtara, eso desplaza el marco y el
centro usado por el nombre.

La captura de objetos utiliza ahora GetRenderBounds y
RenderableToWorldTransform del renderable del modelo. Transforma las ocho
esquinas locales a coordenadas del mundo y las proyecta con el mismo recorte
del plano cercano que usa el ESP. Los nombres y distancias se colocan sobre
la caja proyectada. Se conservan rotación, pivote asimétrico y escala que
proporciona el motor; no se convierte primero a una AABB del mundo más grande.

La interfaz pública de Valve describe los
[límites locales y la transformación del renderable](https://github.com/ValveSoftware/source-sdk-2013/blob/master/src/public/iclientrenderable.h).
Los índices de esa versión del SDK no se copian a L4D2: se verificaron
separadamente en el client.dll instalado mediante RTTI y desensamblado.
En este binario los slots son 17 para los límites y 32 para la matriz.

Las esquinas se capturan en el hilo del juego y se publican con el snapshot;
el render no consulta entidades. No se cachean poses de objetos entre frames.
Se rechazan límites invertidos, matrices no finitas o singulares y matrices
cuya posición no concuerda con el objeto. Si falta geometría válida no se
inventa una caja de colisión: puede mantenerse la etiqueta sobre el origen
para efectos sin un modelo físico.

El cambio es automático para armas, consumibles y otros objetos del mundo.
Los jugadores mantienen su cálculo anterior y box precision sigue apagado
por defecto. No se sustituyen perfiles, colores, filtros, glow o chams.

Validación geométrica: 193 comprobaciones con yaw, roll, escala, pivotes
asimétricos, recorte de cámara y geometría ausente. La API simulada comprueba
los dos métodos nativos y el descarte de datos anteriores ante fallos. El
snapshot comprueba la copia de las nuevas esquinas en el límite del pool.
Estas pruebas no constituyen una validación visual en una partida real.
