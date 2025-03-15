//
// Created by lysko on 13.03.2025.
//

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <complex>
#include <cstdint>
#include <fcntl.h>
#include <sys/mman.h>   // Для mmap(), munmap()
#include <sys/stat.h>   // Для fstat()
#include <unistd.h>     // Для close()
#include <cstring>      // Для memcpy()

class Matrix {
    using Complex = std::complex<double>;

    class ProxyRow {
    public:
        ProxyRow();
        explicit ProxyRow(int32_t cols_count);

        ProxyRow(const ProxyRow&) = delete;
        ProxyRow& operator=(const ProxyRow&) = delete;

        ProxyRow(ProxyRow&&) noexcept;
        ProxyRow& operator=(ProxyRow&&) noexcept;

        Complex& operator[](int32_t col) const;

        ~ProxyRow();

        int32_t getColsNumber() const;

    private:
        Complex* data_ = nullptr;
        int32_t cols_number_ = 0;
    };

public:
    // ----------- copy not approved ----------- //
    Matrix() = delete;
    Matrix(const Matrix&) = delete;
    Matrix& operator=(const Matrix&) = delete;

    // ----------- move ----------- //
    Matrix(Matrix&& rhs) noexcept;
    Matrix& operator=(Matrix&& other) noexcept;

    // ----------- constr ----------- //
    Matrix (int32_t rows, int32_t cols);
    Matrix (const std::string& filename);

    ~Matrix();

    // ----------- accessing elements ----------- //
    [[nodiscard]] ProxyRow& operator[](int32_t row) const;
    [[nodiscard]] Complex& at(int32_t row, int32_t col) const;

    // ----------- utilities ----------- //
    [[nodiscard]] Matrix clone() const;
    [[nodiscard]] int32_t getRows() const;
    [[nodiscard]] int32_t getCols() const;

    // ----------- operators ----------- //
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

    // ----------- math calc ----------- //
    [[nodiscard]] Complex det() const; //determinant
    [[nodiscard]] Matrix solveSystem(const Matrix& b) const; // solve system of linear equations

    friend std::ostream& operator<<(std::ostream& out, const Matrix& m);

private:
    static Complex onlyPositiveZeros(const Complex& mat);

    void checkSameSize(const Matrix& rhs) const;
    void checkBounds(int32_t row, int32_t col) const;
    static void swapRows(const Matrix& mat, int32_t r1, int32_t r2);

private:
    int32_t rows_ = 0;
    int32_t cols_ = 0;
    ProxyRow* proxy_rows_ = nullptr;

    int fd = -1;
    std::size_t fileSize = 0;
    void* mapPtr = nullptr;
};


#endif //MATRIX_H
