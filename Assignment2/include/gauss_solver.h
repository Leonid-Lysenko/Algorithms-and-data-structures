/* Leonid Lysenko st128618@student.spbu.ru
   Assignment2
*/

#ifndef GAUSS_SOLVER_H
#define GAUSS_SOLVER_H

#include <Eigen/Dense>
#include <string>
#include <vector>
#include <memory>

class GaussSolver {
public:
    // Решение системы методом Гаусса с частичным выбором
    static Eigen::VectorXd solve(const Eigen::MatrixXd& A, const Eigen::VectorXd& b);
    
    // Оптимизированное чтение системы из CSV
    static void readSystemFromCSV(const std::string& filename, 
                                 Eigen::MatrixXd& A, 
                                 Eigen::VectorXd& b);
    
    // Запись решения в CSV
    static void writeSolutionToCSV(const std::string& filename, 
                                  const Eigen::VectorXd& solution);
    
    // Генерация больших систем с контролем памяти
    static void generateRandomSystem(Eigen::MatrixXd& A, 
                                    Eigen::VectorXd& b, 
                                    int size, 
                                    unsigned int seed,
                                    bool make_diagonally_dominant = true);
};

#endif
