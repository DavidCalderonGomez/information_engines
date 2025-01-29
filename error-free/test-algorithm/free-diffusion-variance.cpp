#include <iostream>
#include <cmath>
#include "..\Random64.h"
#include "..\ThermoSystem.h"

//We'll simulate free diffusion
//and adjust the variance over time to recover diffusion constant D


using std::cout, std::endl;

int total_time = 1000;
int steps = total_time/dt; 
const int runs = 5000;
double positions[runs]; // creates an array that will store position of each particle

double variance(const double* data);

int main(){
  
  Crandom ran64(1);//create the generator of random numbers
  Particle particle[runs]; //create an array of particles  
  double Pos=0, Pot=0,rise;
  int t=0;

    for(int k=0;k<runs;k++){
      particle[k].Initialize(0,0); 
      particle[k].Launch();
    }  
    for(int j=1;j<steps;j++){
      for(int k=0;k<runs;k++) {
	      particle[k].ThermoEvolution(ran64);//evolves the system     
        positions[k]=particle[k].get_Pos();      
      }
      cout<<j*dt<<" "<<variance(positions)<<endl;
    }
  return 0;
}

double variance(const double* data){
    // 1) Compute the mean
    double sum = 0.0;
    for (int i = 0; i < runs; ++i) {
        sum += data[i];
    }
    // sum is a double, so sum / n will be a double division automatically.
    double mean = sum / runs;
    // 2) Compute sum of squared differences from the mean
    double sq_diff_sum = 0.0;
    for (int i = 0; i < runs; ++i) {
        double diff = data[i] - mean;
        sq_diff_sum += diff * diff;
    }
    // Return sample variance = M2 / (n - 1)
    return sq_diff_sum / (runs - 1);
}
  