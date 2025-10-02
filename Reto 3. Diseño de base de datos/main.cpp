#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>    
#include <ctime> // Necesario para obtener la hora y fecha del sistema
#include <map> // Necesario para usar std::map
#include <limits> // Necesario para std::numeric_limits


using namespace std;

// <<< PEGA LA FUNCIÓN AQUÍ <<<

// Función de ayuda para validar que un string solo contenga letras y espacios
bool esSoloTexto(const string& str) {
    if (str.empty()) return false; // El campo no debe estar vacío
    for (char c : str) {
        // isalpha() verifica letras. isspace() verifica espacios.
        if (!isalpha(c) && !isspace(c)) {
            return false;
        }
    }
    return true;
}

using namespace std;

// === 1. STRUCTS (Tablas del Modelo Relacional 3FN) ===

struct Autor {
    int id_autor;       // PK
    string nombre;
    string nacionalidad;
};

struct Libro {
    int id_libro;       // PK
    string titulo;
    string isbn;
    int ano_publicacion;
    int id_autor;       // FK: Se conecta con Autor
    bool disponible = true; // Estado de préstamo
};

struct Estudiante {
    int id_estudiante;  // PK
    string nombre;
    string grado;
};

struct Prestamo {
    int id_prestamo;    // PK
    string fecha_prestamo;
    string fecha_devolucion;
    int id_libro;       // FK: Se conecta con Libro
    int id_estudiante;  // FK: Se conecta con Estudiante
    bool activo = true; // true si está prestado, false si fue devuelto
};

// === 2. CLASE GESTORA DE LA BASE DE DATOS EN MEMORIA ===

class BibliotecaDB {
public:
    // Vectores para almacenar los datos en memoria
    vector<Autor> autores;
    vector<Libro> libros;
    vector<Estudiante> estudiantes;
    vector<Prestamo> prestamos;

    // Constructor: Carga todos los datos al iniciar
    BibliotecaDB();
    
    // Destructor: Guarda todos los datos al cerrar
    ~BibliotecaDB();

    // --- Métodos PÚBLICOS de Utilidad (Corrección del error de privacidad) ---
    int obtenerSiguienteId(const string& tipo);
    string leerLinea();
    
    // --- MÉTODOS DE BÚSQUEDA ---
    Autor* buscarAutor(int id);
    Libro* buscarLibro(int id);
    Estudiante* buscarEstudiante(int id);

    // --- CRUD: AUTOR ---
    void agregarAutor(Autor a);
    void listarAutores();
    bool actualizarAutor(int id, const string& nuevo_nombre, const string& nueva_nacionalidad);
    bool eliminarAutor(int id);
    
    // --- CRUD: LIBRO ---
    void agregarLibro(Libro l);
    void listarLibros();
    bool actualizarLibro(int id, const string& nuevo_titulo, const string& nuevo_isbn, int nuevo_ano, int nuevo_autor_id);
    bool eliminarLibro(int id);
 
    // --- CRUD: ESTUDIANTE ---
    void agregarEstudiante(Estudiante e);
    void listarEstudiantes();
    bool actualizarEstudiante(int id, const string& nuevo_nombre, const string& nuevo_grado);
    bool eliminarEstudiante(int id);

    // --- TRANSACCIONES: PRÉSTAMO ---
    void realizarPrestamo(Prestamo p);
    void devolverLibro(int id_prestamo);
    void buscarPrestamosPorEstudiante(int id_estudiante); // Simula JOIN
    bool eliminarPrestamo(int id); 
    void mostrarAutorConMasLibros();

private:
    // --- Métodos de Persistencia (Guardar/Cargar) ---
    void guardarAutores();
    void guardarLibros();
    void guardarEstudiantes();
    void guardarPrestamos();

    void cargarAutores();
    void cargarLibros();
    void cargarEstudiantes();
    void cargarPrestamos();
};

// ====================================================================
//                          IMPLEMENTACIONES
// ====================================================================

