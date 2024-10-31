#ifndef NETWORK_H
#define NETWORK_H

#include "Router.h"
#include <vector>
#include <string>

class Network {
private:
    std::vector<Router> enrutadores;
    std::vector<std::vector<float>> matrizCostos;
    void inicializarMatriz();
    void calcularCostosIndirectos();
    std::vector<std::vector<int>> rutas; // Matriz para guardar rutas
public:
    void agregarRouter(const std::string &nombre, const std::map<std::string, float> &conexiones);
    void eliminarRouter(const std::string &nombre);
    Router* encontrarRouter(const std::string &nombre);
    float calcularCostoEnvio(const std::string &origen, const std::string &destino, std::vector<std::string> &camino) const;
    void opcionCalcularCostoEnvio();
    void calcularCostoYRuta(const std::string &origen, const std::string &destino) const;
    void mostrarRed() const;
    void menu();
    void cargarDesdeCSV(const std::string &nombreArchivo);
};

#endif // NETWORK_H
