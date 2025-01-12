#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <set>

using Conjunto = std::vector<int>;
using Solucion = std::vector<Conjunto>;

int suma(const Conjunto& set, const Conjunto &mask)
{
    int sum = 0;
    for (size_t i = 0; i < set.size(); i++)
        sum += set.at(i) * mask.at(i);
    
    return sum;
}

bool no_repetidos(const Conjunto &actual, const Solucion soluciones)
{
    return std::find(soluciones.begin(), soluciones.end(), actual) == soluciones.end();
}

void imprimir_soluciones(const Solucion &soluciones)
{
    for (const auto &solucion : soluciones)
    {
        std::cout << "{ ";
        for (const auto &num : solucion)
        {
            std::cout << num << " ";
        }
        std::cout<< "}" << std::endl;
    }
}

bool suma_subconjuntos(const Conjunto &inicial, Conjunto actual, const int valor, Solucion &soluciones)
{

    int sum = suma(inicial, actual);

    // Si supera el valor, terminamos
    if (sum > valor)
        return false;
    
    // Si llegamos al valor
    if (sum == valor)
    {
        // Si la solucion no esta en el conjunto de soluciones
        if (no_repetidos(actual,soluciones))
        {
            soluciones.push_back(actual);
            return true;
        }
        
        return false;
    }
    
    for (size_t i = 0; i < inicial.size(); i++)
    {
        if (actual.at(i) == 0)
        {
            actual.at(i) = 1;
            suma_subconjuntos(inicial, actual, valor, soluciones);
            actual.at(i) = 0; 
        }
    }

    return !soluciones.empty();
}


int main()
{
    Conjunto set = {5, 10, 12, 13, 15, 18};

    std::sort(set.begin(),set.end());
    Solucion sol = {};
    Conjunto mask(set.size(),0);

    suma_subconjuntos(set,mask,30,sol);

    imprimir_soluciones(sol);
}
