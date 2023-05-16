#ifndef __METODO_POTENCIA__EIGEN__
#define __METODO_POTENCIA__EIGEN__

#include "../Eigen/Dense"
#include "../Eigen/Sparse"

using Eigen::MatrixXd;

typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> Matrix;
typedef Eigen::SparseMatrix<double> SparseMatrix;

typedef Eigen::VectorXd Vector;

std::pair<double, Vector> metodoPotencia(const Matrix& A, Vector x0, int iteraciones, double error_tolerable);

std::pair<Vector, Matrix> calcularAutovectoresyValores(const Matrix& X, int num, int iteraciones, double error_tolerable);

#endif