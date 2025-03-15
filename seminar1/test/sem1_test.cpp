#include <fstream>

#include "matrix.h"
#include <iostream>
#include <gtest/gtest.h>

using Complex = std::complex<double>;

static void expectComplexEq(const Complex& lhs,
                            const Complex& rhs,
                            double eps = 1e-12)
{
    EXPECT_NEAR(lhs.real(), rhs.real(), eps);
    EXPECT_NEAR(lhs.imag(), rhs.imag(), eps);
}

TEST(MatrixTest, CreateWithPositiveDimensions) {
    Matrix m(3, 3);
    EXPECT_EQ(m.getRows(), 3);
    EXPECT_EQ(m.getCols(), 3);

    for (int r = 0; r < m.getRows(); ++r) {
        for (int c = 0; c < m.getCols(); ++c) {
            expectComplexEq(m[r][c], Complex(0.0, 0.0));
        }
    }
}

TEST(MatrixTest, CreateWithNegativeDimensionsThrows) {
    EXPECT_THROW({
        Matrix bad(-1, 2);
    }, std::invalid_argument);

    EXPECT_THROW({
        Matrix bad(2, -1);
    }, std::invalid_argument);
}

TEST(MatrixTest, AtMethod) {
    Matrix m(2, 3);
    m[0][0] = Complex(1.0, 2.0);
    expectComplexEq(m.at(0, 0), Complex(1.0, 2.0));

    m.at(1, 2) = Complex(3.0, -4.0);
    expectComplexEq(m[1][2], Complex(3.0, -4.0));
}

TEST(MatrixTest, AtMethodInvalid) {
    Matrix m(2, 3);
    EXPECT_THROW(auto a = m.at(-1, 0); a+=1, std::out_of_range);
    EXPECT_THROW(auto a = m.at(2, 0); a+=1,  std::out_of_range);
    EXPECT_THROW(auto a = m.at(0, 3); a+=1,  std::out_of_range);

}

TEST(MatrixTest, FileConstructorInvalidFile) {
    Matrix m("blablabla");
    EXPECT_EQ(m.getRows(), 0);
    EXPECT_EQ(m.getCols(), 0);
}

TEST(MatrixTest, FileConstructorGoodData) {
    const char* fname = "good_test_matrix.txt";
    {
        std::ofstream ofs(fname);
        ofs << "1 1 1 2\n";
    }

    Matrix m(fname);
    EXPECT_EQ(m.getRows(), 1);
    EXPECT_EQ(m.getCols(), 1);

    expectComplexEq(m[0][0], {1.0, 2.0});

    std::remove(fname);
}

TEST(MatrixTest, MoveConstructor) {
    Matrix original(2, 2);
    original[0][0] = {1.0, 1.0};
    original[1][1] = {2.0, -2.0};

    Matrix moved(std::move(original));
    EXPECT_EQ(original.getRows(), 0);
    EXPECT_EQ(original.getCols(), 0);

    EXPECT_EQ(moved.getRows(), 2);
    EXPECT_EQ(moved.getCols(), 2);
    expectComplexEq(moved[0][0], {1.0, 1.0});
    expectComplexEq(moved[1][1], {2.0, -2.0});
}

