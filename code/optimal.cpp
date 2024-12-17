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
  double SNR=11;
  double Pos=0, Pot=0, rise;
  int t=0;
  double mean_work=0;

 for (double gain = 1.62; gain <= 1.64; gain+=0.001){ //iterate over different gains
    
    for(int k=0;k<runs;k++){
      particle[k].Initialize(0,0); 
      particle[k].CalculateForce();
      particle[k].Launch();
    } 

    for(int j=1;j<steps;j++){
        for(int k=0;k<runs;k++) {

        if(j%parameter==0){ 
            Pot = particle[k].get_Pot(); //store potential zero
            Pos = particle[k].get_Pos(); //stores the position before the evolution        
            rise = particle[k].Protocol(gain,ran64,SNR);//calculate the one will be used in the next step
            particle[k].set_Pot(Pot+rise);//change the potential with the result from the previous step
            particle[k].CalculateWork(Pot);//calculates the work with the past potential
            }	        
        particle[k].CalculateForce();//calculates all forces
	      particle[k].ThermoEvolution(ran64);//evolves the system     
        }
    }

    for(int k=0;k<runs;k++) {
      mean_work+= particle[k].get_Work();
    }
    cout<<gain<<"   "<<mean_work/runs<<endl;
    mean_work=0;
    }
  return 0;
}