#ifndef FIT_SOLVER_H__
#define FIT_SOLVER_H__

#include "fit_function.h"
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_multifit_nlin.h>

class FitSolver{
    public:
        FitSolver(FitFunction* function, 
		  double *x, double *y, double *sigma, int n);

        virtual ~FitSolver();
        
        void begin(const gsl_vector *parameter_guess);
        int iterate();
        void end();
        
        static int calculateChi(const gsl_vector *x, void *pt, 
				gsl_vector *f);
        
	void getParameters(double *param);
        void getDeviations(double *dev);
	
	double getChiReduced();
	double getChi();
		      
	

    protected:
	double *x, *y, *sigma;
	FitFunction *function;
	int numberOfMeasurments, numberOfParameters;
	gsl_matrix *covar;

	gsl_multifit_function_fdf fdf;
	gsl_multifit_fdfsolver *solver;

	int iter;
  
    private:
	void initializeSolver();
	void initializeFDF();
};

#endif
