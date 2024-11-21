#include <iostream>
#include <cmath>
#include "Random64.h"
#include "ThermoSystem.h"

using std::cout, std::endl;

int steps = parameter*samp_freq*100; 

int main(){
  
  Crandom ran64(1);//create the generator of random numbers
  Particle particle; //create an array of particles  
  double gain=1.5;double SNR=11;
  double Pos=0, Pot=0;
  double past_up=0,up=0; //to apply the one step delay
  
    particle.Initialize(0,0); 
    particle.Launch();
    
    for(int j=1;j<steps;j++){
	//evolve one step each particle:
        if(j%parameter==0){ 
            Pot = particle.get_Potential_zero(); //store potential zero
            Pos = particle.get_Pos(); //stores the position before the evolution        
            up = particle.Protocol(gain,ran64,Pos,SNR);//calculate the one will be used in the next step
            particle.set_Potential_zero(Pot+past_up);//change the potential with the result from the previous step
            
            cout<<Pos<<" "<<Pot<<endl;

            past_up=up; //for the next iteration this is now past 
        }	        
      particle.CalculateForce();//calculates all forces
	    particle.ThermoEvolution(ran64);//evolves the system     
    }
  return 0;
}
  





