#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <cassert>
#include <functional>

using Vector = std::vector<float>;
using Camino = std::vector<int>;
using Caminos = std::vector<Camino>;
using Registro = std::vector<bool>;
using Grafo =  std::vector<Vector>;

const float inf = std::numeric_limits<float>::infinity();

bool caminos_hamiltonianos(const Grafo &g, Registro visitados, Camino camino, Caminos &caminos, const int &inicio, const int nodo, const int numero_nodos)
{
    const int n = g.size();

    if (n == numero_nodos)
    {
        if (g[nodo][inicio] != inf)
        {
            //std::reverse(camino.begin(), camino.end());
            camino.push_back(inicio);
            caminos.push_back(camino);
            return true;
        }
        
        return false;
    }
    

    for (size_t v = 0; v < n; v++)
    {
        if (g[nodo][v] != inf && !visitados[v])
        {
            visitados[v] = true;
            camino.push_back(v);
            caminos_hamiltonianos(g, visitados, camino, caminos, inicio, v, numero_nodos + 1);
            visitados[v] = false;
            camino.pop_back();
        }
    }

    return !camino.empty();
}

void imprimir_caminos(const Caminos &caminos)
{
    for (const auto &camino : caminos)
    {
        for (const auto &nodo : camino)
        {
            std::cout << nodo << " ";
        }
        std::cout << std::endl;
    }
}

int main()
{
    // Definir un grafo de ejemplo
    Grafo g = {
        {inf, 1, 1, inf},
        {1, inf, 1, 1},
        {1, 1, inf, 1},
        {inf, 1, 1, inf}
    };

    const int n = g.size();
    Registro visitados(n, false);
    Camino camino;
    Caminos caminos;

    int inicio = 0;
    int num_nodos = 0;
    visitados[inicio] = true;
    camino.push_back(inicio);

    caminos_hamiltonianos(g, visitados, camino, caminos, inicio, num_nodos, 1);

    std::cout << "Caminos Hamiltonianos encontrados:" << std::endl;
    imprimir_caminos(caminos);

    return 0;
}