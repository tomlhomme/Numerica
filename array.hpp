#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <iostream>
#include <memory>
#include <initializer_list>
#include <math.h>
#include <lapacke.h>
#include "numerica_constants.hpp"
#include <cblas.h>

#if NUMERICA_RUNTIME_CHECKS_OFF
#define NDEBUG
#endif
#include <cassert>

namespace Numerica
{
    class Array
    {
    public:
        Array();
        Array(std::initializer_list<std::size_t> shape);
        ~Array();

        void print_shape() const;
        std::size_t axis_size(std::size_t axis) const;
        std::size_t num_axes() const;
    protected:
        std::size_t mSize;
    private:
        std::size_t numAxis;
        std::size_t* mShape=NULL;
    };

    class Dense: public Array
    {
    public:
        Dense();
        Dense(std::initializer_list<std::size_t> shape);
        ~Dense();

        void print_raw() const;
        virtual void print() const;

        virtual double& operator() (std::initializer_list<std::size_t> index_list) const;
        //void operator= (const Dense& arr);

        double*& data_ptr();
        double* mDummyData=NULL;
    protected:
        double* mData=NULL;
    };

    class Vec1d: public Dense
    {
    public:
        Vec1d();
        Vec1d(std::size_t len);
        ~Vec1d();

        virtual void print() const;

        double& operator() (const std::size_t ind) {return mData[ind];}
        const double& operator() (const std::size_t ind) const {return mData[ind];}
        
        virtual double& operator() (std::initializer_list<std::size_t> index_list) const;
    };

    class Vec2d: public Dense
    {
    public:
        Vec2d();
        Vec2d(std::size_t l1, std::size_t l2);
        ~Vec2d();

        //virtual inline std::size_t axis_size(std::size_t axis) const;

        double& operator() (const std::size_t ind1, const std::size_t ind2) { return mData[ind1*mSz1 + ind2]; }
        const double& operator() (const std::size_t ind1, const std::size_t ind2) const { return mData[ind1*mSz1 + ind2]; }
        
        //double& operator() (const int ind1, const int ind2) const;
        inline virtual double& operator() (std::initializer_list<std::size_t> index_list) const;
        //inline double& operator() (const int ind1, const int ind2) const;

        void multiply(Vec1d& out, Vec1d& in);
        void multiply(Vec2d& out, Vec2d& in);

        virtual void print() const;

        const std::size_t mSz0;
        const std::size_t mSz1;
    };

    class Vec3d: public Dense
    {
    public:
        Vec3d();
        Vec3d(std::size_t l1, std::size_t l2, std::size_t l3);
        ~Vec3d();

        double& operator() (const std::size_t ind1, const std::size_t ind2, const std::size_t ind3) const {return mData[ind1*mSz1Sz2 + ind2*mSz2 + ind3];}
        virtual double& operator() (std::initializer_list<std::size_t> index_list) const;

        virtual void print() const;

        const std::size_t mSz0;
        const std::size_t mSz1;
        const std::size_t mSz2;
        const std::size_t mSz1Sz2;
    }; 

    class Square: public Vec2d
    {
    public:
        Square(size_t size);
        ~Square();
        void solve_A_x_eq_b(Vec1d& result, Vec1d& rhs);
    private:
        int* ipiv;
    };

    class Banded: public Array
    {
    public:
        Banded(std::size_t sz, std::size_t nbands_up, std::size_t nbands_down);
        ~Banded();

        double& operator() (std::size_t row, std::size_t col) const;

        std::size_t bands_up();
        std::size_t bands_down();

        void solve_Ax_eq_b(Vec1d& result, Vec1d& rhs);

        void print_raw() const;
        void print_compact() const;
        void print() const;
    protected:
        double* mData=NULL;
    private:
        std::size_t mBandsUp;
        std::size_t mBandsDown;
        double* mDummyData=NULL;
        int* ipiv=NULL;
    };

}

#endif
