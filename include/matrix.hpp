#pragma once
#include <type_traits>
#include <initializer_list>
#include <algorithm>
#include <utility>
#include <stdexcept>
#include <initializer_list>
#include <string>
#include "my_type_traits.hpp"

namespace matrix {

template <typename T, size_t nrows_, size_t ncols_>
class Matrix;

template <typename T, size_t nrows_, size_t ncols_>
requires (std::is_arithmetic_v<T> && !std::is_reference_v<T> && !std::is_array_v<T> ||
is_Matrix_specialization<T>::value)
class Matrix {

protected: //fields
    T data_[nrows_][ncols_] = {};

public: //types
    using row_type = T[ncols_];
    using value_type = T;

public: //friends
    template <typename T2, size_t nrows2_, size_t ncols2_>
    friend class Matrix;

public: //standart functions
    constexpr Matrix() {}
    consteval Matrix(const std::initializer_list < std::initializer_list<T> > data)
    /*-> Matrix<T, data.size(), (*(data.begin())).size()>*/ { //it has to work like in this line but I don't know because it doesn't work

        if (data.size() != nrows_ || data.begin()->size() != ncols_)
            throw std::length_error("initializer_list size != Matrix size");

        auto iter_i = data.begin();
        for (size_t i = 0; i < nrows_; ++i, ++iter_i) {
            auto iter_j = iter_i->begin();
            for (size_t j = 0; j < ncols_; ++j, ++iter_j)
                data_[i][j] = *iter_j;
        }
    }

    template <typename T2, size_t nrows2_, size_t ncols2_>
    requires std::is_convertible_v<T2, T>
    constexpr Matrix(const Matrix<T2, nrows2_, ncols2_>& other) {
        constexpr size_t nrows = std::min(nrows_, nrows2_);
        constexpr size_t ncols = std::min(ncols_, ncols2_);

        for (size_t i = 0; i < nrows; ++i)
            for (size_t j = 0; j < ncols; ++j)
                data_[i][j] = other.data_[i][j];

    }

    constexpr Matrix(const Matrix&& other) {

        for (size_t i = 0; i < nrows_; ++i)
            for (size_t j = 0; j < ncols_; ++j) {
                data_[i][j] = std::move(other.data_[i][j]);
                if constexpr (std::is_class_v<T>)
                    data_[i][j] = std::move(other.data_[i][j]);

                else {
                    data_[i][j] = other.data_[i][j];
                    if constexpr (std::is_pointer_v<T>)
                        other.data_[i][j] = nullptr;
                    else
                        other.data_[i][j] = 0;
                }
            }

    }

    template <typename T2, size_t nrows2_, size_t ncols2_>
    requires std::is_convertible_v<T2, T>
    constexpr Matrix& operator=(const Matrix<T2, nrows2_, ncols2_>& other) {
        constexpr size_t nrows = std::min(nrows_, nrows2_);
        constexpr size_t ncols = std::min(ncols_, ncols2_);

        for (size_t i = 0; i < nrows_; ++i)
            for (size_t j = 0; j < ncols_; ++j)

                if (i < nrows && ncols < ncols)
                    data_[i][j] = other.data_[i][j];
                else
                    data_[i][j] = 0;

        return *this;
    }

    constexpr Matrix& operator=(Matrix&& other) {

        for (size_t i = 0; i < nrows_; ++i)
            for (size_t j = 0; j < nrows_; ++j)

                if constexpr (std::is_class_v<T>)
                    data_[i][j] = std::move(other.data_[i][j]);

                else {
                    data_[i][j] = other.data_[i][j];
                    if constexpr (std::is_pointer_v<T>)
                        other.data_[i][j] = nullptr;
                    else
                        other.data_[i][j] = 0;
                }

        return *this;
    }

    constexpr ~Matrix() = default;



public: //handing

    constexpr inline row_type& operator[](size_t i) {
        return data_[i];
    }

    constexpr inline const row_type& operator[](size_t i) const {
        return data_[i];
    }

    row_type& at(size_t i) {
        if (i < nrows_)
            return data_[i];

        throw std::out_of_range("out of range in class Matrix");
    }

    const row_type& at(size_t i) const {
        if (i < nrows_)
            return data_[i];

        throw std::out_of_range("out of range in class Matrix");
    }

    T& at(size_t i, size_t j) {
        if (i < nrows_ && j << ncols_)
            return data_[i][j];

        throw std::out_of_range("out of range in class Matrix");
    }

    const T& at(size_t i, size_t j) const {
        if (i < nrows_ && j << ncols_)
            return data_[i][j];

        throw std::out_of_range("out of range in class Matrix");
    }

public: //math operations

    template <typename T2, size_t nrows2_, size_t ncols2_>
    requires std::is_convertible_v<T2, T>
    constexpr Matrix& operator+=(const Matrix<T2, nrows2_, ncols2_>& other) {
        constexpr size_t nrows = std::min(nrows_, nrows2_);
        constexpr size_t ncols = std::min(ncols_, ncols2_);

#ifndef INTRISICS
        for (size_t i = 0; i < nrows; ++i)
            for (size_t j = 0; j < ncols; ++j)
                data_[i][j] += other.data_[i][j];
#else

#endif

    return *this;

    }
    template <typename T2, size_t nrows2_, size_t ncols2_>
    constexpr auto operator+(const Matrix<T2, nrows2_, ncols2_>& other) const
    -> Matrix<std::common_type_t<T, T2>, std::max(nrows_, nrows2_), std::max(ncols_, ncols2_)> {

        constexpr size_t nrows = std::max(nrows_, nrows2_);
        constexpr size_t ncols = std::max(ncols_, ncols2_);

        Matrix<std::common_type_t<T, T2>, ncols, ncols> m;
        m += *this;
        m += other;
    return m;

    }
};
}
