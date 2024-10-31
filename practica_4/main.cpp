/*#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

vector<vector<string>> leerCSV(const string &nombreArchivo) {
    vector<vector<string>> matriz;
    ifstream archivo(nombreArchivo);
    string linea;

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string dato;
        vector<string> fila;

        while (getline(ss, dato, ',')) {
            fila.push_back(dato);
        }

        matriz.push_back(fila);
    }

    return matriz;
}

void mostrarMatriz(const vector<vector<string>> &matriz) {
    for (const auto &fila : matriz) {
        for (const auto &valor : fila) {
            cout << setw(10) << valor << " ";
        }
        cout << endl;
    }
}

vector<vector<float>> convertirAMatrizFloat(const vector<vector<string>> &matriz) {
    vector<vector<float>> matrizFloat(matriz.size() - 1, vector<float>(matriz[0].size() - 1, 0.0));

    for (size_t i = 1; i < matriz.size(); ++i) {
        for (size_t j = 1; j < matriz[i].size(); ++j) {
            matrizFloat[i - 1][j - 1] = stof(matriz[i][j]);
        }
    }

    return matrizFloat;
}

void mostrarMatrizFloat(const vector<vector<float>> &matrizFloat, const vector<vector<string>> &matriz) {
    // Mostrar nombres de columnas
    cout << setw(10) << " ";
    for (size_t j = 1; j < matriz[0].size(); ++j) {
        cout << setw(10) << matriz[0][j] << " ";
    }
    cout << endl;

    // Mostrar filas con nombres
    for (size_t i = 0; i < matrizFloat.size(); ++i) {
        cout << setw(10) << matriz[i + 1][0] << " "; // Mostrar nombre de la fila
        for (size_t j = 0; j < matrizFloat[i].size(); ++j) {
            cout << setw(10) << matrizFloat[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    string nombreArchivo = "matriz.csv"; // Nombre del archivo CSV
    vector<vector<string>> matriz = leerCSV(nombreArchivo);

    cout << "Contenido de la matriz (con strings):" << endl;
    mostrarMatriz(matriz);

    vector<vector<float>> matrizFloat = convertirAMatrizFloat(matriz);

    cout << "\nContenido de la matriz (floats):" << endl;
    mostrarMatrizFloat(matrizFloat, matriz);

    return 0;
}
*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // Para usar la función std::find

int encontrarPosicionEnrutador(const std::vector<std::string> &enrutadores, const std::string &enrutadorBuscado) {
    auto it = std::find(enrutadores.begin(), enrutadores.end(), enrutadorBuscado);

    if (it != enrutadores.end()) {
        return std::distance(enrutadores.begin(), it);
    } else {
        return -1; // Retorna -1 si no se encuentra el enrutador
    }
}

void actualizarCostos(std::vector<float> &costos, const std::vector<std::string> &enrutadores) {
    std::vector<std::string> enlaces;
    std::string enrutadorBuscado;
    float nuevoCosto;
    char continuar = 'y';

    while (continuar == 'y') {
        std::cout << "Ingresa el nombre del enrutador al que está enlazado: ";
        std::cin >> enrutadorBuscado;
        enlaces.push_back(enrutadorBuscado);

        std::cout << "Esta enlazado con otro? (y/n): ";
        std::cin >> continuar;
    }

    for (const auto &enrutador : enlaces) {
        int posicion = encontrarPosicionEnrutador(enrutadores, enrutador);
        if (posicion != -1) {
            std::cout << "Ingresa el nuevo costo de enlace con " << enrutador << ": ";
            std::cin >> nuevoCosto;
            costos[posicion] = nuevoCosto;
            std::cout << "El costo de enlace con  " << enrutador << " ha sido actualizado a " << nuevoCosto << std::endl;
        } else {
            std::cout << "Enrutador " << enrutador << " no encontrado." << std::endl;
        }
    }
}

int main() {
    std::vector<std::string> enrutadores = {"Router1", "Router2", "Router3", "Router4"};
    std::vector<float> costos = {1.5, 2.5, 3.5, 4.5};

    actualizarCostos(costos, enrutadores);

    // Mostrar costos actualizados
    std::cout << "\nCostos actualizados:" << std::endl;
    for (size_t i = 0; i < enrutadores.size(); ++i) {
        std::cout << enrutadores[i] << ": " << costos[i] << std::endl;
    }

    return 0;
}



