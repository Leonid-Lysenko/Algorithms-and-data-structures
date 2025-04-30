/* Leonid Lysenko st128618@student.spbu.ru
   Assignment2
*/

#ifndef GAUSS_SOLVER_H
#define GAUSS_SOLVER_H

#include <Eigen/Dense>
#include <string>

class GaussSolver {
public:
    // Решает систему Ax = b методом Гаусса
    static Eigen::VectorXd solve(const Eigen::MatrixXd& A, const Eigen::VectorXd& b);
    
    // Читает матрицу A и вектор b из CSV файла
    static void readSystemFromCSV(const std::string& filename, Eigen::MatrixXd& A, Eigen::VectorXd& b);
    
    // Записывает решение в CSV файл
    static void writeSolutionToCSV(const std::string& filename, const Eigen::VectorXd& solution);
    
    // Генрирует случайную систему уравнений
    static void generateRandomSystem(Eigen::MatrixXd& A, Eigen::VectorXd& b, int size, unsigned int seed);
};

#endif
