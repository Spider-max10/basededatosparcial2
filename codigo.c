using System.Diagnostics;
using System.Reflection.Metadata;
using System.Text;


using System;
using System.Diagnostics;
using System.Reflection.Metadata;
using MySql.Data.MySqlClient;
using Document = iText.Layout.Document; // Alias para el espacio de nombres iText.Layout.Document
using System.Collections.Generic;
using iText.Kernel.Pdf;
using iText.Layout;
using iText.Layout.Element;
using System.Text;
using iText.Kernel.Colors;
using iText.Layout.Properties;
namespace DatabaseSelection
{
    class Program
    {
        static string connectionString = "Server=localhost;Port=3306;Uid=root;Pwd=19982012;Database=restaurante;";

        static void Main(string[] args)
        {
            while (true)
            {
                Console.WriteLine("");
                Console.WriteLine("Seleccione una opción:");
                Console.WriteLine("1. Crear usuario");
                Console.WriteLine("2. Modificar usuario");
                Console.WriteLine("3. Eliminar usuario");
                Console.WriteLine("4. Crear rol");
                Console.WriteLine("5. Asignar rol a usuario");
                Console.WriteLine("6. Consultar roles");
                Console.WriteLine("7. Consultar usuarios");
                Console.WriteLine("8. Realizar respaldo");
                Console.WriteLine("9. Restaurar base de datos");
                Console.WriteLine("10.Listar Entidades");
                Console.WriteLine("11. Listar atributos por entidad");
                Console.WriteLine("12. Agregar entidad con atributos");
                Console.WriteLine("13. Generar informe pdf");
                Console.WriteLine("14. Procedimientos almacenados");
                Console.WriteLine("15. Salir");
                Console.WriteLine("16. Crear tabla auditoria");
                Console.WriteLine("17. Generar triggers para auditoría");
                Console.WriteLine("18. Gestionar logs de eventos y auditoría");
                Console.WriteLine("19. Generar informe PDF de tabla o descargar triggers");
                Console.WriteLine("");
                Console.Write("Ingrese la opción seleccionada: ");

                int opcion;
                if (!int.TryParse(Console.ReadLine(), out opcion))
                {
                    Console.WriteLine("Opción no válida. Por favor, ingrese un número.");
                    continue;
                }

                switch (opcion)
                {
                    case 1:
                        CrearUsuario();
                        break;
                    case 2:
                        ModificarUsuario();
                        break;
                    case 3:
                        EliminarUsuario();
                        break;
                    case 4:
                        CrearRol();
                        break;
                    case 5:
                        AsignarRolUsuario();
                        break;
                    case 6:
                        ConsultarRoles();
                        break;
                    case 7:
                        ConsultarUsuarios();
                        break;
                    case 8:
                        RealizarRespaldo();
                        break;
                    case 9:
                        RestaurarBaseDeDatos();
                        break;
                    case 10:
                        ListarEntidadesBaseDatos();
                        break;
                    case 11:
                        ListarAtributosEntidad();
                        break;
                    case 12:
                        AgregarEntidadConAtributos();
                        break;
                    case 13:
                        GenerarInformePDF();
                        break;
                    case 14:
                        GenerarProcedimientosAlmacenados();
                        break;
                    case 15:
                        return;
                    case 16:
                        CrearTablaAuditoria();
                        break;
                    case 17:
                        GenerarTriggersAuditoria();
                        break;
                    case 18:
                        GestionarLogsEventosAuditoria();
                        break;
                    case 19:
                        GenerarInformeODescargarTriggers();
                        break;
                    default:
                        Console.WriteLine("Opción no válida.");
                        break;
                }
            }
        }
        static void GenerarInformeODescargarTriggers()
        {
            while (true)
            {
                Console.WriteLine("\nGenerar informe PDF de tabla o descargar triggers");
                Console.WriteLine("1. Generar informe PDF de una tabla");
                Console.WriteLine("2. Descargar triggers de todas las tablas");
                Console.WriteLine("3. Volver al menú principal");
                Console.Write("Seleccione una opción: ");

                if (int.TryParse(Console.ReadLine(), out int opcion))
                {
                    switch (opcion)
                    {
                        case 1:
                            GenerarInformePDFTabla();
                            break;
                        case 2:
                            DescargarTriggers();
                            break;
                        case 3:
                            return;
                        default:
                            Console.WriteLine("Opción no válida.");
                            break;
                    }
                }
                else
                {
                    Console.WriteLine("Por favor, ingrese un número válido.");
                }
            }
        }

        static void GenerarInformePDFTabla()
        {
            try
            {
                using (MySqlConnection connection = new MySqlConnection(connectionString))
                {
                    connection.Open();

                    // Obtener la lista de tablas
                    List<string> tablas = new List<string>();
                    string queryTablas = "SHOW TABLES";
                    using (MySqlCommand cmd = new MySqlCommand(queryTablas, connection))
                    using (MySqlDataReader reader = cmd.ExecuteReader())
                    {
                        while (reader.Read())
                        {
                            tablas.Add(reader.GetString(0));
                        }
                    }

                    // Mostrar las tablas disponibles
                    Console.WriteLine("Tablas disponibles:");
                    for (int i = 0; i < tablas.Count; i++)
                    {
                        Console.WriteLine($"{i + 1}. {tablas[i]}");
                    }

                    Console.Write("Seleccione el número de la tabla para generar el informe: ");
                    if (int.TryParse(Console.ReadLine(), out int seleccion) && seleccion > 0 && seleccion <= tablas.Count)
                    {
                        string tablaSeleccionada = tablas[seleccion - 1];

                        // Obtener los datos de la tabla seleccionada
                        string query = $"SELECT * FROM {tablaSeleccionada}";
                        List<Dictionary<string, string>> datos = new List<Dictionary<string, string>>();
                        List<string> columnas = new List<string>();

                        using (MySqlCommand cmd = new MySqlCommand(query, connection))
                        using (MySqlDataReader reader = cmd.ExecuteReader())
                        {
                            // Obtener los nombres de las columnas
                            for (int i = 0; i < reader.FieldCount; i++)
                            {
                                columnas.Add(reader.GetName(i));
                            }

                            // Obtener los datos
                            while (reader.Read())
                            {
                                var fila = new Dictionary<string, string>();
                                foreach (var columna in columnas)
                                {
                                    fila[columna] = reader[columna].ToString();
                                }
                                datos.Add(fila);
                            }
                        }

                        // Generar el informe PDF
                        string rutaPDF = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.Desktop), $"Informe_{tablaSeleccionada}.pdf");

                        using (PdfWriter writer = new PdfWriter(rutaPDF))
                        using (PdfDocument pdf = new PdfDocument(writer))
                        using (Document document = new Document(pdf, iText.Kernel.Geom.PageSize.A4.Rotate())) // Usar orientación horizontal
                        {
                            document.SetMargins(20, 20, 20, 20);

                            document.Add(new Paragraph($"Informe de la tabla: {tablaSeleccionada}"));
                            document.Add(new Paragraph($"Fecha de generación: {DateTime.Now}"));
                            document.Add(new Paragraph("\n"));

                            Table table = new Table(UnitValue.CreatePercentArray(columnas.Count)).UseAllAvailableWidth();

                            // Estilo para las celdas del encabezado
                            Cell headerCell = new Cell().SetBackgroundColor(ColorConstants.LIGHT_GRAY).SetTextAlignment(TextAlignment.CENTER);

                            foreach (var columna in columnas)
                            {
                                table.AddHeaderCell(headerCell.Clone(false).Add(new Paragraph(columna)));
                            }

                            // Estilo para las celdas de datos
                            Cell dataCell = new Cell().SetTextAlignment(TextAlignment.CENTER);

                            foreach (var fila in datos)
                            {
                                foreach (var columna in columnas)
                                {
                                    table.AddCell(dataCell.Clone(false).Add(new Paragraph(fila[columna])));
                                }
                            }

                            document.Add(table);
                        }

                        Console.WriteLine($"Informe PDF generado: {rutaPDF}");
                    }
                    else
                    {
                        Console.WriteLine("Selección no válida.");
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error al generar el informe PDF: {ex.Message}");
            }
        }

