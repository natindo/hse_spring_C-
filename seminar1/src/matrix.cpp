#include "matrix.h"

#include <sys/stat.h>

using Complex = std::complex<double>;

Matrix::ProxyRow::ProxyRow() = default;

Matrix::ProxyRow::ProxyRow(const int32_t cols_count)
    : data_(new Complex[cols_count]),
      cols_number_(cols_count)
{
    std::fill_n(data_, cols_number_, Complex(0.0, 0.0));
}

Matrix::ProxyRow::ProxyRow(ProxyRow&& rhs) noexcept
    : data_(rhs.data_),
      cols_number_(rhs.cols_number_)
{
    rhs.data_ = nullptr;
    rhs.cols_number_ = 0;
}

Matrix::ProxyRow& Matrix::ProxyRow::operator=(ProxyRow&& rhs) noexcept {
    if (this != &rhs) {
        delete[] data_;

        data_ = rhs.data_;
        cols_number_ = rhs.cols_number_;

        rhs.data_ = nullptr;
        rhs.cols_number_ = 0;
    }
    return *this;
}

Matrix::ProxyRow::~ProxyRow() {
    delete[] data_;
    data_ = nullptr;
    cols_number_ = 0;
}

Complex& Matrix::ProxyRow::operator[](const int32_t col) const {
    return data_[col];
}

Matrix::Matrix (const int32_t rows, const int32_t cols) : rows_(rows), cols_(cols) {
    if (rows < 0 || cols < 0) {
        throw std::invalid_argument("Matrix dimensions must be positive");
    }
    proxy_rows_ = new ProxyRow[rows_];
    for (int32_t r = 0; r < rows_; ++r) {
        proxy_rows_[r] = ProxyRow(cols_);
    }
}


Matrix::Matrix (const std::string& filename) {
    fd = open(filename.c_str(), O_RDWR);
    if (fd == -1) {
        throw std::runtime_error("Не удалось открыть файл");
        return;
    }

    struct stat sb{};
    if (fstat(fd, &sb) == -1) {
        throw std::runtime_error("Не удалось выполнить fstat");
        close(fd);
        return;
    }

    fileSize = sb.st_size;
    if (fileSize == 0) {
        throw std::runtime_error("Файл пустой");
        close(fd);
        return;
    }


    mapPtr = mmap(
        nullptr,
        fileSize,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fd,
        0
    );

    if (mapPtr == MAP_FAILED) {
        throw std::runtime_error("Не удалось выполнить mmap");
        close(fd);
        return;
    }

    char* data = static_cast<char*>(mapPtr);
    std::string fileData(data, fileSize);
    std::istringstream iss(fileData);

    iss >> rows_ >> cols_;
    if (!iss) {
        throw std::runtime_error("Ошибка чтения размеров матрицы");
        return;
    }

    proxy_rows_ = new ProxyRow[rows_];
    for (int32_t i = 0; i < rows_; ++i) {
        proxy_rows_[i] = ProxyRow(cols_);
    }

    for (int32_t r = 0; r < rows_; ++r) {
        for (int32_t c = 0; c < cols_; ++c) {
            double re = 0.0, im = 0.0;
            iss >> re >> im;
            if (!iss) {
                throw std::runtime_error("Ошибка чтения размеров матрицы");
                return;
            }
            proxy_rows_[r][c] = Complex(re, im);
        }
    }
    std::cout << std::endl;
}

Matrix::Matrix(Matrix&& rhs) noexcept {
    rows_ = rhs.rows_;
    cols_ = rhs.cols_;
    proxy_rows_ = rhs.proxy_rows_;

    mapPtr = rhs.mapPtr;
    fileSize = rhs.fileSize;
    fd = rhs.fd;

    rhs.proxy_rows_ = nullptr;
    rhs.rows_ = 0;
    rhs.cols_ = 0;

    rhs.mapPtr = nullptr;
    rhs.fileSize = 0;
    rhs.fd = -1;
}

Matrix& Matrix::operator=(Matrix&& rhs) noexcept {
    if (this != &rhs) {
        delete[] proxy_rows_;
        proxy_rows_ = nullptr;

        if (mapPtr != nullptr) {
            msync(mapPtr, fileSize, MS_SYNC);
            munmap(mapPtr, fileSize);
            close(fd);
        }

        rows_ = rhs.rows_;
        cols_ = rhs.cols_;
        proxy_rows_ = rhs.proxy_rows_;

        mapPtr = rhs.mapPtr;
        fileSize = rhs.fileSize;
        fd = rhs.fd;

        rhs.proxy_rows_ = nullptr;
        rhs.rows_ = 0;
        rhs.cols_ = 0;

        rhs.mapPtr = nullptr;
        rhs.fileSize = 0;
        rhs.fd = -1;
    }
    return *this;
}

Matrix::~Matrix() {
    delete[] proxy_rows_;
    proxy_rows_ = nullptr;
    if (mapPtr != nullptr) {
        msync(mapPtr, fileSize, MS_SYNC);
        if (munmap(mapPtr, fileSize) == -1) {
            std::cerr << "Не удалось выполнить munmap" << std::endl;
        }
        close(fd);
    }
}

