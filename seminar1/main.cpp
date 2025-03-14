//
// Created by lysko on 13.03.2025.
//
#include "matrix.h"
#include <iostream>

int main() {
    using Complex = std::complex<double>;
    try {
        constexpr int32_t curRow = 2;
        constexpr int32_t curCol = 2;
        const Matrix A(curRow, curCol);
        A[0][0] = Complex(1, 1);
        A[0][1] = Complex(2, 2);
        A[1][0] = Complex(3, 3);
        A[1][1] = Complex(4, 4);

        Matrix B(curRow, curCol);
        B[0][0] = Complex(5, 5);
        B[0][1] = Complex(6, 6);
        B[1][0] = Complex(7, 7);
        B[1][1] = Complex(8, 8);

        std::cout << "\nMatrix A:\n" << A << std::endl;
        std::cout << "Matrix B:\n" << B << std::endl;

        if (A.getRows() == B.getRows() && A.getCols() == B.getCols()) {
            Matrix sum = A + B;
            std::cout << "A + B:\n" << sum << std::endl;
        } else {
            std::cout << "A and B have different sizes, cannot do (A + B)\n";
        }

        // Пример: умножение на число (комплекс)
        Complex scalar(2.0, 3.0); // (2 + 3i)
        Matrix Scaled = A * scalar;
        std::cout << "A * (2 + 3i):\n" << Scaled << std::endl;

        // Пример: детерминант (для квадратной матрицы)
        if (A.getRows() == A.getCols()) {
            auto detA = A.det();
            std::cout << "det(A) = " << detA << " (real=" << detA.real()
            << ", imag=" << detA.imag() << ")\n";
        } else {
            std::cout << "Matrix A is not square, skipping det(A)\n";
        }

        // Пример: унарный минус
        Matrix negA = -A;
        std::cout << "-A:\n" << negA << std::endl;

        // Пример: clone() — глубокая копия
        Matrix Aclone = A.clone();
        std::cout << "A.clone():\n" << Aclone << std::endl;

        // Пример: перемещение
        Matrix C(0,0);
        C = std::move(Aclone); // Aclone теперь «пустая», а C получил её данные
        std::cout << "Matrix C (moved from Aclone):\n" << C << std::endl;

        // Aclone теперь опустела, посмотрим размеры
        std::cout << "Aclone rows: " << Aclone.getRows()
        << ", cols: " << Aclone.getCols() << "\n";

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }
    Matrix A(2, 2);
    A[0][0] = Complex(1.0, 0.0);
    A[0][1] = Complex(2.0, 0.0);
    A[1][0] = Complex(4.0, 0.0);
    A[1][1] = Complex(5.0, 0.0);

    Matrix b(2, 1);
    b[0][0] = Complex(3.0, 0.0);
    b[1][0] = Complex(6.0, 0.0);

    try {
        Matrix x = A.solveSystem(b);
        std::cout << "Solution x:\n" << x << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
    }
    return 0;
}