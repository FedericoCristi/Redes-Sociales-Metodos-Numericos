#ifndef __PARSER__
#define __PARSER__

#include <map>
#include <math.h>
#include "definiciones.h"
#include "matriz_coo.h"
#include "auxiliares_vectores.h"

#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Sparse>


using Eigen::MatrixXd;


typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> Matrix;
typedef Eigen::SparseMatrix<double> SparseMatrix;

typedef Eigen::VectorXd Vector;


matriz_coo* parseToCoo(char* path);
matriz_coo* parseToCooGrafo(char* path, std::map<int, int> dicc_user_id);
std::tuple<std::vector<double>, matriz_coo*> parseFacebookAtributos(char* path);

#endif