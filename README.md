--- README ---

This tool helps you to easily fit a Bloch-Grüneisen formula to your
measured data.

The Bloch-Grüneisen formula is given by 

\rho(T) = \rho_0 + \sum_{n} \rho_n \cdot \left(\frac{T}{\Theta_D}\right)^n \cdot\int_{0}^{\frac{\Theta_D}{T}} \frac{x^n}{(\exp(x) - 1)(1 - \exp(-x))}

In this fitting tool only n equals 2,3,5 can be used for different scattering processes.  
