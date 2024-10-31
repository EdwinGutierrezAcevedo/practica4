#include "Network.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <sstream>

void Network::agregarRouter(const std::string &nombre, const std::map<std::string, float> &conexiones) {
    Router nuevoRouter(nombre);
    for (const auto &conexion : conexiones) {
        nuevoRouter.agregarConexion(conexion.first, conexion.second);
    }
    enrutadores.push_back(nuevoRouter);
    inicializarMatriz();
}

void Network::eliminarRouter(const std::string &nombre) {
    enrutadores.erase(std::remove_if(enrutadores.begin(), enrutadores.end(),
                                     [&nombre](const Router &router) { return router.getNombre() == nombre; }),
                      enrutadores.end());
    inicializarMatriz();
}

Router* Network::encontrarRouter(const std::string &nombre) {
    auto it = std::find_if(enrutadores.begin(), enrutadores.end(),
                           [&nombre](const Router &router) { return router.getNombre() == nombre; });
    if (it != enrutadores.end()) {
        return &(*it);
    }
    return nullptr;
}

void Network::inicializarMatriz() {
    size_t n = enrutadores.size();
    matrizCostos.resize(n + 1, std::vector<float>(n + 1, -1));

    for (size_t i = 0; i < n; ++i) {
        matrizCostos[0][i + 1] = 0; // Posición irrelevante en [0][0]
        matrizCostos[i + 1][0] = 0; // Posición irrelevante en [0][0]
    }

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            if (i == j) {
                matrizCostos[i + 1][j + 1] = 0;
            } else {
                float costo = enrutadores[i].obtenerCosto(enrutadores[j].getNombre());
                matrizCostos[i + 1][j + 1] = (costo == -1) ? -1 : costo;
            }
        }
    }

    calcularCostosIndirectos();
}

void Network::calcularCostosIndirectos() {
    size_t n = enrutadores.size();
    rutas.resize(n + 1, std::vector<int>(n + 1, -1));

    for (size_t i = 1; i <= n; ++i) {
        for (size_t j = 1; j <= n; ++j) {
            if (i != j && matrizCostos[i][j] != -1) {
                rutas[i][j] = i;
            }
        }
    }

    for (size_t k = 1; k <= n; ++k) {
        for (size_t i = 1; i <= n; ++i) {
            for (size_t j = 1; j <= n; ++j) {
                if (matrizCostos[i][k] != -1 && matrizCostos[k][j] != -1) {
                    float costo = matrizCostos[i][k] + matrizCostos[k][j];
                    if (matrizCostos[i][j] == -1 || costo < matrizCostos[i][j]) {
                        matrizCostos[i][j] = costo;
                        rutas[i][j] = rutas[k][j];
                    }
                }
            }
        }
    }
}

void Network::calcularCostoYRuta(const std::string &origen, const std::string &destino) const {
    size_t n = enrutadores.size();
    std::vector<std::string> nombresEnrutadores;
    for (const auto &router : enrutadores) {
        nombresEnrutadores.push_back(router.getNombre());
    }

    int origenIndex = std::distance(nombresEnrutadores.begin(), std::find(nombresEnrutadores.begin(), nombresEnrutadores.end(), origen)) + 1;
    int destinoIndex = std::distance(nombresEnrutadores.begin(), std::find(nombresEnrutadores.begin(), nombresEnrutadores.end(), destino)) + 1;

    if (origenIndex == 0 || destinoIndex == 0 || origenIndex > n || destinoIndex > n) {
        std::cout << "Enrutador no encontrado." << std::endl;
        return;
    }

    if (matrizCostos[origenIndex][destinoIndex] == -1) {
        std::cout << "No existe una ruta del enrutador " << origen << " al enrutador " << destino << std::endl;
        return;
    }

    std::cout << "El costo para enviar un paquete desde " << origen << " hasta " << destino << " es: " << matrizCostos[origenIndex][destinoIndex] << std::endl;
    std::cout << "El camino a seguir es: ";

    std::vector<int> camino;
    for (int at = destinoIndex; at != -1; at = rutas[origenIndex][at]) {
        camino.push_back(at);
    }
    std::reverse(camino.begin(), camino.end());

    for (size_t i = 0; i < camino.size(); ++i) {
        if (i > 0) std::cout << "->";
        std::cout << nombresEnrutadores[camino[i] - 1];
    }
    std::cout << std::endl;
}


