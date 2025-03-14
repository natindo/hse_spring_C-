#include "matrix.h"

using Complex = std::complex<double>;

Matrix::ProxyRow::ProxyRow() = default;

Matrix::ProxyRow::ProxyRow(Complex* row_ptr, const int32_t cols)
        : row_ptr_(row_ptr)
        , cols_(cols)
        {}

Complex& Matrix::ProxyRow::operator[](const int32_t col) const {
    return row_ptr_[col];
}

Matrix::Matrix(Matrix&& rhs) noexcept {
    rows_ = rhs.rows_;
    cols_ = rhs.cols_;
    data_ = rhs.data_;
    row_proxies_ = rhs.row_proxies_;

    rhs.row_proxies_ = nullptr;
    rhs.data_ = nullptr;
    rhs.rows_ = 0;
    rhs.cols_ = 0;
}

Matrix& Matrix::operator=(Matrix&& other) noexcept {
    if (this != &other) {
        delete[] data_;
        delete[] row_proxies_;

        rows_ = other.rows_;
        cols_ = other.cols_;
        data_ = other.data_;
        row_proxies_ = other.row_proxies_;

        other.rows_ = 0;
        other.cols_ = 0;
        other.data_ = nullptr;
        other.row_proxies_ = nullptr;
    }
    return *this;
}

Matrix::Matrix (const int32_t rows, const int32_t cols) : rows_(rows), cols_(cols) {
    if (rows < 0 || cols < 0) {
        throw std::invalid_argument("Matrix dimensions must be positive");
    }
    data_ = new Complex[rows_ * cols_];
    std::fill_n(data_, rows * cols, Complex(0.0, 0.0));

    row_proxies_ = new ProxyRow[rows];
    for (int r = 0; r < rows; ++r) {
        row_proxies_[r] = ProxyRow(data_ + r*cols, cols);
    }
}

Matrix::~Matrix() {
    delete[] data_;
    delete[] row_proxies_;
}

[[nodiscard]] Matrix Matrix::clone() const {
    Matrix m(rows_, cols_);
    for (int32_t i = 0; i < rows_ * cols_; ++i) {
        m.data_[i] = data_[i];
    }
    return m;
}

Matrix::ProxyRow& Matrix::operator[](const int32_t row) const {
    return row_proxies_[row];
}

[[nodiscard]] Complex& Matrix::at(const int32_t row, const int32_t col) const {
    checkBounds(row, col);
    return data_[row * cols_ + col];
}

// ----------- operator + ----------- //
Matrix Matrix::operator + (const Matrix& rhs) const {
    checkSameSize(rhs);
    Matrix result(rows_, cols_);
    for (int i = 0; i < rows_ * cols_; ++i) {
        result.data_[i] = data_[i] + rhs.data_[i];
    }
    return result;
}

Matrix Matrix::operator + (const Complex& val) const {
    Matrix result(rows_, cols_);
    for (int i = 0; i < rows_ * cols_; ++i) {
        result.data_[i] = data_[i] + val;
    }
    return result;
}

Matrix& Matrix::operator += (const Matrix& rhs) {
    checkSameSize(rhs);
    for (int32_t i = 0; i < rows_ * cols_; ++i) {
        data_[i] += rhs.data_[i];
    }
    return *this;
}

Matrix& Matrix::operator += (const Complex& val) {
    for (int32_t i = 0; i < rows_ * cols_; ++i) {
        data_[i] += val;
    }
    return *this;
}

// ----------- operator - ----------- //

Matrix Matrix::operator - (const Matrix& rhs) const {
    checkSameSize(rhs);
    Matrix result(rows_, cols_);
    for (int i = 0; i < rows_ * cols_; ++i) {
        result.data_[i] = data_[i] - rhs.data_[i];
    }
    return result;
}

Matrix Matrix::operator - (const Complex& val) const {
    Matrix result(rows_, cols_);
    for (int i = 0; i < rows_ * cols_; ++i) {
        result.data_[i] = data_[i] - val;
    }
    return result;
}

Matrix& Matrix::operator -= (const Matrix& rhs) {
    checkSameSize(rhs);
    for (int32_t i = 0; i < rows_ * cols_; ++i) {
        data_[i] -= rhs.data_[i];
    }
    return *this;
}

Matrix& Matrix::operator -= (const Complex& val) {
    for (int32_t i = 0; i < rows_ * cols_; ++i) {
        data_[i] -= val;
    }
    return *this;
}

// ----------- operator * ----------- //

Matrix Matrix::operator * (const Complex& val) const {
    Matrix result(rows_, cols_);
    for (int32_t i = 0; i < rows_ * cols_; ++i) {
        result.data_[i] = data_[i] * val;
    }
    return result;
}

Matrix Matrix::operator * (const Matrix& rhs) const {
    checkSameSize(rhs);
    Matrix result(rows_, cols_);
    for (int32_t i = 0; i < rows_ * cols_; ++i) {
        result.data_[i] = data_[i] * rhs.data_[i];
    }
    return result;
}

// ----------- operator / ----------- //

