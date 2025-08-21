#include <iostream>
#include <string>
#include <vector>

class Libro {
    public :
        std :: string titulo;
        std :: string autor;
        int anioPublicacion;
        bool estaDisponible; 

    //Inicializar valores
    Libro() : anioPublicacion(0), estaDisponible(true) {}

    void mostrarDetalles(){
        std:: cout << "---------------------"<< std::endl;
        std:: cout << "Titulo: "<< titulo<< std::endl;
        std:: cout << "Autor: "<< autor<< std::endl;
        std:: cout << "Año: "<< anioPublicacion<< std::endl;
        std:: cout << "Disponible: "<< estaDisponible<< std::endl;

    }
};

class Biblioteca {
private:
    std::vector<Libro> coleccion;

public:
    void agregarLibro(const Libro& nuevoLibro) {
        coleccion.push_back(nuevoLibro);
        std::cout << "Libro agregado correctamente.\n";
    }

    void mostrarInventario() {
        std::cout << "\n--- INVENTARIO ---\n";
        if (coleccion.empty()) {
            std::cout << "La biblioteca está vacía.\n";
            return;
        }
        for (const auto& libro : coleccion) {
            //libro.mostrarDetallesCompletos();
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

    void prestarLibro(const std::string& tituloPrestamo) {
        Libro* libro = buscarLibro(tituloPrestamo);
        if (libro == nullptr) {
            std::cout << "El libro no se encuentra en la biblioteca.\n";
        } else if (!libro->estaDisponible) {
            std::cout << "El libro ya está prestado.\n";
        } else {
            libro->estaDisponible = false;
            std::cout << "Has prestado el libro: " << libro->titulo << "\n";
        }
    }

    void devolverLibro(const std::string& tituloDevolucion) {
        Libro* libro = buscarLibro(tituloDevolucion);
        if (libro == nullptr) {
            std::cout << "El libro no se encuentra en la biblioteca.\n";
        } else if (libro->estaDisponible) {
            std::cout << "El libro ya estaba disponible.\n";
        } else {
            libro->estaDisponible = true;
            std::cout << "Has devuelto el libro: " << libro->titulo << "\n";
        }
    }
};

int main (){
    Biblioteca miBiblioteca;
    int opcion = 0;


     Libro libroInicial;
    libroInicial.titulo = "El Hobbit";
    libroInicial.autor = "J.R.R. Tolkien";
    miBiblioteca.agregarLibro(libroInicial);

    

    while (opcion != 5) {
        std::cout << "\n--- BIBLIOTECA DIGITAL ---" << std::endl;
        std::cout << "1. Anadir libro" << std::endl;
        std::cout << "2. Mostrar inventario" << std::endl;
        std::cout << "3. Prestar libro" << std::endl;
        std::cout << "4. Devolver libro" << std::endl;
        std::cout << "5. Salir" << std::endl;
        std::cout << "Seleccione una opcion: ";
        std::cin >> opcion;
        
        // Limpiar el buffer de entrada para futuras lecturas de texto
        std::cin.ignore();

        // Usar un switch o if-else para manejar la opción del usuario
        // ... Lógica del menú aquí ...
    }

    return 0;
}



//hola