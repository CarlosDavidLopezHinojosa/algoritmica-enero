#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>

const float inf = std::numeric_limits<float>::infinity();

struct Material
{
    Material(float volumen, float precio)
        : volumen_(volumen), precio_(precio) {}

    float volumen() const { return volumen_; }
    float precio() const { return precio_; }
    float valor() const { return volumen_ * precio_; }

private:
    float volumen_;
    float precio_;
};

using Materiales = std::vector<Material>;

// Función para calcular un límite superior
float limite(const Materiales::const_iterator inicio, 
             const Materiales::const_iterator fin, 
             float volumen_maximo)
{
    float volumen_total = 0.0f;
    float valor_total = 0.0f;

    for (Materiales::const_iterator it = inicio; it != fin; it++)
    {
        if (volumen_total + it->volumen() <= volumen_maximo)
        {
            volumen_total += it->volumen();
            valor_total += it->valor();
        }
        else
        {
            // Parte fraccional del último material
            float espacio_restante = volumen_maximo - volumen_total;
            valor_total += it->precio() * espacio_restante;
            return valor_total;
        }
    }

    return valor_total;
}

// Algoritmo de búsqueda recursiva
void mochila(float volumen_maximo, const Materiales &materiales,
             Materiales actuales, float valor_actual, float &mejor_valor,
             Materiales &solucion, size_t indice)
{
    // Caso base: no hay más materiales o el volumen se agotó
    if (indice >= materiales.size() || volumen_maximo <= 0)
    {
        if (valor_actual > mejor_valor)
        {
            mejor_valor = valor_actual;
            solucion = actuales;
        }
        return;
    }

    // Poda por límite superior
    float valor_limite = limite(materiales.cbegin() + indice, materiales.cend(), volumen_maximo);
    if (valor_actual + valor_limite <= mejor_valor)
        return;

    // Opciones: incluir o excluir el material actual
    const Material &m = materiales[indice];

    // Incluir el material actual si cabe
    if (m.volumen() <= volumen_maximo)
    {
        actuales.push_back(m);
        mochila(volumen_maximo - m.volumen(), materiales, actuales,
                valor_actual + m.valor(), mejor_valor, solucion, indice + 1);
        actuales.pop_back();
    }

    // Excluir el material actual
    mochila(volumen_maximo, materiales, actuales, valor_actual, mejor_valor, solucion, indice + 1);
}

// Función para imprimir la solución
void imprimir_solucion(const Materiales &solucion)
{
    for (const auto &m : solucion)
    {
        std::cout << "Volumen: " << m.volumen() << ", Precio: " << m.precio() << std::endl;
    }
}

int main()
{
    Materiales materiales = 
    {
        Material(20, 5),
        Material(30, 4),
        Material(40, 3),
        Material(50, 2.5),
        Material(60, 1)
    };

    float volumen_maximo = 100;
    Materiales solucion;
    float mejor_valor = 0;

    mochila(volumen_maximo, materiales, {}, 0, mejor_valor, solucion, 0);

    std::cout << "Mejor valor: " << mejor_valor << std::endl;
    std::cout << "Materiales en la solución:" << std::endl;
    imprimir_solucion(solucion);

    return 0;
}
