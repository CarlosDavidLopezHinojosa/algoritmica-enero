#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

void vector_rep(const std::vector<int> &vec)
{
    assert(!vec.empty());
    std::cout << "[";
    for (size_t i = 0; i < vec.size(); i++)
        std::cout << " " << vec.at(i);
    std::cout << " ]";
}

void preparar_tareas(std::vector<int> &plazos, std::vector<int> &beneficios)
{
    assert(plazos.size() == beneficios.size());


    if (std::is_sorted(beneficios.begin(),beneficios.end(), std::greater<int>()))
        return;
    
    
    const size_t n = plazos.size();
    std::vector<std::pair<int,int>> pb(n);


    for (size_t i = 0; i < n; i++)
        pb.at(i) = {plazos.at(i), beneficios.at(i)};

    auto sort_profit = 
    [](const std::pair<int,int> &a, const std::pair<int,int> &b)
    { 
        return a.second > b.second;
    };

    std::sort(pb.begin(),pb.end(),sort_profit);

    for (size_t i = 0; i < n; i++)
        std::tie(plazos.at(i), beneficios.at(i)) = pb.at(i);
    
    
    
    assert(std::is_sorted(beneficios.begin(),beneficios.end(), std::greater<int>()));  

}

void tareas_a_plazo(const std::vector<int> &plazos, 
                    std::vector<int> &secuencia)
{
    assert(secuencia.empty());

    std::vector<int> p = plazos;
    std::vector<int> &s = secuencia;

    int curr = 1;
    int last = 0;
    const size_t n = plazos.size(); // O tambien beneficios.size()

    // Colocamos el centinela
    p.insert(p.begin(),0);

    // La primera tarea siempre se pone
    s.assign(n + 1, 0);
    s.at(1) = 1;
    

    for (size_t i = 2; i <= n; i++) // Orden decreciente de los beneficios (Beneficios de mayor a menor)
    {
        last = curr;
               // Si la tarea tiene un plazo anterior y si no es el centinela
        while (p.at(s.at(last)) > p.at(i) && last > 0)
            last--;
           // Si se ha encontrado la posición de inserción
        if(p.at(s.at(last)) <= p.at(i) && p.at(i) > last)
        {
            for (size_t j = curr; j >= last + 1; j--)
                s.at(j + 1) = s.at(j);

            s.at(last + 1) = i;

            curr++; 
        }
    }

    s.resize(curr + 1);
    s.erase(s.begin());

    for (size_t i = 0; i < s.size(); i++) 
        s.at(i)--;
    
}

bool es_factible(const std::vector<int> &secuencia, 
                 const std::vector<int> &plazos)
{
    std::vector<int> plazos_secuencia(secuencia.size());

    for (size_t i = 0; i < secuencia.size(); i++)
        plazos_secuencia.at(i) = -plazos.at(secuencia.at(i));

    auto s = secuencia;
    preparar_tareas(s,plazos_secuencia);


    for (size_t i = 0; i < s.size(); i++)
        if(s.at(i) > i)
            return false;

    return true;
}

int main()
{   
    std::vector<int> p = {3, 1, 1, 3, 1, 3};
    std::vector<int> b = {20, 15, 10, 7, 5, 3};
    std::vector<int> s;
    preparar_tareas(p,b);

    vector_rep(p);
    vector_rep(b);

    tareas_a_plazo(p,s);

    vector_rep(s);

    std::cout << es_factible(s,p);

    // std::vector<int> v;
    // beneficios_secuencia(s,v);

    // vector_rep(v);

    // tareas_a_plazo(p,b,s);
    return 0;
}