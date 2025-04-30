/* Leonid Lysenko st128618@student.spbu.ru
   Assignment2
*/

#include "gauss_solver.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Использование: " << argv[0] << " <входной_файл.csv> [выходной_файл.csv]" << std::endl;
        return 1;
    }
    
    try {
        Eigen::MatrixXd A;
        Eigen::VectorXd b;
        
        // Читаем систему из файла
        GaussSolver::readSystemFromCSV(argv[1], A, b);
        
        // Решаем систему
        Eigen::VectorXd solution = GaussSolver::solve(A, b);
        
        // Выводим результат
        if (argc >= 3) {
            GaussSolver::writeSolutionToCSV(argv[2], solution);
        } else {
            std::cout << "Решение:\n" << solution << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
