#include <iostream>
#include <fstream>

#include "file_load.h"
#include "fit_function_bloch.h"
#include "fit_solver.h"
#include <gsl/gsl_vector.h>

#include <getopt.h>

using namespace std;

void print_help(){
  cout << "-? --help \t print this" << endl << endl;

  cout << "---------- Data settings" << endl;
  cout << "-f --file <path> \t set file path"  << endl;
  cout << "-x <number> \t set column for x data" << endl;
  cout << "-y <number> \t set column for y data" << endl;
  cout << "-S --std <number> \t set column for deviation data" << endl <<endl;

  cout << "---------- Fitting settings"  << endl;

  cout << "-e --ee-scattering <number> \t use e-e-Scattering with initial guess" << endl;

  cout << "-d --sd-scattering <number> \t use s-d-Scattering with inital guess" << endl;

  cout << "-s --ss-scattering <number> \t use s-s-Scattering with initial guess" << endl;

  cout << "-t --theta <number> \t initial guess of debye temperature" << endl;

  cout << "-r --rho <number> \t initial guess of residual resistance" << endl;

  cout << endl << endl;

}


int main(int argc, char** argv){

  double *x = NULL, *y = NULL, *std = NULL;

  double p_init[] = {1, 0, 0, 0, 0};
  gsl_vector_view v_param = gsl_vector_view_array(p_init, 5);

  FitFunctionBloch bloch;

  int x_column = 0
    , y_column = 1
    , std_column = -1;

  bool file_set = false;

  ifstream stream;

  while (1)
    {
      static struct option long_options[] =
        {
          {"help",     no_argument,       0, '?'},
          {"file", required_argument, 0, 'f'},
          {"x", required_argument, 0, 'x'},
          {"y", required_argument, 0, 'y'},
          {"std", required_argument, 0, 'S'},
          {"ee-scattering",  required_argument, 0, 'e'},
          {"sd-scattering",  required_argument, 0, 'd'},
          {"ss-scattering",  required_argument, 0, 's'},
          {"theta",    required_argument, 0, 't'},
          {"rho",    required_argument, 0, 'r'},
          {0, 0, 0, 0}
        };
      /* getopt_long stores the option index here. */
      int option_index = 0;

      int c = getopt_long (argc, argv, "?f:e:d:s:t:r:x:y:S:",
                       long_options, &option_index);

      /* Detect the end of the options. */
      if (c == -1)
        break;

      switch (c)
        {
        case 'f':
          file_set = true;
          cout << "# open file " << optarg << endl;
          stream.open(optarg);
          break;

        case 'e':
          cout << "# use electron-electron scattering: " << optarg << endl;
          p_init[2] = atof(optarg);
	  bloch.useElectronElectronScattering();
          break;

        case 'd':
          cout << "# use s-d scattering: " << optarg <<  endl;
          p_init[3] = atof(optarg);
          bloch.useSDScattering();
          break;

        case 's':
          cout << "# use s-s scattering: " << optarg << endl;
          p_init[4] = atof(optarg);
          bloch.useSSScattering();
          break;

        case 't':
          cout << "# set debye temperature: " << optarg << endl;
          p_init[0] = atof(optarg);
          break;

        case 'r':
          cout << "# set residual resistance: " << optarg << endl;
          p_init[1] = atof(optarg);
          break;

        case 'x':
          cout << "# set x column: " << optarg << endl;
          x_column = atoi(optarg);
          break;          
 
        case 'y':
          cout << "# set y column: " << optarg << endl;
          y_column = atoi(optarg);
          break;     
          
        case 'S':
	  cout << "# set deviation column: " << optarg << endl;
          std_column = atoi(optarg);
	  break;

        case '?':
        default:
	  print_help();
          exit(-1);
        }
    }

  if(!file_set){
    cerr << "no data to fit" << endl;
    print_help();
    exit(-1);
  }

  FileData data(&stream);

  int numData = data.getNumberOfRows();
  x = new double[numData];
  data.getColumn(x_column, x);

  y = new double[numData];
  data.getColumn(y_column, y);

  if(std_column >= 0){
    std = new double[std_column];
    data.getColumn(std_column, std);
  }

  FitSolver solver(&bloch, x, y, std, numData);

  double newParameter[] = {1, 1, 1, 1, 1};
  double dev[] = {1, 1, 1, 1, 1};

  solver.begin(&v_param.vector);
  while(solver.iterate() == GSL_CONTINUE){
    solver.getParameters(newParameter);
    clog << "#";
    for(int i = 0; i < 5; i++){
      clog << newParameter[i] << " ";
    }
    clog << " -> " << solver.getChiReduced() << endl;
  }

  solver.getParameters(newParameter);
  solver.getDeviations(dev);

  cout << "# Debye Temperature: " 
       << newParameter[0] << " +/- " 
       << dev[0] << endl;

  cout << "# Residual Resistance: " 
       << newParameter[1] << " +/- " 
       << dev[1] << endl;
  
  cout << "# e-e Scattering: " 
       << newParameter[2] << " +/- " 
       << dev[2] << endl;

  cout << "# S-D Scattering: " 
       << newParameter[3] << " +/- " 
       << dev[3] << endl;

  cout << "# S-S Scattering: " 
       << newParameter[4] << " +/- " 
       << dev[4] << endl;


  gsl_vector_view new_params = gsl_vector_view_array(newParameter, 5);

  for(double T = 0.0; T < 300.0; T+=0.1){
    cout << T << " " << bloch.evaluate(T, &new_params.vector) << endl;
  }


  solver.end();
}