void Network::mostrarRed() const {
    size_t n = enrutadores.size();

    // Encabezado de la matriz
    std::cout << std::setw(12) << " ";
    for (size_t i = 1; i <= n; ++i) {
        std::cout << std::setw(12) << enrutadores[i - 1].getNombre() << " ";
    }
    std::cout << std::endl;

    // Cuerpo de la matriz
    for (size_t i = 1; i <= n; ++i) {
        std::cout << std::setw(12) << enrutadores[i - 1].getNombre() << " "; // Nombre del enrutador en la fila
        for (size_t j = 1; j <= n; ++j) {
            if (matrizCostos[i][j] == -1) {
                std::cout << std::setw(12) << "inf ";
            } else {
                std::cout << std::setw(12) << matrizCostos[i][j] << " ";
            }
        }
        std::cout << std::endl;
    }
}


void Network::cargarDesdeCSV(const std::string &nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return;
    }

    std::string linea, enrutador;
    std::getline(archivo, linea); // Leer la primera línea (encabezados)

    std::vector<std::string> enrutadores;
    std::stringstream ss(linea);
    std::getline(ss, enrutador, ','); // Salta la primera celda vacía
    while (std::getline(ss, enrutador, ',')) {
        enrutadores.push_back(enrutador);
    }

    while (std::getline(archivo, linea)) {
        std::stringstream ss(linea);
        std::string origen;
        std::getline(ss, origen, ',');

        std::map<std::string, float> conexiones;
        std::string costoStr;
        float costo;
        for (const auto& destino : enrutadores) {
            std::getline(ss, costoStr, ',');
            costo = std::stof(costoStr);
            if (costo >= 0) { // Considerar solo costos válidos
                conexiones[destino] = costo;
            }
        }
        agregarRouter(origen, conexiones);
    }

    archivo.close();
    inicializarMatriz();
}


void Network::menu() {
    int opcion;
    std::string nombre, destino, nombreArchivo;
    float costo;

    do {
        std::cout << "\n1. Agregar enrutador\n"
                  << "2. Eliminar enrutador\n"
                  << "3. Mostrar red\n"
                  << "4. Cargar red desde archivo .csv\n"
                  << "5. Costo y ruta para enviar un paquete\n"
                  << "6. Salir\n"
                  << "Selecciona una opcion: ";
        std::cin >> opcion;

        switch (opcion) {
        case 1: {
            std::cout << "Ingresa el nombre del enrutador: ";
            std::cin >> nombre;

            std::map<std::string, float> conexiones;
            char continuar = 'y';

            while (continuar == 'y') {
                std::cout << "Ingresa el nombre del enrutador de destino: ";
                std::cin >> destino;
                std::cout << "Ingresa el costo de conexion: ";
                std::cin >> costo;
                conexiones[destino] = costo;
                std::cout << "¿Deseas ingresar otra conexion? (y/n): ";
                std::cin >> continuar;
            }

            agregarRouter(nombre, conexiones);
            break;
        }
        case 2: {
            std::cout << "Ingresa el nombre del enrutador a eliminar: ";
            std::cin >> nombre;
            eliminarRouter(nombre);
            break;
        }
        case 3:
            mostrarRed();
            break;
        case 4: {
            std::cout << "Ingresa el nombre del archivo .csv: ";
            std::cin >> nombreArchivo;
            cargarDesdeCSV(nombreArchivo);
            break;
        }
        case 5: {
            std::cout << "Ingresa el nombre del enrutador de origen: ";
            std::cin >> nombre;
            std::cout << "Ingresa el nombre del enrutador de destino: ";
            std::cin >> destino;
            calcularCostoYRuta(nombre,destino);
            break;
        }
        case 6:
            std::cout << "Saliendo...\n";
            break;
        default:
            std::cout << "Opcion invalida, intenta nuevamente.\n";
            break;
        }
    } while (opcion != 5);
}
