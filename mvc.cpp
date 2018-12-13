#include "mvc.h"
#include <cmath>
#include <Eigen/Dense>

#define EPS 1e-4

void mvc(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const Eigen::MatrixXd & X,
  Eigen::MatrixXd & W)
{
  W = Eigen::MatrixXd::Zero(X.rows(), V.rows());
  for (int xi = 0; xi < X.rows(); xi++) {
    Eigen::RowVector3d x = X.row(xi);
    
    Eigen::MatrixXd U(V.rows(), 3);
    Eigen::VectorXd D(V.rows());
    bool abort = false;
    for (int vi = 0; vi < V.rows(); vi++) {
      Eigen::RowVector3d p = V.row(vi);
      D[vi] = (p - x).norm();
      if (D[vi] < EPS) {
        W(xi, vi) = 1.0;
        abort = true;
        break;
      }
      U.row(vi) = (p - x) / D[vi];
    }
    if (abort) continue;
    
    for (int fi = 0; fi < F.rows(); fi++) {
      int p1i = F.row(fi)[0];
      int p2i = F.row(fi)[1];
      int p3i = F.row(fi)[2];
      
      double l1 = (U.row(p2i) - U.row(p1i)).norm();
      double l2 = (U.row(p3i) - U.row(p2i)).norm();
      double l3 = (U.row(p1i) - U.row(p3i)).norm();
      
      double th1 = 2*asin(l1/2.0);
      double th2 = 2*asin(l2/2.0);
      double th3 = 2*asin(l3/2.0);
      
      double h = (th1 + th2 + th3) / 2.0;
      
      if (M_PI - h < EPS) {
        W.row(xi).setZero();
        W(xi, p1i) = sin(th1) * D.row(p3i) * D.row(p2i);
        W(xi, p2i) = sin(th2) * D.row(p1i) * D.row(p3i);
        W(xi, p3i) = sin(th3) * D.row(p2i) * D.row(p1i);
        break;
      }
      
      double c1 = 2*sin(h)*sin(h-th1)/(sin(th3)*sin(th2)) - 1;
      double c2 = 2*sin(h)*sin(h-th2)/(sin(th1)*sin(th3)) - 1;
      double c3 = 2*sin(h)*sin(h-th3)/(sin(th2)*sin(th1)) - 1;
      
      Eigen::Matrix3d UM;
      UM << U.row(p1i), U.row(p2i), U.row(p3i);
      double usgn = UM.determinant() > 0 ? 1.0 : -1.0;
      
      double s1 = usgn * sqrt(1-c1*c1);
      double s2 = usgn * sqrt(1-c2*c2);
      double s3 = usgn * sqrt(1-c3*c3);
      
      if (abs(s1) < EPS || abs(s2) < EPS || abs(s3) < EPS) {
        continue;
      }
        
      W(xi, p1i) += (th1 - c2*th3 - c3*th2) / (D[p1i] * sin(th2) * s3);
      W(xi, p2i) += (th2 - c3*th1 - c1*th3) / (D[p2i] * sin(th3) * s1);
      W(xi, p3i) += (th3 - c1*th2 - c2*th1) / (D[p3i] * sin(th1) * s2);
    }
    
    W.row(xi).array() /= W.row(xi).sum();
  }
}