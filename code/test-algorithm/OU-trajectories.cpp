#include <iostream>
#include <iostream>
#include <cmath>
#include "..\Random64.h"
#include "ThermoSystemTest.h"

//We'll simulate free diffusion
//and adjust the variance over time to recover diffusion constant D

using std::cout, std::endl;

int total_time = 25;
int steps = total_time/dt; 
const int runs = 2000;

int main(){
  
  Crandom ran64(1);//create the generator of random numbers
  Particle particle[runs]; //create an array of particles  

    for(int k=0;k<runs;k++){
      particle[k].Initialize(0,100); //initial non zero velocity 
      particle[k].Launch();
    }  
    for(int j=1;j<steps;j++){
      cout<<j*dt<<" ";
      for(int k=0;k<runs;k++) {
	      particle[k].ThermoEvolution(ran64);//evolves the system         
        cout<<particle[k].get_Vel()<< " ";
      }
      cout<<endl;
    }
  return 0;
}
