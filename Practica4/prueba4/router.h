#ifndef ROUTER_H
#define ROUTER_H

#include <string>
#include <map>
#include <limits>
class Router {
private:
    std::string nombre;
    std::map<std::string, float> conexiones;
public:
    Router(const std::string &nombre);
    std::string getNombre() const;
    void agregarConexion(const std::string &destino, float costo);
    float obtenerCosto(const std::string &destino) const;
    const std::map<std::string, float>& obtenerConexiones() const;
    void mostrarConexiones() const;
};

#endif // ROUTER_H