TEST(MatrixTest, OperatorsMatrix) {
    Matrix m1(2,2);
    m1[0][0] = {1.0, 1.0};
    m1[1][1] = {2.0, 2.0};

    Matrix m2(2,2);
    m2[0][0] = {5.0, -1.0};
    m2[1][1] = {-2.0, 7.0};

    Matrix sum = m1 + m2;
    expectComplexEq(sum[0][0], {6.0, 0.0});
    expectComplexEq(sum[1][1], {0.0, 9.0});

    Matrix sumVal = m1 + Complex(3.0, 4.0);
    expectComplexEq(sumVal[0][0], {4.0, 5.0});
    expectComplexEq(sumVal[1][1], {5.0, 6.0});

    m1 += m2;
    expectComplexEq(m1[0][0], {6.0, 0.0});
    expectComplexEq(m1[1][1], {0.0, 9.0});

    m1 -= m2;
    expectComplexEq(m1[0][0], {1.0, 1.0});
    expectComplexEq(m1[1][1], {2.0, 2.0});

    m1 += Complex(1.0, 1.0);
    expectComplexEq(m1[0][0], {2.0, 2.0});
    expectComplexEq(m1[1][1], {3.0, 3.0});

    m1 -= Complex(1.0, 1.0);
    expectComplexEq(m1[0][0], {1.0, 1.0});
    expectComplexEq(m1[1][1], {2.0, 2.0});

    Matrix sub = m1 - m2;
    expectComplexEq(sub[0][0], {-4.0, 2.0});
    expectComplexEq(sub[1][1], {4.0, -5.0});

    Matrix subVal = m1 - Complex(1.0, 1.0);
    expectComplexEq(subVal[0][0], {0.0, 0.0});
    expectComplexEq(subVal[1][1], {1.0, 1.0});

    Matrix mul = m1 * m2;
    expectComplexEq(mul[0][0], {6.0, 4.0});
    expectComplexEq(mul[1][1], {-18.0, 10.0});

    Matrix mulVal = m1 * Complex(1.0, 2.0);
    expectComplexEq(mulVal[0][0], {-1.0, 3.0});
    expectComplexEq(mulVal[1][1], {-2.0, 6.0});

    Matrix div = m1 / Complex(1.0, 2.0);
    expectComplexEq(div[0][0], {0.6, -0.2});
    expectComplexEq(div[1][1], {1.2, -0.4});

    EXPECT_THROW(m1 / Complex(0.0, 0.0), std::runtime_error);

    Matrix neg = -m1;
    expectComplexEq(neg[0][0], {-1.0, -1.0});
    expectComplexEq(neg[1][1], {-2.0, -2.0});
}

TEST(MatrixTest, Determ) {
    Matrix m2(2,2);
    m2[0][0] = {5.0, -1.0};
    m2[1][1] = {-2.0, 7.0};
    expectComplexEq(m2.det(), {-3.0, 37.0});
}

//solve system of linear equations
TEST(MatrixTest, SoLE) {
    Matrix m1(2,2);
    m1[0][0] = {1.0, 0.0};
    m1[0][1] = {2.0, 0.0};
    m1[1][0] = {2.0, 0.0};
    m1[1][1] = {5.0, 0.0};
    Matrix m2(2,1);
    m2[0][0] = {5.0, 0.0};
    m2[1][0] = {12.0, 0.0};

    Matrix res = m1.solveSystem(m2);
    EXPECT_EQ(res.getRows(), 2);
    EXPECT_EQ(res.getCols(), 1);

    expectComplexEq(res[0][0], {1.0, 0.0});
    expectComplexEq(res[1][0], {2.0, 0.0});
}

TEST(MatrixTest, OutputOperatorCheck) {
    Matrix m(2,2);
    m[0][0] = {1.0, -0.0};
    m[0][1] = {-0.0, -0.0};
    m[1][0] = {1.5, 2.5};
    m[1][1] = {-2, 0.0};

    std::ostringstream oss;
    oss << m;

    std::string out = oss.str();
    EXPECT_NE(out.find("1 0"), std::string::npos);
    EXPECT_NE(out.find("0 0"), std::string::npos);
    EXPECT_NE(out.find("1.5 2.5"), std::string::npos);
    EXPECT_NE(out.find("-2 0"), std::string::npos);
}

TEST(MatrixTest, MoveAssignment)
{
    Matrix a(2,2);
    a[0][1] = {10.0, -1.0};

    Matrix b(3,3);
    b = std::move(a);

    EXPECT_EQ(a.getRows(), 0);
    EXPECT_EQ(a.getCols(), 0);

    EXPECT_EQ(b.getRows(), 2);
    EXPECT_EQ(b.getCols(), 2);
    expectComplexEq(b[0][1], {10.0, -1.0});
}

TEST(MatrixTest, DetSingularMatrixZeroPivot)
{
    Matrix m(2,2);
    m[0][0] = {0, 0};
    m[0][1] = {1, 0};
    m[1][0] = {0, 0};
    m[1][1] = {2, 0};

    auto d = m.det();
    expectComplexEq(d, {0.0, 0.0});
}

TEST(MatrixTest, SolveSystemWithErrors) {
    Matrix A(2,3);
    Matrix b(2,1);
    EXPECT_THROW(auto res = A.solveSystem(b), std::runtime_error);

    Matrix aa(2,2);
    Matrix bb(3,1);
    EXPECT_THROW(auto res = aa.solveSystem(bb), std::runtime_error);

    Matrix bbb(2,2);
    EXPECT_THROW(auto res = aa.solveSystem(bbb), std::runtime_error);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