// --- Utilidades ---
// --- ANÁLISIS DE DATOS ---
// Encuentra y muestra el autor que tiene más libros registrados.
void BibliotecaDB::mostrarAutorConMasLibros() {
    if (libros.empty()) {
        cout << "\n[INFO] No hay libros registrados para realizar el conteo.\n";
        return;
    }

    // 1. Contar la frecuencia de cada ID de autor
    // map<ID_Autor, Cantidad_Libros>
    map<int, int> conteo_autores;
    
    for (const auto& libro : libros) {
        // Incrementa el contador para el id_autor de cada libro
        conteo_autores[libro.id_autor]++;
    }

    // 2. Encontrar el autor con el conteo más alto
    int id_autor_mas_libros = -1;
    int max_libros = -1;

    for (const auto& par : conteo_autores) {
        int id_actual = par.first;
        int cantidad_actual = par.second;

        if (cantidad_actual > max_libros) {
            max_libros = cantidad_actual;
            id_autor_mas_libros = id_actual;
        }
    }

    // 3. Mostrar el resultado
    if (id_autor_mas_libros != -1) {
        // Buscar el objeto Autor usando el ID
        Autor* autor = buscarAutor(id_autor_mas_libros); 
        
        if (autor != nullptr) {
            cout << "\n============================================\n";
            cout << "  AUTOR CON MÁS LIBROS\n";
            cout << "============================================\n";
            cout << "  ID: " << autor->id_autor << "\n";
            cout << "  Nombre: " << autor->nombre << "\n";
            cout << "  Nacionalidad: " << autor->nacionalidad << "\n";
            cout << "  Libros Registrados: " << max_libros << "\n";
            cout << "============================================\n";
        } else {
            // Caso de error (llave foránea rota)
            cout << "\n[ADVERTENCIA] El autor con ID " << id_autor_mas_libros << " no existe en la base de datos de autores.\n";
        }
    } else {
         // Este caso es redundante si libros.empty() se maneja arriba, pero es un buen control
        cout << "\n[INFO] No se pudo encontrar un autor con libros registrados.\n";
    }
}

string BibliotecaDB::leerLinea() {
    string linea;
    if (cin.peek() == '\n' || cin.peek() == EOF) {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    getline(cin, linea);
    return linea;
}

int BibliotecaDB::obtenerSiguienteId(const string& tipo) {
    if (tipo == "Autor") return autores.empty() ? 1 : autores.back().id_autor + 1;
    if (tipo == "Libro") return libros.empty() ? 1 : libros.back().id_libro + 1;
    if (tipo == "Estudiante") return estudiantes.empty() ? 1 : estudiantes.back().id_estudiante + 1;
    if (tipo == "Prestamo") return prestamos.empty() ? 1 : prestamos.back().id_prestamo + 1;
    return -1;
}

// Función de ayuda para obtener el año actual
int obtenerAnoActual() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    // El struct tm guarda el año desde 1900, por eso sumamos 1900
    return 1900 + ltm->tm_year; 
}

// --- Buscadores ---

Autor* BibliotecaDB::buscarAutor(int id) {
    auto it = find_if(autores.begin(), autores.end(), [id](const Autor& a){ return a.id_autor == id; });
    return (it != autores.end()) ? &(*it) : nullptr;
}

Libro* BibliotecaDB::buscarLibro(int id) {
    auto it = find_if(libros.begin(), libros.end(), [id](const Libro& l){ return l.id_libro == id; });
    return (it != libros.end()) ? &(*it) : nullptr;
}

Estudiante* BibliotecaDB::buscarEstudiante(int id) {
    auto it = find_if(estudiantes.begin(), estudiantes.end(), [id](const Estudiante& e){ return e.id_estudiante == id; });
    return (it != estudiantes.end()) ? &(*it) : nullptr;
}

// --- Constructores/Destructores (Persistencia) ---

BibliotecaDB::BibliotecaDB() {
    cargarAutores();
    cargarLibros();
    cargarEstudiantes();
    cargarPrestamos();
}

BibliotecaDB::~BibliotecaDB() {
    guardarAutores();
    guardarLibros();
    guardarEstudiantes();
    guardarPrestamos();
}

// --- Persistencia (Funciones simplificadas de guardar/cargar) ---
// NOTA: La lógica de guardado y cargado se repite intencionalmente para la claridad del formato CSV.

