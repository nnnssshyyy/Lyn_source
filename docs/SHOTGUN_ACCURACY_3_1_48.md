# 3.1.48 — Accuracy de escopetas

En **Combate → Armas**, Pump, Chrome, Auto y SPAS tienen **Accuracy (cliente)** y un slider **Accuracy de 0 a 100 %**. Cada escopeta conserva su propio porcentaje y bind en el perfil. Clic derecho sobre «Accuracy (cliente)» permite asignar una tecla y los modos alternar, mantener activado o mantener desactivado.

Este control regula la compensación angular del cliente aceptada por el usuario: 0 % no corrige el eje, 50 % aplica la mitad del offset y 100 % aplica la compensación completa. No modifica el patrón de perdigones calculado por el servidor. El aim y el ajuste genérico de dispersión respetan este control para las escopetas; las demás armas conservan su corrección anterior. Los perfiles antiguos con la compensación de escopeta activada mantienen su estado y bind, con 100 % si aún no guardaban un porcentaje.

La muestra aleatoria original se conserva en caché y la intensidad se aplica al corregir el ángulo. Si cambia al repredicir el mismo comando, se recupera primero el eje original para evitar acumular correcciones. Valores inválidos desactivan la corrección y los superiores al límite se acotan a 100 %. No se modifica la semilla ni el número de comando.

Validación: Win32 Release; 163 comprobaciones de compensación y 58 de perfiles, incluidos límites, independencia por arma, bind y migración. La vista previa ejecuta además el procesador real de binds y captura las cuatro escopetas con distintos porcentajes. No se han medido impactos en una partida.
