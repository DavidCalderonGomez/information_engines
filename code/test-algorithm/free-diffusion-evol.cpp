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
int delta = 40*32*50;

int main(){
  
  Crandom ran64(1);//create the generator of random numbers
  Particle particle[runs]; //create an array of particles  

    for(int k=0;k<runs;k++){
      particle[k].Initialize(0,0); 
      particle[k].Launch();
    }  

    for(int j=1;j<steps;j++){
      if (j%delta==0)cout<<j*dt<<" ";
      for(int k=0;k<runs;k++) {
	      particle[k].ThermoEvolution(ran64);//evolves the system   
        if(j%delta==0)cout<<particle[k].get_Pos()<<" ";
      }
      if(j%delta==0)cout<<endl;
    }
  return 0;
}
  