#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <cassert>
#include <functional>
#include <random>

using Vector = std::vector<float>;
using Camino = std::vector<int>;
using Candidatos = Camino;
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
    

    for (int v = 0; v < n; v++)
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


int uniforme(const int a, const int b)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(a, b);
    return dis(gen);
}

void camino_hamiltoniano_las_vegas(const Grafo &g, const int inicio, Camino &camino_hamiltoniano)
{
    const int n = g.size();
    Registro visitados(n, false);

    camino_hamiltoniano = {inicio};
    visitados[inicio] = true;

    int u = inicio;
    bool encontrado = false;

    while (!encontrado) // DFS
    {
        int max = -1;
        Candidatos cand(n, -1);

        for (int v = 0; v < n; v++)
        {
            if (!visitados[v] && g[u][v] != inf) // Seleccionamos a los candidatos
            {
                max++;
                cand[max] = v;
            }
        }

        if (max == -1) // Si no hay más candidatos
        {
            if (camino_hamiltoniano.size() == n && g[camino_hamiltoniano.back()][inicio] != inf) // Hemos encontrado el camino
            {
                camino_hamiltoniano.push_back(inicio);
                encontrado = true; // Salimos
            }
            else // No lo hemos encontrado, volvemos a buscar
            {
                int ultimo = camino_hamiltoniano.back();
                visitados[ultimo] = false;
                camino_hamiltoniano.pop_back();
                
                u = ultimo;
            }
        }
        else // Seleccion de un candidato de manera aleatoria
        {
            int siguiente = uniforme(0, max);
            siguiente = cand[siguiente];
            visitados[siguiente] = true;
            camino_hamiltoniano.push_back(siguiente);

            u = siguiente;
        }
    }
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
    Camino camino, camino_vegas;
    Caminos caminos;

    int inicio = 0;
    int num_nodos = 0;
    visitados[inicio] = true;
    camino.push_back(inicio);

    caminos_hamiltonianos(g, visitados, camino, caminos, inicio, num_nodos, 1);

    std::cout << "Caminos Hamiltonianos encontrados (Backtracking):" << std::endl;

    imprimir_caminos(caminos);

    camino_hamiltoniano_las_vegas(g,0,camino_vegas);

    std::cout << "Caminos Hamiltonianos encontrados (Vegas):" << std::endl;
    imprimir_caminos({camino_vegas});
    return 0;
}