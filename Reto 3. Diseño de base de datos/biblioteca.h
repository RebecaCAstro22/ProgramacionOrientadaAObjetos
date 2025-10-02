#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm> // Para std::find_if
#include <map> // Necesario para usar std::map (Ayuda de IA)
#include <limits> // Necesario para std::numeric_limits (Ayuda de IA)

using namespace std;

// === 1.(Tablas del Modelo Relacional 3FN) ===

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

// === 2. CLASE DE LA BASE DE DATOS EN MEMORIA ===

class BibliotecaDB {
public:
void mostrarAutorConMasLibros(); // <<-- Mostrara los LIbros
    // Vectores para almacenar los datos en memoria
    vector<Autor> autores;
    vector<Libro> libros;
    vector<Estudiante> estudiantes;
    vector<Prestamo> prestamos;

    // Constructor: Carga todos los datos al iniciar
    BibliotecaDB();
    
    // Destructor: Guarda todos los datos al cerrar
    ~BibliotecaDB();

    // --- Métodos PÚBLICOS (CORRECCIÓN DEL ERROR DE PRIVACIDAD) ---
    int obtenerSiguienteId(const string& tipo);
    string leerLinea();

    // --- Métodos CRUD ---

    // Autor
    void agregarAutor(Autor a);
    void listarAutores();
    Autor* buscarAutor(int id);
    
    // Libro
    void agregarLibro(Libro l);
    void listarLibros(); // Simula JOIN con Autor
    Libro* buscarLibro(int id);

    // Estudiante
    void agregarEstudiante(Estudiante e);
    
    // Préstamo
    void realizarPrestamo(Prestamo p);
    void devolverLibro(int id_prestamo);
    void buscarPrestamosPorEstudiante(int id_estudiante); // Simula JOIN con Libro

    

private:
    // --- Métodos de Persistencia ---
    void guardarAutores();
    void guardarLibros();
    void guardarEstudiantes();
    void guardarPrestamos();

    void cargarAutores();
    void cargarLibros();
    void cargarEstudiantes();
    void cargarPrestamos();
};

#endif // BIBLIOTECA_H