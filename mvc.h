#include <Eigen/Core>

// Inputs:
//   V: #V by 3 matrix of vertices
//   F: #F by 3 matrix of faces
//   X: n by 3 matrix of interior query points
//   W: n by #V matrix of weights

void mvc(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const Eigen::MatrixXd & X,
  Eigen::MatrixXd & W);