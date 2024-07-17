INSERT INTO prueba2 (nombre_tabla, operacion, usuario_actual, detalle_anterior, detalle_nuevo)
VALUES ('nombre_de_la_tabla', 'INSERT', 'usuario', 'detalle previo', 'insert');


UPDATE prueba2
SET nombre_tabla = 'maximiliano',
    operacion = 'UPDATE',
    usuario_actual = 'nuevo_usuario',
    detalle_anterior = 'nuevo_detalle_previo',
    detalle_nuevo = 'nuevo_detalle_nuevo'
WHERE id = 1; -- Cambia '1' por el ID del registro que deseas actualizar



DELETE FROM prueba2
WHERE id = 1; -- Cambia '1' por el ID del registro que deseas eliminar



SELECT * FROM auditoria;
SELECT * FROM prueba1;
SELECT * FROM prueba2;

-- Para filtrar por alguna condición específica, por ejemplo, por nombre de tabla
SELECT * FROM prueba2
WHERE nombre_tabla = 'nombre_de_la_tabla';
