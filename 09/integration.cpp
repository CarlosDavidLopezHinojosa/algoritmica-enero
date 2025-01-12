#include <iostream>
#include <vector>
#include <functional>
#include <random>

using Extremos = std::vector<std::pair<float, float>>;

float uniforme(const std::pair<float, float> &extremos)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(extremos.first, extremos.second);
    return dis(gen);
}

std::vector<float> generar_punto(const Extremos &extremos)
{
    std::vector<float> punto;
    for (const auto &ext : extremos)
    {
        punto.push_back(uniforme(ext));
    }
    return punto;
}

float integral_monte_carlo_multivariable(std::function<float(const std::vector<float>&)> f, const Extremos &extremos, const int n)
{
    float sum = 0;
    for (int i = 0; i < n; i++)
    {
        std::vector<float> punto = generar_punto(extremos);
        sum += f(punto);
    }

    float volumen = 1;
    for (const auto &ext : extremos)
    {
        volumen *= (ext.second - ext.first);
    }

    return volumen * sum / n;
}

int main()
{
    // Definir una función multivariable, por ejemplo, f(x, y) = x^2 + y^2
    auto f = [](const std::vector<float> &punto) -> float {
        return punto[0] * punto[0] + punto[1] * punto[1];
    };

    // Definir los extremos para cada variable, por ejemplo, [0, 1] para x y [0, 1] para y
    Extremos extremos = {{0, 1}, {0, 1}};

    // Número de muestras
    int n = 100000; // 100^2 -> 100^(n_variables)

    // Calcular la integral usando Monte Carlo
    float resultado = integral_monte_carlo_multivariable(f, extremos, n);

    std::cout << "Resultado de la integral: " << resultado << std::endl;

    return 0;
}