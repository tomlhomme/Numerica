#include "array.hpp"

Numerica::Array::Array(std::initializer_list<std::size_t> shape)
{
    numAxis = shape.size();
    mShape = new size_t[numAxis];
    size_t i=0;
    for (std::size_t dim : shape)
    {
        mShape[i]=dim;
        i+=1;
    }
}

Numerica::Array::~Array()
{
    delete mShape;
}

void Numerica::Array::print_shape() const
{
    for (std::size_t i=0;i<numAxis;i++)
    {
        std::cout << mShape[i] << std::endl;
    }
}

std::size_t Numerica::Array::axis_size(size_t axis) const
{
    return mShape[axis];
}

std::size_t Numerica::Array::num_axes() const
{
    return numAxis;
}

Numerica::Dense::Dense(std::initializer_list<std::size_t> shape) : Array(shape)
{
    mSize=1;
    for (std::size_t i=0;i<num_axes();i++)
    {
        mSize*=axis_size(i);
    }

    assert(mSize<NUMERICA_MAX_ARRAY_SIZE && 
        "Declared array exceeds max allowed size. Check shape values are non-negative.");

    mData = new double[mSize];
}

Numerica::Dense::~Dense()
{
    delete mData;
}

void Numerica::Dense::print_raw() const
{
    for (std::size_t i=0; i<mSize; i++)
    {
        std::cout << mData[i] << " ";
    }
}

void Numerica::Dense::print() const
{
    print_raw();
}

double& Numerica::Dense::operator() (std::initializer_list<std::size_t> index_list) const
{
    std::size_t idx=0;
    std::size_t chunk_size=1;
    std::size_t num_ax=num_axes();

    assert(index_list.size()==num_axes() &&
        "Number of axes must equal number of axes of Dense object.");

    assert(index_list.size()==num_axes() &&
        "Index size does not match array size.");

    for (std::size_t rev_i=0; rev_i<num_ax;rev_i++)
    {
        std::size_t i = num_ax-rev_i-1;

        assert(*(index_list.begin() + i)<axis_size(i) && 
            "Index out of bounds.");

        idx+= *(index_list.begin() + i) *chunk_size;
        chunk_size*=axis_size(i);
    }
    return mData[idx];
}

void Numerica::Dense::operator= (const Dense& arr)
{
    assert(num_axes()==arr.num_axes() && 
        "Number of axes must be equal in assignment.");

    for (std::size_t i=0;i<arr.num_axes();i++)
    {
        assert(axis_size(i)==arr.axis_size(i) &&
            "Shapes of Dense arrays must match on assignment.");
    }

    for (std::size_t i=0; i< mSize; i++)
    {
        mData[i]=arr.mData[i];
    }
}

Numerica::Vec1d::Vec1d(std::size_t len) : Dense({len})
{
}

Numerica::Vec1d::~Vec1d()
{
}

void Numerica::Vec1d::print() const
{
    print_raw();
}

double& Numerica::Vec1d::operator() (const std::size_t ind) const
{
    assert(ind<axis_size(0) &&
        "Index out of bounds.");

    return mData[ind];
}

double& Numerica::Vec1d::operator() (std::initializer_list<std::size_t> index_list) const
{
    assert(index_list.size()==num_axes() &&
        "Number of axes must be 1.");
    assert(*(index_list.begin())<axis_size(0) &&
        "Index out of bounds.");

    return mData[*(index_list.begin())];
}

Numerica::Vec2d::Vec2d(std::size_t l1, std::size_t l2): Dense({l1,l2})
{
}

Numerica::Vec2d::~Vec2d()
{
}

double& Numerica::Vec2d::operator() (const std::size_t ind1, const std::size_t ind2) const
{
    assert(ind1<axis_size(0) && ind2<axis_size(1) &&
        "Index out of bounds.");

    return mData[ind1*axis_size(1) + ind2];
}

double& Numerica::Vec2d::operator() (std::initializer_list<std::size_t> index_list) const
{
    assert(index_list.size()==num_axes() &&
        "Number of axes must be 2.");

    assert(*(index_list.begin())<axis_size(0) && *(index_list.begin()+1)<axis_size(1) &&
        "Index out of bounds.");

    return mData[*(index_list.begin())*axis_size(1) + *(index_list.begin()+1)];
}

void Numerica::Vec2d::print() const
{
    for (std::size_t i=0; i<axis_size(0); i++)
    {
        for (std::size_t j=0; j<axis_size(1); j++)
        {
            std::cout << mData[j+axis_size(1)*i] << " ";
        }
        std::cout << std::endl;
    }
}

Numerica::Vec3d::Vec3d(std::size_t l1, std::size_t l2, std::size_t l3): Dense({l1,l2, l3})
{
}

Numerica::Vec3d::~Vec3d()
{
}

double& Numerica::Vec3d::operator() (const std::size_t ind1, const std::size_t ind2, const std::size_t ind3) const
{
    assert(ind1<axis_size(0) && ind2<axis_size(1) && ind3<axis_size(3) &&
        "Index out of bounds.");

    return mData[ind1*axis_size(1)*axis_size(2) + ind2*axis_size(2) + ind3];
}

double& Numerica::Vec3d::operator() (std::initializer_list<std::size_t> index_list) const
{
    assert(index_list.size()==num_axes() &&
        "Number of axes must be 3.");

    assert(*(index_list.begin())<axis_size(0) && *(index_list.begin()+1)<axis_size(1) && *(index_list.begin()+2)<axis_size(3) &&
        "Index out of bounds.");

    return mData[*(index_list.begin())*axis_size(1)*axis_size(2) + *(index_list.begin()+1)*axis_size(2) + 
        *(index_list.begin()+2)];
}

void Numerica::Vec3d::print() const
{
    for (std::size_t i=0;i<axis_size(0);i++)
    {
        for (std::size_t j=0; j<axis_size(1); j++)
        {
            for (std::size_t k=0; k<axis_size(2); k++)
            {
                std::cout << (*this)(i,j,k) << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}