void BibliotecaDB::guardarAutores() {
    ofstream archivo("autores.txt");
    for (const auto& a : autores) {
        archivo << a.id_autor << "," << a.nombre << "," << a.nacionalidad << "\n";
    }
    archivo.close();
}
void BibliotecaDB::cargarAutores() {
    ifstream archivo("autores.txt");
    string linea, segmento;
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        Autor a;
        getline(ss, segmento, ','); a.id_autor = stoi(segmento);
        getline(ss, a.nombre, ',');
        getline(ss, a.nacionalidad, '\n');
        autores.push_back(a);
    }
    archivo.close();
}

void BibliotecaDB::guardarLibros() {
    ofstream archivo("libros.txt");
    for (const auto& l : libros) {
        archivo << l.id_libro << "," << l.titulo << "," << l.isbn << "," << l.ano_publicacion
                << "," << l.id_autor << "," << l.disponible << "\n";
    }
    archivo.close();
}
void BibliotecaDB::cargarLibros() {
    ifstream archivo("libros.txt");
    string linea, segmento;
    int disp_int;
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        Libro l;
        getline(ss, segmento, ','); l.id_libro = stoi(segmento);
        getline(ss, l.titulo, ',');
        getline(ss, l.isbn, ',');
        getline(ss, segmento, ','); l.ano_publicacion = stoi(segmento);
        getline(ss, segmento, ','); l.id_autor = stoi(segmento);
        getline(ss, segmento, '\n'); disp_int = stoi(segmento);
        l.disponible = (disp_int == 1);
        libros.push_back(l);
    }
    archivo.close();
}

void BibliotecaDB::guardarEstudiantes() {
    ofstream archivo("estudiantes.txt");
    for (const auto& e : estudiantes) {
        archivo << e.id_estudiante << "," << e.nombre << "," << e.grado << "\n";
    }
    archivo.close();
}
void BibliotecaDB::cargarEstudiantes() {
    ifstream archivo("estudiantes.txt");
    string linea, segmento;
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        Estudiante e;
        getline(ss, segmento, ','); e.id_estudiante = stoi(segmento);
        getline(ss, e.nombre, ',');
        getline(ss, e.grado, '\n');
        estudiantes.push_back(e);
    }
    archivo.close();
}

void BibliotecaDB::guardarPrestamos() {
    ofstream archivo("prestamos.txt");
    for (const auto& p : prestamos) {
        archivo << p.id_prestamo << "," << p.fecha_prestamo << "," << p.fecha_devolucion
                << "," << p.id_libro << "," << p.id_estudiante << "," << p.activo << "\n";
    }
    archivo.close();
}
void BibliotecaDB::cargarPrestamos() {
    ifstream archivo("prestamos.txt");
    string linea, segmento;
    int activo_int;
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        Prestamo p;
        getline(ss, segmento, ','); p.id_prestamo = stoi(segmento);
        getline(ss, p.fecha_prestamo, ',');
        getline(ss, p.fecha_devolucion, ',');
        getline(ss, segmento, ','); p.id_libro = stoi(segmento);
        getline(ss, segmento, ','); p.id_estudiante = stoi(segmento);
        getline(ss, segmento, '\n'); activo_int = stoi(segmento);
        p.activo = (activo_int == 1);
        prestamos.push_back(p);
    }
    archivo.close();
}

// --- IMPLEMENTACIÓN CRUD COMPLETO ---

// CREATE
void BibliotecaDB::agregarAutor(Autor a) {
    a.id_autor = obtenerSiguienteId("Autor");
    autores.push_back(a);
    cout << "\n[OK] Autor '" << a.nombre << "' agregado con ID: " << a.id_autor << ".\n";
}
void BibliotecaDB::agregarEstudiante(Estudiante e) {
    e.id_estudiante = obtenerSiguienteId("Estudiante");
    estudiantes.push_back(e);
    cout << "\n[OK] Estudiante '" << e.nombre << "' agregado con ID: " << e.id_estudiante << ".\n";
}
void BibliotecaDB::agregarLibro(Libro l) {
    if (buscarAutor(l.id_autor) == nullptr) { // FK Validation
        cout << "\n[ERROR] El ID de Autor (" << l.id_autor << ") no existe. Libro no agregado.\n";
        return;
    }
    l.id_libro = obtenerSiguienteId("Libro");
    libros.push_back(l);
    cout << "\n[OK] Libro '" << l.titulo << "' agregado con ID: " << l.id_libro << ".\n";
}

