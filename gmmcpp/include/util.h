#pragma once

#include <vector>
#include <stdexcept>
#include <algorithm>

// Elementwise vector addition operator
template <typename T>
std::vector<T> operator+(const std::vector<T> &x, 
                         const std::vector<T> &y) 
{
    std::vector<T> r(x.size());
    std::transform(x.begin(), x.end(),
                   y.begin(),
                   r.begin(),
                   std::plus<T>{});
    return r;
}

// Elementwise vector multiplication operator
template <typename T>
std::vector<T> operator*(const std::vector<T> &x, 
                         const std::vector<T> &y) 
{
    std::vector<T> r(x.size());
    std::transform(x.begin(), x.end(),
                   y.begin(),
                   r.begin(),
                   std::multiplies<T>{});
    return r;
}

// Elementwise vector multiplication operator
template <typename T>
std::vector<T> operator/(const std::vector<T> &x, 
                         const std::vector<T> &y)
{
    std::vector<T> r(x.size());
    std::transform(x.begin(), x.end(),
                   y.begin(),
                   r.begin(),
                   std::divides<T>{});
    return r;
}

// Vector inner product
template <typename T>
T inner_product(const std::vector<T> &x, 
                const std::vector<T> &y)
{
    std::vector<T> rv(x.size()); // Vector for products
    T r = 0;
    rv = x * y;
    for (auto &z : rv){
        r += z;
    }
    return r;
}

// Normal density
template <typename T>
T dnorm(const T &x, const T &mu, 
        const T &sigma)
{
    static const T inv_sqrt_2pi = 0.3989422804014327;
    T z = (x - mu) / sigma;

    return (inv_sqrt_2pi / sigma) * std::exp(-0.5 * std::pow(z,2));
}