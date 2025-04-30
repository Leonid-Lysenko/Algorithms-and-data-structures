/* Leonid Lysenko st128618@student.spbu.ru
   Assignment2
*/

#include "gauss_solver.h"
#include <gtest/gtest.h>
#include <Eigen/Dense>
#include <fstream>
#include <chrono>

class GaussSolverTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Создаем тестовый CSV файл
        std::ofstream file("test_system.csv");
        file << "2,1,5\n1,-1,1";
        file.close();
    }
    
    void TearDown() override {
        remove("test_system.csv");
        remove("test_solution.csv");
    }
};

TEST_F(GaussSolverTest, SolvesSimpleSystem) {
    Eigen::MatrixXd A(2, 2);
    Eigen::VectorXd b(2);
    
    GaussSolver::readSystemFromCSV("test_system.csv", A, b);
    
    Eigen::VectorXd solution = GaussSolver::solve(A, b);
    
    EXPECT_NEAR(solution(0), 2.0, 1e-6);
    EXPECT_NEAR(solution(1), 1.0, 1e-6);
}

TEST_F(GaussSolverTest, WritesSolutionToCSV) {
    Eigen::VectorXd solution(2);
    solution << 2.0, 1.0;
    
    GaussSolver::writeSolutionToCSV("test_solution.csv", solution);
    
    std::ifstream file("test_solution.csv");
    std::string content;
    std::getline(file, content);
    
    EXPECT_EQ(content, "2,1");
}

TEST_F(GaussSolverTest, GeneratesConsistentRandomSystem) {
    Eigen::MatrixXd A1, A2;
    Eigen::VectorXd b1, b2;
    
    GaussSolver::generateRandomSystem(A1, b1, 10, 42);
    GaussSolver::generateRandomSystem(A2, b2, 10, 42);
    
    EXPECT_TRUE(A1.isApprox(A2));
    EXPECT_TRUE(b1.isApprox(b2));
}

TEST_F(GaussSolverTest, SolvesGeneratedSystem) {
    Eigen::MatrixXd A;
    Eigen::VectorXd b;
    
    GaussSolver::generateRandomSystem(A, b, 5, 123);
    
    Eigen::VectorXd solution = GaussSolver::solve(A, b);
    
    // Проверяем, что решение действительно удовлетворяет системе
    Eigen::VectorXd Ax = A * solution;
    EXPECT_TRUE(Ax.isApprox(b, 1e-6));
}

TEST_F(GaussSolverTest, SolutionAccuracy) {
    Eigen::MatrixXd A(3, 3);
    Eigen::VectorXd b(3);
    A << 4,1,1, 1,4,1, 1,1,4;
    b << 6,6,6;
    
    Eigen::VectorXd solution = GaussSolver::solve(A, b);
    Eigen::VectorXd expected(3);
    expected << 1,1,1;
    
    ASSERT_TRUE(solution.isApprox(expected, 1e-6));
}

TEST_F(GaussSolverTest, PerformanceTest) {
    const int sizes[] = {10, 100, 500};
    for (int n : sizes) {
        Eigen::MatrixXd A;
        Eigen::VectorXd b;
        GaussSolver::generateRandomSystem(A, b, n, 42);
        
        auto start = std::chrono::high_resolution_clock::now();
        Eigen::VectorXd solution = GaussSolver::solve(A, b);
        auto end = std::chrono::high_resolution_clock::now();
        
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "Size " << n << "x" << n 
                  << ": " << elapsed.count() << "s\n";
        
        // Реалистичный критерий для больших систем
        if (n <= 100) {
            ASSERT_LT(elapsed.count(), n*n*1e-5);
        } else {
            ASSERT_LT(elapsed.count(), n*n*n*1e-7);
        }
    }
}

TEST_F(GaussSolverTest, ZeroMatrix) {
    Eigen::MatrixXd A = Eigen::MatrixXd::Zero(3,3);
    Eigen::VectorXd b = Eigen::VectorXd::Random(3);
    EXPECT_THROW(GaussSolver::solve(A, b), std::runtime_error);
}

TEST_F(GaussSolverTest, SingularMatrix) {
    Eigen::MatrixXd A(2,2);
    A << 1, 2, 2, 4; // Вырожденная матрица (вторая строка = 2 * первая)
    Eigen::VectorXd b = Eigen::VectorXd::Random(2);
    EXPECT_THROW(GaussSolver::solve(A, b), std::runtime_error);
}

TEST_F(GaussSolverTest, LargeSystem) {
    Eigen::MatrixXd A;
    Eigen::VectorXd b;
    GaussSolver::generateRandomSystem(A, b, 1000, 42);
    
    Eigen::VectorXd solution = GaussSolver::solve(A, b);
    double residual = (A * solution - b).norm();
    EXPECT_LT(residual, 1e-8);
}

TEST_F(GaussSolverTest, PartiallyDegenerate) {
    Eigen::MatrixXd A(3,3);
    A << 1, 2, 3,
         0, 0, 0,
         4, 5, 6;
    Eigen::VectorXd b(3);
    b << 6, 0, 15;
    EXPECT_THROW(GaussSolver::solve(A, b), std::runtime_error);
}

TEST_F(GaussSolverTest, BackSubstitutionAccuracy) {
    Eigen::MatrixXd A(4,4);
    A << 1, 2, 3, 4,
         0, 1, 2, 3,
         0, 0, 1, 2,
         0, 0, 0, 1;
    Eigen::VectorXd b(4);
    b << 10, 6, 3, 1;
    
    Eigen::VectorXd solution = GaussSolver::solve(A, b);
    Eigen::VectorXd expected(4);
    expected << 1, 1, 1, 1;
    EXPECT_TRUE(solution.isApprox(expected, 1e-10));
}

TEST_F(GaussSolverTest, InconsistentSystem) {
    Eigen::MatrixXd A(2,2);
    A << 1, 1,
         1, 1;
    Eigen::VectorXd b(2);
    b << 1, 2;
    EXPECT_THROW(GaussSolver::solve(A, b), std::runtime_error);
}
