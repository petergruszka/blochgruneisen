#include "../src/file_load.h"
#include "gtest/gtest.h"

#include <iostream>
#include <sstream>
#include <string>
#include <cmath>

using namespace std;

class FileDataTest : public testing::Test {
    private:
        
        void SetUp_correct_data(){
            stringstream correct_data_stream;
            correct_data_stream << "# hello i'm a comment" << endl;
            correct_data_stream << "1.0 -1.2 1.5" << endl;
            correct_data_stream << "0.8 9.1 1e-7" << endl;
            correct_data_stream << "# i'm another comment" << endl;
            correct_data_stream << "8.1 -1e-10 1.7";
            
            correct_data_ = new FileData((istream *)&correct_data_stream);  
            
            correct_data_string_ = correct_data_stream.str();
        }
        
        void SetUp_wrong_data(){
            stringstream wrong_data_stream;
            wrong_data_stream << "# hello i'm a comment" << endl;
            wrong_data_stream << "1.0 -1.2 1.5" << endl;
            wrong_data_stream << "0.8 9.1" << endl;
            wrong_data_stream << "i'm another comment" << endl << endl;
            wrong_data_stream << "8.1 -1e-10 1.7 1.9" << endl << endl;
            
            wrong_data_string_ = wrong_data_stream.str();
        }
        
    protected:
        virtual void SetUp() {
            SetUp_correct_data();
            SetUp_wrong_data();
        }
        
        virtual void TearDown(){
            delete correct_data_;
        }
       
    FileData *correct_data_;
    string correct_data_string_;
    string wrong_data_string_;
};

TEST_F(FileDataTest, VerifyNoExceptionsOnRightData){
    ASSERT_NO_THROW({
        stringstream stream;
        stream << correct_data_string_;
        FileData f((istream *)&stream);
    });
}

TEST_F(FileDataTest, ColumnTest){
    ASSERT_EQ(3, correct_data_->getNumberOfColumns());
}

TEST_F(FileDataTest, LineTest){
    ASSERT_EQ(3, correct_data_->getNumberOfRows());
}

TEST_F(FileDataTest, CheckValueAtPosition12){
    double* data = new double[correct_data_->getNumberOfColumns()];
    
    correct_data_->getRow(1, data);
    
    ASSERT_GE(2e-15, fabs(data[2] - 1e-7));
    
    delete[] data;
}


TEST_F(FileDataTest, VerifyExceptionsOnWrongData){
    ASSERT_ANY_THROW({
        stringstream stream;
        stream << wrong_data_string_;
        FileData f((istream *)&stream);
    });
}
