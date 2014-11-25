
SRC_DIR = ./src
OBJ_DIR = ./object
BIN_DIR = ./bin

LIBS = -lgsl -lgslcblas


fitter: $(BIN_DIR)/fitter
	cp $(BIN_DIR)/fitter fitter

clean:
	rm fitter $(BIN_DIR)/fitter $(OBJ_DIR)/*

$(BIN_DIR)/fitter: $(OBJ_DIR)/file_load.o $(OBJ_DIR)/fit_function_bloch.o $(OBJ_DIR)/fit_solver.o $(OBJ_DIR)/main.o
	g++ -o $(BIN_DIR)/fitter $(OBJ_DIR)/file_load.o $(OBJ_DIR)/fit_function_bloch.o $(OBJ_DIR)/fit_solver.o $(OBJ_DIR)/main.o $(LIBS)  

$(OBJ_DIR)/file_load.o: $(SRC_DIR)/file_load.h $(SRC_DIR)/file_load.cc
	g++ -o $(OBJ_DIR)/file_load.o -c $(SRC_DIR)/file_load.cc

$(OBJ_DIR)/fit_function_bloch.o: $(SRC_DIR)/fit_function_bloch.h $(SRC_DIR)/fit_function_bloch.cc $(SRC_DIR)/fit_function.h
	g++ -o $(OBJ_DIR)/fit_function_bloch.o -c $(SRC_DIR)/fit_function_bloch.cc

$(OBJ_DIR)/fit_solver.o: $(SRC_DIR)/fit_solver.h $(SRC_DIR)/fit_solver.cc $(SRC_DIR)/fit_function.h
	g++ -o $(OBJ_DIR)/fit_solver.o -c $(SRC_DIR)/fit_solver.cc

$(OBJ_DIR)/main.o: $(SRC_DIR)/file_load.h $(SRC_DIR)/fit_function_bloch.h $(SRC_DIR)/fit_solver.h $(SRC_DIR)/main.cc
	g++ -o $(OBJ_DIR)/main.o -c $(SRC_DIR)/main.cc