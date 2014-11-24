#include "../src/fit_function_bloch.h"
#include "gtest/gtest.h"

#include <gsl/gsl_vector.h>
#include <cmath>

class FitFunctionBlochTest : public testing::Test {
    protected:
        virtual void SetUp() {
            bloch_ = new FitFunctionBloch();
            error = 1e-10;
            error_integral = 1e-5;
        }
        
        virtual void TearDown(){
            delete bloch_;
        }
        
        FitFunctionBloch *bloch_;
        double error;
        double error_integral;
};

TEST_F(FitFunctionBlochTest, NumberOfParameters){
    ASSERT_EQ(bloch_->getNumberOfParameters(), 5);
}

TEST_F(FitFunctionBlochTest, ConstantTerm){
    double parameters[] = {0.0, 1.0, 0, 0, 0};
    gsl_vector_view parameter_view = gsl_vector_view_array(parameters, 5);    
    ASSERT_EQ(1.0, bloch_->evaluate(1.0, &parameter_view.vector));
    
    parameters[1] = 2.0;
    ASSERT_EQ(2.0, bloch_->evaluate(1.0, &parameter_view.vector));    

    parameters[1] = -1.0;
    ASSERT_EQ(-1.0, bloch_->evaluate(1.0, &parameter_view.vector));  
}

TEST_F(FitFunctionBlochTest, WrongNumberOfParameters){

   double parameters[] = {0.0, 1.0, 0.0};	
   gsl_vector_view parameter_view = gsl_vector_view_array(parameters, 3);    

   ASSERT_ANY_THROW(bloch_->evaluate(1.0, &parameter_view.vector));

   double parameters2[] = {0.0, 1.0, 0.0, 0.0, 0.0, 0.0};	
   gsl_vector_view parameter_view2 = gsl_vector_view_array(parameters2, 6);    

   ASSERT_ANY_THROW(bloch_->evaluate(1.0, &parameter_view2.vector));

}

TEST_F(FitFunctionBlochTest, Integrand1Value){    
    double n2 = 2;
    double n3 = 3;
    double n5 = 5;
    
    double result = 0.9206735942077923189;
    ASSERT_LE(fabs(FitFunctionBloch::integrand(1.0, &n2) - result), error);
    ASSERT_LE(fabs(FitFunctionBloch::integrand(1.0, &n3) - result), error);
    ASSERT_LE(fabs(FitFunctionBloch::integrand(1.0, &n5) - result), error);
    
}

TEST_F(FitFunctionBlochTest, IntegrandLowValues){
    double n2 = 2;
    double n3 = 3;
    double n5 = 5;
        
    double  result = 0.9999999999999999999;
    ASSERT_LE(fabs(FitFunctionBloch::integrand(1e-10, &n2) - result) / result, error);
    result = 9.999999999999999999e-11;
    ASSERT_LE(fabs(FitFunctionBloch::integrand(1e-10, &n3) - result) / result, error);
    result = 9.999999999999999999e-31;
    ASSERT_LE(fabs(FitFunctionBloch::integrand(1e-10, &n5) - result) / result, error);
}


TEST_F(FitFunctionBlochTest,ElectronElectronTerm){
    double parameters[] = {1.0, 0.0, 1.0, 0, 0};
    gsl_vector_view parameter_view = gsl_vector_view_array(parameters, 5);  
    
    bloch_->useElectronElectronScattering();
    
    double result = 0.973033;
    ASSERT_LE(fabs(bloch_->evaluate(1.0, &parameter_view.vector)- result)/result, error_integral);
 
    result = 0.032843291144979525;
    ASSERT_LE(fabs(bloch_->evaluate(0.1, &parameter_view.vector)- result)/result, error_integral);
       

}

TEST_F(FitFunctionBlochTest,SDTerm){
    double parameters[] = {1.0, 0.0, 0.0, 1.0, 0};
    gsl_vector_view parameter_view = gsl_vector_view_array(parameters, 5);  
    
    bloch_->useSDScattering();
    
    double result = 0.479841;
    ASSERT_LE(fabs(bloch_->evaluate(1.0, &parameter_view.vector)- result)/result, error_integral);

    result = 0.007150322712008594;
    ASSERT_LE(fabs(bloch_->evaluate(0.1, &parameter_view.vector)- result)/result, error_integral);
    
    

}

TEST_F(FitFunctionBlochTest,SSTerm){
    double parameters[] = {1.0, 0.0, 0.0, 0.0, 1.0};
    gsl_vector_view parameter_view = gsl_vector_view_array(parameters, 5);  
   
    bloch_->useSSScattering();
    
    double result = 0.236616;
    ASSERT_LE(fabs(bloch_->evaluate(1.0, &parameter_view.vector)- result)/result, error_integral);
    
    result = 0.0011638074540242078;
    ASSERT_LE(fabs(bloch_->evaluate(0.1, &parameter_view.vector)- result)/result, error_integral);
    

}

TEST_F(FitFunctionBlochTest, ZeroAllTerms){
    double parameters[] = {1.0, 0.0, 1.0, 1.0, 1.0};
    gsl_vector_view parameter_view = gsl_vector_view_array(parameters, 5);  
   
    bloch_->useElectronElectronScattering();
    bloch_->useSDScattering();
    bloch_->useSSScattering();
    
    ASSERT_LE(fabs(bloch_->evaluate(0, &parameter_view.vector)), error_integral);
       
}

