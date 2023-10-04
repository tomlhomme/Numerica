#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <iostream>
#include <memory>
#include <initializer_list>
#include "numerica_constants.hpp"

#if NUMERICA_RUNTIME_CHECKS_OFF
#define NDEBUG
#endif
#include <cassert>

namespace Numerica
{
    class Array
    {
    public:
        Array(std::initializer_list<std::size_t> shape);
        ~Array();

        void print_shape() const;
        std::size_t axis_size(size_t axis) const;
        std::size_t num_axes() const;
    private:
        std::size_t numAxis;
        std::size_t* mShape;
        std::size_t mSize;
    };

    class Dense: public Array
    {
    public:
        Dense(std::initializer_list<std::size_t> shape);
        ~Dense();

        void print_raw() const;
        virtual void print() const;

        virtual double& operator() (std::initializer_list<std::size_t> index_list) const;
        void operator= (const Dense& arr);
    protected:
        double* mData;
    private:
        std::size_t mSize;
    };

    class Vec1d: public Dense
    {
    public:
        Vec1d(std::size_t len);
        ~Vec1d();

        virtual void print() const;

        double& operator() (const std::size_t ind) const;
        virtual double& operator() (std::initializer_list<std::size_t> index_list) const;
    };

    class Vec2d: public Dense
    {
    public:
        Vec2d(std::size_t l1, std::size_t l2);
        ~Vec2d();

        double& operator() (const std::size_t ind1, const std::size_t ind2) const;
        virtual double& operator() (std::initializer_list<std::size_t> index_list) const;

        virtual void print() const;
    };

    class Vec3d: public Dense
    {
    public:
        Vec3d(std::size_t l1, std::size_t l2, std::size_t l3);
        ~Vec3d();

        double& operator() (const std::size_t ind1, const std::size_t ind2, const std::size_t ind3) const;
        virtual double& operator() (std::initializer_list<std::size_t> index_list) const;

        virtual void print() const;
    }; 
}

#endif
