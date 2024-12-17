#include <iostream>
#include <cmath>
#include "Random64.h"
#include "ThermoSystem.h"

using std::cout, std::endl;

int ttime = 100;
int steps = parameter*samp_freq*ttime; 
int runs = 1000;

int main(){
  
  Crandom ran64(1);//create the generator of random numbers
  Particle particle[runs]; //create an array of particles  
  double gain=1.63;double SNR=11;
  double Pos=0, Pot=0, rise;
  int t=0;

    for(int k=0;k<runs;k++){
      particle[k].Initialize(0,0); 
      particle[k].CalculateForce();
      particle[k].Launch();
    } 
    
    for(int j=1;j<steps;j++){
        if(j%parameter==0) {t+=ts;cout<<t<<"   ";} 
        for(int k=0;k<runs;k++) {//evolve one step each particle:
            if(j%parameter==0){ 
              Pot = particle[k].get_Pot(); //store potential zero
              Pos = particle[k].get_Pos(); //stores the position before the evolution        
              rise = particle[k].Protocol(gain,ran64,SNR);//calculate the one will be used in the next step
              particle[k].set_Pot(Pot+rise);//change the potential with the result from the previous step
          
              cout<<Pos<<" ";
            }	        
        particle[k].CalculateForce();//calculates all forces
	      particle[k].ThermoEvolution(ran64);//evolves the system     
        }
        if(j%parameter==0)cout<<endl;
    }
  return 0;
}
  