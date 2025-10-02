Este proyecto implementa un sistema básico de gestión de biblioteca con funcionalidad CRUD (Crear, Leer, Actualizar, Eliminar) para Libros, Autores y Estudiantes, además de funcionalidades de Préstamo/Devolución y análisis de datos.

La aplicación fue desarrollada en C++ y utiliza la persistencia de datos mediante archivos de texto en formato CSV (.txt).
1. Para compilar y ejecutar este código, necesitarás:

Compilador C++: Se requiere el compilador GCC/G++ (utilizando MinGW o MSYS2/UCRT64).

Sistema Operativo: Windows.

2. Pasos de Compilación (Usando el Comando del Makefile / tasks.json)
El comando que utilizaste para compilar es el más directo y funciona perfectamente en la terminal de VS Code Comando de Compilación:

cmd /c chcp 65001>nul && C:\msys64\ucrt64\bin\g++.exe -fdiagnostics-color=always -g main.cpp -o main.exe
Abre tu terminal en la carpeta del proyecto.

Copia y pega el comando anterior (ajustando la ruta a tu g++.exe si es necesario).

Esto generará el archivo ejecutable llamado main.exe.
3. Pasos de Ejecución
Una vez compilado, ejecuta el programa con el siguiente comando: .\main.exe

Carga: Al iniciar el programa, intenta leer los datos de cuatro archivos .txt (autores.txt, libros.txt, estudiantes.txt, prestamos.txt). Si los archivos no existen, el programa inicia con las listas vacías.

Guardado: Al seleccionar la opción 0. GUARDAR DATOS Y SALIR en el menú principal, el programa escribe los datos actuales de la memoria en los archivos .txt correspondientes, usando un formato CSV (Valores Separados por Coma).

Importante: Asegúrate de salir siempre con la opción 0 para garantizar que los datos se guarden correctamente.

Opción de Menú	Funcionalidad	Descripción
1. Gestionar Libros	CRUD Libros	Permite Crear, Actualizar, Eliminar y Listar libros.
2. Realizar Préstamo	Transacción	Registra un préstamo, vinculando un libro a un estudiante.
3. Realizar Devolución	Transacción	Marca un préstamo como devuelto.
4. Listar Préstamos	Consulta (JOIN)	Muestra los préstamos activos de un estudiante específico.
5. Gestionar CRUD	CRUD Autor/Est.	Submenú para realizar CRUD en la tabla de Autores o Estudiantes.
6. Autor con más Libros	Análisis de Datos	Consulta qué autor tiene el mayor número de libros registrados.
0. SALIR	Persistencia	Guarda todos los datos en archivos .txt antes de cerrar.