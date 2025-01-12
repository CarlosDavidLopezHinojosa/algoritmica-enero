#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>
#include <cassert>

using Indices = std::vector<int>;
using Error = std::vector<double>;
using Errores = std::vector<Error>;
using Father = std::vector<Indices>;

struct Punto
{
public:
    Punto(double x, double y){x_ = x; y_ = y;}
    double x() const {return x_;}
    double y() const {return y_;}
private:
    double x_;
    double y_;
};

const double inf = std::numeric_limits<double>::infinity();

using Segmento = std::vector<Punto>;

double ISE(const Segmento &seg, const int inicio, const int fin)
{
    const Punto &p1 = seg[inicio];
    const Punto &p2 = seg[fin];

    const double a = p1.y() - p2.y();
    const double b = p1.x() - p2.x();
    const double c = p2.y() * p1.x() - p1.x() * p2.y();

    double error = 0.0;

    for (int i = inicio + 1; i < fin; i++)
    {
        const Punto &p = seg[i];
        double distancia = std::abs(a * p.x() + b * p.y() + c) / std::sqrt(a * a + b * b); 
        error += distancia * distancia;
    }

    return error;
}

/**
 * M = Número de puntos para aproximar
 */
Indices aprox_polininomial_optima(const Segmento &seg, const int M)
{

    const int N = seg.size(); // Número de puntos
    assert(M <= N);
    

    Errores e(N + 1, Error(M + 1, inf));
    Father f(N + 1, Indices(M + 1, -1));

    //Caso base e(1, 1) = 0.0
    for (int i = 1; i <= N; i++)
        e[i][1] = 0.0;
    
    for (int m = 2; m <= M; m++)
    {
        for (int n = m; n <= N; n++)
        {
            for (int j = m - 1; j < n; j++)
            {
                double error = ISE(seg,j - 1, n - 1);
                double new_error = error + e[j][m - 1];

                if (new_error < e[n][m])
                {
                    e[n][m] = new_error;
                    f[n][m] = j;
                }
            }
        }
    }

    Indices resultado;

    int current = N;
    int segments = M;

    while (segments > 0) 
    {
        resultado.push_back(current - 1); // Convertir a índice 0
        current = f[current][segments];
        --segments;
    }

    std:reverse(resultado.begin(), resultado.end());
    return resultado;
}


int main() 
{
    // Crear un conjunto de puntos para el segmento
    Segmento puntos = {
        Punto(0.0, 0.0),
        Punto(1.0, 2.0),
        Punto(2.0, 3.0),
        Punto(3.0, 5.0),
        Punto(4.0, 7.0),
        Punto(5.0, 10.0),
        Punto(0,0)
    };

    // Número de segmentos deseados
    int M = 7;

    // Calcular la aproximación óptima
    Indices indices_optimos = aprox_polininomial_optima(puntos, M);

    // Mostrar los índices de los puntos seleccionados
    std::cout << "Índices óptimos seleccionados para los segmentos: ";
    for (int indice : indices_optimos) {
        std::cout << indice << " ";
    }
    std::cout << std::endl;

    // Mostrar los puntos correspondientes
    std::cout << "Puntos seleccionados: " << std::endl;
    for (int indice : indices_optimos) {
        const Punto& p = puntos[indice];
        std::cout << "(" << p.x() << ", " << p.y() << ")" << std::endl;
    }

    return 0;
}
