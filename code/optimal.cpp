#include <iostream>
#include <cmath>
#include "Random64.h"
#include "ThermoSystem.h"

using std::cout, std::endl;


int steps = parameter*samp_freq*100; 
int runs = 100;

int main(){
  
  Crandom ran64(1);//create the generator of random numbers
  Particle particle[runs]; //create an array of particles  
  double gain=0.5;double SNR=11;
  double Pos=0, Pot=0;
  double past_up[runs]={},up[runs]={}; //to apply the one step delay
  int time=0;

  double mean_work=0;

    


 for (gain = 0.5; gain < 2; gain+=0.1){ //iterate over different gains
    
    for(int k=0;k<runs;k++) particle[k].Initialize(0,0); 
    for(int k=0;k<runs;k++) particle[k].Launch();

    for(int j=1;j<steps;j++){
        for(int k=0;k<runs;k++) {
        
	//evolve one step each particle:
        if(j%parameter==0){ 
            Pot = particle[k].get_Potential_zero(); //store potential zero
            Pos = particle[k].get_Pos(); //stores the position before the evolution        
            up[k] = particle[k].Protocol(gain,ran64,Pos,SNR);//calculate the one will be used in the next step
            particle[k].set_Potential_zero(Pot+past_up[k]);//change the potential with the result from the previous step
            particle[k].CalculateWork(Pot);//calculates the work with the past potential
            past_up[k]=up[k]; //for the next iteration this is now past 
        }	        
        particle[k].CalculateForce();//calculates all forces
	    particle[k].ThermoEvolution(ran64);//evolves the system     
    }
    }

    for(int k=0;k<runs;k++) {
      mean_work+= particle[k].get_Work();
      particle[k].reset();
      }
    cout<<gain<<"   "<<mean_work/runs<<endl;
    mean_work=0;
    }
  return 0;
}