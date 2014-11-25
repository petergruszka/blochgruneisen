#include "fit_solver.h"

#include <gsl/gsl_multifit_nlin.h>

FitSolver::FitSolver(FitFunction* function, double *x, double *y, 
		     double *sigma, int numberOfMeasurments){

  this->x = x;
  this->y = y;
  this->sigma = sigma;
  this->function = function;
  
  this->numberOfMeasurments = numberOfMeasurments;
  numberOfParameters = function->getNumberOfParameters();

  covar = gsl_matrix_alloc(numberOfParameters, numberOfParameters);

  initializeSolver();
  initializeFDF();
}

FitSolver::~FitSolver(){

}

void FitSolver::initializeSolver(){
  const gsl_multifit_fdfsolver_type *T = gsl_multifit_fdfsolver_lmdr;
  solver = gsl_multifit_fdfsolver_alloc(T, numberOfMeasurments, 
					numberOfParameters);  
}

void FitSolver::initializeFDF(){

  fdf.f = &FitSolver::calculateChi;
  fdf.df = NULL;
  fdf.fdf = NULL;

  fdf.n = numberOfMeasurements;
  fdf.p = numberOfParameters;
  
}

void FitSolver::begin(const gsl_vector *parameter_guess){
  gsl_multifit_fdfsolver_set(solver, &fdf, parameter_guess);
  iter = 0;
}

int FitSolver::iterate(){
  iter++;
  gsl_multifit_fdfsolver_iterate(solver);
  status = gsl_multifit_test_delta(solver->dx, solver->x, 1e-13, 0);
  return stat
}

void FitSolver::end(){

}

void FitSolver::getParameters(double *param){
  for(int i = 0; i < numberOfParamters; i++){
    param[i] = get_vector_get(solver->x, i);
  }
}

int FitSolver::calculateChi(const gsl_vector *parameters, 
			    void *pt, gsl_vector *f){
  FitSolver* solver = (FitSolver *)pt;
  double *x = solver->x;
  double *y = solver->y;
  double *sigma = solver->sigma;

  for(int i = 0; i < solver->numberOfMeasurements; i++){
    double foo = solver->function->evaluate(x[i], parameters) - y[i];
    if(sigma != NULL){
      foo /= sigma[i];
    }
    gsl_vector_set(f, i, foo);
  }

  return GSL_SUCCESS;
}
