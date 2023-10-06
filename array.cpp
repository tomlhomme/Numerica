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
    delete[] mShape;
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
    delete[] mData;
    delete[] mDummyData;
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
    std::cout << std::endl;
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

void Numerica::Vec2d::multiply(Vec1d& out, Vec1d& in)
{
    assert(axis_size(1)==in.axis_size(0) && 
        "Axis 1 of Matrix and 0 of in vector must match.");
    assert(axis_size(0)==out.axis_size(0) && 
        "Axis 0 of Matrix and 0 of out vector must match.");
    /*
    if (in.mDummyData==NULL)
        in.mDummyData = new double[axis_size(1)];
    if (out.mDummyData==NULL)
        out.mDummyData = new double[axis_size(0)];

    for (std::size_t i=0;i<in.axis_size(0);i++)
    {
        in.mDummyData[i]=in(i);
    }
    cblas_dgemv(CblasRowMajor, CblasNoTrans, axis_size(0),axis_size(1), 1.,
        mData,axis_size(1),in.mDummyData,1, 0., out.mDummyData,1);
    
    for (std::size_t i=0;i<in.axis_size(1);i++)
    {
        out(i)=out.mDummyData[i];
    }
    */

   for (std::size_t i=0; i<axis_size(0); i++)
   {
        out(i)=0;
        for (std::size_t j=0; j<axis_size(1); j++)
        {
            out(i)+=(*this)(i,j) * in(j);
            //std::cout << i << " " << j << std::endl;
        }
   }
}

void Numerica::Vec2d::multiply(Vec2d& out, Vec2d& in)
{
    assert(axis_size(1)==in.axis_size(0) && 
        "Axis 1 of Matrix and 0 of in vector must match.");
    assert(in.axis_size(1)==out.axis_size(1) && 
        "Axis 0 of Matrix and 0 of out vector must match.");
    assert(axis_size(0)==out.axis_size(0) && 
        "Axis 0 of Matrix and 0 of out vector must match.");

    for (std::size_t i=0; i<out.axis_size(0); i++)
    {
        for (std::size_t j=0; j<out.axis_size(1); j++)
        {
            out(i,j)=0;
            for (std::size_t k=0; k<in.axis_size(0);k++)
            {
                out(i,j)+=(*this)(i,k)*in(k,j);
            }
        }
    }
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

Numerica::Square::Square(size_t size): Vec2d(size,size)
{
}

Numerica::Square::~Square()
{
    delete[] ipiv;
}

void Numerica::Square::solve_A_x_eq_b(Vec1d& result, Vec1d& rhs)
{
    assert(result.axis_size(0)==rhs.axis_size(0) && 
        "Result and rhs must be the same size.");
    assert(axis_size(0)==result.axis_size(0) &&
        "Result and matrix must have same first dimension.");

    if (ipiv==NULL)
        ipiv = new int[axis_size(0)];
    
    if (mDummyData==NULL)
        mDummyData = new double[axis_size(0)*axis_size(1)];
    
    if (rhs.mDummyData==NULL)
        rhs.mDummyData = new double[axis_size(0)];

    for (size_t i=0;i<axis_size(0);i++)
    {
        for (size_t j=0;j<axis_size(1);j++)
        {
            mDummyData[j+axis_size(0)*i]=mData[j+axis_size(0)*i];
        }
    }
    for (size_t i=0; i<rhs.axis_size(0); i++)
    {
        rhs.mDummyData[i]=rhs(i);
    }

    int info = LAPACKE_dgesv(LAPACK_ROW_MAJOR,axis_size(0),1,mDummyData,
        axis_size(0),ipiv,rhs.mDummyData,1);
    
    if (info!=0)
        std::cout << "LAPACKE error: " << info << std::endl;
    for (size_t i=0; i<rhs.axis_size(0); i++)
    {
        std::cout<< rhs.mDummyData[i] << std::endl;
        result(i)=rhs.mDummyData[i];
    }
}

Numerica::Banded::Banded(std::size_t sz, std::size_t nbands_up, std::size_t nbands_down): Array({sz,sz})
{
    mSize=sz*(2*nbands_down+nbands_up+2);
    mData = new double[mSize];
    mBandsUp=nbands_up;
    mBandsDown=nbands_down;

    for (std::size_t i=0; i<mSize; i++)
    {
        mData[i]=0;
    }
}

Numerica::Banded::~Banded()
{
    delete[] mData;
    delete[] ipiv;
}

double& Numerica::Banded::operator() (std::size_t row, std::size_t col) const
{
    assert(row<axis_size(0) && col<axis_size(1) &&
        "Row and column must be within matrix.");
    assert(row+mBandsUp>=col && row <=col+mBandsDown &&
        "Row and col values must be within a band.");
    std::size_t idx=axis_size(0)*(mBandsDown + mBandsUp + row - col) + col;
    std::cout << "("<< idx <<")" <<"|" <<mSize<<"|";
    return mData[idx];
}

std::size_t Numerica::Banded::bands_up()
{
    return mBandsUp;
}

std::size_t Numerica::Banded::bands_down()
{
    return mBandsDown;
}

void Numerica::Banded::solve_Ax_eq_b(Vec1d& result, Vec1d& rhs)
{
    assert(result.axis_size(0)==rhs.axis_size(0) && 
        "Result and rhs must be the same size.");
    assert(axis_size(0)==result.axis_size(0) &&
        "Result and matrix must have same first dimension.");

    if (ipiv==NULL)
        ipiv = new int[axis_size(0)];
    
    if (mDummyData==NULL)
        mDummyData = new double[mSize];
    
    if (rhs.mDummyData==NULL)
        rhs.mDummyData = new double[axis_size(0)];

    for (size_t i=0;i<mSize;i++)
    {
        mDummyData[i]=mData[i];
    }
    for (size_t i=0; i<rhs.axis_size(0); i++)
    {
        rhs.mDummyData[i]=rhs(i);
    }

    //std::cout << mData << " " << mDummyData << " " << ipiv << " " << rhs.mDummyData << std::endl;

    int info = LAPACKE_dgbsv(LAPACK_ROW_MAJOR,axis_size(0),mBandsDown,mBandsUp,1,mData,axis_size(0),ipiv,rhs.mDummyData,1);
    
    if (info!=0)
        std::cout << "LAPACKE error: " << info << std::endl;
    for (size_t i=0; i<rhs.axis_size(0); i++)
    {
        std::cout<< rhs.mDummyData[i] << std::endl;
        result(i)=rhs.mDummyData[i];
    }

}

void Numerica::Banded::print_raw() const
{
    for (std::size_t i=0; i<mSize; i++)
    {
        std::cout << mData[i] << " ";
    }
    std::cout << std::endl;
}

void Numerica::Banded::print_compact() const
{
    for (std::size_t j=0; j<mBandsDown+mBandsUp+1; j++)
    {
        for (std::size_t i=0; i<axis_size(1); i++)
        {
            std::cout << mData[j+i*(mBandsDown+mBandsUp+1)] << " ";
        }
        std::cout << std::endl;
    }
}

void Numerica::Banded::print() const
{
    for (std::size_t i=0; i<axis_size(0); i++)
    {
        for (std::size_t j=0; j<axis_size(1); j++)
        {
            if (i+mBandsUp>=j && i <=j+mBandsDown)
            {
                std::cout << (*this)(i,j) << " ";
            } else {
                std::cout << 0 << " ";
            }
        }
        std::cout << std::endl;
    }
}