        static void DescargarTriggers()
        {
            try
            {
                using (MySqlConnection connection = new MySqlConnection(connectionString))
                {
                    connection.Open();

                    // Obtener la lista de tablas
                    List<string> tablas = new List<string>();
                    string queryTablas = "SHOW TABLES";
                    using (MySqlCommand cmd = new MySqlCommand(queryTablas, connection))
                    using (MySqlDataReader reader = cmd.ExecuteReader())
                    {
                        while (reader.Read())
                        {
                            tablas.Add(reader.GetString(0));
                        }
                    }

                    StringBuilder allTriggers = new StringBuilder();

                    foreach (string tabla in tablas)
                    {
                        string queryTriggers = $"SHOW TRIGGERS LIKE '{tabla}'";
                        using (MySqlCommand cmd = new MySqlCommand(queryTriggers, connection))
                        using (MySqlDataReader reader = cmd.ExecuteReader())
                        {
                            while (reader.Read())
                            {
                                string triggerName = reader["Trigger"].ToString();
                                string triggerEvent = reader["Event"].ToString();
                                string triggerStatement = reader["Statement"].ToString();

                                allTriggers.AppendLine($"DELIMITER $$");
                                allTriggers.AppendLine($"CREATE TRIGGER `{triggerName}` {triggerEvent} ON `{tabla}`");
                                allTriggers.AppendLine($"FOR EACH ROW");
                                allTriggers.AppendLine($"BEGIN");
                                allTriggers.AppendLine(triggerStatement);
                                allTriggers.AppendLine($"END$$");
                                allTriggers.AppendLine($"DELIMITER ;");
                                allTriggers.AppendLine();
                            }
                        }
                    }

                    string rutaArchivo = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.UserProfile), "Downloads", "triggers_all_tables.sql");
                    File.WriteAllText(rutaArchivo, allTriggers.ToString());

                    Console.WriteLine($"Triggers descargados en: {rutaArchivo}");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error al descargar los triggers: {ex.Message}");
            }
        }


















        static List<string> filtros = new List<string>();

        static void GestionarLogsEventosAuditoria()
        {
            while (true)
            {
                Console.WriteLine("\nGestión de Logs de Eventos y Auditoría");
                Console.WriteLine("1. Acceder a logs de eventos y auditoría");
                Console.WriteLine("2. Agregar filtros");
                Console.WriteLine("3. Generar informe PDF con información filtrada");
                Console.WriteLine("4. Volver al menú principal");
                Console.Write("Seleccione una opción: ");

                if (int.TryParse(Console.ReadLine(), out int opcion))
                {
                    switch (opcion)
                    {
                        case 1:
                            AccederLogs();
                            break;
                        case 2:
                            AgregarFiltros();
                            break;
                        case 3:
                            GenerarInformePDFFiltrado();
                            break;
                        case 4:
                            return;
                        default:
                            Console.WriteLine("Opción no válida.");
                            break;
                    }
                }
                else
                {
                    Console.WriteLine("Por favor, ingrese un número válido.");
                }
            }
        }

