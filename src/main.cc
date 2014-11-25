#include "fit_function_bloch.h"
#include "fit_solver.h"
#include <gsl/gsl_vector.h>


int main(){

  double x[] = {1, 2, 3, 4, 5, 6};
  double y[] = {1, 2, 3, 4, 5, 6};
  double sigma[] = {1e-9, 2e-9, 3e-9, 4e-9, 5e-9, 6e-9};

  double parameter[] = {1, 1, 1, 1, 1};
  gsl_vector_view v_param = gsl_vector_view_array(parameter, 5);


  FitFunctionBloch bloch;
  FitSolver solver(&bloch, x, y, sigma, 6);

  double newParameter[] = {1, 1, 1, 1, 1};

  solver.begin(&v_param.vector);
  solver.iterate();
  solver.getParameters(newParameter);
  for(int i = 0; i < 5; i++){
    cout << newParameter[i] << endl
  }
  solver.end();
}
