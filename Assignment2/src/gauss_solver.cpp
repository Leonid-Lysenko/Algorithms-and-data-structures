/* Leonid Lysenko st128618@student.spbu.ru
   Assignment2
*/

#include "gauss_solver.h"
#include <Eigen/Dense>
#include <fstream>
#include <random>
#include <stdexcept>
#include <vector>

Eigen::VectorXd GaussSolver::solve(const Eigen::MatrixXd& A, const Eigen::VectorXd& b) {
    const int n = A.rows();
    
    if (n != A.cols() || n != b.size()) {
        throw std::invalid_argument("Invalid matrix or vector dimensions");
    }

    // Проверка на нулевую матрицу
    if (A.isZero(1e-10)) {
        throw std::runtime_error("Zero matrix encountered");
    }

    Eigen::PartialPivLU<Eigen::MatrixXd> lu(A);
    
    // Проверка на вырожденность через determinant
    if (lu.determinant() == 0) {
        throw std::runtime_error("Matrix is singular");
    }
    
    return lu.solve(b);
}

void GaussSolver::readSystemFromCSV(const std::string& filename, 
                                  Eigen::MatrixXd& A, 
                                  Eigen::VectorXd& b) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::vector<double> buffer;
    std::string line;
    int rows = 0;
    int cols = 0;

    // Первый проход для определения размеров
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        int current_cols = 0;
        
        while (std::getline(ss, cell, ',')) {
            buffer.push_back(std::stod(cell));
            current_cols++;
        }

        if (rows == 0) {
            cols = current_cols;
        } else if (current_cols != cols) {
            throw std::runtime_error("Inconsistent column count in CSV");
        }
        rows++;
    }

    // Проверка на квадратную матрицу + вектор
    if (cols != rows + 1) {
        throw std::runtime_error("Invalid system dimensions in CSV");
    }

    A.resize(rows, rows);
    b.resize(rows);

    // Заполнение матрицы и вектора
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < rows; ++j) {
            A(i, j) = buffer[i * cols + j];
        }
        b(i) = buffer[i * cols + rows];
    }
}

void GaussSolver::writeSolutionToCSV(const std::string& filename, 
                                   const Eigen::VectorXd& solution) {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to create file: " + filename);
    }

    const int n = solution.size();
    for (int i = 0; i < n; ++i) {
        file << solution(i);
        if (i < n - 1) file << ",";
    }
}

void GaussSolver::generateRandomSystem(Eigen::MatrixXd& A, 
                                     Eigen::VectorXd& b, 
                                     int size, 
                                     unsigned int seed,
                                     bool make_diagonally_dominant) {
    if (size <= 0) {
        throw std::invalid_argument("System size must be positive");
    }

    // Изменяем размер матрицы A до size x size
    // Изменяем размер вектора b до size
    A.resize(size, size);
    b.resize(size);

    // Воспроизводимый генератор
    std::mt19937 gen(seed);
    std::uniform_real_distribution<double> dist(-10.0, 10.0);

    // Заполнение матрицы и вектора
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            A(i, j) = dist(gen);
        }
        b(i) = dist(gen);
    }

    // Делаем матрицу диагонально доминирующей если требуется (на вский пожарный)
    if (make_diagonally_dominant) {
        for (int i = 0; i < size; ++i) {
            double row_sum = A.row(i).cwiseAbs().sum() - std::abs(A(i, i));
            A(i, i) = row_sum + 1.0; // Гарантируем преобладание
        }
    }
}