// READ (LISTAR/SIMULAR JOIN)
void BibliotecaDB::listarAutores() {
    cout << "\n--- LISTADO DE AUTORES ---\n";
    for (const auto& a : autores) {
        cout << "ID: " << a.id_autor << " | Nombre: " << a.nombre << " | Nacionalidad: " << a.nacionalidad << "\n";
    }
}
void BibliotecaDB::listarEstudiantes() {
    cout << "\n--- LISTADO DE ESTUDIANTES ---\n";
    for (const auto& e : estudiantes) {
        cout << "ID: " << e.id_estudiante << " | Nombre: " << e.nombre << " | Grado: " << e.grado << "\n";
    }
}
void BibliotecaDB::listarLibros() {
    cout << "\n--- LISTADO DE LIBROS (JOIN con Autor) ---\n";
    for (const auto& l : libros) {
        Autor* a = buscarAutor(l.id_autor); // SIMULACIÓN DE JOIN
        string nombre_autor = (a != nullptr) ? a->nombre : "Autor Desconocido";

        cout << "ID: " << l.id_libro << " | Título: " << l.titulo 
             << " | Autor: " << nombre_autor 
             << " | ISBN: " << l.isbn
             << " | Disponible: " << (l.disponible ? "SI" : "NO") << "\n";
    }
}

// UPDATE
bool BibliotecaDB::actualizarAutor(int id, const string& nuevo_nombre, const string& nueva_nacionalidad) {
    Autor* a = buscarAutor(id);
    if (a != nullptr) {
        a->nombre = nuevo_nombre;
        a->nacionalidad = nueva_nacionalidad;
        cout << "\n[OK] Autor " << id << " actualizado.\n";
        return true;
    }
    cout << "\n[ERROR] Autor " << id << " no encontrado para actualizar.\n";
    return false;
}
bool BibliotecaDB::actualizarEstudiante(int id, const string& nuevo_nombre, const string& nuevo_grado) {
    Estudiante* e = buscarEstudiante(id);
    if (e != nullptr) {
        e->nombre = nuevo_nombre;
        e->grado = nuevo_grado;
        cout << "\n[OK] Estudiante " << id << " actualizado.\n";
        return true;
    }
    cout << "\n[ERROR] Estudiante " << id << " no encontrado para actualizar.\n";
    return false;
}
// **UPDATE LIBRO**
bool BibliotecaDB::actualizarLibro(int id, const string& nuevo_titulo, const string& nuevo_isbn, int nuevo_ano, int nuevo_autor_id) {
    Libro* l = buscarLibro(id);
    if (l != nullptr) {
        // Validación de la nueva FK
        if (buscarAutor(nuevo_autor_id) == nullptr) {
            cout << "\n[ERROR] Nuevo ID de Autor " << nuevo_autor_id << " no existe. Actualización cancelada.\n";
            return false;
        }
        
        // --- AHORA INCLUYE TODOS LOS CAMPOS ---
        l->titulo = nuevo_titulo;
        l->isbn = nuevo_isbn;
        l->ano_publicacion = nuevo_ano; 
        l->id_autor = nuevo_autor_id;
        // ------------------------------------
        
        cout << "\n[OK] Libro " << id << " actualizado.\n";
        return true;
    }
    cout << "\n[ERROR] Libro " << id << " no encontrado para actualizar.\n";
    return false;
}
// DELETE (Incluye Integridad Referencial)
bool BibliotecaDB::eliminarAutor(int id) {
    for (const auto& l : libros) { // Chequeo de FK: Libros
        if (l.id_autor == id) {
            cout << "\n[ERROR] No se puede eliminar al Autor " << id << ". Aún tiene libros registrados.\n";
            return false;
        }
    }
    auto it = remove_if(autores.begin(), autores.end(), [id](const Autor& a){ return a.id_autor == id; });
    if (it != autores.end()) {
        autores.erase(it, autores.end());
        cout << "\n[OK] Autor " << id << " eliminado con éxito.\n";
        return true;
    }
    return false;
}
bool BibliotecaDB::eliminarEstudiante(int id) {
    for (const auto& p : prestamos) { // Chequeo de FK: Préstamos Activos
        if (p.id_estudiante == id && p.activo) {
            cout << "\n[ERROR] No se puede eliminar al Estudiante " << id << ". Aún tiene un préstamo activo.\n";
            return false;
        }
    }
    auto it = remove_if(estudiantes.begin(), estudiantes.end(), [id](const Estudiante& e){ return e.id_estudiante == id; });
    if (it != estudiantes.end()) {
        estudiantes.erase(it, estudiantes.end());
        cout << "\n[OK] Estudiante " << id << " eliminado con éxito.\n";
        return true;
    }
    return false;
}
bool BibliotecaDB::eliminarLibro(int id) {
    for (const auto& p : prestamos) { // Chequeo de FK: Préstamos Activos
        if (p.id_libro == id && p.activo) {
            cout << "\n[ERROR] No se puede eliminar el Libro " << id << ". Está activo en un préstamo.\n";
            return false;
        }
    }
    auto it = remove_if(libros.begin(), libros.end(), [id](const Libro& l){ return l.id_libro == id; });
    if (it != libros.end()) {
        libros.erase(it, libros.end());
        cout << "\n[OK] Libro " << id << " eliminado con éxito.\n";
        return true;
    }
    return false;
}

