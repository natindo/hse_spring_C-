//
// Created by lysko on 13.03.2025.
//
#include "matrix.h"
#include <iostream>

int main() {
    using Complex = std::complex<double>;

    int n = 0;
    std::cout << "1 - Enter matrix elements, 2 - Enter file name: ";
    std::cin >> n;

    Matrix* matrix = nullptr;

    switch (n) {
    case 1: {
        int32_t cols = 0;
        int32_t rows = 0;

        std::cout << "Enter number of columns: ";
        std::cin >> cols;
        std::cout << "Enter number of rows: ";
        std::cin >> rows;
        matrix = new Matrix (rows, cols);
        std::cout << "Enter matrix elements: ";
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int real, imag;
                std::cin >> real >> imag;
                (*matrix)[i][j] = Complex(real, imag);
            }
        }
    }
    break;
    case 2: {
        std::string filename;
        std::cout << "Enter file name: ";
        std::cin >> filename;
        matrix = new Matrix (filename);
        break;
    }
    default:
        break;
    }

    delete matrix;

    return 0;
}