        static void AccederLogs()
        {
            try
            {
                using (MySqlConnection connection = new MySqlConnection(connectionString))
                {
                    connection.Open();
                    string query = "SELECT * FROM auditoria";

                    if (filtros.Count > 0)
                    {
                        query += " WHERE " + string.Join(" AND ", filtros);
                    }

                    using (MySqlCommand command = new MySqlCommand(query, connection))
                    using (MySqlDataReader reader = command.ExecuteReader())
                    {
                        bool hasRows = false;
                        while (reader.Read())
                        {
                            hasRows = true;
                            Console.WriteLine($"ID: {reader["id"]}, Tabla: {reader["nombre_tabla"]}, Operación: {reader["operacion"]}, Usuario: {reader["usuario_actual"]}, Fecha: {reader["fecha_hora"]}, Detalle Anterior: {reader["detalle_anterior"]}, Detalle Nuevo: {reader["detalle_nuevo"]}");
                        }

                        if (!hasRows)
                        {
                            Console.WriteLine("No se encontraron registros con los filtros aplicados.");
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error al acceder a los logs: {ex.Message}");
            }
        }

        static void AgregarFiltros()
        {
            Console.WriteLine("Ingrese el filtro en formato SQL (ejemplo: operacion = 'INSERT'):");
            string filtro = Console.ReadLine();

            if (ValidarFiltro(filtro))
            {
                filtros.Add(filtro);
                Console.WriteLine("Filtro agregado correctamente.");
            }
            else
            {
                Console.WriteLine("Filtro no válido. Por favor, ingrese un filtro en formato SQL correcto.");
            }
        }

        static bool ValidarFiltro(string filtro)
        {
            string[] validColumns = { "id", "nombre_tabla", "operacion", "usuario_actual", "fecha_hora", "detalle_anterior", "detalle_nuevo" };
            string[] filtroParts = filtro.Split(new[] { '=' }, StringSplitOptions.RemoveEmptyEntries);

            if (filtroParts.Length == 2 && validColumns.Contains(filtroParts[0].Trim()))
            {
                return true;
            }

            return false;
        }

        static void GenerarInformePDFFiltrado()
        {
            try
            {
                using (MySqlConnection connection = new MySqlConnection(connectionString))
                {
                    connection.Open();
                    string query = "SELECT * FROM auditoria";
                    if (filtros.Count > 0)
                    {
                        query += " WHERE " + string.Join(" AND ", filtros);
                    }
                    List<Dictionary<string, string>> datos = new List<Dictionary<string, string>>();
                    using (MySqlCommand command = new MySqlCommand(query, connection))
                    using (MySqlDataReader reader = command.ExecuteReader())
                    {
                        while (reader.Read())
                        {
                            datos.Add(new Dictionary<string, string>
                    {
                        {"ID", reader["id"].ToString()},
                        {"Tabla", reader["nombre_tabla"].ToString()},
                        {"Operación", reader["operacion"].ToString()},
                        {"Usuario", reader["usuario_actual"].ToString()},
                        {"Fecha", reader["fecha_hora"].ToString()},
                        {"Detalle Anterior", reader["detalle_anterior"].ToString()},
                        {"Detalle Nuevo", reader["detalle_nuevo"].ToString()}
                    });
                        }
                    }
                    string rutaPDF = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.Desktop), "InformeLogsFiltrados.pdf");
                    using (PdfWriter writer = new PdfWriter(rutaPDF))
                    using (PdfDocument pdf = new PdfDocument(writer))
                    using (Document document = new Document(pdf, iText.Kernel.Geom.PageSize.A4.Rotate()))
                    {
                        document.SetMargins(20, 20, 20, 20);

                        document.Add(new Paragraph("Informe de Logs de Eventos y Auditoría (Filtrado)").SetFontSize(14).SetBold());
                        document.Add(new Paragraph($"Fecha de generación: {DateTime.Now}").SetFontSize(10));
                        document.Add(new Paragraph($"Filtros aplicados: {string.Join(", ", filtros)}").SetFontSize(10));
                        document.Add(new Paragraph("\n"));

                        Table table = new Table(UnitValue.CreatePercentArray(new float[] { 5, 10, 10, 10, 15, 25, 25 })).UseAllAvailableWidth();

                        Cell headerCell = new Cell().SetBackgroundColor(ColorConstants.LIGHT_GRAY).SetTextAlignment(TextAlignment.CENTER);
                        string[] headers = { "ID", "Tabla", "Operación", "Usuario", "Fecha", "Detalle Anterior", "Detalle Nuevo" };
                        foreach (var header in headers)
                        {
                            table.AddHeaderCell(headerCell.Clone(false).Add(new Paragraph(header).SetFontSize(9).SetBold()));
                        }

                        foreach (var dato in datos)
                        {
                            foreach (var key in dato.Keys)
                            {
                                table.AddCell(new Cell().SetTextAlignment(TextAlignment.CENTER).Add(new Paragraph(dato[key]).SetFontSize(8)));
                            }
                        }

                        document.Add(table);
                    }
                    Console.WriteLine($"Informe PDF generado: {rutaPDF}");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error al generar el informe PDF: {ex.Message}");
            }
        }
































        static void GenerarTriggersAuditoria()
        {
            try
            {
                using (MySqlConnection connection = new MySqlConnection(connectionString))
                {
                    connection.Open();

                    // Obtener todas las tablas excepto la tabla de auditoría
                    string tableQuery = @"
                SELECT TABLE_NAME 
                FROM INFORMATION_SCHEMA.TABLES 
                WHERE TABLE_SCHEMA = DATABASE() 
                AND TABLE_NAME != 'auditoria'";

                    List<string> tables = new List<string>();

                    using (MySqlCommand command = new MySqlCommand(tableQuery, connection))
                    using (MySqlDataReader reader = command.ExecuteReader())
                    {
                        while (reader.Read())
                        {
                            tables.Add(reader.GetString(0));
                        }
                    }

                    foreach (string tableName in tables)
                    {
                        GenerarTriggerParaTabla(connection, tableName);
                    }

                    Console.WriteLine("Triggers de auditoría generados correctamente para todas las tablas.");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error al generar triggers de auditoría: {ex.Message}");
            }
        }

        static void GenerarTriggerParaTabla(MySqlConnection connection, string tableName)
        {
            string triggerInsert = $@"
CREATE TRIGGER `{tableName}_AFTER_INSERT` AFTER INSERT ON `{tableName}`
 FOR EACH ROW
BEGIN
    INSERT INTO `auditoria` (`nombre_tabla`, `operacion`, `usuario_actual`, `detalle_nuevo`)
    VALUES ('{tableName}', 'INSERT', USER(), JSON_OBJECT(
        {string.Join(", ", GetColumnList(connection, tableName).Select(col => $"'{col}', NEW.`{col}`"))}
    ));
END;";

            string triggerUpdate = $@"
CREATE TRIGGER `{tableName}_AFTER_UPDATE` AFTER UPDATE ON `{tableName}`
 FOR EACH ROW
BEGIN
    INSERT INTO `auditoria` (`nombre_tabla`, `operacion`, `usuario_actual`, `detalle_anterior`, `detalle_nuevo`)
    VALUES ('{tableName}', 'UPDATE', USER(), 
    JSON_OBJECT(
        {string.Join(", ", GetColumnList(connection, tableName).Select(col => $"'{col}', OLD.`{col}`"))}
    ),
    JSON_OBJECT(
        {string.Join(", ", GetColumnList(connection, tableName).Select(col => $"'{col}', NEW.`{col}`"))}
    ));
END;";

            string triggerDelete = $@"
CREATE TRIGGER `{tableName}_AFTER_DELETE` AFTER DELETE ON `{tableName}`
 FOR EACH ROW
BEGIN
    INSERT INTO `auditoria` (`nombre_tabla`, `operacion`, `usuario_actual`, `detalle_anterior`)
    VALUES ('{tableName}', 'DELETE', USER(), JSON_OBJECT(
        {string.Join(", ", GetColumnList(connection, tableName).Select(col => $"'{col}', OLD.`{col}`"))}
    ));
END;";

            using (MySqlCommand command = new MySqlCommand(triggerInsert, connection))
            {
                command.ExecuteNonQuery();
            }

            using (MySqlCommand command = new MySqlCommand(triggerUpdate, connection))
            {
                command.ExecuteNonQuery();
            }

            using (MySqlCommand command = new MySqlCommand(triggerDelete, connection))
            {
                command.ExecuteNonQuery();
            }

            Console.WriteLine($"Triggers generados para la tabla {tableName}");
        }



























        static void CrearTablaAuditoria()
        {
            Console.Write("Ingrese el nombre de la tabla: ");
            string nombreTabla = Console.ReadLine();

            using (MySqlConnection connection = new MySqlConnection(connectionString))
            {
                connection.Open();
                string query = $@"
                    CREATE TABLE IF NOT EXISTS {nombreTabla} (
                        id SERIAL PRIMARY KEY,
                        nombre_tabla VARCHAR(255),
                        operacion VARCHAR(10),
                        usuario_actual VARCHAR(255),
                        fecha_hora TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                        detalle_anterior TEXT,
                        detalle_nuevo TEXT
                    );";

                using (MySqlCommand command = new MySqlCommand(query, connection))
                {
                    try
                    {
                        command.ExecuteNonQuery();
                        Console.WriteLine($"Tabla '{nombreTabla}' creada correctamente.");
                    }
                    catch (MySqlException ex)
                    {
                        Console.WriteLine($"Error al crear la tabla '{nombreTabla}': {ex.Message}");
                    }
                }
            }
        }





























        static void CrearUsuario()
        {
            Console.Write("Ingrese el nombre del nuevo usuario: ");
            string nombreUsuario = Console.ReadLine();

            Console.Write("Ingrese la contraseña del nuevo usuario: ");
            string contraseñaUsuario = Console.ReadLine();

            Console.Write("Ingrese el nombre de la base de datos para crear el usuario: ");
            string nombreBaseDeDatos = SeleccionarBaseDeDatos();

            if (nombreBaseDeDatos == null)
                return;

            using (MySqlConnection connection = new MySqlConnection(connectionString))
            {
                connection.Open();

                // Verificar si el usuario ya existe
                string verificarUsuarioQuery = "SELECT COUNT(*) FROM mysql.user WHERE user = @nombreUsuario";
                using (MySqlCommand verificarUsuarioCmd = new MySqlCommand(verificarUsuarioQuery, connection))
                {
                    verificarUsuarioCmd.Parameters.AddWithValue("@nombreUsuario", nombreUsuario);
                    long count = (long)verificarUsuarioCmd.ExecuteScalar();
                    if (count > 0)
                    {
                        Console.WriteLine($"El usuario '{nombreUsuario}' ya existe.");
                        return;
                    }
                }

                // Crear el usuario
                string createUsuarioQuery = $"CREATE USER '{nombreUsuario}'@'localhost' IDENTIFIED BY '{contraseñaUsuario}'";
                using (MySqlCommand createUsuarioCommand = new MySqlCommand(createUsuarioQuery, connection))
                {
                    try
                    {
                        createUsuarioCommand.ExecuteNonQuery();
                        Console.WriteLine($"Usuario '{nombreUsuario}' creado correctamente en la base de datos '{nombreBaseDeDatos}'.");
                        Console.WriteLine(" ");
                    }
                    catch (MySqlException ex)
                    {
                        Console.WriteLine($"Error al crear el usuario: {ex.Message}");
                        return;
                    }
                }
            }
        }

        static string SeleccionarBaseDeDatos()
        {
            Console.WriteLine("\nBases de Datos Disponibles:");

            string selectedDatabase = null;
            using (MySqlConnection connection = new MySqlConnection(connectionString))
            {
                connection.Open();

                // Consultar las bases de datos disponibles
                string query = "SELECT schema_name FROM information_schema.schemata";
                using (MySqlCommand command = new MySqlCommand(query, connection))
                {
                    using (MySqlDataReader reader = command.ExecuteReader())
                    {
                        int index = 1;
                        while (reader.Read())
                        {
                            string databaseName = reader.GetString("schema_name");
                            Console.WriteLine($"{index}. {databaseName}");
                            index++;
                        }
                    }
                }

                Console.WriteLine("\nIngrese el número de la base de datos a seleccionar (0 para cancelar):");
                string input = Console.ReadLine();

                if (input == "0")
                {
                    Console.WriteLine("Operación cancelada.");
                    return null;
                }

                int databaseIndex;
                if (!int.TryParse(input, out databaseIndex) || databaseIndex < 1)
                {
                    Console.WriteLine("Número de base de datos no válido.");
                    return null;
                }

                query = "SELECT schema_name FROM information_schema.schemata LIMIT @databaseIndex, 1";
                using (MySqlCommand command = new MySqlCommand(query, connection))
                {
                    command.Parameters.AddWithValue("@databaseIndex", databaseIndex - 1);
                    selectedDatabase = (string)command.ExecuteScalar();
                }
            }

            Console.WriteLine($"Ha seleccionado la base de datos '{selectedDatabase}'.");
            return selectedDatabase;
        }


        static void ModificarUsuario()
        {
            using (MySqlConnection connection = new MySqlConnection(connectionString))
            {
                connection.Open();

                // Obtener la lista de usuarios disponibles en la base de datos
                Dictionary<int, string> usuarios = new Dictionary<int, string>();
                string obtenerUsuariosQuery = "SELECT User FROM mysql.user";
                using (MySqlCommand obtenerUsuariosCmd = new MySqlCommand(obtenerUsuariosQuery, connection))
                {
                    using (MySqlDataReader reader = obtenerUsuariosCmd.ExecuteReader())
                    {
                        int index = 1;
                        while (reader.Read())
                        {
                            string nombreUser = reader.GetString(0); // Obtener el nombre de usuario
                            usuarios.Add(index, nombreUser);
                            Console.WriteLine($"{index}. {nombreUser}");
                            index++;
                        }
                    }
                }

                // Solicitar al usuario que seleccione el número del usuario a modificar
                Console.Write("\nIngrese el número del usuario a modificar: ");
                if (!int.TryParse(Console.ReadLine(), out int seleccion))
                {
                    Console.WriteLine("Entrada no válida. Debe ingresar un número.");
                    return;
                }

                // Verificar si el número de usuario seleccionado existe
                if (!usuarios.ContainsKey(seleccion))
                {
                    Console.WriteLine("Número de usuario no válido.");
                    return;
                }

                string nombreUsuario = usuarios[seleccion];

                // Solicitar al usuario que ingrese el nuevo nombre para el usuario seleccionado
                Console.Write("Ingrese el nuevo nombre para el usuario: ");
                string nuevoNombreUsuario = Console.ReadLine();

                // Modificar el nombre del usuario
                string query = $"RENAME USER '{nombreUsuario}'@'localhost' TO '{nuevoNombreUsuario}'@'localhost'";

                using (MySqlCommand command = new MySqlCommand(query, connection))
                {
                    try
                    {
                        command.ExecuteNonQuery();
                        Console.WriteLine($"Usuario '{nombreUsuario}' modificado correctamente. Nuevo nombre: '{nuevoNombreUsuario}'");
                    }
                    catch (MySqlException ex)
                    {
                        Console.WriteLine($"Error al modificar el usuario: {ex.Message}");
                    }
                }
            }
        }

        static void EliminarUsuario()
        {
            using (MySqlConnection connection = new MySqlConnection(connectionString))
            {
                connection.Open();

                // Obtener la lista de usuarios disponibles en la base de datos
                Dictionary<int, string> usuarios = new Dictionary<int, string>();
                string obtenerUsuariosQuery = "SELECT User FROM mysql.user";
                using (MySqlCommand obtenerUsuariosCmd = new MySqlCommand(obtenerUsuariosQuery, connection))
                {
                    using (MySqlDataReader reader = obtenerUsuariosCmd.ExecuteReader())
                    {
                        int index = 1;
                        while (reader.Read())
                        {
                            string nombreUser = reader.GetString(0); // Obtener el nombre de usuario
                            usuarios.Add(index, nombreUser);
                            Console.WriteLine($"{index}. {nombreUser}");
                            index++;
                        }
                    }
                }

                // Solicitar al usuario que seleccione el número del usuario a eliminar
                Console.Write("\nIngrese el número del usuario a eliminar: ");
                if (!int.TryParse(Console.ReadLine(), out int seleccion))
                {
                    Console.WriteLine("Entrada no válida. Debe ingresar un número.");
                    return;
                }

                // Verificar si el número de usuario seleccionado existe
                if (!usuarios.ContainsKey(seleccion))
                {
                    Console.WriteLine("Número de usuario no válido.");
                    return;
                }

                string nombreUsuario = usuarios[seleccion];

                // Eliminar el usuario
                string eliminarUsuarioQuery = $"DROP USER '{nombreUsuario}'@'localhost'";
                using (MySqlCommand eliminarUsuarioCmd = new MySqlCommand(eliminarUsuarioQuery, connection))
                {
                    try
                    {
                        eliminarUsuarioCmd.ExecuteNonQuery();
                        Console.WriteLine($"Usuario '{nombreUsuario}' eliminado correctamente.");
                    }
                    catch (MySqlException ex)
                    {
                        Console.WriteLine($"Error al eliminar el usuario: {ex.Message}");
                    }
                }
            }
        }
        static void CrearRol()
        {
            Console.Write("Ingrese el nombre del nuevo rol: ");
            string nombreRol = Console.ReadLine();

            using (MySqlConnection connection = new MySqlConnection(connectionString))
            {
                connection.Open();

                // Verificar si el rol ya existe
                string verificarRolQuery = $"SELECT COUNT(*) FROM mysql.user WHERE user = @nombreRol";
                using (MySqlCommand verificarRolCmd = new MySqlCommand(verificarRolQuery, connection))
                {
                    verificarRolCmd.Parameters.AddWithValue("@nombreRol", nombreRol);
                    object resultado = verificarRolCmd.ExecuteScalar();
                    int count = Convert.ToInt32(resultado);
                    if (count > 0)
                    {
                        Console.WriteLine($"El rol '{nombreRol}' ya existe.");
                        return;
                    }
                }

                string query = $"CREATE ROLE '{nombreRol}'";
                MySqlCommand command = new MySqlCommand(query, connection);

                try
                {
                    command.ExecuteNonQuery();
                    Console.WriteLine($"Rol '{nombreRol}' creado correctamente.");
                    Console.WriteLine(" ");
                }
                catch (MySqlException ex)
                {
                    Console.WriteLine($"Error al crear el rol: {ex.Message}");
                }
            }
        }
        static void AsignarRolUsuario()
        {
            using (MySqlConnection connection = new MySqlConnection(connectionString))
            {
                connection.Open();

                // Obtener la lista de usuarios disponibles en la base de datos
                Dictionary<int, string> usuarios = new Dictionary<int, string>();
                string obtenerUsuariosQuery = "SELECT User FROM mysql.user";
                using (MySqlCommand obtenerUsuariosCmd = new MySqlCommand(obtenerUsuariosQuery, connection))
                {
                    using (MySqlDataReader reader = obtenerUsuariosCmd.ExecuteReader())
                    {
                        int index = 1;
                        Console.WriteLine("Usuarios disponibles:");
                        while (reader.Read())
                        {
                            string nombreUser = reader.GetString(0);
                            usuarios.Add(index, nombreUser);
                            Console.WriteLine($"{index}. {nombreUser}");
                            index++;
                        }
                    }
                }

                // Solicitar al usuario que seleccione el número del usuario
                Console.Write("\nIngrese el número del usuario: ");
                if (!int.TryParse(Console.ReadLine(), out int seleccionUsuario))
                {
                    Console.WriteLine("Entrada no válida. Debe ingresar un número.");
                    return;
                }

                // Verificar si el número de usuario seleccionado existe
                if (!usuarios.ContainsKey(seleccionUsuario))
                {
                    Console.WriteLine("Número de usuario no válido.");
                    return;
                }

                string nombreUsuario = usuarios[seleccionUsuario];

                // Obtener la lista de roles disponibles en la base de datos
                Dictionary<int, string> roles = new Dictionary<int, string>();
                string obtenerRolesQuery = "SELECT DISTINCT role FROM mysql.roles_mapping";
                using (MySqlCommand obtenerRolesCmd = new MySqlCommand(obtenerRolesQuery, connection))
                {
                    using (MySqlDataReader reader = obtenerRolesCmd.ExecuteReader())
                    {
                        int index = 1;
                        Console.WriteLine("\nRoles disponibles:");
                        while (reader.Read())
                        {
                            string nameRol = reader.GetString(0);
                            roles.Add(index, nameRol);
                            Console.WriteLine($"{index}. {nameRol}");
                            index++;
                        }
                    }
                }

                // Solicitar al usuario que seleccione el número del rol
                Console.Write("\nIngrese el número del rol: ");
                if (!int.TryParse(Console.ReadLine(), out int seleccionRol))
                {
                    Console.WriteLine("Entrada no válida. Debe ingresar un número.");
                    return;
                }

                // Verificar si el número de rol seleccionado existe
                if (!roles.ContainsKey(seleccionRol))
                {
                    Console.WriteLine("Número de rol no válido.");
                    return;
                }

                string nombreRol = roles[seleccionRol];

                // Asignar el rol al usuario
                string query = $"GRANT {nombreRol} TO '{nombreUsuario}'@'localhost'";
                using (MySqlCommand command = new MySqlCommand(query, connection))
                {
                    try
                    {
                        command.ExecuteNonQuery();
                        Console.WriteLine($"Rol '{nombreRol}' asignado al usuario '{nombreUsuario}' correctamente.");
                    }
                    catch (MySqlException ex)
                    {
                        Console.WriteLine($"Error al asignar el rol al usuario: {ex.Message}");
                    }
                }
            }
        }
        static void ConsultarRoles()
        {
            Console.WriteLine("Roles en la base de datos:");
            using (MySqlConnection connection = new MySqlConnection(connectionString))
            {
                connection.Open();
                string query = "SELECT DISTINCT Role FROM mysql.roles_mapping";
                using (MySqlCommand command = new MySqlCommand(query, connection))
                {
                    using (MySqlDataReader reader = command.ExecuteReader())
                    {
                        Console.WriteLine("");
                        while (reader.Read())
                        {
                            Console.WriteLine(reader.GetString(0));
                        }
                    }
                }
            }
        }

        static void ConsultarUsuarios()
        {
            Console.WriteLine("Usuarios en la base de datos:");
            using (MySqlConnection connection = new MySqlConnection(connectionString))
            {
                connection.Open();

                //string query = "SELECT DISTINCT USER FROM mysql.user WHERE USER != 'root'";
                string query = "SELECT DISTINCT USER FROM mysql.user WHERE USER != 'root'";
                using (MySqlCommand command = new MySqlCommand(query, connection))
                {
                    using (MySqlDataReader reader = command.ExecuteReader())
                    {
                        Console.WriteLine("");
                        while (reader.Read())
                        {
                            Console.WriteLine(reader.GetString(0));
                        }
                    }
                }
            }
        }

        static void RealizarRespaldo()
        {
            try
            {
                Console.Write("Ingrese el nombre de la base de datos a respaldar: ");
                string databaseName = Console.ReadLine();

                Console.Write("Ingrese el nombre del archivo de respaldo (.sql): ");
                string fileName = Console.ReadLine();

                // Ruta de la carpeta de respaldo (escritorio)
                string desktopFolder = Environment.GetFolderPath(Environment.SpecialFolder.Desktop);
                string backupFilePath = Path.Combine(desktopFolder, fileName);

                if (File.Exists(backupFilePath))
                {
                    Console.WriteLine("El archivo de respaldo ya existe en la ubicación especificada.");
                    return;
                }

                // Comando mysqldump para realizar el respaldo
                string mysqlDumpPath = @"C:\Program Files\MariaDB 11.3\bin\mysqldump.exe"; // Ruta al ejecutable mysqldump
                string mysqlDumpCommand = $"--user=root --password=Argentina619 {databaseName}";

                // Ejecutar el comando mysqldump utilizando ProcessStartInfo
                using (Process mysqlDumpProcess = new Process())
                {
                    mysqlDumpProcess.StartInfo.FileName = "cmd.exe";
                    mysqlDumpProcess.StartInfo.Arguments = $"/c \"\"{mysqlDumpPath}\" {mysqlDumpCommand} > \"{backupFilePath}\"\"";
                    mysqlDumpProcess.StartInfo.UseShellExecute = false;
                    mysqlDumpProcess.StartInfo.RedirectStandardOutput = true;
                    mysqlDumpProcess.StartInfo.RedirectStandardError = true;

                    mysqlDumpProcess.Start();

                    // Capturar la salida estándar y la salida de error
                    string output = mysqlDumpProcess.StandardOutput.ReadToEnd();
                    string error = mysqlDumpProcess.StandardError.ReadToEnd();

                    mysqlDumpProcess.WaitForExit();

                    if (mysqlDumpProcess.ExitCode == 0)
                    {
                        Console.WriteLine($"Respaldo de la base de datos '{databaseName}' creado correctamente en '{backupFilePath}'.");
                    }
                    else
                    {
                        Console.WriteLine($"Error al realizar el respaldo: {error}");
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error al realizar el respaldo: {ex.Message}");
            }
        }
        static void RestaurarBaseDeDatos()
        {
            try
            {
                // Ruta donde se encuentran los archivos de respaldo
                string backupPath = Environment.GetFolderPath(Environment.SpecialFolder.Desktop);

                // Obtener todos los archivos de respaldo disponibles
                string[] backupFiles = Directory.GetFiles(backupPath, "*.sql");

                if (backupFiles.Length > 0)
                {
                    Console.WriteLine("Seleccione el archivo de respaldo a restaurar:");
                    for (int i = 0; i < backupFiles.Length; i++)
                    {
                        Console.WriteLine($"{i + 1}. {Path.GetFileName(backupFiles[i])}");
                    }

                    Console.Write("Ingrese el número correspondiente: ");
                    if (int.TryParse(Console.ReadLine(), out int selectedIndex) && selectedIndex >= 1 && selectedIndex <= backupFiles.Length)
                    {
                        string backupFilePath = backupFiles[selectedIndex - 1];
                        string backupFileName = Path.GetFileName(backupFilePath);

                        // Nombre de la base de datos a restaurar
                        string dbName = "restaurante";

                        // Comando para restaurar la base de datos utilizando mysql
                        string mysqlPath = @"C:\Program Files\MariaDB 11.3\bin\mysql.exe";
                        string mysqlCommand = $"--user=root --password=Argentina619 {dbName} < \"{backupFilePath}\"";

                        // Ejecutar el comando mysql utilizando ProcessStartInfo
                        using (Process mysqlProcess = new Process())
                        {
                            mysqlProcess.StartInfo.FileName = "cmd.exe";
                            mysqlProcess.StartInfo.Arguments = $"/c \"\"{mysqlPath}\" --user=root --password=Argentina619 {dbName} < \"{backupFilePath}\"\"";
                            mysqlProcess.StartInfo.UseShellExecute = false;
                            mysqlProcess.StartInfo.RedirectStandardOutput = true;
                            mysqlProcess.StartInfo.RedirectStandardError = true;

                            mysqlProcess.Start();

                            // Capturar la salida estándar y la salida de error
                            string output = mysqlProcess.StandardOutput.ReadToEnd();
                            string error = mysqlProcess.StandardError.ReadToEnd();

                            mysqlProcess.WaitForExit();

                            if (mysqlProcess.ExitCode == 0)
                            {
                                Console.WriteLine($"Se restauró la base de datos '{dbName}' desde el archivo '{backupFileName}'.");
                            }
                            else
                            {
                                Console.WriteLine($"Error al restaurar la base de datos: {error}");
                            }
                        }
                    }
                    else
                    {
                        Console.WriteLine("Selección inválida.");
                    }
                }
                else
                {
                    Console.WriteLine("No hay archivos de respaldo disponibles.");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error al restaurar la base de datos: {ex.Message}");
            }
        }

        static void ListarEntidadesBaseDatos()
        {

            try
            {
                using (MySqlConnection connection = new MySqlConnection(connectionString))
                {
                    connection.Open();

                    // Consulta SQL para obtener la lista de tablas en la base de datos actual usando INFORMATION_SCHEMA
                    string query = "SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA = DATABASE()";

                    using (MySqlCommand command = new MySqlCommand(query, connection))
                    {
                        using (MySqlDataReader reader = command.ExecuteReader())
                        {
                            Console.WriteLine("Lista de entidades en la base de datos:");

                            // Iterar sobre los resultados y mostrar el nombre de cada entidad
                            while (reader.Read())
                            {
                                string entityName = reader.GetString(0);
                                Console.WriteLine("- " + entityName);
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error al listar las entidades: " + ex.Message);
            }
        }

        static void ListarAtributosEntidad()
        {

            try
            {
                using (MySqlConnection connection = new MySqlConnection(connectionString))
                {
                    connection.Open();

                    // Consulta SQL para obtener la lista de tablas en la base de datos actual usando INFORMATION_SCHEMA
                    string query = "SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA = DATABASE()";

                    Dictionary<int, string> entityDictionary = new Dictionary<int, string>();

                    using (MySqlCommand command = new MySqlCommand(query, connection))
                    {
                        using (MySqlDataReader reader = command.ExecuteReader())
                        {
                            Console.WriteLine("Lista de entidades en la base de datos:");

                            // Iterar sobre los resultados y mostrar el nombre de cada entidad con un ID asignado
                            int entityId = 1;
                            while (reader.Read())
                            {
                                string entityName = reader.GetString(0);
                                Console.WriteLine($"{entityId}. {entityName}");
                                entityDictionary.Add(entityId, entityName);
                                entityId++;
                            }
                        }
                    }

                    Console.Write("Seleccione el número de la entidad para ver sus atributos (o 0 para salir): ");
                    if (int.TryParse(Console.ReadLine(), out int selectedEntityId) && entityDictionary.ContainsKey(selectedEntityId))
                    {
                        string selectedEntity = entityDictionary[selectedEntityId];
                        MostrarAtributosEntidad(connection, selectedEntity);
                    }
                    else
                    {
                        Console.WriteLine("Selección no válida o no seleccionada. Saliendo del programa.");
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error al listar los atributos de las entidades: " + ex.Message);
            }
        }

        static void MostrarAtributosEntidad(MySqlConnection connection, string entityName)
        {
            try
            {
                // Consulta SQL para obtener la lista de columnas de la tabla seleccionada
                string query = $"SELECT COLUMN_NAME, DATA_TYPE FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = DATABASE() AND TABLE_NAME = '{entityName}'";

                using (MySqlCommand command = new MySqlCommand(query, connection))
                {
                    using (MySqlDataReader reader = command.ExecuteReader())
                    {
                        Console.WriteLine($"Atributos de la entidad '{entityName}':");

                        // Iterar sobre los resultados y mostrar el nombre de cada columna y su tipo de datos
                        while (reader.Read())
                        {
                            string columnName = reader.GetString(0);
                            string dataType = reader.GetString(1);
                            Console.WriteLine($"- {columnName} ({dataType})");
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error al mostrar los atributos de la entidad: " + ex.Message);
            }
        }
        static void AgregarEntidadConAtributos()
        {

            try
            {
                Console.Write("Ingrese el nombre de la entidad: ");
                string nombreEntidad = Console.ReadLine();

                Console.WriteLine("Ingrese los atributos de la entidad (separados por coma): ");
                string atributosInput = Console.ReadLine();
                string[] atributos = atributosInput.Split(',');

                using (MySqlConnection connection = new MySqlConnection(connectionString))
                {
                    connection.Open();

                    // Verificar si la entidad ya existe en la base de datos
                    string verificarEntidadQuery = "SELECT COUNT(*) FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA = DATABASE() AND TABLE_NAME = @nombreEntidad";
                    using (MySqlCommand verificarEntidadCmd = new MySqlCommand(verificarEntidadQuery, connection))
                    {
                        verificarEntidadCmd.Parameters.AddWithValue("@nombreEntidad", nombreEntidad);
                        int count = Convert.ToInt32(verificarEntidadCmd.ExecuteScalar());
                        if (count > 0)
                        {
                            Console.WriteLine($"La entidad '{nombreEntidad}' ya existe en la base de datos.");
                            return;
                        }
                    }

                    // Consulta SQL para agregar una nueva tabla
                    string createTableQuery = $"CREATE TABLE {nombreEntidad} (ID INT AUTO_INCREMENT PRIMARY KEY";
                    foreach (var atributo in atributos)
                    {
                        createTableQuery += $", {atributo.Trim()} VARCHAR(50)";
                    }
                    createTableQuery += ")";

                    using (MySqlCommand command = new MySqlCommand(createTableQuery, connection))
                    {
                        command.ExecuteNonQuery();
                        Console.WriteLine($"Se ha creado la nueva entidad '{nombreEntidad}' correctamente.");
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error al agregar la nueva entidad con atributos: " + ex.Message);
            }
        }
        static void GenerarInformePDF()
        {

            string rutaPDF = "C:/Users/maxic/OneDrive/Escritorio/Informe.pdf"; // Ruta para el archivo PDF
            try
            {
                using (MySqlConnection connection = new MySqlConnection(connectionString))
                {
                    connection.Open();
                    // Consulta SQL para obtener la lista de tablas en la base de datos
                    string query = "SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA = @Database ORDER BY TABLE_NAME";

                    List<string> entities = new List<string>();

                    using (MySqlCommand command = new MySqlCommand(query, connection))
                    {
                        command.Parameters.AddWithValue("@Database", connection.Database);

                        using (MySqlDataReader reader = command.ExecuteReader())
                        {
                            Console.WriteLine("Entidades disponibles:");
                            int entityId = 1;
                            while (reader.Read())
                            {
                                string tableName = reader.GetString(0);
                                entities.Add(tableName);
                                Console.WriteLine($"{entityId}. {tableName}");
                                entityId++;
                            }
                        }
                    }

                    // Permitir al usuario la selección de la entidad
                    Console.Write("Seleccione el número de la entidad que desea mostrar en el informe: ");
                    if (int.TryParse(Console.ReadLine(), out int selectedEntityId) && selectedEntityId > 0 && selectedEntityId <= entities.Count)
                    {
                        string selectedEntity = entities[selectedEntityId - 1];

                        // Consulta SQL para obtener los atributos de la entidad seleccionada
                        string attributeQuery = "SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = @Database AND TABLE_NAME = @TableName";

                        List<string> attributes = new List<string>();

                        using (MySqlCommand attributeCommand = new MySqlCommand(attributeQuery, connection))
                        {
                            attributeCommand.Parameters.AddWithValue("@Database", connection.Database);
                            attributeCommand.Parameters.AddWithValue("@TableName", selectedEntity);

                            using (MySqlDataReader attributeReader = attributeCommand.ExecuteReader())
                            {
                                // Mostrar los atributos disponibles para seleccionar
                                Console.WriteLine($"Atributos disponibles para la entidad '{selectedEntity}':");
                                while (attributeReader.Read())
                                {
                                    string attributeName = attributeReader.GetString(0);
                                    attributes.Add(attributeName);
                                    Console.WriteLine(attributeName);
                                }
                            }
                        }
                        Console.WriteLine("Ingrese los nombres de los atributos que desea mostrar (separados por coma):");
                        string selectedAttributesInput = Console.ReadLine();
                        string[] selectedAttributes = selectedAttributesInput.Split(',');

                        // Verificar que los atributos seleccionados son válidos
                        foreach (var attr in selectedAttributes)
                        {
                            if (!attributes.Contains(attr.Trim()))
                            {
                                Console.WriteLine($"El atributo '{attr.Trim()}' no es válido para la entidad '{selectedEntity}'.");
                                return;
                            }
                        }

                        // Crear la consulta SQL para obtener los datos de los atributos seleccionados
                        string columns = string.Join(", ", selectedAttributes);
                        string dataQuery = $"SELECT {columns} FROM {selectedEntity}";

                        List<Dictionary<string, object>> data = new List<Dictionary<string, object>>();

                        using (MySqlCommand dataCommand = new MySqlCommand(dataQuery, connection))
                        {
                            using (MySqlDataReader dataReader = dataCommand.ExecuteReader())
                            {
                                // Leer los datos y almacenarlos en una lista de diccionarios
                                while (dataReader.Read())
                                {
                                    Dictionary<string, object> row = new Dictionary<string, object>();
                                    foreach (var attr in selectedAttributes)
                                    {
                                        row[attr.Trim()] = dataReader[attr.Trim()];
                                    }
                                    data.Add(row);
                                }
                            }
                        }

                        // Mostrar el informe en pantalla
                        Console.WriteLine($"\nInforme de la entidad: {selectedEntity}");
                        foreach (var row in data)
                        {
                            foreach (var kvp in row)
                            {
                                Console.WriteLine($"{kvp.Key}: {kvp.Value}");
                            }
                            Console.WriteLine();
                        }

                        // Preguntar al usuario si desea guardar el informe en PDF
                        Console.WriteLine("¿Desea guardar el informe en PDF? (s/n):");
                        string savePdfResponse = Console.ReadLine();

                        if (savePdfResponse.ToLower() == "s")
                        {
                            // Crear un nuevo documento PDF
                            using (PdfWriter writer = new PdfWriter(rutaPDF))
                            using (PdfDocument pdf = new PdfDocument(writer))
                            using (Document document = new Document(pdf))
                            {
                                document.Add(new Paragraph($"Entidad: {selectedEntity}"));

                                // Agregar los datos de los atributos seleccionados de la entidad al documento PDF
                                foreach (var row in data)
                                {
                                    foreach (var kvp in row)
                                    {
                                        document.Add(new Paragraph($"{kvp.Key}: {kvp.Value}"));
                                    }
                                    document.Add(new Paragraph("\n"));
                                }
                            }

                            Console.WriteLine("Informe PDF generado correctamente.");
                        }
                        else
                        {
                            Console.WriteLine("El informe no se guardó en PDF.");
                        }
                    }
                    else
                    {
                        Console.WriteLine("Selección no válida. Saliendo del programa.");
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error al generar el informe: " + ex.Message);
            }
        }

        static void GenerarProcedimientosAlmacenados()
        {
            try
            {
                using (MySqlConnection connection = new MySqlConnection(connectionString))
                {
                    connection.Open();

                    // Consulta SQL para obtener el nombre de todas las tablas en el esquema actual
                    string tableQuery = "SELECT table_name FROM information_schema.tables WHERE table_schema = DATABASE()";

                    List<string> tableNames = new List<string>();

                    using (MySqlCommand tableCommand = new MySqlCommand(tableQuery, connection))
                    using (MySqlDataReader tableReader = tableCommand.ExecuteReader())
                    {
                        // Obtener los nombres de las tablas
                        while (tableReader.Read())
                        {
                            tableNames.Add(tableReader.GetString(0));
                        }
                    }

                    foreach (var tableName in tableNames)
                    {
                        StringBuilder sb = new StringBuilder();

                        sb.AppendLine($"-- Procedimientos almacenados para la tabla {tableName}");
                        sb.AppendLine();

                        // Procedimiento almacenado para INSERT
                        sb.AppendLine($"CREATE PROCEDURE Insertar_{tableName}(");
                        sb.AppendLine(string.Join(", ", GetParameterList(connection, tableName)));
                        sb.AppendLine(")");
                        sb.AppendLine("BEGIN");
                        sb.AppendLine($"    INSERT INTO {tableName} (");

                        sb.AppendLine(string.Join(", ", GetColumnList(connection, tableName)));

                        sb.AppendLine("    )");
                        sb.AppendLine("    VALUES (");

                        sb.AppendLine(string.Join(", ", GetParameterList(connection, tableName).Select(p => p.Split(' ')[1])));
                        sb.AppendLine("    );");
                        sb.AppendLine("END;");
                        sb.AppendLine();

                        // Procedimiento almacenado para UPDATE
                        string idColumn = GetIdColumn(connection, tableName);
                        sb.AppendLine($"CREATE PROCEDURE Actualizar_{tableName}(");
                        sb.AppendLine(string.Join(", ", GetParameterList(connection, tableName)) + $", IN p_{idColumn} {GetColumnType(connection, tableName, idColumn)}");
                        sb.AppendLine(")");
                        sb.AppendLine("BEGIN");
                        sb.AppendLine($"    UPDATE {tableName}");
                        sb.AppendLine("    SET");

                        sb.AppendLine(string.Join(", ", GetUpdateSetList(connection, tableName)));

                        sb.AppendLine($"    WHERE {idColumn} = p_{idColumn};");
                        sb.AppendLine("END;");
                        sb.AppendLine();

                        // Procedimiento almacenado para DELETE
                        sb.AppendLine($"CREATE PROCEDURE Eliminar_{tableName}(IN p_{idColumn} {GetColumnType(connection, tableName, idColumn)})");
                        sb.AppendLine("BEGIN");
                        sb.AppendLine($"    DELETE FROM {tableName}");
                        sb.AppendLine($"    WHERE {idColumn} = p_{idColumn};");
                        sb.AppendLine("END;");
                        sb.AppendLine();

                        // Procedimiento almacenado para SELECT
                        sb.AppendLine($"CREATE PROCEDURE Seleccionar_{tableName}()");
                        sb.AppendLine("BEGIN");
                        sb.AppendLine($"    SELECT * FROM {tableName};");
                        sb.AppendLine("END;");
                        sb.AppendLine();

                        Console.WriteLine(sb.ToString());
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error al generar los procedimientos almacenados: " + ex.Message);
            }
        }

        static List<string> GetColumnList(MySqlConnection connection, string tableName)
        {
            List<string> columns = new List<string>();

            // Consulta SQL para obtener los nombres de las columnas de la tabla
            string columnQuery = $"SELECT column_name FROM information_schema.columns WHERE table_name = '{tableName}' AND table_schema = DATABASE()";

            using (MySqlCommand columnCommand = new MySqlCommand(columnQuery, connection))
            using (MySqlDataReader columnReader = columnCommand.ExecuteReader())
            {
                while (columnReader.Read())
                {
                    columns.Add(columnReader.GetString(0));
                }
            }

            return columns;
        }

        static string GetIdColumn(MySqlConnection connection, string tableName)
        {
            // Consulta SQL para obtener el nombre de la columna de la clave primaria
            string idQuery = $@"
        SELECT kcu.column_name
        FROM information_schema.table_constraints tc
        JOIN information_schema.key_column_usage kcu
        ON tc.constraint_name = kcu.constraint_name
        AND tc.table_name = kcu.table_name
        WHERE tc.constraint_type = 'PRIMARY KEY' AND tc.table_name = '{tableName}' AND tc.table_schema = DATABASE();";

            using (MySqlCommand idCommand = new MySqlCommand(idQuery, connection))
            {
                return idCommand.ExecuteScalar().ToString();
            }
        }

        static List<string> GetParameterList(MySqlConnection connection, string tableName)
        {
            List<string> parameters = new List<string>();

            // Obtener los nombres de las columnas de la tabla
            List<string> columns = GetColumnList(connection, tableName);

            foreach (var column in columns)
            {
                parameters.Add($"IN p_{column} {GetColumnType(connection, tableName, column)}");
            }

            return parameters;
        }

        static List<string> GetUpdateSetList(MySqlConnection connection, string tableName)
        {
            List<string> updateSetList = new List<string>();

            List<string> columns = GetColumnList(connection, tableName);

            foreach (var column in columns)
            {
                updateSetList.Add($"{column} = p_{column}");
            }

            return updateSetList;
        }

        static string GetColumnType(MySqlConnection connection, string tableName, string columnName)
        {
            // Consulta SQL para obtener el tipo de datos de la columna
            string columnTypeQuery = $"SELECT data_type FROM information_schema.columns WHERE table_name = '{tableName}' AND column_name = '{columnName}' AND table_schema = DATABASE()";

            using (MySqlCommand columnTypeCommand = new MySqlCommand(columnTypeQuery, connection))
            {
                return columnTypeCommand.ExecuteScalar().ToString();
            }
        }

    }
}
