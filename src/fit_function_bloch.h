#ifndef FIT_FUNCTION_BLOCH_H
#define FIT_FUNCTION_BLOCH_H

#include "fit_function.h"
#include <gsl/gsl_vector.h>

class FitFunctionBloch : public FitFunction{

    public: 
        FitFunctionBloch();
        virtual ~FitFunctionBloch();
        
        virtual double evaluate(double x, const gsl_vector *parameters);
        virtual int getNumberOfParameters();
        
        static double integrand(double x, void *param);
        
        void useElectronElectronScattering();
        void useSDScattering();
        void useSSScattering();
        
    private:
        double integral(double x, double n);
        
        bool use_e_e, use_s_d, use_s_s;        
        

};

#endif