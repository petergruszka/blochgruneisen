#include "fit_function_bloch.h"

#include <cmath>
#include <gsl/gsl_integration.h>

FitFunctionBloch::FitFunctionBloch(){
    use_e_e = false;
    use_s_d = false;
    use_s_s = false;
}

FitFunctionBloch::~FitFunctionBloch(){

}

void FitFunctionBloch::useElectronElectronScattering(){
    use_e_e = true;
}

void FitFunctionBloch::useSDScattering(){
    use_s_d = true;
}

void FitFunctionBloch::useSSScattering(){
    use_s_s = true;
}

double FitFunctionBloch::evaluate(double x, const gsl_vector *parameters){
    if(parameters->size != 5){
        throw false;
    }
    
    double theta = gsl_vector_get(parameters, 0);
    double rho0 = gsl_vector_get(parameters, 1);
    double rhoee = gsl_vector_get(parameters, 2);
    double rhosd = gsl_vector_get(parameters, 3);
    double rhoss = gsl_vector_get(parameters, 4);
    
    double foo = rho0;
    
    double T_red = x / theta;
    double T_inv = theta / x;
    
    if(use_e_e)
        foo += rhoee * pow(T_red, 2.0) * integral(T_inv, 2.0);
        
    if(use_s_d)
        foo += rhosd * pow(T_red, 3.0) * integral(T_inv, 3.0);
        
    if(use_s_s)
        foo += rhoss * pow(T_red, 5.0) * integral(T_inv, 5.0);
    
    return foo;
}


int FitFunctionBloch::getNumberOfParameters(){
    return 5;
}

double FitFunctionBloch::integral(double x, double n){
    gsl_integration_workspace *w
        = gsl_integration_workspace_alloc (2000);
    double result, error;

    gsl_function F;
    F.function = FitFunctionBloch::integrand;
    F.params = &n;    
    
    if(isinf(x)){
        gsl_integration_qagiu(&F, 0, 0, 1e-13, 2000, w, &result, &error);
    }else{
        gsl_integration_qags(&F, 0, x, 0, 1e-13, 2000, w, &result, &error);
    }
    gsl_integration_workspace_free (w);
    
    return result;
}

double FitFunctionBloch::integrand(double x, void *param){
    double n = *(double *)param;
    double f = 0;
    
    if( x < 1e-4){
	    double m = n - 2.0;
	    double x2 = x * x;
	    f = pow(x, m) *(1 - x2/12.0 *(1- x2/4.0 *(0.2 - x2/18.0 *(1/7.0 - x2/40.0 * ( 1 - x2/154.0)))));
    }else{
        f = pow(x, n) / ((exp(x) - 1.0) * (1.0 - exp(-x)));
    }
    
    return f;
}