// --- TRANSACCIONES ---

void BibliotecaDB::realizarPrestamo(Prestamo p) {
    Libro* libro = buscarLibro(p.id_libro);
    Estudiante* estudiante = buscarEstudiante(p.id_estudiante);

    if (libro == nullptr || estudiante == nullptr) {
        cout << "\n[ERROR] ID de Libro o Estudiante no encontrado.\n";
        return;
    }
    if (!libro->disponible) { // MANEJO DE ERROR: Libro no disponible
        cout << "\n[ERROR] El libro '" << libro->titulo << "' ya está prestado.\n";
        return;
    }

    p.id_prestamo = obtenerSiguienteId("Prestamo");
    p.activo = true;
    prestamos.push_back(p);
    libro->disponible = false; // UPDATE de Libro (Transacción)

    cout << "\n[OK] PRÉSTAMO REGISTRADO. ID Préstamo: " << p.id_prestamo << ".\n";
}

void BibliotecaDB::devolverLibro(int id_prestamo) {
    Prestamo* p = nullptr;
    for(auto& prest : prestamos) {
        if (prest.id_prestamo == id_prestamo && prest.activo) {
            p = &prest;
            break;
        }
    }

    if (p == nullptr) {
        cout << "\n[ERROR] Préstamo ID " << id_prestamo << " no encontrado o ya fue devuelto.\n";
        return;
    }

    Libro* libro = buscarLibro(p->id_libro);
    p->activo = false; // UPDATE de Préstamo (Transacción)
    
    if (libro != nullptr) {
        libro->disponible = true; // UPDATE de Libro (Transacción)
        cout << "\n[OK] DEVOLUCIÓN EXITOSA. Libro '" << libro->titulo << "' regresado.\n";
    } else {
        cout << "\n[ADVERTENCIA] Devolución exitosa, pero el libro original no fue encontrado.\n";
    }
}

void BibliotecaDB::buscarPrestamosPorEstudiante(int id_estudiante) {
    Estudiante* est = buscarEstudiante(id_estudiante);
    if (est == nullptr) {
        cout << "\n[ERROR] Estudiante ID " << id_estudiante << " no encontrado.\n";
        return;
    }

    cout << "\n--- PRÉSTAMOS DE: " << est->nombre << " ---\n";
    bool encontrado = false;
    for (const auto& p : prestamos) {
        if (p.id_estudiante == id_estudiante) {
            Libro* libro = buscarLibro(p.id_libro); // SIMULACIÓN DE JOIN
            
            cout << "  - ID Préstamo: " << p.id_prestamo
                 << " | Libro: " << (libro ? libro->titulo : "Libro Eliminado")
                 << " | Estado: " << (p.activo ? "ACTIVO" : "DEVUELTO") << "\n";
            encontrado = true;
        }
    }
    if (!encontrado) {
        cout << "No se encontraron préstamos para este estudiante.\n";
    }
}


// ====================================================================
//                          MENÚ PRINCIPAL Y LÓGICA DE EJECUCIÓN
// ====================================================================



