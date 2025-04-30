/* Leonid Lysenko st128618@student.spbu.ru
   Assignment2
*/

#include "gauss_solver.h"
#include <fstream>
#include <stdexcept>
#include <random>
#include <vector>

Eigen::VectorXd GaussSolver::solve(const Eigen::MatrixXd& A, const Eigen::VectorXd& b) {
    const int n = A.rows();
    
    // Проверяем, что матрица квадратная и размеры совпадают
    if (n != A.cols() || n != b.size()) { throw std::invalid_argument("Хм-м-м... Неправильные размеры матрицы или вектора"); }

    // Создаем расширенную матрицу, а именноо [A|b]
    Eigen::MatrixXd Ab(n, n + 1);
    Ab << A, b;

    // Прямой ход метода Гаусса
    for (int k = 0; k < n; ++k) {
        // Ищем строку с максимальным элементом в текущем столбце
        int max_row = k;
        for (int i = k + 1; i < n; ++i) {
            if (std::abs(Ab(i, k))>std::abs(Ab(max_row, k))) {
                max_row = i;
            }
        }
        
        // Меняем строки местами, если нужно
        if (max_row != k) { Ab.row(k).swap(Ab.row(max_row)); }
        
        // Проверяем, что главный элемент не нулевой
        if (std::abs(Ab(k, k)) < 1e-10) { throw std::runtime_error("Матрица вырожденная, решение невозможно!"); }
        
        // Приводим матрицу к ступенчатому виду
        for (int i = k + 1; i < n; ++i) {
            double factor = Ab(i, k) / Ab(k, k);
            for (int j = k; j < n + 1; ++j) {
                Ab(i, j) -= factor * Ab(k, j);
            }
        }
    }
    
    // Обратный ход метода Гаусса
    Eigen::VectorXd x(n);
    for (int i = n - 1; i >= 0; --i) {
        x(i) = Ab(i, n)/Ab(i, i);
        for (int j = i - 1; j >= 0; --j) {
            Ab(j, n) -= Ab(j, i) * x(i);
        }
    }
    
    return x;
}

void GaussSolver::readSystemFromCSV(const std::string& filename, Eigen::MatrixXd& A, Eigen::VectorXd& b) {
    std::ifstream file(filename);
    if (!file) { throw std::runtime_error("Не удалось открыть файл. Печалька...: " + filename); }

    std::vector<double> buffer;
    std::string line;
    int rows = 0;
    int cols = 0;

    // Сначала считаем количество строк и столбцов
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
        } else if (current_cols != cols) { throw std::runtime_error("Хм-м-м... количество столбцов в CSV не совпадает."); }
        
        rows++;
    }

    // Проверяем, что матрица квадраттная и есть вектор b
    if (cols != rows + 1) { throw std::runtime_error("Неправильные размеры системы в CSV. ヽ(°□° )ノ"); }

    A.resize(rows, rows);
    b.resize(rows);

    // Заполняем саму матрицу A и еще вектор b
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < rows; ++j) {
            A(i, j) = buffer[i * cols + j];
        }
        b(i) = buffer[i * cols + rows];
    }
}

void GaussSolver::writeSolutionToCSV(const std::string& filename, const Eigen::VectorXd& solution) {
    std::ofstream file(filename);
    if (!file) { throw std::runtime_error("Не удалось создать файл. Печалька...: " + filename); }

    const int n = solution.size();
    for (int i = 0; i < n; ++i) {
        file << solution(i);
        if (i < n - 1) file << ",";
    }
}

void GaussSolver::generateRandomSystem(Eigen::MatrixXd& A, Eigen::VectorXd& b, int size, unsigned int seed) {
    if (size <= 0) { throw std::invalid_argument("Размер системы должен быть положительным. ヽ(°□° )ノ"); }

    A.resize(size, size);
    b.resize(size);

    // Генератор случайных чисел
    std::mt19937 gen(seed);
    std::uniform_real_distribution<double> dist(-50.0, 50.0);

    // Заполняем матрицу и вектор случайными числами
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            A(i, j) = dist(gen);
        }
        
        b(i) = dist(gen);
    }
}
