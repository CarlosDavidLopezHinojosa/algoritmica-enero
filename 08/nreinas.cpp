#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>

/**
 * Vamos a representar el tablero como un vector de indices
 * 
 * Ejemplo:
 * 
 * si x = [1, 3, 0, 2]
 * sería una solución en la que hay una reina en:
 * 
 * fila = 0, columna = 1
 * fila = 1, columna = 3
 * fila = 2, columna = 0
 * fila = 3, columna = 2
 * 
 * [ * Q * * ] -> (0, 1)
 * [ * * * Q ] -> (1, 3)
 * [ Q * * * ] -> (2, 0)
 * [ * * Q * ] -> (3, 2)
 */
using Tablero = std::vector<int>;
using Soluciones = std::vector<Tablero>;


/**
 * 1. Iteramos el tablero en la fila en la que estamos
 * 2. Si hay una reina en la misma columna no colocamos reina
 * 3. Si hay una reina con la misma distancia de Manhattan que otra no se coloca reina 
 *    (distancia de manhattan = [x, y] -> x == y)
 * 
 * - 4. Si esta la flag mover_como_caballo activa (mover como caballo y reina)
 * - 5. Iteramos el tablero en la fila en la que estamos
 * - 6. Comprobamos L izquierda sup/inf (distancia de manhattan = [1, 2])
 * - 7. Comprobamos L derecha sup/inf (distancia de manhattan = [2, 1])
 */
bool lugar(const Tablero &x, const int fila, const bool mover_como_caballo = false)
{
    for (int i = 0; i < fila; i++)
        if (x[i] == x[fila] || std::abs(x[i] - x[fila]) == std::abs(i - fila))
            return false;
    
    if (mover_como_caballo)
    {
        for (int i = 0; i < fila; i++)
        {
            // L izquierda superior e inferior
            bool diagonales_atacadas = std::abs(i - fila) == 1 &&  std::abs(x[i] - x[fila]) == 2; 
            // L derecha superior e inferior
            diagonales_atacadas |= std::abs(i - fila) == 2 &&  std::abs(x[i] - x[fila]) == 1;

            if (diagonales_atacadas) return false;
        }
    }

    return true;
}


void nreinas(Soluciones &X, const int n, bool mover_como_caballo = false)
{
    assert(mover_como_caballo && n >= 10 || n >= 4);

    Tablero x(n, -1);
    int fila = 0;
    while (fila >= 0)
    {
        x[fila]++; // Intentar una columna
        while (x[fila] < n && !lugar(x,fila)) // Si no se puede colocar se intenta la siguiente columna
            x[fila]++;
        
        if (x[fila] < n) // Hemos encontrado una columna valida
        {
            if (fila == n - 1) X.emplace_back(x); // Hemos llegado al final (Guardamos Sol)
            else fila++; // No hemos llegado al final (siguiente fila)
        }
        else // No es posición válida (Volvemos hacia atrás)
        {
            x[fila] = -1;
            fila --;
        }
    }
}

void nreinas(Soluciones &X, const int n,  int fila = 0, bool mover_como_caballo = false, Tablero x = {})
{
    assert(mover_como_caballo && n >= 10 || n >= 4);
    assert(x.empty() && fila != 0);

    if (x.empty()) x.assign(n, -1);
    
    if (fila < 0) return;

    x[fila]++;
    while (x[fila] < n && !lugar(x,fila)) // Si no se puede colocar se intenta la siguiente columna
        x[fila]++;

    if (x[fila] < n)
    {
        if (fila == n - 1) X.emplace_back(x); // Hemos llegado al final (Guardamos Sol)
        else fila++; // No hemos llegado al final (siguiente fila)
    }
    else
    {
        x[fila] = -1;
        fila--;
    }
    
    nreinas(X, n, fila, mover_como_caballo, x);
}

void vector_rep(const Tablero &vec)
{
    std::cout << "[";
    for (size_t i = 0; i < vec.size(); i++)
        std::cout << " " << vec.at(i);
    std::cout << " ]";
}

void tablero_rep(const Tablero &t)
{
    for (size_t i = 0; i < t.size(); i++)
    {
        for (size_t j = 0; j < t.size(); j++)
        {
            std::cout << ((t[i] == j) ? "Q ": "* ");
        }
        std::cout << std::endl;
    }
}

void sol_rep(const Soluciones &sol)
{
    for(const auto s: sol)
    {
        tablero_rep(s);
        std::cout << "" << std::endl;
    }

    std::cout << std::endl;
}

int main()
{
    Soluciones X;
    nreinas(X,4,0);
    sol_rep(X);
}