Matrix::ProxyRow& Matrix::operator[](const int32_t row) const {
    return proxy_rows_[row];
}

[[nodiscard]] Complex& Matrix::at(const int32_t row, const int32_t col) const {
    checkBounds(row, col);
    return proxy_rows_[row][col];
}

[[nodiscard]] Matrix Matrix::clone() const {
    Matrix m(rows_, cols_);
    for (int32_t r = 0; r < rows_; ++r) {
        for (int32_t c = 0; c < cols_; ++c) {
            m[r][c] = proxy_rows_[r][c];
        }
    }
    return m;
}

// ----------- operator + ----------- //
Matrix Matrix::operator + (const Matrix& rhs) const {
    checkSameSize(rhs);
    Matrix result(rows_, cols_);
    for (int32_t i = 0; i < rows_; ++i) {
        for (int32_t j = 0; j < cols_; ++j) {
            result[i][j] = proxy_rows_[i][j] + rhs[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator + (const Complex& val) const {
    Matrix result(rows_, cols_);
    for (int32_t i = 0; i < rows_; ++i) {
        for (int32_t j = 0; j < cols_; ++j) {
            result[i][j] = proxy_rows_[i][j] + val;
        }
    }
    return result;
}

Matrix& Matrix::operator += (const Matrix& rhs) {
    checkSameSize(rhs);
    for (int32_t i = 0; i < rows_; ++i) {
        for (int32_t j = 0; j < cols_; ++j) {
            proxy_rows_[i][j] = proxy_rows_[i][j] + rhs[i][j];
        }
    }
    return *this;
}

Matrix& Matrix::operator += (const Complex& val) {
    for (int32_t i = 0; i < rows_; ++i) {
        for (int32_t j = 0; j < cols_; ++j) {
            proxy_rows_[i][j] = proxy_rows_[i][j] + val;
        }
    }
    return *this;
}

// ----------- operator - ----------- //

Matrix Matrix::operator - (const Matrix& rhs) const {
    checkSameSize(rhs);
    Matrix result(rows_, cols_);
    for (int32_t i = 0; i < rows_; ++i) {
        for (int32_t j = 0; j < cols_; ++j) {
            result[i][j] = proxy_rows_[i][j] - rhs[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator - (const Complex& val) const {
    Matrix result(rows_, cols_);
    for (int32_t i = 0; i < rows_; ++i) {
        for (int32_t j = 0; j < cols_; ++j) {
            result[i][j] = proxy_rows_[i][j] - val;
        }
    }
    return result;
}

Matrix& Matrix::operator -= (const Matrix& rhs) {
    checkSameSize(rhs);
    for (int32_t i = 0; i < rows_; ++i) {
        for (int32_t j = 0; j < cols_; ++j) {
            proxy_rows_[i][j] = proxy_rows_[i][j] - rhs[i][j];
        }
    }
    return *this;
}

Matrix& Matrix::operator -= (const Complex& val) {
    for (int32_t i = 0; i < rows_; ++i) {
        for (int32_t j = 0; j < cols_; ++j) {
            proxy_rows_[i][j] = proxy_rows_[i][j] - val;
        }
    }
    return *this;
}

// ----------- operator * ----------- //

Matrix Matrix::operator * (const Complex& val) const {
    Matrix result(rows_, cols_);
    for (int32_t i = 0; i < rows_; ++i) {
        for (int32_t j = 0; j < cols_; ++j) {
            result[i][j] = proxy_rows_[i][j] * val;
        }
    }
    return result;
}

Matrix Matrix::operator * (const Matrix& rhs) const {
    checkSameSize(rhs);
    Matrix result(rows_, cols_);
    for (int32_t i = 0; i < rows_; ++i) {
        for (int32_t j = 0; j < rhs.cols_; ++j) {
            result[i][j] = 0;
            for (int32_t k = 0; k < cols_; ++k) {
                result[i][j] += proxy_rows_[i][k] * rhs[k][j];
            }
        }
    }
    return result;
}

// ----------- operator / ----------- //

Matrix Matrix::operator / (const Complex& val) {
    if (val == Complex(0.0, 0.0)) {
        throw std::runtime_error("Division by zero complex number");
    }
    Matrix result(rows_, cols_);

    for (int32_t r = 0; r < rows_; ++r) {
        for (int32_t c = 0; c < cols_; ++c) {
            result[r][c] = proxy_rows_[r][c] / val;
        }
    }
    return result;
}

// ----------- operator (-) ----------- //

Matrix Matrix::operator - () const {
    Matrix res(rows_, cols_);
    for (int32_t r = 0; r < rows_; ++r) {
        for (int32_t c = 0; c < cols_; ++c) {
            res[r][c] = -proxy_rows_[r][c];
        }
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

    // Gauss method
    const int32_t n = rows_;
    Matrix aug(n, n+1);

    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            aug[r][c] = (*this)[r][c];
        }
        aug[r][n] = b[r][0];
    }

    // straight move
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

    // reverse move
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