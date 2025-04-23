/* Leonid Lysenko st128618@student.spbu.ru
   Assignment2
*/

#include "gauss_solver.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_csv> [output_csv]" << std::endl;
        return 1;
    }
    
    try {
        Eigen::MatrixXd A;
        Eigen::VectorXd b;
        
        // Чтение системы из файла
        GaussSolver::readSystemFromCSV(argv[1], A, b);
        
        // Решение системы
        Eigen::VectorXd solution = GaussSolver::solve(A, b);
        
        // Вывод результата
        if (argc >= 3) {
            GaussSolver::writeSolutionToCSV(argv[2], solution);
        } else {
            std::cout << "Solution:\n" << solution << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
