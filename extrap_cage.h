#include <Eigen/Core>

// Given two "sufficiently similar" meshes (V1, F) and (V2, F) with a one-to-one
// vertex and face correspondence, and a cage (V1_C, F_C) exterior to (V1, F), 
// compute a cage (V2_C, F_C) exterior to (V2, F2).
//
// Inputs:
//   V1    #V by 3 matrix of vertices for the first mesh
//   V2    #V by 3 matrix of vertices for the second mesh
//   F     #F by 3 matrix of faces for both meshes
//   V1_C  #V_C by 3 matrix of vertices for the first exterior cage
//   F_C   #F_C by 3 matrix of faces for both exterior cages
//   method  method used to generate cage
// Outputs:
//   V2_C  #V_C by 3 matrix of vertices for the second exterior cage

enum ExtrapMethod
{
  SIMPLE = 0,
  SIMPLE_ALIGNED = 1,
  SIMPLE_EXPAND = 2,
  MVC = 3
};

void extrap_cage(
  const Eigen::MatrixXd & V1,
  const Eigen::MatrixXd & V2,
  const Eigen::MatrixXi & F,
  const Eigen::MatrixXd & V1_C,
  const Eigen::MatrixXi & F_C,
  const ExtrapMethod method,
  Eigen::MatrixXd & V2_C);