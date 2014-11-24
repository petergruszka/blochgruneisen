#include "file_load.h"
#include <sstream>
#include <string>

FileData::FileData(istream *stream){

    initialize_shape(*stream);
    stream->clear();
    stream->seekg(0, stream->beg);
    initialize_data(*stream);
}


FileData::~FileData(){
    delete[] data;
}


void FileData::initialize_shape(istream &stream){
    columns = 0;
    rows = 0;
    
    double f;
    
    string line;

    while(getline(stream, line)){
        if(line[0] != '#'){
            if(columns==0){
                istringstream is(line);
                while(is >> f){
                    columns++;
                }
            }else{
                istringstream is(line);
                int test_columns = 0;
                while(is >> f){
                    test_columns++;
                }
                if(test_columns != columns){
                    throw false;
                }
            }
            rows++;
        }
    }
    
    n = columns * rows;     
}

void FileData::initialize_data(istream &stream){
    data = new double[n];

    string line;
    int j = 0;
    
    float f;

    while(getline(stream, line)){    
        if(line[0] != '#'){
            istringstream is(line);
            while(is >> f){
                data[j] = f;
                j++;
            }
        }
    }    
    
}


void FileData::getColumn(int i, double* data){
    for(int j = 0; j < rows; j++){
        data[j] = this->data[i + columns * j];
    }
}

void FileData::getRow(int i, double *data){
    for(int j = 0; j < columns; j++){
        data[j] = this->data[i * columns + j];
    }
}

int FileData::getNumberOfColumns(){
    return columns;
}

int FileData::getNumberOfRows(){
    return rows;
}