Matrix Matrix::operator / (const Complex& val) {
    if (val == Complex(0.0, 0.0)) {
        throw std::runtime_error("Division by zero complex number");
    }
    Matrix result(rows_, cols_);

    for (int32_t i = 0; i < rows_ * cols_; ++i) {
        result.data_[i] /= val;
    }
    return result;
}

// ----------- operator (-) ----------- //

Matrix Matrix::operator - () const {
    Matrix res(rows_, cols_);
    for (int32_t i = 0; i < rows_ * cols_; ++i) {
        res.data_[i] = -data_[i];
    }
    return res;
}

[[nodiscard]] int32_t Matrix::getRows() const { return rows_; }
[[nodiscard]] int32_t Matrix::getCols() const { return cols_; }

[[nodiscard]] Complex Matrix::det() const {
    if (rows_ != cols_) {
        throw std::runtime_error("Matrix is not square");
    }
    Matrix tmp = clone();

    Complex determinant(1.0, 0.0);
    const int32_t n = rows_;
    for (int32_t i = 0; i < n; ++i) {
        if (tmp[i][i] == Complex(0.0, 0.0)) {
            bool found = false;
            for (int32_t r = i + 1; r < n; ++r) {
                if (tmp[r][i] != Complex(0.0, 0.0)) {
                    swapRows(tmp, i, r);
                    determinant = -determinant;
                    found = true;
                    break;
                }
            }
            if (!found) {
                return {0.0, 0.0};
            }
        }

        determinant *= tmp[i][i];

        for (int32_t r = i + 1; r < n; ++r) {
            if (tmp[r][i] != Complex(0.0, 0.0)) {
                Complex factor = tmp[r][i] / tmp[i][i];
                for (int32_t c = i; c < n; ++c) {
                    tmp[r][c] -= factor * tmp[i][c];
                }
            }
        }
    }
    return determinant;
}

Matrix Matrix::solveSystem(const Matrix& b) const {
    if (rows_ != cols_) {
        throw std::runtime_error("solveSystem: A is not square");
    }

    if (b.rows_ != rows_ || b.cols_ != 1) {
        throw std::runtime_error("solveSystem: b must be (N x 1)");
    }

    //Метод Гаусса

    const int32_t n = rows_;
    Matrix aug(n, n+1);

    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            aug[r][c] = (*this)[r][c];
        }
        aug[r][n] = b[r][0];
    }

    // Прямой ход
    for (int i = 0; i < n; ++i) {
        if (aug[i][i] == Complex(0.0, 0.0)) {
            bool found = false;
            for (int r = i+1; r < n; ++r) {
                if (aug[r][i] != Complex(0.0, 0.0)) {
                    swapRows(aug, i, r);
                    found = true;
                    break;
                }
            }
            if (!found) {
                throw std::runtime_error("solveSystem: no unique solution (singular matrix)");
            }
        }

        Complex pivot = aug[i][i];
        for (int r = i+1; r < n; ++r) {
            Complex factor = aug[r][i] / pivot;
            for (int c = i; c < n+1; ++c) {
                aug[r][c] -= factor * aug[i][c];
            }
        }
    }

    // Обратный ход
    Matrix x(n, 1);
    for (int i = n - 1; i >= 0; --i) {
        Complex sum = aug[i][n];
        for (int c = i+1; c < n; ++c) {
            sum -= aug[i][c] * x[c][0];
        }
        Complex pivot = aug[i][i];
        if (pivot == Complex(0.0, 0.0)) {
            throw std::runtime_error("solveSystem: pivot is zero, singular system");
        }
        x[i][0] = sum / pivot;
    }

    return x;
}

std::ostream& operator<<(std::ostream& out, const Matrix& m) {
    for (int32_t i = 0; i < m.rows_; ++i) {
        for (int32_t j = 0; j < m.cols_; ++j) {
            const auto& val = Matrix::onlyPositiveZeros(m[i][j]);
                out << val.real() << " " << val.imag();
            if (j + 1 < m.cols_) out << "   ";
        }
        out << "\n";
    }
    return out;
}

Complex Matrix::onlyPositiveZeros(const Complex& val) {
    Complex result;
    if (val.imag() == -0) {
        if (val.real() == -0) {
            result = Complex(0.0, 0.0);
        } else {
            result = Complex(val.real(), 0.0);
        }
    } else {
        if (val.real() == -0) {
            result = Complex(0.0, val.imag());
        } else {
            result = Complex(val.real(), val.imag());
        }
    }
    return result;
}

void Matrix::checkSameSize(const Matrix& rhs) const {
    if (rows_ != rhs.rows_ || cols_ != rhs.cols_) {
        throw std::runtime_error("Matrix size mismatch");
    }
}

void Matrix::checkBounds(const int32_t row, const int32_t col) const {
    if (row < 0 || row >= rows_ || col < 0 || col >= cols_) {
        throw std::out_of_range("Matrix index is out of range");
    }
}

void Matrix::swapRows(const Matrix& mat, const int32_t r1, const int32_t r2) {
    if (r1 == r2) return;
    for (int32_t col = 0; col < mat.cols_; ++col) {
        std::swap(mat[r1][col], mat[r2][col]);
    }
}