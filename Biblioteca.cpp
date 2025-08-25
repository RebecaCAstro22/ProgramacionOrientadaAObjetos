#include <iostream>
#include <string>
#include <vector>
#include <fstream>  // Guarda y Carga archivos
#include <sstream>  // ← Necesario para usar std::stringstream

class Libro {
    public :
        std :: string titulo;
        std :: string autor;
        int anioPublicacion;
        bool estaDisponible; 

    //Inicializar valores
    Libro() : anioPublicacion(0), estaDisponible(true) {}

    void mostrarDetalles() const{
        std:: cout << "---------------------"<< std::endl;
        std:: cout << "Titulo: "<< titulo<< std::endl;
        std:: cout << "Autor: "<< autor<< std::endl;
        std:: cout << "Año: "<< anioPublicacion<< std::endl;
        std:: cout << "Disponible: "<< (estaDisponible ? "Si": "No")<< std::endl;

    }
};

class Biblioteca {
private:
    std::vector<Libro> coleccion;

public:
    void agregarLibro(const Libro& nuevoLibro) {
        for (const auto& libro : coleccion){
            if(libro.titulo== nuevoLibro.titulo){
                std::cout <<"El libro ya existe en la biblioteca. \n";
            }
        }

        coleccion.push_back(nuevoLibro);
        std::cout << "Libro agregado correctamente.\n";
    }

    void mostrarInventario() const {
        std::cout << "\n--- INVENTARIO ---\n";
        if (coleccion.empty()) {
            std::cout << "La biblioteca está vacía.\n";
            return;
        }
        for (const auto& libro : coleccion) {
            libro.mostrarDetalles();
            std::cout << "-------------------\n";
        }
    }

    Libro* buscarLibro(const std::string& tituloBuscado) {
        for (auto& libro : coleccion) {
            if (libro.titulo == tituloBuscado) {
                return &libro;
            }
        }
        return nullptr;
    }

    void prestarLibro(const std::string& titulo) {
        Libro* libro = buscarLibro(titulo);
        if (!libro) {
            std::cout << "El libro no se encuentra en la biblioteca.\n";
        } else if (!libro->estaDisponible) {
            std::cout << "El libro ya está prestado.\n";
        } else {
            libro->estaDisponible = false;
            std::cout << "Has prestado el libro: " << libro->titulo << "\n";
        }
    }

    void devolverLibro(const std::string& titulo) {
        Libro* libro = buscarLibro(titulo);
        if (!libro) {
            std::cout << "El libro no se encuentra en la biblioteca.\n";
        } else if (libro->estaDisponible) {
            std::cout << "El libro ya estaba disponible.\n";
        } else {
            libro->estaDisponible = true;
            std::cout << "Has devuelto el libro: " << libro->titulo << "\n";
        }
    }

void guardarBiblioteca(const std::string& nombreArchivo) {
    std::ofstream archivo(nombreArchivo);
    for (const auto& libro : coleccion) {
        archivo << libro.titulo << "," << libro.autor << "," << libro.anioPublicacion << "," << libro.estaDisponible << "\n";
    }
    archivo.close();
    std::cout << " Biblioteca guardada en " << nombreArchivo << "\n";
}

void cargarBiblioteca(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    std::string linea;
    while (std::getline(archivo, linea)) {
        std::stringstream ss(linea);
        Libro libro;
        std::string disponibleStr;

        std::getline(ss, libro.titulo, ',');
        std::getline(ss, libro.autor, ',');
        ss >> libro.anioPublicacion;
        ss.ignore(); // saltar la coma
        std::getline(ss, disponibleStr);
        libro.estaDisponible = (disponibleStr == "1");

        coleccion.push_back(libro);
    }
    archivo.close();
    std::cout << "Biblioteca cargada desde " << nombreArchivo << "\n";
}
    
};

int main (){
    Biblioteca miBiblioteca;
    miBiblioteca.cargarBiblioteca("datos.txt");

    int opcion = 0;


     Libro libroInicial;
    libroInicial.titulo = "El Hobbit";
    libroInicial.autor = "J.R.R. Tolkien";
    miBiblioteca.agregarLibro(libroInicial);

    

    

    while (opcion != 5) {
        std::cout << "\n--- BIBLIOTECA DIGITAL ---" << std::endl;
        std::cout << "1. Añadir libro" << std::endl;
        std::cout << "2. Mostrar inventario" << std::endl;
        std::cout << "3. Prestar libro" << std::endl;
        std::cout << "4. Devolver libro" << std::endl;
        std::cout << "5. Salir" << std::endl;
        std::cout << "Seleccione una opcion: ";
        std::cin >> opcion;
        std ::cin.ignore();// Limpiar el buffer de entrada para futuras lecturas de texto

        if (opcion ==1){
            Libro nuevo;
            std::cout << "Título: ";
            std::getline(std::cin, nuevo.titulo);
            std::cout << "Autor: ";
            std::getline(std::cin, nuevo.autor);
            std::cout << "Año de publicación: ";
            std::cin >> nuevo.anioPublicacion;
            std::cin.ignore();
            nuevo.estaDisponible = true;
            miBiblioteca.agregarLibro(nuevo);
        } else if (opcion == 2) {
            miBiblioteca.mostrarInventario();
        } else if (opcion == 3) {
            std::string titulo;
            std::cout << "Título a prestar: ";
            std::getline(std::cin, titulo);
            miBiblioteca.prestarLibro(titulo);
        } else if (opcion == 4) {
            std::string titulo;
            std::cout << "Título a devolver: ";
            std::getline(std::cin, titulo);
            miBiblioteca.devolverLibro(titulo);
        } else if (opcion != 5) {
            std::cout << "Opción inválida. Intente de nuevo.\n";
        }
    }
miBiblioteca.guardarBiblioteca("datosdeBiblioteca.txt");
    return 0;
}




