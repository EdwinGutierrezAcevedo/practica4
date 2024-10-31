#include "Router.h"
#include <iostream>

Router::Router(const std::string &nombre) : nombre(nombre) {}

std::string Router::getNombre() const {
    return nombre;
}

void Router::agregarConexion(const std::string &destino, float costo) {
    conexiones[destino] = costo;
}

float Router::obtenerCosto(const std::string &destino) const {
    auto it = conexiones.find(destino);
    if (it != conexiones.end()) {
        return it->second;
    } else {
        return -1; // Usar -1 para indicar que no hay conexión directa
    }
}

const std::map<std::string, float>& Router::obtenerConexiones() const {
    return conexiones;
}

void Router::mostrarConexiones() const {
    std::cout << "Conexiones desde " << nombre << ":" << std::endl;
    for (const auto &par : conexiones) {
        std::cout << "  a " << par.first << ": " << par.second << std::endl;
    }
}
