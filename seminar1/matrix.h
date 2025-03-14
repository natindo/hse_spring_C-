//
// Created by lysko on 13.03.2025.
//

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <complex>
#include <cstdint>

class Matrix {
    using Complex = std::complex<double>;

    class ProxyRow {
    public:
        ProxyRow();
        ProxyRow(Complex* row_ptr, int32_t cols);

        Complex& operator[](int32_t col) const;

    private:
        Complex* row_ptr_ = nullptr;
        int32_t cols_ = 0;
    };

public:
    Matrix() = delete;
    Matrix(const Matrix&) = delete;
    Matrix& operator=(const Matrix&) = delete;

    Matrix(Matrix&& rhs) noexcept;
    Matrix& operator=(Matrix&& other) noexcept;

    Matrix (int32_t rows, int32_t cols);
    ~Matrix();

    [[nodiscard]] Matrix clone() const;
    ProxyRow& operator[](int32_t row) const;
    [[nodiscard]] Complex& at(int32_t row, int32_t col) const;

    Matrix& operator -= (const Complex& val);
    Matrix& operator -= (const Matrix& rhs);
    Matrix operator - (const Complex& val) const;
    Matrix operator - (const Matrix& rhs) const;

    Matrix& operator += (const Complex& val);
    Matrix& operator += (const Matrix& rhs);
    Matrix operator + (const Complex& val) const;
    Matrix operator + (const Matrix& rhs) const;

    Matrix operator * (const Complex& val) const;
    Matrix operator * (const Matrix& rhs) const;

    Matrix operator / (const Complex& val);

    Matrix operator - () const;

    [[nodiscard]] int32_t getRows() const;
    [[nodiscard]] int32_t getCols() const;

    [[nodiscard]] Complex det() const;

    [[nodiscard]] Matrix solveSystem(const Matrix& b) const;

    friend std::ostream& operator<<(std::ostream& out, const Matrix& m);

private:
    void checkSameSize(const Matrix& rhs) const;
    void checkBounds( int32_t row, int32_t col) const;
    static void swapRows(const Matrix& mat, int32_t r1, int32_t r2);
    static Complex onlyPositiveZeros(const Complex& mat);

private:
    int32_t rows_ = 0;
    int32_t cols_ = 0;
    Complex* data_ = nullptr;
    ProxyRow* row_proxies_ = nullptr;
};


#endif //MATRIX_H
