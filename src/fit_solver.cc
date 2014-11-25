#include "fit_solver.h"

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
  const gsl_multifit_fdfsolver_type *T = gsl_multifit_fdfsolver_lmder;
  solver = gsl_multifit_fdfsolver_alloc(T, numberOfMeasurments, 
					numberOfParameters);  
}

void FitSolver::initializeFDF(){

  fdf.f = &FitSolver::calculateChi;
  fdf.df = NULL;
  fdf.fdf = NULL;

  fdf.n = numberOfMeasurments;
  fdf.p = numberOfParameters;

  fdf.params = this;
}

void FitSolver::begin(const gsl_vector *parameter_guess){
  gsl_multifit_fdfsolver_set(solver, &fdf, parameter_guess);
  iter = 0;
}

int FitSolver::iterate(){
  iter++;
  gsl_multifit_fdfsolver_iterate(solver);
  return gsl_multifit_test_delta(solver->dx, solver->x, 1e-13, 0);
}

void FitSolver::end(){
  gsl_multifit_fdfsolver_free(solver);
}

void FitSolver::getParameters(double *param){
  for(int i = 0; i < numberOfParameters; i++){
    param[i] = gsl_vector_get(solver->x, i);
  }
}

void FitSolver::getDeviations(double *dev){
  double chi = getChiReduced();
  double c = GSL_MAX_DBL(1, chi);

  gsl_multifit_covar(solver->J, 0.0, covar);

  for(int i = 0; i < numberOfParameters; i++){
    dev[i] = sqrt(gsl_matrix_get(covar, i, i)) * c;
  }
}

double FitSolver::getChi(){
  return gsl_blas_dnrm2(solver->f);
}

double FitSolver::getChiReduced(){
  double degreesOfFreedom = 
    (double)(numberOfMeasurments - numberOfParameters);

    return getChi() / sqrt(degreesOfFreedom);
}


int FitSolver::calculateChi(const gsl_vector *parameters, 
			    void *pt, gsl_vector *f){
  FitSolver* solver = (FitSolver *)pt;
  double *x = solver->x;
  double *y = solver->y;
  double *sigma = solver->sigma;

  for(int i = 0; i < solver->numberOfMeasurments; i++){
    double foo = solver->function->evaluate(x[i], parameters) - y[i];
    if(sigma != NULL){
      foo /= sigma[i];
    }
    gsl_vector_set(f, i, foo);
  }

  return GSL_SUCCESS;
}