// FunciOn de ayuda para el MenU de GestiOn (OpciOn 5)
// --- Nueva función para manejar el menú CRUD completo (Opción 5) ---
void gestionarCRUD(BibliotecaDB& db) {
    int op_entidad, id, op_accion;
    string nuevo_nombre, nueva_nacionalidad, nuevo_grado;
    // int nuevo_autor_id; (No se usa aquí)

    cout << "\n--- GESTIÓN COMPLETA (CRUD) ---\n";
    cout << "1. Autor\n2. Estudiante\n";
    cout << "Seleccione entidad a gestionar: ";
    if (!(cin >> op_entidad)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "[ERROR] Entrada no válida. Saliendo de la gestión CRUD.\n";
        return;
    }
    
    // VALIDACIÓN: Asegura que la opción sea 1 o 2
    if (op_entidad != 1 && op_entidad != 2) {
        cout << "[ERROR] Opción de entidad no válida (" << op_entidad << "). Debe ser 1 o 2. Saliendo.\n";
        return;
    }
    
    cout << "\n-- Acciones --\n";
    cout << "1. Crear (Agregar)\n2. Actualizar (Modificar)\n3. Eliminar (Borrar)\n4. Listar Todos\n";
    cout << "Seleccione acción: ";
    if (!(cin >> op_accion)) {
         cin.clear();
         cin.ignore(numeric_limits<streamsize>::max(), '\n');
         cout << "[ERROR] Entrada no válida para acción.\n";
         return; 
    }
    
    // =========================================================================
    // LÓGICA DE CREATE
    // =========================================================================
    if (op_accion == 1) { 
        if (op_entidad == 1) { // Autor
            Autor a;
            // VALIDACIÓN DEL NOMBRE DEL AUTOR (CREATE)
            do {
                cout << "Nombre: "; a.nombre = db.leerLinea();
                if (!esSoloTexto(a.nombre)) {
                    cout << "[ERROR] El nombre solo debe contener letras y espacios. Intente de nuevo.\n";
                }
            } while (!esSoloTexto(a.nombre));

            // VALIDACIÓN DE LA NACIONALIDAD (CREATE)
            do {
                cout << "Nacionalidad: "; a.nacionalidad = db.leerLinea();
                if (!esSoloTexto(a.nacionalidad)) {
                    cout << "[ERROR] La nacionalidad solo debe contener letras y espacios. Intente de nuevo.\n";
                }
            } while (!esSoloTexto(a.nacionalidad));
            
            db.agregarAutor(a);

        } else if (op_entidad == 2) { // Estudiante
            Estudiante e;
            
            // VALIDACIÓN DEL NOMBRE DEL ESTUDIANTE (CREATE)
            do {
                cout << "Nombre: "; e.nombre = db.leerLinea();
                if (!esSoloTexto(e.nombre)) {
                    cout << "[ERROR] El nombre solo debe contener letras y espacios. Intente de nuevo.\n";
                }
            } while (!esSoloTexto(e.nombre));
            
            // Grado se acepta libremente (permite "5to B", "8dr", etc.)
            cout << "Grado: "; e.grado = db.leerLinea();

            db.agregarEstudiante(e);
        }
    } 
    // =========================================================================
    // LÓGICA DE UPDATE
    // =========================================================================
    else if (op_accion == 2) { 
        cout << "ID a actualizar: "; cin >> id;
        
        if (op_entidad == 1) { // Autor
            // VALIDACIÓN DEL NOMBRE DEL AUTOR (UPDATE)
            do {
                cout << "Nuevo Nombre: "; nuevo_nombre = db.leerLinea();
                if (!esSoloTexto(nuevo_nombre)) {
                    cout << "[ERROR] El nombre solo debe contener letras y espacios. Intente de nuevo.\n";
                }
            } while (!esSoloTexto(nuevo_nombre));

            // VALIDACIÓN DE LA NACIONALIDAD (UPDATE)
            do {
                cout << "Nueva Nacionalidad: "; nueva_nacionalidad = db.leerLinea();
                if (!esSoloTexto(nueva_nacionalidad)) {
                    cout << "[ERROR] La nacionalidad solo debe contener letras y espacios. Intente de nuevo.\n";
                }
            } while (!esSoloTexto(nueva_nacionalidad));

            db.actualizarAutor(id, nuevo_nombre, nueva_nacionalidad);

        } else if (op_entidad == 2) { // Estudiante
            // VALIDACIÓN DEL NOMBRE DEL ESTUDIANTE (UPDATE)
            do {
                cout << "Nuevo Nombre: "; nuevo_nombre = db.leerLinea();
                if (!esSoloTexto(nuevo_nombre)) {
                    cout << "[ERROR] El nombre solo debe contener letras y espacios. Intente de nuevo.\n";
                }
            } while (!esSoloTexto(nuevo_nombre));

            // El Grado aún se acepta libremente
            cout << "Nuevo Grado: "; nuevo_grado = db.leerLinea();
            db.actualizarEstudiante(id, nuevo_nombre, nuevo_grado);
        }
    } 
    // =========================================================================
    // LÓGICA DE DELETE/LIST
    // =========================================================================
    else if (op_accion == 3) { // DELETE
        cout << "ID a eliminar: "; cin >> id;
        if (op_entidad == 1) db.eliminarAutor(id);
        else if (op_entidad == 2) db.eliminarEstudiante(id);
    } 
    else if (op_accion == 4) { // LIST (READ)
        if (op_entidad == 1) db.listarAutores();
        else if (op_entidad == 2) db.listarEstudiantes();
    }
}

