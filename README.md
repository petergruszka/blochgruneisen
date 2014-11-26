# README

## Introduction

This tool helps you to easily fit a Bloch-Grüneisen formula to your measured data.

The Bloch-Grüneisen formula is given by 

\rho(T) = \rho_0 + \sum_{n} \rho_n \cdot \left(\frac{T}{\Theta_D}\right)^n \cdot\int_{0}^{\frac{\Theta_D}{T}} \frac{x^n}{(\exp(x) - 1)(1 - \exp(-x))}

In this fitting tool only n equals 2,3,5 can be used for different scattering processes.  


## Build

Make sure you have installed GNU Scientific Library and g++ on your linux system.
Just execute  `make`  and run the application ./fitter with the appropriate parameters


## Parameter

### Properties
-f <Path>
-x <int>  set x column of your input file. normally it is set to 0
-y <int>  set y column of your input file. normally it is set to 1
-S <int>  set deviation column of your input file. normally it is not used!

## Fit Parameter

-e <double>	use Electron-Electron Scattering (T^2) and set initial guess
-s <double>	use Electron-Phonon Scattering (T^5) and set initial guess
-d <double> 	use Electron-Phonon Scattering (T^3) and set initial guess
-t <double>	set initial guess of debye temperature
-r <double>	set initial guess of residual resistance



