#ifndef FILE_LOAD_H
#define FILE_LOAD_H


#include <iostream>

using namespace std;

class FileData{
    private:
        int columns;
        int rows;
        int n;
        double *data;
        
        void initialize_shape(istream &stream);
        void initialize_data(istream &stream);
        
    public:
        FileData(istream *stream);
        ~FileData();
        
        void getColumn(int i, double* data);
        void getRow(int i, double* data);
        
        int getNumberOfColumns();
        int getNumberOfRows();

};


#endif