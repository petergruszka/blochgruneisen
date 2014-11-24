#ifndef FIT_FUNCTION_H
#define FIT_FUNCTION_H

#include <gsl/gsl_vector.h>

class FitFunction{
    public:
        virtual double evaluate(double x, const gsl_vector *parameters) = 0;
        virtual int getNumberOfParameters() = 0;

};



#endif