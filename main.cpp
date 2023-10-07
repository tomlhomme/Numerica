#include "numerica.hpp"

int main()
{
    /*
    size_t N=3;
    size_t N2=5;
    size_t N3=7;
    Numerica::Vec2d A(N,N2);
    Numerica::Vec2d res(N, N3);
    Numerica::Vec2d in(N2,N3);

    for (size_t i = 0;i< A.axis_size(0); i++)
    {
        for (size_t j=0;j<A.axis_size(1);j++)
        {
            if (i==j)
            {
                A(i,j)=2;
            }
            else{
                A(i,j)=1;
            }
        }
    }
    for (size_t i = 0;i< in.axis_size(0); i++)
    {
        for (size_t j=0;j<in.axis_size(1);j++)
        {
            in(i,j)=j+i*in.axis_size(1);
        }
    }

    A.print();
    in.print();
    A.multiply(res,in);
    res.print();
    */
    
    Numerica::Banded A(5,3,1);
    Numerica::Vec1d res(5);
    Numerica::Vec1d rhs(5);

    std::cout << res.axis_size(0) << " " << rhs.axis_size(0) << std::endl;
    for (std::size_t i=0; i<A.axis_size(0); i++)
    {
        for (std::size_t j=0; j<A.axis_size(1); j++)
        {
            if (i+A.bands_up()>=j && i <=j+A.bands_down())
            {
                if (i==j){
                    A(i,j)=2;
                    std::cout << i << " " << j <<" "<< A(i,j) <<std::endl;
                } else {
                    A(i,j)=1;
                }
            }
            else {
                std::cout << i << " " << j << " no" <<std::endl;
            }
            std::cout << res.axis_size(0) << " " << rhs.axis_size(0) << std::endl;
        }
        rhs(i)=i;
    }

    A.print_raw();
    A.print_compact();
    A.print();

    std::cout << res.axis_size(0) << " " << rhs.axis_size(0) << std::endl;

    A.solve_Ax_eq_b(res,rhs);
    res.print();
    
    //a.print();
    //b=a;
    //b.print();
    //b.print_raw();

    
}