// FunciOn de ayuda para el MenU de Libros (OpciOn 1)
// FunciOn de ayuda para el MenU de Libros (OpciOn 1)
// FunciOn de ayuda para el MenU de Libros (OpciOn 1)
void gestionarLibros(BibliotecaDB& db) {
    int op_libro, id;
    string nuevo_titulo, nuevo_isbn;
    int nuevo_autor_id, nuevo_ano;

    cout << "\n--- GESTIÓN DE LIBROS ---\n";
    cout << "1. Agregar Nuevo Libro\n2. Listar Todos los Libros\n3. Actualizar Libro\n4. Eliminar Libro\n";
    cout << "Seleccione: ";
    if (!(cin >> op_libro)) return;

    if (op_libro == 1) { // CREATE LIBRO (Validación de Año aplicada)
        Libro l;
        cout << "Título: "; l.titulo = db.leerLinea();
        cout << "ISBN: "; l.isbn = db.leerLinea();

        // VALIDACIÓN DEL AÑO DE PUBLICACIÓN (Crear)
        int ano_actual = obtenerAnoActual();
        do {
            cout << "Año Publicación (Max " << ano_actual << "): "; 
            if (!(cin >> l.ano_publicacion) || l.ano_publicacion > ano_actual || l.ano_publicacion < 1000) {
                cout << "[ERROR] Año no válido. Debe ser un número entre 1000 y el año actual (" << ano_actual << ").\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                l.ano_publicacion = -1; // Fuerza la repetición
            }
        } while (l.ano_publicacion == -1);

        cout << "ID del Autor (Debe existir): "; cin >> l.id_autor;
        db.agregarLibro(l);
    } else if (op_libro == 2) { // READ
        db.listarLibros();
    } else if (op_libro == 3) { // UPDATE LIBRO (Validación de Año aplicada)
        cout << "ID del Libro a actualizar: "; cin >> id;
        cout << "Nuevo Título: "; nuevo_titulo = db.leerLinea();
        cout << "Nuevo ISBN: "; nuevo_isbn = db.leerLinea();

        // VALIDACIÓN DEL AÑO DE PUBLICACIÓN (Actualizar)
        int ano_actual = obtenerAnoActual();
        do {
            cout << "Nuevo Año de Publicación (Max " << ano_actual << "): "; 
            if (!(cin >> nuevo_ano) || nuevo_ano > ano_actual || nuevo_ano < 1000) {
                cout << "[ERROR] Año no válido. Debe ser un número entre 1000 y el año actual (" << ano_actual << ").\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                nuevo_ano = -1; // Fuerza la repetición
            }
        } while (nuevo_ano == -1);

        cout << "Nuevo ID de Autor: "; cin >> nuevo_autor_id;
        
        db.actualizarLibro(id, nuevo_titulo, nuevo_isbn, nuevo_ano, nuevo_autor_id);
    } else if (op_libro == 4) { // DELETE
        cout << "ID del Libro a eliminar: "; cin >> id;
        db.eliminarLibro(id);
    }
}
// Función de ayuda para validar el formato DD/MM/AAAA
bool esFormatoFechaValido(const string& fecha) {
    // 1. Verificar longitud (10 caracteres: DD/MM/AAAA)
    if (fecha.length() != 10) {
        return false;
    }
    
    // 2. Verificar separadores (/)
    if (fecha[2] != '/' || fecha[5] != '/') {
        return false;
    }
    
    // 3. Verificar que los demás caracteres sean dígitos
    for (int i = 0; i < 10; ++i) {
        if (i == 2 || i == 5) continue; // Ignorar las barras
        if (!isdigit(fecha[i])) {
            return false;
        }
    }
    
    // NOTA: Esta es una validación de formato BÁSICA.
    // No verifica si el día es <= 31 o si el mes es <= 12, 
    // pero es suficiente para validar la estructura del string.
    
    return true;
}
// Función de ayuda para validar que un string solo contenga letras y espacios
// NOTA: Esta función DEBE estar definida ANTES de int main()
void mostrarMenu() {
    cout << "\n\n--- MENÚ PRINCIPAL ---\n";
    cout << "1. Gestionar Libros\n";
    cout << "2. Realizar Préstamo\n";
    cout << "3. Realizar Devolución\n";
    cout << "4. Listar Préstamos por Estudiante\n";
    cout << "5. Gestionar Autores/Estudiantes \n";
    cout << "6. Mostrar Autor(es) con más libros\n";
    cout << "0. GUARDAR DATOS Y SALIR\n";
    cout << "Seleccione una opción: ";
}

