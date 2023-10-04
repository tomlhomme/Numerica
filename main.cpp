#include "numerica.hpp"

int main()
{
    Numerica::Vec2d a({10,9});
    a.print_shape();

    Numerica::Dense b({10,9});
    Numerica::Dense c({10,9});

    for (std::size_t i=0; i<a.axis_size(0);i++)
    {
        for (std::size_t j=0; j<a.axis_size(1);j++)
        {
            a({i,j}) = j+a.axis_size(1)*i;
        }
    }

    b=a;

    c.print();





    //b=a;
    //b.print();
    //b.print_raw();

    
}