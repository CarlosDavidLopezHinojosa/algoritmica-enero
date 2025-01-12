#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <cassert>
#include <functional>

using Vector = std::vector<float>;
using Camino = std::vector<int>;
using Tabla = std::vector<Vector>;
using Grafo = Tabla;
const float inf = std::numeric_limits<float>::infinity();


#include <chrono>

using namespace std::chrono;

void vector_rep(const Camino &vec)
{
    std::cout << "[";
    for (size_t i = 0; i < vec.size(); i++)
        std::cout << " " << vec.at(i);
    std::cout << " ]";
}

bool mejorar_camino(const Grafo &g, const Tabla & t, const int i, const int u, const int v, float &peso)
{
    peso = t[i - 1][u] + g[u][v];
    return t[i][v] > peso;
}

void camino_minimo_grafo_polietapico(const Grafo &g, const int etapas, const int inicio, const int fin, Camino &camino)
{
    const int num_vertices = g.size();
    float peso = inf;

    Tabla tabla = Tabla(etapas + 1, Vector(num_vertices, inf));
    camino = Camino(num_vertices, -1);

    tabla[1][inicio] = 0;

    for (int i = 2; i <= etapas; i++)
    {
        for (int u = 0; u < num_vertices; u++)
        {
            if (tabla[i - 1][u] != inf)
            {
                for (int v = 0; v < num_vertices; v++)
                {
                    if (g[u][v] != inf && mejorar_camino(g, tabla, i, u, v, peso))
                    {
                        tabla[i][v] = peso;
                        camino[v] = u; 
                    }
                }   
            }
        }
    }

    if (tabla[etapas][fin] == inf)
    {
        camino = {};
        return;
    }

    Camino camino_invertido;
    for (int v = fin; v != -1; v = camino[v])
        camino_invertido.push_back(v);
    
    camino = camino_invertido;
    std::reverse(camino.begin(), camino.end());
}


float camino_minimo_grafo_polietapico(const Grafo &g, 
                                      Tabla &tabla, 
                                      const int etapas, 
                                      const int inicio, 
                                      const int fin,
                                      int u,
                                      int etapa_actual, 
                                      Camino &camino)
{
    assert(!tabla.empty());
    assert(!camino.empty());


    if (etapa_actual == etapas) // Si hemos llegado a la última etapa
        return (u == fin) ? 0 : inf;

    if (tabla[etapa_actual][u] != inf) // Si hay una conexión
        return tabla[etapa_actual][u];
    
    float mejor_peso = inf;
    int mejor_vecino = -1;

    for (int v = 0; v < g.size(); v++)
    {
        if (g[u][v] != inf)
        {
            float peso = g[u][v] + camino_minimo_grafo_polietapico(g, tabla, etapas, inicio, fin, v, etapa_actual + 1, camino);

            if (peso < mejor_peso)
            {
                mejor_peso = peso;
                mejor_vecino = v;
            }
        }
    }
    
    if (mejor_vecino != -1 )
        camino[u] = mejor_vecino;
    
    return tabla[etapa_actual][u] = mejor_peso;
}

int main()
{
    int n = 5;
    int etapas = 4;

    // Grafo representado como una matriz de adyacencia
    Grafo g(n, std::vector<float>(n, inf));

    // Definir las aristas entre los vértices de las distintas etapas
    g[0][1] = 2; // De S (vértice 0) a A (vértice 1)
    g[0][2] = 4; // De S (vértice 0) a B (vértice 2)
    g[1][2] = 1; // De A (vértice 1) a B (vértice 2)
    g[1][3] = 3; // De A (vértice 1) a C (vértice 3)
    g[2][4] = 5; // De B (vértice 2) a D (vértice 4)
    g[3][4] = 1; // De C (vértice 3) a D (vértice 4)

    Camino camino_iterativo;
    
    auto inicio_iterativo = high_resolution_clock::now();
    camino_minimo_grafo_polietapico(g, etapas, 0, 4, camino_iterativo);
    auto final_iterativo = high_resolution_clock::now();



    std::cout << "Iterativo: ";
    if (camino_iterativo.empty())
    {
        std::cout << "No hay camino desde el inicio hasta el fin." << std::endl;
    }
    else
    {
        for (int v : camino_iterativo)
            std::cout << v << " ";
        std::cout << std::endl;
    }

    Camino camino_recursivo(n, -1);
    Tabla tabla_recursiva(etapas + 1, Vector(n, inf));

    auto inicio_recursivo = high_resolution_clock::now();
    float costo_recursivo = camino_minimo_grafo_polietapico(g, tabla_recursiva, etapas, 0, 4, 0, 1, camino_recursivo);
    auto fin_recursivo = high_resolution_clock::now();

    std::cout << "Recursivo: ";
    if (costo_recursivo == inf)
    {
        std::cout << "No hay camino desde el inicio hasta el fin." << std::endl;
    }
    else
    {
        for (int v = 0; v != -1; v = camino_recursivo[v])
        {
            std::cout << v << " ";
            if (v == 4) // Detener al llegar al destino
                break;
        }
        std::cout << "\nCosto: " << costo_recursivo << std::endl;
    }

    std::cout << "Tiempo de ejecución iterativo: " << duration_cast<microseconds>(inicio_iterativo - final_iterativo).count() << std::endl;
    std::cout << "Tiempo de ejecución recursivo: " << duration_cast<microseconds>(inicio_recursivo - fin_recursivo).count() << std::endl;

    return 0;
}