int main() {
    BibliotecaDB db; 
    int opcion, id;
    
    cout << ">>> Sistema de Biblioteca Inicializado. Datos cargados/creados.\n";
do {
        mostrarMenu();
        
        // Manejo robusto de la entrada para evitar bucles o errores
        if (!(cin >> opcion)) {
             cin.clear();
             cin.ignore(numeric_limits<streamsize>::max(), '\n');
             opcion = -1;
             continue;
        }

         switch (opcion) {
            case 1: 
                gestionarLibros(db); 
                break;

            case 2: { // Realizar Préstamo
                Prestamo p;
                
                // Lee IDs
                cout << "ID del Libro a prestar: "; cin >> p.id_libro;
                cout << "ID del Estudiante: "; cin >> p.id_estudiante;
                db.leerLinea(); // Limpiar el buffer después de leer IDs

                // VALIDACIÓN DE FECHA DE PRÉSTAMO
                do {
                    cout << "Fecha de Préstamo (DD/MM/AAAA): "; p.fecha_prestamo = db.leerLinea();
                    if (!esFormatoFechaValido(p.fecha_prestamo)) {
                        cout << "[ERROR] Formato de fecha de préstamo NO válido. Use DD/MM/AAAA.\n";
                    }
                } while (!esFormatoFechaValido(p.fecha_prestamo));

                // VALIDACIÓN DE FECHA DE DEVOLUCIÓN
                do {
                    cout << "Fecha de Devolución (DD/MM/AAAA): "; p.fecha_devolucion = db.leerLinea();
                    if (!esFormatoFechaValido(p.fecha_devolucion)) {
                        cout << "[ERROR] Formato de fecha de devolución NO válido. Use DD/MM/AAAA.\n";
                    }
                } while (!esFormatoFechaValido(p.fecha_devolucion));
                
                db.realizarPrestamo(p);
                break;
            }

            case 3: { // Devolver Libro
                cout << "ID del Préstamo a devolver: "; cin >> id;
                db.devolverLibro(id);
                break;
            }

            case 4: { // Buscar Préstamos por Estudiante
                cout << "ID del Estudiante a buscar: "; cin >> id;
                db.buscarPrestamosPorEstudiante(id);
                break;
            }

            case 5: 
                gestionarCRUD(db); 
                break;
                
            case 6: // Muestra el Autor más productivo (nueva característica)
                db.mostrarAutorConMasLibros();
                break;

            case 0:
                cout << "[INFO] Guardando datos y saliendo...\n";
                // El destructor de db se llama automáticamente al salir de main() y guarda los datos.
                break;

            default:
                cout << "[ERROR] Opción no válida. Intente de nuevo.\n";
                break;
        }
    } while (opcion != 0);

    return 0;
}